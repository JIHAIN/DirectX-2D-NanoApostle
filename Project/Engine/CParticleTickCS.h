#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CParticleTickCS :
    public CComputeShader
{
private:
    CStructuredBuffer*  m_ParticleBuffer;
    CStructuredBuffer*  m_SpawnCountBuffer;
    CStructuredBuffer*  m_ModuleBuffer;

    Ptr<CTexture>       m_NoiseTex1;
    Ptr<CTexture>       m_NoiseTex2;
    Ptr<CTexture>       m_NoiseTex3;
    Vec3                m_ParticleWorldPos;

public:
    void SetParticleBuffer(CStructuredBuffer* _Buffer) { m_ParticleBuffer = _Buffer; }
    void SetSpawnCount(CStructuredBuffer* _Buffer) { m_SpawnCountBuffer = _Buffer; }
    void SetModuleBuffer(CStructuredBuffer* _ModuleBuffer) { m_ModuleBuffer = _ModuleBuffer; }

    void SetParticleWorldPos(Vec3 _Pos) { m_ParticleWorldPos = _Pos; }
    void SetNoiseTexture1(Ptr<CTexture> _Noise) { m_NoiseTex1 = _Noise; }
    void SetNoiseTexture2(Ptr<CTexture> _Noise) { m_NoiseTex2 = _Noise; }
    void SetNoiseTexture3(Ptr<CTexture> _Noise) { m_NoiseTex3 = _Noise; }

private:
    virtual int Binding() override;
    virtual void CalcGroupCount() override;
    virtual void Clear() override;

public:
    CParticleTickCS();
    ~CParticleTickCS();
};

