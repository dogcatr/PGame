//-----------------------------------------------------------------------------
// File : SimpleVS.hlsl
// Desc : Simple Vertex Shader.
// Copyright(c) Pocol. All right reserved.
//-----------------------------------------------------------------------------

//以下はサンプルプログラム5.1を改造したもの

struct VSInput
{
    float3  Position : POSITION;
    float3  Normal   : NORMAL;
    float2  TexCoord : TEXCOORD;
    float3  Tangent  : TANGENT;
};

struct VSOutput
{
    float4  Position : SV_POSITION;
    float2  TexCoord : TEXCOORD;
    float3 Normal: NORMAL;
    float4 WorldPos: WORLD_POS;
    
    float3 LightPosition : LIGHT_POS;
    float3 LightColor : LIGHT_COLOR;
    float4 CameraPos : CAMERA_POS;
};

cbuffer Transform : register( b0 )
{
    float4x4 World : packoffset( c0 );
    float4x4 View  : packoffset( c4 );
    float4x4 Proj  : packoffset( c8 );
    
    //float3 LightPosition : packoffset(c12);
    //float3 LightColor : packoffset(c13);
    float4x4 Light : packoffset(c12);
}

VSOutput main( VSInput input )
{
    VSOutput output = (VSOutput)0;

    float4 localPos = float4( input.Position, 1.0f );
    float4 worldPos = mul( World, localPos );
    float4 viewPos  = mul( View,  worldPos );
    float4 projPos  = mul( Proj,  viewPos );

    output.Position = projPos;
    output.TexCoord = input.TexCoord;
    output.WorldPos = worldPos;
    output.Normal = normalize(mul((float3x3)World, input.Normal));
    
    //output.LightPosition = LightPosition;
    //output.LightColor = LightColor;
    
    //output.LightPosition = Light * unit_x();
    //output.LightColor = Light * unit_y();
    
    output.LightPosition = (float3)mul(Light, float4(1.0f, 0.0f, 0.0f, 0.0f));
    output.LightColor = (float3)mul(Light, float4(0.0f, 1.0f, 0.0f, 0.0f)); //4を3にしなくていい？
    output.CameraPos = mul(Light, float4(0.0f, 0.0f, 1.0f, 0.0f));

    return output;
}