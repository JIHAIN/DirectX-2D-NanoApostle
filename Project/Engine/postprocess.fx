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

// 1. 픽셀쉐이더의 픽셀 좌표
// 2. 렌더 타겟 해상도

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // SV_Position -> Pixelshader (픽셀좌표)
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;    
    
    // NoiseTexture 가 있으면
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
    
    // 텍스처 샘플링 최적화
    float2 baseUV = _in.vUV;
    float2 stepDir = _in.InstID == 0 ? float2(vUVStep.x, 0.f) : float2(0.f, vUVStep.y);
    
    // 중심점 먼저 계산
    vColor += g_tex_0.Sample(g_sam_0, baseUV) * CrossFilter[6];
    
    // 양쪽으로 퍼져나가며 계산 (대칭성 활용)
    [unroll]
    for (int i = 1; i <= 6; ++i)
    {
        float2 offset = stepDir * i;
        float weight = CrossFilter[6 + i];
        
        // 한 번의 가중치로 양쪽 적용 (대칭)
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
    
    // HDR 효과 개선
    float intensity = 1.2f; // 블룸 강도 조절
    float4 vBloom = pow(abs(pow(vEffect, 2.1f)) + abs(pow(vEffectBlur * intensity, 2.1f)), 1.f / 2.1f);
    
    return vBloom;
}



VS_OUT VS_Roar(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 화면 전체를 대상으로
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}


float4 PS_Roar(VS_OUT _in) : SV_Target
{
    // 첫 번째 파동 효과용
    float Scale1 = 0.4f;
    float MaxScaleFactor1 = 10.0f;
    float FadeSpeed1 = 1.f;
    float Intensity1 = 1.0f;
    float WaveCount1 = 1.5f;
    float WaveSpeed1 = 5.2f;
    
    // 두 번째 파동 효과용
    float Scale2 = 0.4f; // 더 작은 초기 크기
    float MaxScaleFactor2 = 3.f; // 더 큰 최대 크기
    float FadeSpeed2 = 0.4f; // 더 느린 페이드 아웃
    float Intensity2 = 0.7f; // 다른 강도
    float WaveCount2 = 2.5f; // 한 번만 반복
    float WaveSpeed2 = 1.2f; // 더 느린 속도
    float WaveDelay2 = 0.7f; // 시작 지연 시간
    
    // 진동 효과용
    float ShakeAmount = 0.004f;
    float ShakeSpeed = 50.0f;
    
    float2 shakeOffset = float2(sin(g_Time * ShakeSpeed) * ShakeAmount,
                                cos(g_Time * ShakeSpeed * 1.2) * ShakeAmount);
    
    float2 shakenUV = _in.vUV + shakeOffset;
    float4 finalColor = g_tex_0.Sample(g_sam_0, shakenUV);
    
    // 색상 보정
    finalColor.rgb *= 1.2f;
    finalColor.r = saturate(finalColor.r * 1.1);
    finalColor.g *= 0.8;
    finalColor.b *= 0.8;
    float3 redTint = float3(0.5, 0.1, 0.1);
    finalColor.rgb = lerp(finalColor.rgb, redTint, 0.2);
    float3 contrast = (finalColor.rgb - 0.5) * 1.5 + 0.5;
    finalColor.rgb = saturate(contrast);
    
  
    // 첫 번째 파동 효과 적용
    finalColor = ApplyWaveEffect(finalColor, shakenUV, Scale1, MaxScaleFactor1, FadeSpeed1, Intensity1, WaveCount1, WaveSpeed1, 0, g_tex_2);
    
    // 두 번째 파동 효과 적용
    finalColor = ApplyWaveEffect(finalColor, shakenUV, Scale2, MaxScaleFactor2, FadeSpeed2, Intensity2, WaveCount2, WaveSpeed2, WaveDelay2, g_tex_2);
    
    return finalColor;
}


VS_OUT VS_ShockWave(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // 화면 전체를 대상으로
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}


float4 PS_ShockWave(VS_OUT _in) : SV_Target
{
    //// 입력 UV 좌표
    float2 uv = _in.vUV;
    float2 st = uv;
    
    // 파동 중심점 설정 (화면 중앙)
    float2 WaveCentre = float2(0.5, 0.5);
    // 화면 비율 계산 (세로/가로)
    float ratio = g_RenderResolution.y / g_RenderResolution.x;
    // 중심점의 Y좌표를 화면 비율에 맞게 조정
    WaveCentre.y *= ratio;
    
    // 거리 계산을 위한 스케일 값
    float downScale = 2;
    // 현재 픽셀과 파동 중심과의 거리 계산
    // st.y에 ratio를 곱해 화면 비율 왜곡을 보정
    float dist = distance(float2(st.x, st.y * ratio), WaveCentre) * downScale;
    
    // 거리값을 파동 계산에 사용
    float val = dist;
    // 파동 형태 조절 값 (값이 클수록 파동이 더 뾰족해짐)
    float a = 30.0;
    
    // 시간에 따른 파동 이동 속도
    float pulseSpeed = g_Time * 20.f;
    
    // 파동의 주기를 제어하는 값 (값이 클수록 파동의 주기가 짧아짐)
    float wavePeriod = 30.0;
    // 코사인 함수를 사용하여 주기적인 파동 생성
    // val * wavePeriod: 거리에 따른 파동의 주기 조절
    // - pulseSpeed: 시간에 따른 파동 이동
    float cosFuns = cos(val * wavePeriod - pulseSpeed);
    
    // 거리에 따른 파동의 감쇠를 계산
    float powFuns = pow(val * 2.f, a);
    // 파동의 최대 강도를 제한하고 부드럽게 만듦
    float limtedPowFuns = 0.5 * pow(a / (a + powFuns), 2.0);
    
    // 파동의 전체적인 강도 조절
    float waveStrength = 7.0;
    
    // 최종 파동 효과 계산
    float finalRes = smoothstep(0.0, 1.0, limtedPowFuns * cosFuns) * waveStrength;
    
    // UV 좌표 변환 및 스케일링
    // st * 2.0 - 1.0: UV 좌표를 -1~1 범위로 변환
    st = st * 2.0 - 1.0;
    // 파동 효과에 따라 UV 좌표 왜곡
    st *= 1.0 + finalRes * 0.1;
    // 왜곡된 UV 좌표를 다시 0~1 범위로 변환
    st = st * 0.5 + 0.5;
    
    // 변형된 UV 좌표로 텍스처 샘플링
    float4 texCol = g_tex_0.Sample(g_sam_1, st);
    
    // 파동 효과를 텍스처 색상에 적용
    texCol += (texCol * finalRes) / (dist * 15.0);
    
    // 최종 색상 출력
    float4 fragColor = texCol;
    
    return fragColor;
    
  
}

float4 PS_HitEffect(VS_OUT _in) : SV_Target
{
  
    float MaxRadius = 0.9; // 최대 원형 영역의 반지름
    float Softness = 0.4; // 가장자리 부드러움
    float Intensity = 0.6; // 기본 효과 강도
    float FadeOutSpeed = 3.0; // 페이드 아웃 속도 조절 (값이 클수록 더 빠르게 사라짐)
    
    float EffectProgress = g_float_3;

    // 원본 텍스처 색상
    float4 originalColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // 화면 중앙
    float2 center = float2(0.5, 0.5);
    
    // 현재 픽셀과 화면 중앙과의 거리 계산
    float distance = length(_in.vUV - center);
    
    // 현재 반지름 계산
    float currentRadius = EffectProgress * MaxRadius;
    // 원형 마스크 생성
    float circleMask = 1.0 - smoothstep(currentRadius, currentRadius + Softness, distance);
    // 빨간색 오버레이
    float3 redOverlay = float3(0.2, 0.01, 0.01);
    // 페이드 아웃 효과 (비선형)
    float fadeOut = pow(1.0 - EffectProgress, FadeOutSpeed);
    // 최종 색상 계산
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
    
    // 진행도
    float fadeProgress = g_float_0;
    
    // 화면 중앙점
    float2 center = float2(0.5, 0.5);
    float dist = length(_in.vUV - center);
    
    // 가장자리(1.0)에서 중앙(0.0)으로 오는 마스크 생성
    // 진행도가 증가할수록 중앙으로 모이는 원이 작아짐
    float radius = 1.0 - fadeProgress;
    float circle = 1.0 - smoothstep(radius - 0.1, radius, dist);
    
    // 검은색과 원본 이미지 블렌딩
    float4 finalColor = lerp(float4(0.f, 0.f, 0.f, 1.f), originalColor, circle);
    
    return finalColor;
}

// =================
// Fade Effect
// =================
float4 PS_FadeOut(VS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // g_float_0에 진행도(0~1)를 받음
    float progress = g_float_0;
    
    // 점점 검게 변함
    return lerp(vColor, float4(0.f, 0.f, 0.f, 1.f), progress);
}

float4 PS_FadeIn(VS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // g_float_0에 진행도(0~1)를 받음
    float progress = g_float_0;
    
    // 점점 밝아짐
    return lerp(float4(0.f, 0.f, 0.f, 1.f), vColor, progress);
}

// =================
// Noise Spread Effect
// =================
float4 PS_NoiseSpread(VS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    // 중심점으로부터의 거리 계산
    float2 center = float2(0.5f, 0.5f);
    float dist = length(_in.vUV - center);
    
    float progress = g_float_0;
    float noiseVal = vNoise.r;
    
    // 중앙에서 바깥으로 퍼지는 효과
    float spreadRadius = progress * (1.2f + noiseVal * 0.3f);
    
    // 깨지는 효과를 위한 날카로운 전환
    float crack = smoothstep(spreadRadius - 0.05f, spreadRadius, dist);
    
    // 깨진 부분의 경계선 효과
    float edgeCrack = (1.0f - smoothstep(spreadRadius - 0.02f, spreadRadius, dist)) *
                     smoothstep(spreadRadius - 0.04f, spreadRadius - 0.02f, dist);
    
    // 새로운 공간 색상 (약간 푸른빛이 도는 하얀색)
    float4 targetColor = float4(0.95f, 0.97f, 1.0f, 1.0f);
    
    // 경계선 빛나는 효과
    float3 crackLight = float3(1.0f, 1.0f, 1.2f) * edgeCrack * 2.0f;
    
    // 노이즈를 이용한 유리 깨짐 패턴
    float crackPattern = frac(noiseVal * 8.0f + dist * 5.0f);
    float finalCrack = lerp(crack, crackPattern > 0.5f ? 1.0f : 0.0f, 0.3f);
    
    float4 result = lerp(vColor, targetColor, finalCrack);
    result.rgb += crackLight;
    
    return result;
}


#endif