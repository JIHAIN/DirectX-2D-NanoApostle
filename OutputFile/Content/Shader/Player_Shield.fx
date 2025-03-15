#ifndef _PLAYER_SHELD
#define _PLAYER_SHELD

#include "value.fx"
#include "func.fx"
#include "myfunc.fx"

// Vertex Shader 
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

VS_OUT VS_Player_Shield(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
        
    return output;
}


// 픽셀 쉐이더
float4 PS_Player_Shield(VS_OUT _in) : SV_Target
{
    // UV 좌표와 기본 쉴드 텍스처 샘플링
    float2 uv = _in.vUV;
    float4 baseShield = g_tex_0.Sample(g_sam_0, uv);
    
    // 원형 쉴드 범위 계산
    float2 center = float2(0.5f, 0.5f);
    float dist = length(uv - center) * 2.0f;
    
    // 쉴드 범위를 벗어난 픽셀은 제외
    if (dist > 1.0f)
        discard;

    // 객체가 생성된 이후의 경과 시간 계산 (0~6초 반복)
    float elapsedTime = fmod(g_Time_Engine - g_float_0, 8.0f);
    
   // 2초 이후부터 시작하도록 조정
    float timeProgress = max(0.0f, elapsedTime - 1.0f) / 7.0f; // 8초 - 2초 = 6초로 나누기
    float timeAlpha = (1.0f - pow(abs(timeProgress - 0.5f) * 2.0f, 0.5f));
    
    // 노이즈 텍스처 UV 계산
    float2 noiseUV1 = uv + float2(sin(timeProgress * 3.0f) * 0.2f, -timeProgress * 0.6f);
    float2 noiseUV2 = uv * 1.3f + float2(cos(timeProgress * 2.4f) * 0.3f, -timeProgress * 0.66f);
    
    // 노이즈 텍스처 샘플링
    float4 noise1 = g_tex_1.Sample(g_sam_0, noiseUV1);
    float4 noise2 = g_tex_1.Sample(g_sam_0, noiseUV2);
    
    // 불규칙한 높이 제한 설정 (50~60% 범위)
    float yLimit = lerp(0.2f, 0.8f, noise1.r);
    float yMask = 1.0f - smoothstep(yLimit - 0.1f, yLimit + 0.1f, uv.y);
    
    // 노이즈 패턴 생성 및 높이 제한 적용
    float pattern = lerp(noise2.r, noise2.r, 0.7f);
    pattern = pattern * yMask;
    
// 오로라 색상 설정 및 블렌딩 (색상 강도 낮춤)
    float3 color1 = float3(0.0f, 0.8f, 0.7f); // 채도 낮춤
    float3 color2 = float3(0.1f, 0.7f, 0.8f); // 채도 낮춤
    float3 auroraColor = lerp(color1, color2, pattern) * 1.2; // 추가 곱셈 제거

// 최종 효과 강도 계산 (강도 낮춤)
    float intensity = pattern * timeAlpha * 2.5f; // 2.0f -> 1.2f
    float4 auroraEffect = float4(auroraColor, intensity * 0.7f); // 알파값도 낮춤

// 블렌딩 시 강도 조절
    float4 finalColor = baseShield;
    if (baseShield.a < 0.1f)
        finalColor = auroraEffect;
    else
        finalColor = baseShield + (auroraEffect * 0.8f); // 블렌딩 강도 낮춤

// 최종 알파값도 조절
    finalColor.a = max(baseShield.a, auroraEffect.a * 0.6f);
    
    return finalColor;
}
#endif