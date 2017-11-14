float3 get_vector_color_contribution(float4 light, float3 color)
{
	// Color (.rgb) * Intensity (.a)
	return light.rgb * light.a * color;
}

float3 get_scalar_color_contribution(float4 light, float color)
{
	// Color (.rgb) * Intensity (.a)
	return light.rgb * light.a * color;
}

cbuffer FrameConstantBuffer : register( b0 )
{
    matrix View;
    matrix Projection;

    float4 AmbientColor;
	float4 Padding;         //Don't know why I need that to have Specular
	float4 CameraPosition;  //float3

	matrix World;

	float4 SpecularColor;
	float4 SpecularPower;   //float

	float4 DirectionalColor;
	float4 DirectionalDirection;  //float3

	float4 LightColor;
	float4 LightDirection;  //float3

	float4 LightRadius;     //float
	float4 LightPosition;   //float3

	float4 LightInnerAngle; //float3
	float4 LightOuterAngle; //float3

	float4 LightType;       //int
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
    float4 Position             : SV_POSITION;
    float4 Color                : COLOR0;
    float3 Normal               : NORMAL;

	float3 ViewDirection        : COLOR1;
	float3 LightViewDirection   : COLOR2;

	float4 DirectionalDirection : COLOR3;

	float4 LightDirection       : COLOR4;
};

VS_OUTPUT vertex_shader( VS_INPUT IN )
{
    VS_OUTPUT OUT            = (VS_OUTPUT)0;

	//Object Data
    OUT.Position             = mul( IN.Position, World );
    OUT.Position             = mul( OUT.Position, View );
    OUT.Position             = mul( OUT.Position, Projection );
    OUT.Color                = IN.Color;
    OUT.Normal               = normalize(mul(float4(IN.Normal, 0.0f), World)).xyz;

	//Directional Data
    OUT.DirectionalDirection.xyz = normalize(-(DirectionalDirection.xyz));
	OUT.DirectionalDirection.w   = DirectionalDirection.w;

	//View Data
	float3 worldPosition   = mul(IN.Position, World).xyz;
	OUT.ViewDirection      = normalize(worldPosition - CameraPosition.xyz);

	//Light Data
	if (LightType.x == 1.0f)
	{
		float3 lightDirection = (LightPosition.xyz) - worldPosition;
		OUT.LightDirection.xyz = normalize(-(lightDirection.xyz));
		OUT.LightDirection.w   = saturate(1.0f - (length(-lightDirection) / LightRadius.x));
		OUT.LightViewDirection = normalize(CameraPosition.xyz - worldPosition);
	}

    return OUT;
}

float4 pixel_shader( VS_OUTPUT IN ) : SV_Target
{
    float4 OUT                      = (float4)0;

    float3 normal                   = normalize(IN.Normal);
    float3 directionalDirection     = normalize(IN.DirectionalDirection.xyz);
	float3 directionalViewDirection = normalize(IN.ViewDirection);
    float n_dot_l                   = dot(directionalDirection, normal);

	float4 color                    = IN.Color;

	//A = la * ma
    float3 ambient              = AmbientColor.rgb * AmbientColor.a * color.rgb;

    float3 diffuse              = (float3)0;
	float3 specular             = (float3)0;
	float3 refVector            = (float3)0;

    if(n_dot_l > 0)
    {
		// D = kd * ld * md
        diffuse = max(n_dot_l, 0) * DirectionalColor.rgb * DirectionalColor.a * color.rgb;

		// R = I - 2(n.I) * n
		refVector = normalize(reflect(directionalDirection, normal));

		// S = max(dot(V.R),0)^P * SpecularColor.rgb * SpecularColor.a * color.rgb;
		specular = pow(max(dot(directionalViewDirection, refVector), 0), SpecularPower.x) * SpecularColor.rgb * SpecularColor.a * color.rgb;
	}

	if (LightType.x == 1.0f)
	{
		float3 lightViewDirection = normalize(IN.LightViewDirection);
		float3 lightDirection     = normalize(IN.LightDirection.xyz);
		float  light_n_dot_l      = dot(lightDirection, normal);
		float3 lightHalfVector    = normalize(lightDirection + lightViewDirection);
		float  light_n_dot_h      = dot(lightHalfVector, normal);

		float4 lightColor         = LightColor;
		float4 lightCoefficients  = lit(light_n_dot_l, light_n_dot_h, SpecularPower.x);

		ambient += get_vector_color_contribution(AmbientColor, lightColor.rgb);
		diffuse += get_vector_color_contribution(lightColor, lightCoefficients.y * lightColor.rgb) * IN.LightDirection.w;
		specular += get_scalar_color_contribution(SpecularColor, min(lightCoefficients.z, lightColor.w)) * IN.LightDirection.w;
	}

	OUT.rgb = ambient + diffuse + specular;
	OUT.a = 1.0f;

	return OUT;
}
