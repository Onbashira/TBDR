#define SpriteRootSignature   "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                                    "CBV(b0),"\
                                    "DescriptorTable(CBV(b1,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(CBV(b2,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t0,numDescriptors = 1,space = 0)," \
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

struct SpriteVSInput
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
};

struct SpriteVSOutput
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

struct SpriteRenderInfo
{
    float alpha;
    float2 centerOffset;
    float2 topLeft;
    float2 bottomRight;
};

struct SpritePSOutput
{
    float4 color : SV_TARGET0;
};

cbuffer CameraInfo : register(b0)
{
    Camera camera;
};

cbuffer Transform : register(b1)
{
    matrix transform;
};

cbuffer SpriteInfo : register(b2)
{
    SpriteRenderInfo info;
};

SamplerState spriteSampler : register(s0);

Texture2D<float4> spriteTexture : register(t0);

[RootSignature(SpriteRootSignature)]
SpriteVSOutput VsMain(SpriteVSInput input)
{
    SpriteVSOutput output = (SpriteVSOutput) 0;

    input.pos.z = 0.0f;
    output.pos = mul(camera.Proj, mul(camera.View, mul(transform, float4(input.pos, 1.0f))));

    float3 tempPos = (input.pos / float3(camera.windowWidth, camera.windowHeight, 1.0f));

    matrix rotation = transform;
    rotation._m03_m13_m23 = 0;
    matrix scaleInv = 0;

    float sx = length(float3(transform._11_21_31));;
    float sy = length(float3(transform._12_22_32));;
    float sz = length(float3(transform._13_23_33));;

    scaleInv._11 = rcp(sx);
    scaleInv._22 = rcp(sy);
    scaleInv._33 = rcp(sz);
    scaleInv._44 = 1.0f;


    rotation._m03_m13_m23 = 0;
    matrix rot = mul(scaleInv, rotation);
    output.texCoord = input.texCoord;
    return output;
}

SpritePSOutput PsMain(SpriteVSOutput input)
{
    SpritePSOutput output = (SpritePSOutput) 0;

    output.color = spriteTexture.Sample(spriteSampler, input.texCoord);
    output.color.a *= info.alpha;
    return output;

}