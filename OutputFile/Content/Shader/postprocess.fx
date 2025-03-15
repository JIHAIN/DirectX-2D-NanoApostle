#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"
#include "func.fx"

// Vertex Shader 
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// Mesh : RectMesh

VS_OUT VS_Post(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Post(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
            
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);    
    float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;    
    vColor.rgb = Aver;
        
    return vColor;
}



// =================
// Distortion Shader
// =================
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
        
    return output;
}

// 1. �ȼ����̴��� �ȼ� ��ǥ
// 2. ���� Ÿ�� �ػ�

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // SV_Position -> Pixelshader (�ȼ���ǥ)
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;    
    
    // NoiseTexture �� ������
    if(g_btex_1)
    {
        float4 vNoise = g_tex_1.Sample(g_sam_0, vScreenUV + 0.05f * g_Time);
        vNoise -= 0.5f;    
        vNoise *= 0.05f;        
        vScreenUV += vNoise.xy;
    }
    
    vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}



struct VS_VortexOut
{
    float4 vPosition : SV_Position;
    float2 vObjectUV : TEXCOORD;
};


VS_VortexOut VS_Vortex(VS_IN _in)
{
    VS_VortexOut output = (VS_VortexOut) 0.f;
            
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
        
    float4 vProjPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWVP);
    vProjPos.xyz = vProjPos.xyz / vProjPos.w;
    
    output.vObjectUV.x = (vProjPos.x + 1.f) / 2.f;
    output.vObjectUV.y = 1.f - ((vProjPos.y + 1.f) / 2.f);
        
    return output;
}


float4 PS_Vortex(VS_VortexOut _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float effectRadius = 0.1f;
    float effectAngle = 1.f * PI;
    
    float2 center = _in.vObjectUV;
    
    float2 uv = (_in.vPosition.xy / g_RenderResolution.xy) - center;
    
    float len = length(uv * float2(g_RenderResolution.x / g_RenderResolution.y, 1.));
    float angle = atan2(uv.y, uv.x) + effectAngle * smoothstep(effectRadius, 0., len);
    float radius = length(uv);

    vColor = g_tex_0.Sample(g_sam_0, float2(radius * cos(angle), radius * sin(angle)) + center);
    
    return vColor;
}

// ==========================
// Blur Shader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : Target
// ===========================
static const float CrossFilter[13] =
{
    0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f,
    1.f,
    0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f
};
static const float Total = 6.2108f;

struct VS_OUT_BLUR
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

VS_OUT_BLUR VS_Blur(VS_IN _in, uint _InstID : SV_InstanceID)
{
    VS_OUT_BLUR output = (VS_OUT_BLUR) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    output.InstID = _InstID;
    
    return output;
}

float4 PS_Blur(VS_OUT_BLUR _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    float2 vUVStep = 1.f / g_RenderResolution;
    vUVStep *= 5.4f;
    
    // �ؽ�ó ���ø� ����ȭ
    float2 baseUV = _in.vUV;
    float2 stepDir = _in.InstID == 0 ? float2(vUVStep.x, 0.f) : float2(0.f, vUVStep.y);
    
    // �߽��� ���� ���
    vColor += g_tex_0.Sample(g_sam_0, baseUV) * CrossFilter[6];
    
    // �������� ���������� ��� (��Ī�� Ȱ��)
    [unroll]
    for (int i = 1; i <= 6; ++i)
    {
        float2 offset = stepDir * i;
        float weight = CrossFilter[6 + i];
        
        // �� ���� ����ġ�� ���� ���� (��Ī)
        vColor += g_tex_0.Sample(g_sam_0, baseUV + offset) * weight;
        vColor += g_tex_0.Sample(g_sam_0, baseUV - offset) * weight;
    }
    
    vColor /= Total;
    return saturate(vColor);
}

VS_OUT VS_EffectMerge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_EffectMerge(VS_OUT _in) : SV_Target
{
    float4 vEffect = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vEffectBlur = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    // HDR ȿ�� ����
    float intensity = 1.2f; // ��� ���� ����
    float4 vBloom = pow(abs(pow(vEffect, 2.1f)) + abs(pow(vEffectBlur * intensity, 2.1f)), 1.f / 2.1f);
    
    return vBloom;
}



VS_OUT VS_Roar(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // ȭ�� ��ü�� �������
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}


float4 PS_Roar(VS_OUT _in) : SV_Target
{
    // ù ��° �ĵ� ȿ����
    float Scale1 = 0.4f;
    float MaxScaleFactor1 = 10.0f;
    float FadeSpeed1 = 1.f;
    float Intensity1 = 1.0f;
    float WaveCount1 = 1.5f;
    float WaveSpeed1 = 5.2f;
    
    // �� ��° �ĵ� ȿ����
    float Scale2 = 0.4f; // �� ���� �ʱ� ũ��
    float MaxScaleFactor2 = 3.f; // �� ū �ִ� ũ��
    float FadeSpeed2 = 0.4f; // �� ���� ���̵� �ƿ�
    float Intensity2 = 0.7f; // �ٸ� ����
    float WaveCount2 = 2.5f; // �� ���� �ݺ�
    float WaveSpeed2 = 1.2f; // �� ���� �ӵ�
    float WaveDelay2 = 0.7f; // ���� ���� �ð�
    
    // ���� ȿ����
    float ShakeAmount = 0.004f;
    float ShakeSpeed = 50.0f;
    
    float2 shakeOffset = float2(sin(g_Time * ShakeSpeed) * ShakeAmount,
                                cos(g_Time * ShakeSpeed * 1.2) * ShakeAmount);
    
    float2 shakenUV = _in.vUV + shakeOffset;
    float4 finalColor = g_tex_0.Sample(g_sam_0, shakenUV);
    
    // ���� ����
    finalColor.rgb *= 1.2f;
    finalColor.r = saturate(finalColor.r * 1.1);
    finalColor.g *= 0.8;
    finalColor.b *= 0.8;
    float3 redTint = float3(0.5, 0.1, 0.1);
    finalColor.rgb = lerp(finalColor.rgb, redTint, 0.2);
    float3 contrast = (finalColor.rgb - 0.5) * 1.5 + 0.5;
    finalColor.rgb = saturate(contrast);
    
  
    // ù ��° �ĵ� ȿ�� ����
    finalColor = ApplyWaveEffect(finalColor, shakenUV, Scale1, MaxScaleFactor1, FadeSpeed1, Intensity1, WaveCount1, WaveSpeed1, 0, g_tex_2);
    
    // �� ��° �ĵ� ȿ�� ����
    finalColor = ApplyWaveEffect(finalColor, shakenUV, Scale2, MaxScaleFactor2, FadeSpeed2, Intensity2, WaveCount2, WaveSpeed2, WaveDelay2, g_tex_2);
    
    return finalColor;
}


VS_OUT VS_ShockWave(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // ȭ�� ��ü�� �������
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}


float4 PS_ShockWave(VS_OUT _in) : SV_Target
{
    //// �Է� UV ��ǥ
    float2 uv = _in.vUV;
    float2 st = uv;
    
    // �ĵ� �߽��� ���� (ȭ�� �߾�)
    float2 WaveCentre = float2(0.5, 0.5);
    // ȭ�� ���� ��� (����/����)
    float ratio = g_RenderResolution.y / g_RenderResolution.x;
    // �߽����� Y��ǥ�� ȭ�� ������ �°� ����
    WaveCentre.y *= ratio;
    
    // �Ÿ� ����� ���� ������ ��
    float downScale = 2;
    // ���� �ȼ��� �ĵ� �߽ɰ��� �Ÿ� ���
    // st.y�� ratio�� ���� ȭ�� ���� �ְ��� ����
    float dist = distance(float2(st.x, st.y * ratio), WaveCentre) * downScale;
    
    // �Ÿ����� �ĵ� ��꿡 ���
    float val = dist;
    // �ĵ� ���� ���� �� (���� Ŭ���� �ĵ��� �� ��������)
    float a = 30.0;
    
    // �ð��� ���� �ĵ� �̵� �ӵ�
    float pulseSpeed = g_Time * 20.f;
    
    // �ĵ��� �ֱ⸦ �����ϴ� �� (���� Ŭ���� �ĵ��� �ֱⰡ ª����)
    float wavePeriod = 30.0;
    // �ڻ��� �Լ��� ����Ͽ� �ֱ����� �ĵ� ����
    // val * wavePeriod: �Ÿ��� ���� �ĵ��� �ֱ� ����
    // - pulseSpeed: �ð��� ���� �ĵ� �̵�
    float cosFuns = cos(val * wavePeriod - pulseSpeed);
    
    // �Ÿ��� ���� �ĵ��� ���踦 ���
    float powFuns = pow(val * 2.f, a);
    // �ĵ��� �ִ� ������ �����ϰ� �ε巴�� ����
    float limtedPowFuns = 0.5 * pow(a / (a + powFuns), 2.0);
    
    // �ĵ��� ��ü���� ���� ����
    float waveStrength = 7.0;
    
    // ���� �ĵ� ȿ�� ���
    float finalRes = smoothstep(0.0, 1.0, limtedPowFuns * cosFuns) * waveStrength;
    
    // UV ��ǥ ��ȯ �� �����ϸ�
    // st * 2.0 - 1.0: UV ��ǥ�� -1~1 ������ ��ȯ
    st = st * 2.0 - 1.0;
    // �ĵ� ȿ���� ���� UV ��ǥ �ְ�
    st *= 1.0 + finalRes * 0.1;
    // �ְ�� UV ��ǥ�� �ٽ� 0~1 ������ ��ȯ
    st = st * 0.5 + 0.5;
    
    // ������ UV ��ǥ�� �ؽ�ó ���ø�
    float4 texCol = g_tex_0.Sample(g_sam_1, st);
    
    // �ĵ� ȿ���� �ؽ�ó ���� ����
    texCol += (texCol * finalRes) / (dist * 15.0);
    
    // ���� ���� ���
    float4 fragColor = texCol;
    
    return fragColor;
    
  
}

float4 PS_HitEffect(VS_OUT _in) : SV_Target
{
  
    float MaxRadius = 0.9; // �ִ� ���� ������ ������
    float Softness = 0.4; // �����ڸ� �ε巯��
    float Intensity = 0.6; // �⺻ ȿ�� ����
    float FadeOutSpeed = 3.0; // ���̵� �ƿ� �ӵ� ���� (���� Ŭ���� �� ������ �����)
    
    float EffectProgress = g_float_3;

    // ���� �ؽ�ó ����
    float4 originalColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // ȭ�� �߾�
    float2 center = float2(0.5, 0.5);
    
    // ���� �ȼ��� ȭ�� �߾Ӱ��� �Ÿ� ���
    float distance = length(_in.vUV - center);
    
    // ���� ������ ���
    float currentRadius = EffectProgress * MaxRadius;
    // ���� ����ũ ����
    float circleMask = 1.0 - smoothstep(currentRadius, currentRadius + Softness, distance);
    // ������ ��������
    float3 redOverlay = float3(0.2, 0.01, 0.01);
    // ���̵� �ƿ� ȿ�� (����)
    float fadeOut = pow(1.0 - EffectProgress, FadeOutSpeed);
    // ���� ���� ���
    float3 finalColor = lerp(redOverlay, originalColor.rgb, circleMask);
    finalColor = lerp(originalColor.rgb, finalColor, Intensity * fadeOut);
    
    return float4(finalColor, originalColor.a);
}

// =================
// GameOver Fade Out
// =================
VS_OUT VS_GameOver(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_GameOver(VS_OUT _in) : SV_Target
{
    float4 originalColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // ���൵
    float fadeProgress = g_float_0;
    
    // ȭ�� �߾���
    float2 center = float2(0.5, 0.5);
    float dist = length(_in.vUV - center);
    
    // �����ڸ�(1.0)���� �߾�(0.0)���� ���� ����ũ ����
    // ���൵�� �����Ҽ��� �߾����� ���̴� ���� �۾���
    float radius = 1.0 - fadeProgress;
    float circle = 1.0 - smoothstep(radius - 0.1, radius, dist);
    
    // �������� ���� �̹��� ����
    float4 finalColor = lerp(float4(0.f, 0.f, 0.f, 1.f), originalColor, circle);
    
    return finalColor;
}

// =================
// Fade Effect
// =================
float4 PS_FadeOut(VS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // g_float_0�� ���൵(0~1)�� ����
    float progress = g_float_0;
    
    // ���� �˰� ����
    return lerp(vColor, float4(0.f, 0.f, 0.f, 1.f), progress);
}

float4 PS_FadeIn(VS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // g_float_0�� ���൵(0~1)�� ����
    float progress = g_float_0;
    
    // ���� �����
    return lerp(float4(0.f, 0.f, 0.f, 1.f), vColor, progress);
}

// =================
// Noise Spread Effect
// =================
float4 PS_NoiseSpread(VS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    // �߽������κ����� �Ÿ� ���
    float2 center = float2(0.5f, 0.5f);
    float dist = length(_in.vUV - center);
    
    float progress = g_float_0;
    float noiseVal = vNoise.r;
    
    // �߾ӿ��� �ٱ����� ������ ȿ��
    float spreadRadius = progress * (1.2f + noiseVal * 0.3f);
    
    // ������ ȿ���� ���� ��ī�ο� ��ȯ
    float crack = smoothstep(spreadRadius - 0.05f, spreadRadius, dist);
    
    // ���� �κ��� ��輱 ȿ��
    float edgeCrack = (1.0f - smoothstep(spreadRadius - 0.02f, spreadRadius, dist)) *
                     smoothstep(spreadRadius - 0.04f, spreadRadius - 0.02f, dist);
    
    // ���ο� ���� ���� (�ణ Ǫ������ ���� �Ͼ��)
    float4 targetColor = float4(0.95f, 0.97f, 1.0f, 1.0f);
    
    // ��輱 ������ ȿ��
    float3 crackLight = float3(1.0f, 1.0f, 1.2f) * edgeCrack * 2.0f;
    
    // ����� �̿��� ���� ���� ����
    float crackPattern = frac(noiseVal * 8.0f + dist * 5.0f);
    float finalCrack = lerp(crack, crackPattern > 0.5f ? 1.0f : 0.0f, 0.3f);
    
    float4 result = lerp(vColor, targetColor, finalCrack);
    result.rgb += crackLight;
    
    return result;
}


#endif