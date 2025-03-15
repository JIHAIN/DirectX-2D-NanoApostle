#ifndef _MYFUNC
#define _MYFUNC

#include "value.fx"
#include "struct.fx"


//-------------------------------------
// ������ �Լ���
//-------------------------------------
// �⺻ ������ �Լ�
float BasicNoise(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898, 78.233))) * 43758.5453123);
}

// ������ ��Ÿ�긦 �����ִ� �Լ���
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

// FBM �Լ�
float fbm(float2 st)
{
    float value = 0.0;
    
    // ù ��° ��Ÿ�� (�⺻ ����)
    value += BasicNoise(st) * 1.0;
    
    // �� ��° ��Ÿ�� (�� ���� ������)
    value += BasicNoise(st * 2.0) * 0.5;
    
    // �� ��° ��Ÿ�� (�� ���� ������)
    value += BasicNoise(st * 4.0) * 0.25;
    
    // �� ��° ��Ÿ�� (���� ���� ������)
    value += BasicNoise(st * 8.0) * 0.125;
    
    // 0~1 ���̷� ����ȭ
    return value / (1.0 + 0.5 + 0.25 + 0.125);
}


//---------------------------------
//����ȿ����
//---------------------------------

// ��Ÿ�µ��� �˺����� ȿ��
float4 GetBurningDarkRed(float2 uv)
{
    // ����� �̿��� �����̴� ȿ��
    float noise = fbm(uv * 3.0 + float2(g_DeltaTime * 0.5, g_DeltaTime * 0.3));
    
    // �⺻ �˺�����
    float4 baseColor = float4(0.4, 0.02, 0.02, 1.0);
    
    // ��Ÿ�� ȿ���� ���� ���� ������
    float4 glowColor = float4(0.8, 0.1, 0.1, 1.0);
    
    // ����� ���� ���� ȥ��
    return lerp(baseColor, glowColor, noise * 0.3);
}



#endif 