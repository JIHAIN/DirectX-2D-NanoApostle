#ifndef _SPIKEFFECT
#define _SPIKEFFECT

#include "value.fx"

static const float TOTAL_EFFECT_DURATION = 2.0f;
static const float LIGHTNING_FRAME_RATE = 12.0f;
static const int LIGHTNING_FRAME_COUNT = 7;
static const int CIRCLE_FLASH_COUNT = 5;
static const float CIRCLE_FLASH_DURATION = TOTAL_EFFECT_DURATION * 0.5f;

// ���� ������ ������ ����� ����
static const float2 LIGHTNING_START[7] =
{
    float2(0.f, 357.f), // frame 0
    float2(132.f, 364.f), // frame 1
    float2(273.f, 368.f), // frame 2
    float2(0.f, 195.f), // frame 3
    float2(161.f, 200.f), // frame 4
    float2(351.f, 187.f), // frame 5
    float2(0.f, 49.f) // frame 6
};

static const float2 LIGHTNING_SIZE[7] =
{
    float2(130.f, 154.f), // frame 0
    float2(141.f, 147.f), // frame 1
    float2(148.f, 143.f), // frame 2
    float2(161.f, 162.f), // frame 3
    float2(189.f, 157.f), // frame 4
    float2(158.f, 171.f), // frame 5
    float2(173.f, 135.f) // frame 6
};

// �ؽ�ó ��ü ũ�� (���� ū ������ ����)
static const float2 TEXTURE_SIZE = float2(512.f, 512.f);

float2 GetLightningUV(float2 originalUV, int frameIdx)
{
    // ���� �������� �������� ũ��
    float2 frameStart = LIGHTNING_START[frameIdx] / TEXTURE_SIZE;
    float2 frameSize = LIGHTNING_SIZE[frameIdx] / TEXTURE_SIZE;
    
    // 0~1 ������ UV�� ���� �������� UV �������� ��ȯ
    float2 frameUV = originalUV * frameSize + frameStart;
    
    // UV�� ���� ������ ������ ����� ��ȿȭ
    if (any(originalUV < 0.f) || any(originalUV > 1.f))
    {
        return float2(-1, -1);
    }
    
    return frameUV;
}

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_SpikEffect(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_SpikEffect(VS_OUT _in) : SV_Target
{
    // ���� ���� ���
    float2 center = float2(0.5f, 0.5f);
    float dist = length(_in.vUV - center);
    
    // ������ ��� �ȼ��� ����
    if (dist > 1.0f)
        discard;
        
    float effectTime = g_Time;
    float normalizedTime = saturate(effectTime / TOTAL_EFFECT_DURATION);
    
    // 1. �߾� ������ ȿ�� (FX_Flare)
    float scale = lerp(0.2f, 1.0f, normalizedTime);
    float2 glowUV = (_in.vUV - center) / scale + center;
    float4 glowColor = g_tex_1.Sample(g_sam_0, glowUV);
    glowColor.rgb *= float3(1.0f, 0.2f, 0.2f); // ���� ���� ����
    glowColor.a *= (1.0f - dist);
    
    // ���� ȿ��
    int frameIdx = (int) floor(fmod(effectTime * LIGHTNING_FRAME_RATE, LIGHTNING_FRAME_COUNT));
    float2 lightningUV = GetLightningUV(_in.vUV, frameIdx);
    float4 lightningColor = g_tex_2.Sample(g_sam_0, lightningUV);
 
    // UV�� ��ȿ���� ������ ���� ó��
    if (lightningUV.x < 0)
    {
        lightningColor = float4(0, 0, 0, 0);
    }
    else
    {
        lightningColor = g_tex_2.Sample(g_sam_0, lightningUV);
        lightningColor.rgb *= float3(1.0f, 0.1f, 0.1f); // ������ �� �Ӱ�
        lightningColor.a *= (1.0f - dist) * 0.7f;
    }
    
    // ���� ���� ȿ��
    float circleIntensity = 0.f;
    if (effectTime < CIRCLE_FLASH_DURATION)
    {
        for (int i = 0; i < CIRCLE_FLASH_COUNT; i++)
        {
            float progress = fmod(normalizedTime * 2.0 + i * 0.2f, 1.0);
            float radius = progress * 0.8f;
            float thickness = 0.08f; // �β� ����
            circleIntensity += smoothstep(radius + thickness, radius - thickness, dist) * (1.0 - progress) * 1.5f; // ���� ����
        }
    }
    
    // ���� ���� �ռ�
    float4 finalColor = float4(0.f, 0.f, 0.f, 0.f);
    float4 circleColor = float4(1.0f, 0.1f, 0.1f, circleIntensity * 0.8f); // �� �Ӱ� ���� ���� ����
    
    finalColor = circleColor;
    finalColor += glowColor * scale;
    finalColor += lightningColor;
    
    // ���� ������ �� ���ϰ�
    finalColor.rgb = lerp(finalColor.rgb, float3(1.0f, 0.15f, 0.15f), 0.4f);
    
    // �����ڸ� ���̵�ƿ�
   // finalColor.a *= (1.0f - smoothstep(0.7f, 0.9f, dist));
    
    if (finalColor.a == 0.f)
        discard;
    
    return finalColor;
}

VS_OUT VS_TrapSircle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_TrapSircle(VS_OUT _in) : SV_Target
{ 
  
    // 1. �߾� ������ ȿ�� (FX_Flare)
    float4 glowColor = g_tex_1.Sample(g_sam_0, _in.vUV);
    glowColor.rgb *= float3(1.0f, 0.1f, 0.1f); // ���� ���� ����
    
    // �׵θ� Circle ȿ��
    float4 CircleColor = g_tex_2.Sample(g_sam_0, _in.vUV);
    CircleColor.rgb *= float3(1.0f, 0.0f, 0.0f); // ������ �� �Ӱ�
    
    // ���� ���� �ռ� �ϱ����� ���� ���� ���ص�
    float4 OriginColor = float4(0.f, 0.f, 0.f, 0.f);
    float4 finalColor = float4(0.f, 0.f, 0.f, 0.f);
    
    OriginColor = glowColor + CircleColor;
    finalColor = OriginColor;
    
    if (finalColor.a == 0.f)
        discard;
    
    finalColor.rgb += g_float_0;
    
    return finalColor;
}

VS_OUT VS_Lightningffect(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Lightningffect(VS_OUT _in) : SV_Target
{
    // ���� ���� ���
    float2 center = float2(0.5f, 0.5f);
    float dist = length(_in.vUV - center);
    
    // ������ ��� �ȼ��� ����
    if (dist > 1.0f)
        discard;
        
    float effectTime = g_Time;
    float normalizedTime = saturate(effectTime / TOTAL_EFFECT_DURATION);
     
    // ���� ȿ��
    int frameIdx = (int) floor(fmod(effectTime * LIGHTNING_FRAME_RATE, LIGHTNING_FRAME_COUNT));
    float2 lightningUV = GetLightningUV(_in.vUV, frameIdx);
    float4 lightningColor = g_tex_0.Sample(g_sam_0, lightningUV);
 
    // UV�� ��ȿ���� ������ ���� ó��
    if (lightningUV.x < 0)
    {
        lightningColor = float4(0, 0, 0, 0);
    }
    else
    {
        lightningColor = g_tex_0.Sample(g_sam_0, lightningUV);
        lightningColor.rgb *= float3(1.0f, 0.1f, 0.1f); // ������ �� �Ӱ�
        lightningColor.a *= (1.0f - dist) * 0.7f;
    }
    
    // ���� ���� �ռ�
    float4 finalColor = float4(0.f, 0.f, 0.f, 0.f);
    finalColor += lightningColor;
    
    // ���� ������ �� ���ϰ�
    finalColor.rgb = lerp(finalColor.rgb, float3(0.5f, 0.5f, 0.15f), 0.4f);
    
    // �����ڸ� ���̵�ƿ�
    //finalColor.a *= (1.0f - smoothstep(0.7f, 0.9f, dist));
    
    if (finalColor.a == 0.f)
        discard;
    
    finalColor.a *= 4.f;
    
    return finalColor;
}

#endif