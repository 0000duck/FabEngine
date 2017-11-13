cbuffer FrameConstantBuffer : register( b0 )
{
    matrix View;
    matrix Projection;
}

cbuffer ObjectConstantBuffer : register( b1 )
{
    matrix World;
}

struct V2P
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR0;
};

V2P vertex_shader( float4 Position : POSITION, float4 Color : COLOR )
{
    V2P output = (V2P)0;
    output.Position = mul( Position, World );
    output.Position = mul( output.Position, View );
    output.Position = mul( output.Position, Projection );
    output.Color = Color;
    return output;
}

float4 pixel_shader( V2P input ) : SV_Target
{
    return input.Color;
}
