cbuffer FrameConstantBuffer : register( b0 )
{
    matrix View;
    matrix Projection;

    float4 AmbientColor;
	float4 LightColor;
	float3 LightDirection;

	float3 CameraPosition;
}

cbuffer ObjectConstantBuffer : register( b1 )
{
    matrix World;

    float4 SpecularColor;
    float  SpecularPower;
}

struct VS_INPUT
{
    float4 Position : POSITION;
    float4 Color    : COLOR0;
    float3 Normal   : NORMAL;
	float3 Tangent  : TANGENT;
	float3 Binormal : BINORMAL;
};

struct VS_OUTPUT
{
    float4 Position       : SV_POSITION;
    float4 Color          : COLOR0;
    float3 Normal         : NORMAL;
    float4 LightDirection : COLOR1;
	float3 ViewDirection  : COLOR2;
};

VS_OUTPUT vertex_shader( VS_INPUT IN )
{
    VS_OUTPUT OUT          = (VS_OUTPUT)0;

    OUT.Position           = mul( IN.Position, World );
    OUT.Position           = mul( OUT.Position, View );
    OUT.Position           = mul( OUT.Position, Projection );

    OUT.Color              = IN.Color;
    OUT.Normal             = normalize(mul(float4(IN.Normal, 0.0f), World)).xyz;
    OUT.LightDirection.xyz = normalize(-LightDirection);
	OUT.LightDirection.w   = 1.0f;

	float3 worldPosition   = mul(IN.Position, World).xyz;
	OUT.ViewDirection      = normalize(CameraPosition - worldPosition);

    return OUT;
}

float4 pixel_shader( VS_OUTPUT IN ) : SV_Target
{
    float4 OUT                = (float4)0;

    float3 normal             = normalize(IN.Normal);
    float3 lightDirection     = normalize(IN.LightDirection.xyz);
	float3 viewDirection      = normalize(IN.ViewDirection);
    float n_dot_l             = dot(lightDirection, normal);

	float4 color              = IN.Color;

	//A = la * ma
    float3 ambient            = AmbientColor.rgb * AmbientColor.a * color.rgb;

    float3 diffuse            = (float3)0;
	float3 specular           = (float3)0;
	float3 refVector          = (float3)0;

    if(n_dot_l > 0)
    {
		// D = kd * ld * md
        diffuse = max(n_dot_l, 0) * LightColor.rgb * LightColor.a * color.rgb;

		// R = I - 2(n.I) * n
		refVector = normalize(reflect(lightDirection, normal));

		// S = max(dot(V.R),0)^P * SpecularColor.rgb * SpecularColor.a * color.rgb;
		specular = pow(max(dot(viewDirection, refVector), 0), SpecularPower) * SpecularColor.rgb * SpecularColor.a * color.rgb;
	}

	OUT.rgb = ambient + diffuse + specular;
	OUT.a = 1.0f;

	return OUT;
}
