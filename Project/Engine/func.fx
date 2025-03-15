#ifndef _FUNC
#define _FUNC

#include "value.fx"
#include "struct.fx"

float4 GetDebugColor(float2 _UV, int _Level)
{
    float2 vTexPos = _UV * _Level;
    
    int Col = floor(vTexPos.x);
    int Row = floor(vTexPos.y);
        
    // Ȧ��
    if (Col % 2)
    {
        if (Row % 2)
            return float4(1.f, 0.f, 1.f, 1.f);
        else
            return float4(0.f, 0.f, 0.f, 1.f);
    }           
    // ¦��
    else
    {
        if (Row % 2)
            return float4(0.f, 0.f, 0.f, 1.f);
        else
            return float4(1.f, 0.f, 1.f, 1.f);
    }
}

void CalcLight2D(int _LightIdx, float3 _WorldPos, inout float3 _LightColor)
{
    float DistRatio = 1.f;
    
    // ����ó��
    if (g_Light2DInfo[_LightIdx].Type == 0) // DirLight
    {
        _LightColor += g_Light2DInfo[_LightIdx].vColor;
    }
    else if (g_Light2DInfo[_LightIdx].Type == 1) // PointLight
    {
        float Dist = distance(_WorldPos.xy, g_Light2DInfo[_LightIdx].vWorldPos.xy);
        if (Dist <= g_Light2DInfo[_LightIdx].Radius)
        {
            DistRatio = saturate(1.f - (Dist / g_Light2DInfo[_LightIdx].Radius));
            //DistRatio = saturate(cos(saturate((Dist / g_Light2DInfo[_LightIdx].Radius)) * (PI / 2.f)));
            
            _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;
        }
    }
    else // SpotLight
    {
        float Dist = distance(_WorldPos.xy, g_Light2DInfo[_LightIdx].vWorldPos.xy);
        
        float2 PNVec = normalize(_WorldPos.xy - g_Light2DInfo[_LightIdx].vWorldPos.xy);
        // Light2D�� ���� ��ǥ���� ���ڷ� ���� �ȼ��� ���⺤�͸� ����
        
        float2 LDir = normalize(g_Light2DInfo[_LightIdx].vDir.xy);
        // ���� ����
        
        float Angle = dot(LDir, PNVec);
        // ���� ����� ������ �ȼ��� ���ϴ� ������ ������ �ڻ��� ��
      
        float Light2DAngle = cos(radians(g_Light2DInfo[_LightIdx].Angle / 2));
        // ������ �ִ� ������ 2�� ������ �ڻ��� �Լ��� ����ϱ� ���� ���� ������ ��������
        
        if (Angle >= Light2DAngle)
        {
            if (Dist <= g_Light2DInfo[_LightIdx].Radius)
            {
                DistRatio = saturate(cos(saturate((Dist / g_Light2DInfo[_LightIdx].Radius)) * (PI / 2.f)));
                _LightColor += g_Light2DInfo[_LightIdx].vColor * DistRatio;
            }
        }
    }
}

// ���� ȿ�� �Լ�
float2 ApplyShake(float2 uv, float time, float intensity)
{
    float2 shake = float2(
        sin(time * 100.0 + uv.y * 10.0),
        cos(time * 95.0 + uv.x * 10.0)
    ) * 0.01 * intensity;
    
    return uv + shake;
}

  // �ĵ� ȿ���� ���� �Լ�
float4 ApplyWaveEffect(float4 color, float2 uv, float scale, float maxScaleFactor, float fadeSpeed, float intensity, float waveCount, float waveSpeed, float delay, Texture2D waveTex)
{
    float AspectRatio = g_RenderResolution.x / g_RenderResolution.y;

    for (int i = 0; i < waveCount; i++)
    {
        float timeOffset = (float) i / waveCount;
        float waveTime = max(0, fmod(g_Time * waveSpeed + timeOffset - delay, 1.0));
        float2 center = float2(0.5, 0.5);
        float currentScale = scale * (1.0 + (maxScaleFactor - 1.0) * waveTime);
        float2 centeredUV = uv - center;
        centeredUV.x *= AspectRatio;
        centeredUV /= currentScale;
        float2 adjustedUV = clamp(centeredUV + center, 0, 1);
            
        float4 waveColor = waveTex.Sample(g_sam_0, adjustedUV);
        float fadeOut = max(0, 1 - waveTime * fadeSpeed);
        float colorPhase = frac(i / waveCount * 2);
        float3 tintColor = lerp(float3(0, 0, 0), float3(0.5, 0, 0), abs(sin(colorPhase * 3.14159)));
        float3 effectColor = waveColor.rgb * tintColor * intensity * fadeOut;
        color.rgb = lerp(color.rgb, color.rgb + effectColor, waveColor.a * fadeOut);
    }
    return color;
}



// ���� �Լ�
float Random(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

float2 hash(float2 p)
{
    p = float2(dot(p, float2(127.1, 311.7)),
               dot(p, float2(269.5, 183.3)));
    
    return frac(sin(p) * 43758.5453123);
}

//float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxId)
//{
//    float2 vUV = (float2) 0.f;
    
//    vUV.x = ((float) _ID / (float) (_maxId - 1)) + g_Time_Engine * 0.01f;
//    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_Time_Engine * 0.1f;
//    float3 vRandom = _NoiseTexture.SampleLevel(g_sam_0, vUV, 0).xyz;
    
//    return vRandom;
//}

float3 GetRandom(in Texture2D NoiseTexture, uint ID, uint maxId)
{
    // �⺻ UV ���
    float2 vUV;
    float time = g_Time_Engine * 0.01f;
    
    // ID ��� ������ ��� (�� ������ ����)
    float idNorm = (float) ID / (float) maxId;
    vUV.x = frac(idNorm + time * 0.7f);
    vUV.y = frac(sin(idNorm * 234.5f + time * 1.3f) * 0.5f + 0.5f);
    
    return NoiseTexture.SampleLevel(g_sam_0, vUV, 0).xyz;
}





#endif 