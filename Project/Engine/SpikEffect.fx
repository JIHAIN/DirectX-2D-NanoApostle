#ifndef _SPIKEFFECT
#define _SPIKEFFECT

#include "value.fx"

static const float TOTAL_EFFECT_DURATION = 2.0f;
static const float LIGHTNING_FRAME_RATE = 12.0f;
static const int LIGHTNING_FRAME_COUNT = 7;
static const int CIRCLE_FLASH_COUNT = 5;
static const float CIRCLE_FLASH_DURATION = TOTAL_EFFECT_DURATION * 0.5f;

// 번개 프레임 정보를 상수로 정의
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

// 텍스처 전체 크기 (가장 큰 값으로 설정)
static const float2 TEXTURE_SIZE = float2(512.f, 512.f);

float2 GetLightningUV(float2 originalUV, int frameIdx)
{
    // 현재 프레임의 시작점과 크기
    float2 frameStart = LIGHTNING_START[frameIdx] / TEXTURE_SIZE;
    float2 frameSize = LIGHTNING_SIZE[frameIdx] / TEXTURE_SIZE;
    
    // 0~1 범위의 UV를 현재 프레임의 UV 영역으로 변환
    float2 frameUV = originalUV * frameSize + frameStart;
    
    // UV가 현재 프레임 영역을 벗어나면 무효화
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
    // 원형 범위 계산
    float2 center = float2(0.5f, 0.5f);
    float dist = length(_in.vUV - center);
    
    // 범위를 벗어난 픽셀은 제외
    if (dist > 1.0f)
        discard;
        
    float effectTime = g_Time;
    float normalizedTime = saturate(effectTime / TOTAL_EFFECT_DURATION);
    
    // 1. 중앙 빛나는 효과 (FX_Flare)
    float scale = lerp(0.2f, 1.0f, normalizedTime);
    float2 glowUV = (_in.vUV - center) / scale + center;
    float4 glowColor = g_tex_1.Sample(g_sam_0, glowUV);
    glowColor.rgb *= float3(1.0f, 0.2f, 0.2f); // 붉은 색조 적용
    glowColor.a *= (1.0f - dist);
    
    // 번개 효과
    int frameIdx = (int) floor(fmod(effectTime * LIGHTNING_FRAME_RATE, LIGHTNING_FRAME_COUNT));
    float2 lightningUV = GetLightningUV(_in.vUV, frameIdx);
    float4 lightningColor = g_tex_2.Sample(g_sam_0, lightningUV);
 
    // UV가 유효하지 않으면 투명 처리
    if (lightningUV.x < 0)
    {
        lightningColor = float4(0, 0, 0, 0);
    }
    else
    {
        lightningColor = g_tex_2.Sample(g_sam_0, lightningUV);
        lightningColor.rgb *= float3(1.0f, 0.1f, 0.1f); // 번개는 더 붉게
        lightningColor.a *= (1.0f - dist) * 0.7f;
    }
    
    // 원형 파장 효과
    float circleIntensity = 0.f;
    if (effectTime < CIRCLE_FLASH_DURATION)
    {
        for (int i = 0; i < CIRCLE_FLASH_COUNT; i++)
        {
            float progress = fmod(normalizedTime * 2.0 + i * 0.2f, 1.0);
            float radius = progress * 0.8f;
            float thickness = 0.08f; // 두께 증가
            circleIntensity += smoothstep(radius + thickness, radius - thickness, dist) * (1.0 - progress) * 1.5f; // 강도 증가
        }
    }
    
    // 최종 색상 합성
    float4 finalColor = float4(0.f, 0.f, 0.f, 0.f);
    float4 circleColor = float4(1.0f, 0.1f, 0.1f, circleIntensity * 0.8f); // 더 붉고 강한 원형 파장
    
    finalColor = circleColor;
    finalColor += glowColor * scale;
    finalColor += lightningColor;
    
    // 붉은 색조를 더 강하게
    finalColor.rgb = lerp(finalColor.rgb, float3(1.0f, 0.15f, 0.15f), 0.4f);
    
    // 가장자리 페이드아웃
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
  
    // 1. 중앙 빛나는 효과 (FX_Flare)
    float4 glowColor = g_tex_1.Sample(g_sam_0, _in.vUV);
    glowColor.rgb *= float3(1.0f, 0.1f, 0.1f); // 붉은 색조 적용
    
    // 테두리 Circle 효과
    float4 CircleColor = g_tex_2.Sample(g_sam_0, _in.vUV);
    CircleColor.rgb *= float3(1.0f, 0.0f, 0.0f); // 번개는 더 붉게
    
    // 최종 색상 합성 하기전에 원래 색상 구해둠
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
    // 원형 범위 계산
    float2 center = float2(0.5f, 0.5f);
    float dist = length(_in.vUV - center);
    
    // 범위를 벗어난 픽셀은 제외
    if (dist > 1.0f)
        discard;
        
    float effectTime = g_Time;
    float normalizedTime = saturate(effectTime / TOTAL_EFFECT_DURATION);
     
    // 번개 효과
    int frameIdx = (int) floor(fmod(effectTime * LIGHTNING_FRAME_RATE, LIGHTNING_FRAME_COUNT));
    float2 lightningUV = GetLightningUV(_in.vUV, frameIdx);
    float4 lightningColor = g_tex_0.Sample(g_sam_0, lightningUV);
 
    // UV가 유효하지 않으면 투명 처리
    if (lightningUV.x < 0)
    {
        lightningColor = float4(0, 0, 0, 0);
    }
    else
    {
        lightningColor = g_tex_0.Sample(g_sam_0, lightningUV);
        lightningColor.rgb *= float3(1.0f, 0.1f, 0.1f); // 번개는 더 붉게
        lightningColor.a *= (1.0f - dist) * 0.7f;
    }
    
    // 최종 색상 합성
    float4 finalColor = float4(0.f, 0.f, 0.f, 0.f);
    finalColor += lightningColor;
    
    // 붉은 색조를 더 강하게
    finalColor.rgb = lerp(finalColor.rgb, float3(0.5f, 0.5f, 0.15f), 0.4f);
    
    // 가장자리 페이드아웃
    //finalColor.a *= (1.0f - smoothstep(0.7f, 0.9f, dist));
    
    if (finalColor.a == 0.f)
        discard;
    
    finalColor.a *= 4.f;
    
    return finalColor;
}

#endif