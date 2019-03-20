#define FXAARootSignature   "RootFlags(0),"\
                            "CBV(b0),"\
                            "DescriotorTable(SRV(t0,numDescriptors = 1, space = 0)," \
                                             "visibility = SHADER_VISIBILITY_ALL),"  \



#define TileSize 16
//これらは今回使うゲームですでに決まっているためこっちで定義しても構わないはず。CBV転送するのが面倒
#define WindowWith 1920
#define WindowHeight 1080

#define TestWindowWith 1280
#define TestWindowHeight 720


#ifndef FXAA_PRESET
#define FXAA_PRESET 3
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 0)
#define FXAA_EDGE_THRESHOLD      (1.0/4.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/12.0)
#define FXAA_SEARCH_STEPS        2
#define FXAA_SEARCH_ACCELERATION 4
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       1
#define FXAA_SUBPIX_CAP          (2.0/3.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 1)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/16.0)
#define FXAA_SEARCH_STEPS        4
#define FXAA_SEARCH_ACCELERATION 3
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 2)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        8
#define FXAA_SEARCH_ACCELERATION 2
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 3)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        16
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 4)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        24
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif
/*--------------------------------------------------------------------------*/
#if (FXAA_PRESET == 5)
#define FXAA_EDGE_THRESHOLD      (1.0/8.0)
#define FXAA_EDGE_THRESHOLD_MIN  (1.0/24.0)
#define FXAA_SEARCH_STEPS        32
#define FXAA_SEARCH_ACCELERATION 1
#define FXAA_SEARCH_THRESHOLD    (1.0/4.0)
#define FXAA_SUBPIX              1
#define FXAA_SUBPIX_FASTER       0
#define FXAA_SUBPIX_CAP          (3.0/4.0)
#define FXAA_SUBPIX_TRIM         (1.0/4.0)
#endif

#define FXAA_SUBPIX_TRIM_SCALE (1.0/(1.0 - FXAA_SUBPIX_TRIM))

Texture2D<float4>   InputTexture    : register(t0);
SamplerState        textureSampler  : register(s0);
//rgb入力からルミナ、ルミナンスの推定値を返却する
//FxaaGetLuma（）の範囲は0.0から2.963210702です。
float FxaaGetLuma(float3 rgb)
{
    return rgb.y * (0.587f / 0.299f) + rgb.x;
}

float FxaaLuminance(float4 rgba)
{
    //この場合、ルミナンス値をｙ値とする
    return rgba.y;
}

//補間
float3 FxaaLeapFloat3(float3 a, float3 b, float amount)
{
    return (float3(-amount, -amount, -amount) * b) + ((a * float3(-amount, -amount, -amount)) + b);
}

float3 FxaaFilterReturn(float3 rgb)
{
    return rgb;
}

float4 FxaaTexLod0(float2 pos)
{
    return InputTexture.SampleLevel(textureSampler, pos.xy, 0.0f);
}

float4 FxaaTexGrad(float2 pos, float2 grad)
{
    return InputTexture.SampleGrad(textureSampler, pos.xy, grad, grad);
}

float4 FxaaTexOffset(float2 pos, int2 offset, float2 rcpFrame)
{
    return InputTexture.SampleLevel(textureSampler, pos.xy, 0.0f,offset);
}

struct VSInput
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VSOutput
{
    float2 svPos : SV_POSITION;
    float2 screenPos : CONVERTED_POSITOIN;
    float2 texCoord : TEXCOORD;
};

struct PSOutput
{
    float4 result : SV_Target0;
};

struct FxaaTexture
{

};

[RootSignature(FXAARootSignature)]
VSOutput FxaaVSMain(VSInput input)
{
    VSOutput output;
    output.svPos = float2(input.pos.x * 0.5 + 0.5f, input.pos.y * 0.5f + 0.5f);
    output.texCoord = input.texCoord;
    return output;
};

[RootSignature(FXAARootSignature)]
PSOutput FxaaPSMain(VSOutput input)
{
    PSOutput output;
    float2 rcpFrame = { 1.0f / TestWindowWith, 1.0f / TestWindowHeight };

    float3 rgbN = FxaaTexOffset(input.screenPos.xy, int2(0, -1), rcpFrame).xyz;
    float3 rgbW = FxaaTexOffset(input.screenPos.xy, int2(-1, 0), rcpFrame).xyz;
    float3 rgbM = FxaaTexOffset(input.screenPos.xy, int2(0, 0), rcpFrame).xyz;
    float3 rgbE = FxaaTexOffset(input.screenPos.xy, int2(1, 0), rcpFrame).xyz;
    float3 rgbS = FxaaTexOffset(input.screenPos.xy, int2(0, 1), rcpFrame).xyz;

    float lumaN = FxaaGetLuma(rgbN);
    float lumaW = FxaaGetLuma(rgbW);
    float lumaM = FxaaGetLuma(rgbM);
    float lumaE = FxaaGetLuma(rgbE);
    float lumaS = FxaaGetLuma(rgbS);

    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));

    float range = rangeMax - rangeMin;

    if (range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD))
    {
        output.result = float4(FxaaFilterReturn(rgbM),1.0f);
        return output;
    }
    float3 rgbL;
    if (FXAA_SUBPIX > 0)
    {
#if FXAA_SUBPIX_FASTER
            float3 rgbL = (rgbN + rgbW + rgbE + rgbS + rgbM) * float3(1.0/5.0);
#else
        rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;
#endif
    }
    //
    //４近傍ローパス計算
    //

    float lumaL = 0.0f;
    float rangeL = 0.0f;
    float blendL = 0.0f;

    if (FXAA_SUBPIX != 0)
    {
        lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25f; //(N+W+E+S)/4
        rangeL = abs(lumaL - lumaM);

    }

    if (FXAA_SUBPIX == 1)
    {
        blendL = max(0.0f, (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
        blendL = min(blendL, FXAA_SUBPIX_CAP);
    }

    if (FXAA_SUBPIX == 2)
    {
        blendL = rangeL / range;
    }

    //if (FXAA_DEBUG_PASSTHROUGH)
    //{
    //}

    //
    //水平・垂直サーチ
    //
    float3 rgbNW = FxaaTexOffset(input.screenPos.xy, int2(-1, -1), rcpFrame).xyz;
    float3 rgbNE = FxaaTexOffset(input.screenPos.xy, int2(1, -1), rcpFrame).xyz;
    float3 rgbSW = FxaaTexOffset(input.screenPos.xy, int2(-1, 1), rcpFrame).xyz;
    float3 rgbSE = FxaaTexOffset(input.screenPos.xy, int2(1, 1), rcpFrame).xyz;
    
    if ((FXAA_SUBPIX_FASTER == 0) && (FXAA_SUBPIX > 0))
    {
        rgbL += (rgbNW + rgbNE + rgbSW + rgbSE);
        rgbL *= float3(1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f);
    }

    float lumaNW = FxaaGetLuma(rgbNW);
    float lumaNE = FxaaGetLuma(rgbNE);
    float lumaSW = FxaaGetLuma(rgbSW);
    float lumaSE = FxaaGetLuma(rgbSE);

    float edgeVertical = abs((0.25f * lumaNW) + (-0.5f * lumaN) + (0.25f * lumaNE)) +
                         abs((0.50f * lumaW) + (-1.0f * lumaM) + (0.5f * lumaE)) +
                         abs((0.25f * lumaSW) + (-0.5f * lumaS) + (0.25f * lumaSE));

    float edgeHorizon = abs((0.25f * lumaNW) + (-0.5f * lumaW) + (0.25f * lumaSW)) +
                        abs((0.50f * lumaW) + (-1.0f * lumaM) + (0.5f * lumaE)) +
                        abs((0.25f * lumaNE) + (-0.5f * lumaE) + (0.25f * lumaSE));

    bool horizonSpan = edgeHorizon >= edgeVertical;

    float lengthSign = horizonSpan ? -rcpFrame.y : -rcpFrame.x;

    if (!(horizonSpan))
    {
        lumaN = lumaW;
        lumaS = lumaE;
    }

    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);

    lumaN = (lumaN + lumaM) * 0.5f;
    lumaS = (lumaS + lumaM) * 0.5f;

    //
    //グレードが高い隣接ピクセルの選択ステップ
    //

    bool pairN = gradientN >= gradientS;

    if (!pairN)
    {
        lumaN = lumaS;
        gradientN = gradientS;
        lengthSign *= -1.0f;
    }
    float2 posN;
    posN.x = input.screenPos.x + (horizonSpan ? 0.0f : lengthSign*0.5f);
    posN.y = input.screenPos.y + (horizonSpan ? lengthSign * 0.5f : 0.0f);

    //
    //局所勾配　限界値の検索　再設定
    //

    gradientN *= FXAA_SEARCH_THRESHOLD;
    
    //
    //ルミナンスのペアの平均距離が範囲外になるまでの両方向の検索腕伸ばし
    //

    float2 posP = posN;
    float2 offsetNP = horizonSpan ? float2(rcpFrame.x,0.0f) : float2(0.0f,rcpFrame.y);

    float lumaEndN = lumaN;
    float lumaEndP = lumaN;

    bool doneN = false;
    bool doneP = false;

#if(FXAA_SEARCH_ACCELERATION == 1)
    posN += offsetNP * float2(-1.0, -1.0);
    posP += offsetNP * float2(1.0, 1.0);

#endif
    
#if(FXAA_SEARCH_ACCELERATION == 2)
    posN += offsetNP * float2(-1.5);
    posP += offsetNP * float2(1.0);
    offNP *= float(2.0);
#endif
    
#if(FXAA_SEARCH_ACCELERATION == 3)
    posN += offsetNP * float2(-2.0);
    posP += offsetNP * float2(2.0);
     offNP *= float(3.0);
#endif

#if(FXAA_SEARCH_ACCELERATION == 4)
    posN += offsetNP * float2(-2.5);
    posP += offsetNP * float2(2.5);
     offNP *= float(4.0);
#endif

    for (int i = 0; i < FXAA_SEARCH_STEPS;i++)
    {
        #if FXAA_SEARCH_ACCELERATION == 1
        if (!doneN)
        {
            lumaEndN = FxaaGetLuma(FxaaTexLod0(posN.xy).xyz);
        }
        if (!doneP)
        {
            lumaEndP = FxaaGetLuma(FxaaTexLod0(posP.xy).xyz);
        }
        #else
        if (!doneN)
        {
            lumaEndN = FxaaGetLuma(FxaaTexGrad(posN.xy).xyz);
        }
        if (!doneP)
        {
            lumaEndP = FxaaGetLuma(FxaaTexGrad(posP.xy).xyz);
        }
        #endif
        if (doneN &&doneP)
        {
            break;
        }
        if (!doneN)
        {
            posN -= offsetNP;
        }
        if (!doneP)
        {
            posP -= offsetNP;
        }
    }

    //
    //ピクセルがスパンのネガティブ側かポジティブ側のどちらにあるのかを判定するステップ
    //もしもdstN<dstPならばネガティブ側にあるといえる
    //

    float dstN = horizonSpan ? input.screenPos.x - posN.x : input.screenPos.y - posN.y;
    float dstP = horizonSpan ? posP.x - input.screenPos.x : posP.y - input.screenPos.y;

    bool directionN = dstN < dstP;

    //
    //  ピクセルがフィルタリングされているかどうかを確認するステップ；
    //

    if (((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) 
        lengthSign = 0.0;

    //
    //サブピクセルのオフセットとフィルタスパンの計算
    //双一次変換したやつをつかう
    //

    float spanLength = dstP + dstN;
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = (0.5f + (dstN * (-1.0f / spanLength))) * lengthSign;

    float3 rgbF = FxaaTexLod0(float2(input.screenPos.x + (horizonSpan ? 0.0f : subPixelOffset), input.screenPos.y + (horizonSpan ? subPixelOffset : 0.0f))).xyz;

    #if FXAA_SUBPIX == 0
    output.result = FxaaFilterReturn(rgbF);
    #else
    output.result = FxaaFilterReturn(FxaaLeapFloat3(rgbL,rgbF,blendL));
    #endif

    return output;
};