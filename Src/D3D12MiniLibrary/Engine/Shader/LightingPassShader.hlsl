
#define ComputeRootSignature   "RootFlags(0),"\
                                    "CBV(b0),"\
                                    "DescriptorTable(SRV(t0,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t1,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t2,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t3,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(SRV(t4,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\
                                    "DescriptorTable(UAV(u0,numDescriptors = 1,space = 0)," \
                                                        "visibility = SHADER_VISIBILITY_ALL),"\

struct SurfaceData
{
    float3 posView;
    float3 normalView;
    float4 albedo;
    float specular;
    float depth;
};

struct LightParamater
{
    float3 color;
    float3 pos;
    float3 direction;
    float range;
    float attenuation;
    float intensity;
    int type; // 0 = point,1 = Direction
};

cbuffer CameraData : register(b0)
{
    matrix View;
    matrix Proj;
    float windowWidth;
    float windowHeight;
}

Texture2D<float4> NormalMapTexture : register(t0);
Texture2D<float4> SpecularPowerMapTexture : register(t1);
Texture2D<float4> AlbeboMapTexture : register(t2);
Texture2D<float> DepthTexture : register(t3);
StructuredBuffer<LightParamater> Lights : register(t4);

RWTexture2D<float4> ResultTexture : register(u0);

#define threadX  16
#define threadY  16
#define threadDimension 16
#define ComputeTileSize (threadX*threadY)
#define LightNumMax 1024                        //ライトを1024までサポート
#define Roughness 0.5f //仮のラフネス値

groupshared uint sMinZ; //タイルの最小震度
groupshared uint sMaxZ; //タイルの最大深度
groupshared uint sTileLightIndices[LightNumMax]; //タイルが接触しているライトのインデックス
groupshared uint sTileNumLights;


void GetTileFrustumPlane(out float4 frustumPlanes[6], uint3 groupId, float screenWidth, float screenHeight)
{
	// タイルの最大・最小深度を浮動小数点に変換
    float minTileZ = asfloat(sMinZ);
    float maxTileZ = asfloat(sMaxZ);

    float width = screenWidth;
    float height = screenHeight;
    float2 screenSize = float2(width, height);

    float2 tileScale = screenSize * rcp(float(2 * threadDimension));
    float2 tileBias = tileScale - groupId.xy;

    float4 c1 = float4(Proj._11 * tileScale.x, 0.0, tileBias.x, 0.0);
    float4 c2 = float4(0.0, -Proj._22 * tileScale.y, tileBias.y, 0.0);
    float4 c4 = float4(0.0, 0.0, 1.0, 0.0);

    frustumPlanes[0] = c4 - c1; // 右平面
    frustumPlanes[1] = c1; // 左平面
    frustumPlanes[2] = c4 - c2; // 上平面
    frustumPlanes[3] = c2; // 底平面
    //ニア・ファーの平面
    frustumPlanes[4] = float4(0.0, 0.0, 1.0, -minTileZ);

    frustumPlanes[5] = float4(0.0, 0.0, -1.0, maxTileZ);

	// 法線が正規化されていない4面についてだけ正規化する
	[unroll(4)]
    for (uint i = 0; i < 4; ++i)
    {
        frustumPlanes[i] *= rcp(length(frustumPlanes[i].xyz));
    }

}

//! サーフェイス情報を取得する
SurfaceData GetSurfaceData(uint2 uv)
{
    float depth = DepthTexture[uv];
    float4 albedo = AlbeboMapTexture[uv];
    float4 specPower = SpecularPowerMapTexture[uv];

	// ビュー空間での座標を求める
    float4 norm = (2.0f * NormalMapTexture.Load(int3(uv.x, uv.y, 0))) - 1.0f; //法線のデコード
    
    float sqrZ = 0.0f;
    float sqrtValue = 1.0f - (norm.x * norm.x) - (norm.y * norm.y);
    sqrZ = sqrt(sqrtValue);
    float3 normal = float3(norm.x, norm.y, norm.z); //Z軸ベクトルの算出
    float screenSopaceDepth = 0;
    //深さ
    screenSopaceDepth = Proj._34 / (depth - Proj._33);

    float2 gbufferDim = float2(0.0f, 0.0f);
    uint dummy = 0;
    uint dummy2 = 0;
    NormalMapTexture.GetDimensions(dummy2, gbufferDim.x, gbufferDim.y, dummy);

    float2 screenPixelOffset = (float2(2.0f, -2.0f)) / gbufferDim;
    float2 positionScreen = (float2(uv) + float2(0.5f, 0.5f)) * screenPixelOffset.xy + float2(-1.0f, 1.0f);
    float2 viewRay = positionScreen.xy / float2(Proj._11, Proj._22);
    float3 pos = (float3) 0;

    pos.z = screenSopaceDepth;
    pos.xy = viewRay.xy * screenSopaceDepth;

    if (depth == 1.0f)
    {
    }
    SurfaceData ret =
    {
        pos,
	    normal,
	    albedo.xyzw,
		specPower.y,
        depth
    };
    return ret;
}

float GGX(float3 H, float3 N, float inRoughness)
{
    float NdotH = saturate(dot(H, N));
    float roughness = saturate(inRoughness);
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float t = ((NdotH * NdotH) * (alpha2 - 1.0) + 1.0);
    float PI = 3.1415926535897;
    return alpha2 / (PI * t * t);
}

// フレネルの項
float Flesnel(float3 V, float3 H, float f0)
{
    float VdotH = saturate(dot(V, H));
    float F0 = saturate(f0);
    float F = pow(1.0 - VdotH, 5.0);
    F *= (1.0 - F0);
    F += F0;
    return F;
}

float CookTorrance(float3 L, float3 V, float3 H, float3 N)
{
    float nh = saturate(dot(N, H));
    float nl = saturate(dot(N, L));
    float nv = saturate(dot(N, V));
    float vh = saturate(dot(V, H));

    float nh2 = 2.0 * nh;
    float g1 = (nh2 * nv) / vh;
    float g2 = (nh2 * nl) / vh;
    float G = min(1.0, min(g1, g2));
    return G;
}

// 点光源
float4 CalcPointLight(float3 objPos, float3 objNormal, float3 litPos, float3 litColor, float range, float litAttn)
{
    float3 eyeVec = -normalize(objPos);

	// 減衰計算
    float3 litDir = litPos.xyz - objPos;
    float litLen = length(litDir);
    float litRate = litLen / range;
    float attn = max(1.0 - litRate * litRate, 0.0);
    if (attn == 0.0)
    {
        return float4(0, 0, 0, 0);
    }
    litDir = normalize(litDir);

	// ライト計算
    float nl = saturate(dot(objNormal, litDir));
    float3 halfVec = normalize(eyeVec + litDir);
    float nh = saturate(dot(objNormal, halfVec));
    float power = litAttn;
    float spec = 0.0;
    if (power > 0.0)
    {
        spec = pow(nh, power) * nl * attn;
    }

	// 出力
    return float4(litColor.rgb * nl * attn, spec);
}
//ポイントライトの双方向反射率分布関数
float4 CalcPointLightBRDF(float3 pos, float3 normal, float3 lightPos, float3 lightColor, float range, float lightAttenuation, float gross, float f0)
{
    float3 viewDir = normalize(-pos);

    //減衰計算
    float3 lightDir = lightPos - pos;
    float lightLength = length(lightDir);
    float lightRatio = lightLength / (range);
    float attenuation = max(1.0f - (lightRatio * lightRatio), 0.0f);
    if (attenuation == 0.0f)
    {
        float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    lightDir = normalize(lightDir);
    //ライト計算
    float nl = dot(normal, lightDir);
    float3 halfVec = normalize(viewDir + lightDir);
    float nh = dot(normal, halfVec);
    float power = 0.1f;
    float spec = 0.0f;
    if (power > 0.0f)
    {
        float nv = dot(normal, viewDir);
        float nlnv = nl * nv;
        float F = Flesnel(viewDir, halfVec, f0);
        float G = CookTorrance(lightDir, viewDir, halfVec, normal);
        float D = GGX(halfVec, normal, gross);
        spec = max(F * G * D / (4.0 * nlnv), 0.0);

    }

    return float4(lightColor.rgb * nl * attenuation, spec * attenuation * attenuation);

}

float4 CalcDirectionalLightRim(float surfaceDepth, float3 objPos, float3 objNormal, float3 litDir, float3 rimColor, float rimPower)
{

    if (surfaceDepth >= 1.0f)
    {
        return float4(0.0f, 0.0f, 0.0f, 0.0f);
    }
    float3 eyeVec = normalize(-objPos);
    litDir *= -1;

	// ライト計算
    float nl = saturate(dot(objNormal, litDir));
    float3 halfVec = normalize(eyeVec + litDir);
    float nh = 1.0f - saturate(dot(objNormal, eyeVec));

    float power = pow(max(0.0f, nh), rimPower);
	// 出力
    return float4(saturate(rimColor * float3(1.0f, 1.0f, 1.0f) * power), 0.0f);
}

float4 CalcDirectionalLight(float3 objPos, float3 objNormal, float3 litDir, float3 litColor, float litPower)
{
    float3 eyeVec = -normalize(objPos);

	// ライト計算
    float nl = saturate(dot(objNormal, litDir));
    float3 halfVec = normalize(eyeVec + litDir);
    float nh = saturate(dot(objNormal, halfVec));
    float power = litPower;
    float spec = 0.0f;
    if (power > 0.0f)
    {
        spec = pow(nh, power) * nl;
    }

	// 出力
    return float4(litColor.rgb * nl, spec);
}

float4 CalcDirectionalLightBRDF(float3 objPos, float3 objNormal, float3 litDir, float3 litColor, float litPower, float gloss, float f0)
{
    float3 eyeVec = normalize(-objPos);
    litDir *= -1;

	// ライト計算
    float nl = saturate(dot(objNormal, litDir));
    float3 halfVec = normalize(eyeVec + litDir);
    float nh = saturate(dot(objNormal, halfVec));
    float power = litPower;
    float spec = 0.0f;
    if (power > 0.0f)
    {
        float nv = saturate(dot(objNormal, eyeVec));
        float sn = pow(2.0, 13.0 * gloss) * power;
        float D = (sn + 2.0) * pow(nh, sn) / (2.0 * 3.1415926);
        float F = f0 + (1.0 - f0) * pow((1.0 - nv), 5.0);
        float dv = saturate(dot(eyeVec, (eyeVec + litDir)));
        float V = 4.0 / (dv * dv);
        float nl_nv = nl * nv;
        float G = V * nl_nv;
        spec = max(F * G * D / (4.0 * nl_nv), 0.0f);
    }

	// 出力
    return (float4(litColor.rgb * nl, spec));
}



[RootSignature(ComputeRootSignature)]
[numthreads(threadX, threadY, 1)]
void CSMain(uint3 groupeID : SV_GroupID, uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID)
{
    uint groupIndex = groupThreadID.y * threadDimension + groupThreadID.x;
    uint2 globalIndex = dispatchThreadID.xy;

    //共有メモリの初期化
    if (groupIndex == 0)
    {
        sTileNumLights = 0;
        sMinZ = 0x7F7FFFFF; // floatの最大値
        sMaxZ = 0;
    }

    //最小距離（プロジェクション行列から取得）
    float minZSample = -Proj._34 / (Proj._33 - 1.0f);
    //最大距離（プロジェクション行列から取得）
    float maxZSample = -Proj._34 / Proj._33;

    //サーフェス（サンプリングしたテクセルの情報）を取得
    SurfaceData surfaceData = GetSurfaceData(globalIndex);

    bool validPixel =
                 surfaceData.posView.z >= -Proj._34 / Proj._33 &&
                 surfaceData.posView.z <= -Proj._34 / (Proj._33 - 1.0f);
    [flatten]
    if (validPixel)
    {
        minZSample = min(minZSample, surfaceData.posView.z);
        maxZSample = max(maxZSample, surfaceData.posView.z);
    }

    //ビュースペースでの深度
    //初期化同期
    GroupMemoryBarrierWithGroupSync();
    //同期処理

    if (maxZSample >= minZSample)
    {
        InterlockedMin(sMinZ, asuint(minZSample));
        InterlockedMax(sMaxZ, asuint(maxZSample));
    }

    //同期処理            
    GroupMemoryBarrierWithGroupSync();


    float2 gbufferDim = float2(0, 0);
    uint dummy = 0;
    uint dummy2 = 0;
    NormalMapTexture.GetDimensions(dummy2, gbufferDim.x, gbufferDim.y, dummy);

    float4 frustumPlanes[6];
    GetTileFrustumPlane(frustumPlanes, groupeID, gbufferDim.x, gbufferDim.y);
   
    //ライトの総数およびストライド数（ストライドはダミー）
    //GetDimensionでライトの数とライトのストライド値を取得。
    uint strid = 0;
    uint lightNum = 0;
    Lights.GetDimensions(lightNum, strid);
   
    //ライトカリング
    {
        uint threadCount = ComputeTileSize;
        uint passCount = (lightNum + threadCount - 1) / threadCount;

        for (uint passIt = 0; passIt < passCount; ++passIt)
        {
            uint lightIndex = passIt * threadCount + groupIndex;
            lightIndex = min(lightIndex, lightNum);
            
            //ライトの取得
            LightParamater light = Lights[lightIndex];

            //ライトをカメラ原点のビュー座標にもっていく
            light.pos = mul(View, float4(light.pos.xyz, 1.0)).xyz;

            //フラスタムとの当たり判定の結果
            bool inFrustum = true;

            //実行を停止するまでFor文を展開
            [unroll]
            for (uint i = 0; i < 6; ++i)
            {
                float4 lp = float4(light.pos.xyz, 1.0);
                float d = dot(frustumPlanes[i], lp);

                //もし平行光源ライト（サンライト）ならば
                if (light.type == 1)
                {
                    //無条件でOK
                    inFrustum = true;
                    break;
                }
                inFrustum = inFrustum && (d >= -light.range);
            }

            if (inFrustum)
            {
                uint offset = 0;
                InterlockedAdd(sTileNumLights, 1, offset);
                sTileLightIndices[offset] = lightIndex;
            }
        }
    }
    //ここでスレッド間で同期をとるので、タイルと衝突しているライトの検出が可能になっているはず
    GroupMemoryBarrierWithGroupSync();

    //ライトインデックスを出力バッファに出力
    //ライティング

    float3 diffuse = (float3) 0.0;
    float3 rim = (float3) 0.0;
    float specular = 0.0f;
    float4 result;


    for (uint i = 0; i < sTileNumLights; ++i)
    {
        uint lightIndex = sTileLightIndices[i];
        LightParamater light = Lights[lightIndex];
        float attenuation = light.attenuation;
        if (surfaceData.specular <= 0.0f)
        {
            light.attenuation = 0.0f;
        }

        //ポジションをカメラ空間にもっていく
        float3 lightViewPos = mul(View, float4(light.pos.xyz, 1.0f)).xyz;
        //ベクトルをview空間にもっていく
        float3 lightViewDirection = mul(View, float4(light.direction.xyz, 0.0f)).xyz;

        //タイプで分岐
        if (light.type == 0)
        {
            result = CalcPointLightBRDF(surfaceData.posView, surfaceData.normalView, lightViewPos, light.color, light.range, light.attenuation, 0.5f, 0.9f);
        }
        else
        {
            result = CalcDirectionalLight(surfaceData.posView, surfaceData.normalView, lightViewDirection, light.color, light.attenuation);

        }
        //rim += rimResult.rgb;
        diffuse = diffuse + result.rgb;
        specular = specular + result.a;

    }

    //適当アンビエント
    float4 ambient = surfaceData.albedo * 0.01f;
    float4 color = float4(saturate((diffuse) * surfaceData.albedo.rgb + ambient.rgb + specular), surfaceData.albedo.w);
    
    ResultTexture[globalIndex] = color;

}
    