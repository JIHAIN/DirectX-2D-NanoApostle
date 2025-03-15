#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "func.fx"
#include "myfunc.fx"

#define TileAtlas   g_tex_0
#define COL         g_int_0
#define ROW         g_int_1

#define MainTex g_tex_0
#define RepeatCount g_vec2_0

struct tTileInfo
{
    float2 TileLT;
    float2 TileSlice;
};

StructuredBuffer<tTileInfo> g_Buffer : register(t16);

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

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV * float2(COL, ROW);
    
    return output;
}


float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
                
    if (g_btex_0)
    {
        int2 ColRow = floor(_in.vUV);
        int idx = ColRow.y * COL + ColRow.x;
        
        float2 vAtlasUV = g_Buffer[idx].TileLT + (frac(_in.vUV) * g_Buffer[idx].TileSlice);
        vOutColor = TileAtlas.Sample(g_sam_1, vAtlasUV);
    }
    else
    {
        vOutColor = GetDebugColor(_in.vUV, 10);
    }
        
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    float r = hash(g_DT_Engine);
    
   // vOutColor.rgba = GetBurningDarkRed(_in.vUV);
    vOutColor.rgb *= LightColor;
    
    return vOutColor;
}


VS_OUT VS_RepeatSprite(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV * RepeatCount;
    
    return output;
}

//float4 PS_RepeatSprite(VS_OUT _in) : SV_Target
//{
//    float4 vOutColor = (float4) 0.f;
    
//    if (g_btex_0)
//    {
//        // UV 좌표를 반복시켜 텍스처 샘플링
//        float2 repeatedUV = frac(_in.vUV);
//        vOutColor = MainTex.Sample(g_sam_0, repeatedUV);
//    }
    
//     // 2번째 텍스쳐 사용, 노이즈 텍스쳐, 페이퍼 번 효과
//    if (g_btex_1)
//    {
//        float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);
//        if (1.f < vNoise.r + g_float_0)
//            discard;
//    }
    
//    // 3번째 텍스쳐가 있는 경우는 이제 눈빛으로 보자
//    if(g_btex_2)
//    {
        
//    }
    
//    if(vOutColor.a == 0.f)
//        discard;
    
//    float3 LightColor = float3(0.f, 0.f, 0.f);
    
//    for (int i = 0; i < g_Light2DCount; ++i)
//    {
//        CalcLight2D(i, _in.vWorldPos, LightColor);
//    }
    
//    vOutColor.rgb *= LightColor;
    
//    if(g_float_0 == 0.3) // 서클 함정이 사용함
//        vOutColor *= g_float_0;
//    else if (g_float_0 == 0.5) // 로켓 빨간색
//    {
//        // 부스터 효과
//        float2 center = float2(0.5, 0.5);
//        float dist = length(_in.vUV - center);
        
//        // 기본 색상 설정 (붉은 색)
//        float3 baseColor = float3(1.0, 0.1, 0.1);
        
//        // UV 좌표에 따른 그라데이션 효과
//        float gradient = 1.0 - smoothstep(0.0, 0.8, dist);
        
//        // 시간에 따른 밝기 변화 (깜박임 효과)
//        float flicker = 0.8 + 0.2 * sin(g_DT_Engine * 50.0);
        
//        // 최종 색상 계산
//        vOutColor.rgb = baseColor * gradient * flicker;
//        //vOutColor.a *= gradient; // 알파값도 그라데이션 적용
        
//        // 외곽 페이드아웃
//        if (gradient < 0.1)
//            discard;
//    }
    
//    return vOutColor;
//}

float4 PS_RepeatSprite(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
    
    if (g_btex_0)
    {
        // 반복되는 UV 좌표에 대해서도 각각 효과 적용
        float2 repeatedUV = frac(_in.vUV);
        vOutColor = MainTex.Sample(g_sam_0, repeatedUV);
        
        if (g_float_0 == 0.5) // 로켓 빨간색
        {
            // 각 반복 영역에 대한 중심점 계산
            float2 localUV = repeatedUV; // 0~1 사이의 UV
            float2 center = float2(0.5, 0.5);
            float dist = length(localUV - center);
            
            // 기본 색상 설정 (붉은 색)
            float3 baseColor = float3(1.0, 0.1, 0.1);
            
            // UV 좌표에 따른 그라데이션 효과
            float gradient = 1.0 - smoothstep(0.0, 0.8, dist);
            
            // 시간에 따른 밝기 변화 (깜박임 효과)
            float flicker = 1 + 0.4 * sin(g_Time_Engine * 20.0);
            
            // 최종 색상 계산
            vOutColor.rgb = baseColor * gradient * flicker;
            
            // 외곽 페이드아웃
            if (gradient < 0.1)
                discard;
        }
    }
    
    if (g_btex_1)
    {
        float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);
        if (1.f < vNoise.r + g_float_0)
            discard;
    }
    
    if (vOutColor.a == 0.f)
        discard;
    
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vOutColor.rgb *= LightColor;
    
    if (g_float_0 == 0.3)
        vOutColor *= g_float_0;
    
    return vOutColor;
}

#endif