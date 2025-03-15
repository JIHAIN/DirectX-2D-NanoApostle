#ifndef _STD2D
#define _STD2D

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

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vUV = _in.vUV;
        
    return output;
}

// Std2D
float4 PS_Std2D(VS_OUT _in) : SV_Target
{          
    float4 vColor = (float4) 0.f;    
    
    // Flipbook 의 현재 Sprite 를 재생해야 하는 경우
    if(g_SpriteUse)
    {        
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);        
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
    }
    
    // FlipbookPlayer 가 없거나 재생중인 Flipbook 이 없는 경우
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
       
    if (vColor.a == 0.f)
        discard;
    
    // 광원처리
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vColor.rgb *= LightColor;
    
    return vColor;
}


// Std2DAlphaBlend
float4 PS_Std2D_AlphaBlend(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    
    
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
    }
    
    else
    {    
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
    
    if (vColor.a == 0.f)
        discard;
    
     // 광원처리
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vColor.rgb *= LightColor;
    
    return vColor;
}

float4 PS_Boss(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // Flipbook 의 현재 Sprite 를 재생해야 하는 경우
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
    }
    
    // FlipbookPlayer 가 없거나 재생중인 Flipbook 이 없는 경우
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
       
    if (vColor.a == 0.f)
        discard;
    
    // 광원처리
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vColor.rgb *= LightColor;
    
    //if(vColor.r >= 0.7) // 보스 2페이지 그냥 여기서 조절해보자
    //{
    //    vColor.rb *= 1.5f;
    //}
    
    return vColor;
}


// Std2dPaperBurn
float4 PS_Std2D_PaperBurn(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
    }
    
    else
    {
        // 첫번째 텍스쳐는 물체의 색상
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
    
    
    // 2번째 텍스쳐 사용, 노이즈 텍스쳐
    if (g_btex_1)
    {
        float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);        
        if (1.f < vNoise.r + g_float_0)
            discard;
    }
    
    if (vColor.a == 0.f)
        discard;
    
    // 광원처리
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vColor.rgb *= LightColor;
    
    return vColor;
}

float4 PS_Std2D_Mouse(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float2 offsetUV = _in.vUV - (0.42f, 0.42f);
    
    if (g_btex_0)
        vColor = g_tex_0.Sample(g_sam_0, offsetUV);
    else
        vColor = GetDebugColor(_in.vUV, 10);
    
       // UV 좌표가 [0, 1] 범위를 벗어나면 픽셀을 버림
    if (offsetUV.x < 0.0 || offsetUV.x > 1.0 || offsetUV.y < 0.0 || offsetUV.y > 1.0)
        discard;
    
    if (vColor.a == 0.f)
        discard;
    
    return vColor;
}

float4 PS_Std2D_Mark(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_btex_0)
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    else
        vColor = GetDebugColor(_in.vUV, 10);
    
    if (vColor.a == 0.f)
        discard;
    
    vColor.rgba *= float4(0.5f, 0.5f, 0.5f , 0.5f);
    
    return vColor;
}

float4 PS_Std2D_Ring(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_btex_0)
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    else
        vColor = GetDebugColor(_in.vUV, 10);
    
    if (vColor.a == 0.f)
        discard;
    
    vColor.rgba *= float4(0.6f, 0.6f, 0.6f, 0.4f);
    
    return vColor;
}

float4 PS_Std2D_Atk(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_SpriteUse)
    {
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
    }
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = float4(0.f, 0.f, 0.f, 1.f);
    }
    
    vColor.rgb *= float3(10.f, 10.f, 10.f);
        
    return vColor;
}

float4 PS_Std2D_BossAtk(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
   
    if (g_SpriteUse)
    {
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
       
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
           || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
            
            if (vColor.a == 0)
                discard;
            
            vColor.a -= g_vec4_0.a;

            if (vColor.a >= 0.8f)
            {
                vColor.rgb = float3(1.f, 0.1f, 0.1f);
            }
            else
                vColor.rgb = float3(0.f, 0.f, 0.f);
            
            if (vColor.a <= 0.1)
                vColor.a = 0.1f;
        }
    }
    else // 보스 구체전용
    {
        if (g_btex_0) 
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
       
        if (vColor.a == 0)
            discard;
      
       // 기본 색상을 더 밝게 하고 발광 효과 추가
        float3 baseColor = float3(1.f, 0.3f, 0.3f) * 1.5f; // 기본 밝기 증가
        float3 glow = baseColor * 2.0f; // 발광 색상
        vColor.rgb = lerp(baseColor, glow, 0.6f); // 블렌딩
       
        float3 LightColor = float3(0.f, 0.f, 0.f);
        for (int i = 0; i < g_Light2DCount; ++i)
        {
            CalcLight2D(i, _in.vWorldPos, LightColor);
        }
        vColor.rgb *= LightColor * float3(10.f, 1.f , 1.f);
       
        return vColor;
    }
   
    //if (vColor.a == 0)
    //    discard;
   
    float3 LightColor = float3(0.f, 0.f, 0.f);
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
   
    vColor.rgb = vColor.rgb * LightColor.xyz;
   
    return vColor;
}



float4 PS_Std2D_Tower(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (vColor.a <= 0.8f)
        discard;
    
    //if (vColor.a == 1.f)
    //    vColor.rgb = float3(0.2, 0.2, 0.2);
    //else
    //    vColor.rgb = float3(0.5, 0.5, 0.5);

    //vColor.rgb = float3(0.99f, 0.99f, 0.99f);
    
    vColor.rgb *= 0.5f;
    
    return vColor;
}

// ==============
// Effect Shader
// ==============
VS_OUT VS_Effect(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
  
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Effect(VS_OUT _in) : SV_Target
{
    if (!g_btex_0)
        discard;
    
    //if(g_float_3 == 1.f)
    //{
    //    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
       
    //   // 현재 UV 좌표와 시간값을 이용해 랜덤값 생성
    //    float2 st = _in.vUV; // 시간을 더해서 매 프레임 다른 값이 나오도록
    //    float randomValue = Random(st);
      
    //   // 랜덤값에 따라 다른 샘플러 선택
    //    if (randomValue < 0.33f)
    //    {
    //        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    //    }
    //    else if (randomValue < 0.66f)
    //    {
    //        vColor = g_tex_1.Sample(g_sam_0, _in.vUV);
    //    }
    //    else
    //    {
    //        vColor = g_tex_2.Sample(g_sam_0, _in.vUV);
    //    }

    //    if (0.f == vColor.a)
    //        discard;
   
    //    vColor.rgb *= g_vec4_0.xyz;
   
    //    return vColor;
    //}
    
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (0.f == vColor.a)
        discard;
    
    vColor.rgb *= g_vec4_0.xyz;
    
    return vColor;
}

#endif
