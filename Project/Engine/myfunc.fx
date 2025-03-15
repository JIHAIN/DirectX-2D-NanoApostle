#ifndef _MYFUNC
#define _MYFUNC

#include "value.fx"
#include "struct.fx"


//-------------------------------------
// 노이즈 함수들
//-------------------------------------
// 기본 노이즈 함수
float BasicNoise(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

// 각각의 옥타브를 보여주는 함수들
float FirstOctave(float2 st)
{
    return BasicNoise(st);
}

float SecondOctave(float2 st)
{
    return BasicNoise(st * 2.0) * 0.5;
}

float ThirdOctave(float2 st)
{
    return BasicNoise(st * 4.0) * 0.25;
}

float FourthOctave(float2 st)
{
    return BasicNoise(st * 8.0) * 0.125;
}

// FBM 함수
float fbm(float2 st)
{
    float value = 0.0;
    
    // 첫 번째 옥타브 (기본 형태)
    value += BasicNoise(st) * 1.0;
    
    // 두 번째 옥타브 (더 작은 디테일)
    value += BasicNoise(st * 2.0) * 0.5;
    
    // 세 번째 옥타브 (더 작은 디테일)
    value += BasicNoise(st * 4.0) * 0.25;
    
    // 네 번째 옥타브 (가장 작은 디테일)
    value += BasicNoise(st * 8.0) * 0.125;
    
    // 0~1 사이로 정규화
    return value / (1.0 + 0.5 + 0.25 + 0.125);
}


//---------------------------------
//색상효과들
//---------------------------------

// 불타는듯한 검붉은색 효과
float4 GetBurningDarkRed(float2 uv)
{
    // 노이즈를 이용한 움직이는 효과
    float noise = fbm(uv * 3.0 + float2(g_DeltaTime * 0.5, g_DeltaTime * 0.3));
    
    // 기본 검붉은색
    float4 baseColor = float4(0.4, 0.02, 0.02, 1.0);
    
    // 불타는 효과를 위한 밝은 빨간색
    float4 glowColor = float4(0.8, 0.1, 0.1, 1.0);
    
    // 노이즈에 따라 색상 혼합
    return lerp(baseColor, glowColor, noise * 0.3);
}



#endif 