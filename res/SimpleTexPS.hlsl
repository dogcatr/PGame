//-----------------------------------------------------------------------------
// File : SimplePS.hlsl
// Desc : Simple Pixel Shader.
// Copyright(c) Pocol. All right reserved.
//-----------------------------------------------------------------------------

//以下はサンプルプログラム5.1を改造したもの

struct VSOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float4 WorldPos : WORLD_POS;
    //chg1
    float3 LightPosition : LIGHT_POS;
    float3 LightColor : LIGHT_COLOR;
    float4 CameraPos : CAMERA_POS;
};

struct PSOutput
{
    float4  Color : SV_TARGET0;
};


SamplerState ColorSmp : register( s0 );
Texture2D    ColorMap : register( t0 );



PSOutput main(VSOutput input)
{
    PSOutput output =(PSOutput)0;
    
    float3 LightPosition = input.LightPosition;
    float3 LightColor = input.LightColor;
    float3 Diffuse = { 0.9f,0.9f,0.9f };
    float Alpha = 0.99f;//透過度
    float3 Specular = { 0.6f,0.6f,0.6f };//鏡面反射率
    float Shininess = 100.0f;//鏡面反射強度

    //float4 CameraPosition = { 0.0f, 1.0f, 2.0f, 0.0f };
    float4 CameraPosition = input.CameraPos;

    float3 N = normalize(input.Normal);
    float3 L = normalize(LightPosition - input.WorldPos.xyz);
    float4 preV = CameraPosition - input.WorldPos;
    float3 V = normalize(preV.xyz);
    float3 R = normalize(-reflect(V, N));

    float4 color = ColorMap.Sample(ColorSmp, frac(input.TexCoord * 5));
    float3 diffuse = LightColor * Diffuse * saturate(dot(L, N));
    float3 specular = LightColor * Specular * pow(saturate(dot(L, R)), Shininess);

    //output.Color = float4(color.rgb * (diffuse+specular), color.a * Alpha);
    float3 weakl = { 0.01f, 0.01f, 0.01f };
    output.Color = float4(color.rgb * (diffuse + specular + weakl), color.a * Alpha);
    //chg

    return output;
}