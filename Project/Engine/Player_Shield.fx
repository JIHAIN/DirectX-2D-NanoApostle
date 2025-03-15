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


// �ȼ� ���̴�
float4 PS_Player_Shield(VS_OUT _in) : SV_Target
{
    // UV ��ǥ�� �⺻ ���� �ؽ�ó ���ø�
    float2 uv = _in.vUV;
    float4 baseShield = g_tex_0.Sample(g_sam_0, uv);
    
    // ���� ���� ���� ���
    float2 center = float2(0.5f, 0.5f);
    float dist = length(uv - center) * 2.0f;
    
    // ���� ������ ��� �ȼ��� ����
    if (dist > 1.0f)
        discard;

    // ��ü�� ������ ������ ��� �ð� ��� (0~6�� �ݺ�)
    float elapsedTime = fmod(g_Time_Engine - g_float_0, 8.0f);
    
   // 2�� ���ĺ��� �����ϵ��� ����
    float timeProgress = max(0.0f, elapsedTime - 1.0f) / 7.0f; // 8�� - 2�� = 6�ʷ� ������
    float timeAlpha = (1.0f - pow(abs(timeProgress - 0.5f) * 2.0f, 0.5f));
    
    // ������ �ؽ�ó UV ���
    float2 noiseUV1 = uv + float2(sin(timeProgress * 3.0f) * 0.2f, -timeProgress * 0.6f);
    float2 noiseUV2 = uv * 1.3f + float2(cos(timeProgress * 2.4f) * 0.3f, -timeProgress * 0.66f);
    
    // ������ �ؽ�ó ���ø�
    float4 noise1 = g_tex_1.Sample(g_sam_0, noiseUV1);
    float4 noise2 = g_tex_1.Sample(g_sam_0, noiseUV2);
    
    // �ұ�Ģ�� ���� ���� ���� (50~60% ����)
    float yLimit = lerp(0.2f, 0.8f, noise1.r);
    float yMask = 1.0f - smoothstep(yLimit - 0.1f, yLimit + 0.1f, uv.y);
    
    // ������ ���� ���� �� ���� ���� ����
    float pattern = lerp(noise2.r, noise2.r, 0.7f);
    pattern = pattern * yMask;
    
// ���ζ� ���� ���� �� ���� (���� ���� ����)
    float3 color1 = float3(0.0f, 0.8f, 0.7f); // ä�� ����
    float3 color2 = float3(0.1f, 0.7f, 0.8f); // ä�� ����
    float3 auroraColor = lerp(color1, color2, pattern) * 1.2; // �߰� ���� ����

// ���� ȿ�� ���� ��� (���� ����)
    float intensity = pattern * timeAlpha * 2.5f; // 2.0f -> 1.2f
    float4 auroraEffect = float4(auroraColor, intensity * 0.7f); // ���İ��� ����

// ���� �� ���� ����
    float4 finalColor = baseShield;
    if (baseShield.a < 0.1f)
        finalColor = auroraEffect;
    else
        finalColor = baseShield + (auroraEffect * 0.8f); // ���� ���� ����

// ���� ���İ��� ����
    finalColor.a = max(baseShield.a, auroraEffect.a * 0.6f);
    
    return finalColor;
}
#endif