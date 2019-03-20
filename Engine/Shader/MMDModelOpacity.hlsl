#define MMDModelOpacitySignature   "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                    "CBV(b0),"\
                                    "DescriptorTable(CBV(b1,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(CBV(b2,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t0,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t1,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t2,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "StaticSampler(s0 ,"\
                                             "filter = FILTER_MIN_MAG_MIP_LINEAR,"\
                                             "addressU = TEXTURE_ADDRESS_WRAP,"\
                                             "addressV = TEXTURE_ADDRESS_WRAP,"\
                                             "addressW = TEXTURE_ADDRESS_WRAP,"\
                                             "mipLodBias = 0.0f,"\
                                             "maxAnisotropy = 16,"\
                                             "comparisonFunc  = COMPARISON_NEVER,"\
                                             "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK,"\
                                             "minLOD = 0.0f,"\
                                             "maxLOD = 3.402823466e+38f,"\
                                             "space  =  0,"\
                                             "visibility = SHADER_VISIBILITY_ALL"\
                                            ")"\



#include "CommonStruct.hlsl"


cbuffer CameraInfo : register(b0)
{
    Camera camera;
};
cbuffer Tranform : register(b1)
{
    matrix transform;
};
    
cbuffer Material : register(b2)
{
    //拡散反射光 w = alpha
    float4 Diffuse;
	//鏡面反射光	w = pow
    float4 Specular;
	//環境光
    float3 Ambient;
	//自己発光色
    float3 emissive;
	//スフィアマップのブレンディングタイプ
    int sphereBlendType;
}


//
Texture2D<float4> StanderdTex : register(t0);
Texture2D<float4> SphereTex : register(t1);             // MatCapTex
Texture2D<float4> ToonTex : register(t2);

SamplerState Sampler : register(s0);



struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct VSOutput
{
    float4 svPos : SV_POSITION;
    float3 rawPos : RAW_POSITION;
    float4 normal : NORMAL;
    float2 texCoord : TEXCOORD;

};

struct PSOutput
{
    float4 normalMap : SV_Target0;
    float4 specularPowerMap : SV_Target1;
    float4 albedoMap : SV_Target2;
};

[RootSignature(MMDModelOpacitySignature)]
VSOutput VsMain(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    output.rawPos = input.pos;
    output.svPos = mul(camera.Proj, mul(camera.View, mul(transform,float4(input.pos, 1.0f))));

    matrix rotation = transform;
    matrix scaleInv = 0;
    rotation._m03_m13_m23 = 0;

    float sx = length(float3(transform._11_21_31));;
    float sy = length(float3(transform._12_22_32));;
    float sz = length(float3(transform._13_23_33));;

    scaleInv._11 = rcp(sx);
    scaleInv._22 = rcp(sy);
    scaleInv._33 = rcp(sz);
    scaleInv._44 = 1.0f;
    matrix rot = mul(scaleInv, rotation);

    output.normal = mul(rot, float4(normalize(input.normal), 0.0f));
    output.texCoord = input.texCoord;
    return output;
}

PSOutput PsMain(VSOutput input)
{
    PSOutput output = (PSOutput) 0;
    float4 standardTexColor =   StanderdTex.Sample(Sampler, input.texCoord);

    float4 viewNorm = float4(0.5 * (mul(camera.View, input.normal) + 1.0f)); //法線パッキング

    output.normalMap = float4(viewNorm.xyz, 1.0f);
    output.specularPowerMap = float4(Specular.x, (Specular.w / 256.0f), emissive.x, 1.0f);
    output.albedoMap = float4(standardTexColor.rgb, standardTexColor.a);

    return output;
}