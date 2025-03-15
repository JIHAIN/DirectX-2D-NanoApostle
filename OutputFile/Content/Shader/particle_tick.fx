#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "func.fx"

struct tSpawnCount
{
    int Count;
    int3 padding;
};

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> SpawnCountBuffer : register(u1);
Texture2D NoiseTex1 : register(t20);
Texture2D NoiseTex2 : register(t22);
Texture2D NoiseTex3 : register(t23);
StructuredBuffer<tParticleModule> Module : register(t21);

#define MAX_COUNT           g_int_0
#define Particle            ParticleBuffer[_id.x]
#define ParticleObjectPos   g_vec4_0.xyz

// Module Check
#define SpawnModule         Module[0].Module[0]
#define SpawnBurstModule    Module[0].Module[1]
#define SpawnShapeType      Module[0].SpawnShape
#define AddVelocityModule   Module[0].Module[2]
#define ScaleModule         Module[0].Module[3]
#define DragModule          Module[0].Module[4]
#define NoiseForce          Module[0].Module[5]
#define Render              Module[0].Module[6]

// 1. 스레드간의 동기화처리, 파티클 활성화 문제
// 2. 컴퓨트 쉐이더에서 랜덤 구현하는 방법
//  - 시간을 난수생성의 키값으로 쓸 수 가 없다.
//  - 스레드의 유일한 차이인, 스레드 id 값을 키로 난수를 생성 시켜야 한다.
[numthreads(1024, 1, 1)]
void CS_ParticleTick(int3 _id : SV_DispatchThreadID)
{
    if (MAX_COUNT <= _id.x)
        return;
    
    if (false == Particle.Active)
    {
        int SpawnCount = SpawnCountBuffer[0].Count;
                
        while (0 < SpawnCount)
        {
            int Origin = 0;
            
            InterlockedCompareExchange(SpawnCountBuffer[0].Count
                                      , SpawnCount
                                      , SpawnCountBuffer[0].Count - 1
                                      , Origin);

            if (SpawnCount == Origin)
            {
                // 파티클의 위치를 특정 범위내에서 랜덤한 위치로 잡아준다.                
                float2 vUV = (float2) 0.f;
                
                // 스레드를 UV 로 맵핑하기위해서 ID 를 0~1 범위로 정규화     
                //float3 vRandom0 = GetRandom(NoiseTex1, _id.x, MAX_COUNT);
                //float3 vRandom1 = GetRandom(NoiseTex2, _id.x, MAX_COUNT);
                //float3 vRandom2 = GetRandom(NoiseTex3, _id.x, MAX_COUNT);
                float3 vRandom0 = GetRandom(NoiseTex1, _id.x, MAX_COUNT);
                float3 vRandom1 = GetRandom(NoiseTex2, _id.x * 1.37f + MAX_COUNT, MAX_COUNT * 2); // 다른 입력값
                float3 vRandom2 = GetRandom(NoiseTex3, _id.x * 0.73f + MAX_COUNT * 2, MAX_COUNT * 3); // 더 다른 입력값

                                
                float3 vSpawnPos = (float3) 0.f;
                
                // 0 : Box,  1 : Sphere
                if (0 == SpawnShapeType)
                {
                    vSpawnPos.x = vRandom0.x * Module[0].SpawnShapeScale.x - (Module[0].SpawnShapeScale.x / 2.f);
                    vSpawnPos.y = vRandom0.y * Module[0].SpawnShapeScale.y - (Module[0].SpawnShapeScale.y / 2.f);
                    vSpawnPos.z = vRandom0.z * Module[0].SpawnShapeScale.z - (Module[0].SpawnShapeScale.z / 2.f);
                }
                // 쌤 코드
                //else if (1 == SpawnShapeType)
                //{
                //    float fRadius = Module[0].SpawnShapeScale.x;
                //    float fBlockRadius = Module[0].BlockSpawnShapeScale.x;
                //    float fDifferRadius = fRadius - fBlockRadius;
                        
                //    vSpawnPos = normalize(vRandom1 - 0.5f) * fDifferRadius * vRandom2.x 
                //                    + normalize(vRandom1 - 0.5f) * fBlockRadius;
                //}
                // Sphere 타입일 때의 스폰 위치 계산 개선
                else if (1 == SpawnShapeType)
                {
                    float fRadius = Module[0].SpawnShapeScale.x;
                    float fBlockRadius = Module[0].BlockSpawnShapeScale.x;
    
                     // 구면 좌표계 사용
                    float theta = (vRandom0.x + vRandom1.y) * PI; // 방위각
                    float phi = (vRandom1.x + vRandom2.y) * 2.0f * PI; // 고도각
                    float radius = fBlockRadius +
                    (fRadius - fBlockRadius) *
                    ((vRandom0.z + vRandom1.z + vRandom2.x) / 3.0f); // 세 노이즈의 평균
                 
                    // 구면 좌표계를 직교 좌표계로 변환
                     vSpawnPos = float3(radius * sin(theta) * cos(phi),radius 
                                        * sin(theta) * sin(phi),radius * cos(theta));
                }
                
                                          
                // Add Velocity Module
                Particle.vVelocity = (float3) 0.f;
                
                //쌤 코드
                //if (AddVelocityModule)
                //{      
                //    float fSpeed = Module[0].AddMinSpeed + (Module[0].AddMaxSpeed - Module[0].AddMinSpeed) * vRandom2.x;              

                //    // Random
                //    if (0 == Module[0].AddVelocityType)                        
                //        Particle.vVelocity = normalize(vRandom2 - 0.5f) * fSpeed;
                    
                //    // FromCenter 쌤꺼
                //    else if (1 == Module[0].AddVelocityType)                        
                //        Particle.vVelocity = normalize(vSpawnPos) * fSpeed;
                                
                //    // ToCenter
                //    else if (2 == Module[0].AddVelocityType)
                //        Particle.vVelocity = -normalize(vSpawnPos) * fSpeed;
                                   
                //    // Fixed
                //    else
                //        Particle.vVelocity = normalize(Module[0].AddVelocityFixedDir) * fSpeed;
                //}
                
                // FromCenter와 ToCenter의 방향 계산 개선
                if (AddVelocityModule)
                {
                    float fSpeed = Module[0].AddMinSpeed +
                  (Module[0].AddMaxSpeed - Module[0].AddMinSpeed) *
                  ((vRandom0.x + vRandom1.x + vRandom2.x) / 3.0f); // 세 노이즈의 평균
    
                    if (0 == Module[0].AddVelocityType)
                    {
                        // Random - 세 노이즈를 조합
                        float3 randomDir = normalize(
                            (vRandom0 + vRandom1 + vRandom2) / 3.0f - 0.5f
                        );
                        Particle.vVelocity = randomDir * fSpeed;
                    }
                    else if (1 == Module[0].AddVelocityType)
                    {
                        // FromCenter - 약간의 랜덤성 추가
                        float3 baseDir = normalize(vSpawnPos);
                        float3 randomOffset = (vRandom1 - 0.5f) * 0.2f; // 20% 랜덤 편차
                        Particle.vVelocity = normalize(baseDir + randomOffset) * fSpeed;
                    }
                    else if (2 == Module[0].AddVelocityType)
                    {
                        // ToCenter - 약간의 랜덤성 추가
                        float3 baseDir = -normalize(vSpawnPos);
                        float3 randomOffset = (vRandom2 - 0.5f) * 0.2f; // 20% 랜덤 편차
                        Particle.vVelocity = normalize(baseDir + randomOffset) * fSpeed;
                    }
                    else
                    {
                        // Fixed - 약간의 랜덤성 추가
                        float3 baseDir = normalize(Module[0].AddVelocityFixedDir);
                        float3 randomOffset = (vRandom0 - 0.5f) * 0.1f; // 10% 랜덤 편차
                        Particle.vVelocity = normalize(baseDir + randomOffset) * fSpeed;
                    }
                }
                                   
                Particle.vLocalPos = vSpawnPos;
                Particle.vWorldPos = Particle.vLocalPos + ParticleObjectPos.xyz;
                Particle.vWorldInitScale = (Module[0].vSpawnMaxScale - Module[0].vSpawnMinScale) * vRandom0.x + Module[0].vSpawnMinScale;
                                    
                Particle.vColor = Module[0].vSpawnColor;
                Particle.Mass = 1.f;
                
                Particle.Age = 0.f;
                Particle.NormalizedAge = 0;
                Particle.Life = (Module[0].MaxLife - Module[0].MinLife) * vRandom1.y + Module[0].MinLife;
                Particle.Active = 1;
                
                break;
            }
            
            SpawnCount = SpawnCountBuffer[0].Count;
        }
    }
     
    // 스레드가 담당하는 파티클이 활성화 상태면 Tick 을 수행한다.
    else
    {
        Particle.vForce = float3(0.f, 0.f, 0.f);
        
        // Noise Force Module
        if (NoiseForce)
        {
            // 일정 시간마다 Noise Force 의 방향을 랜덤하게 구함
            if (Module[0].NoiseForceTerm <= Particle.NoiseForceAccTime)
            {
                Particle.NoiseForceAccTime -= Module[0].NoiseForceTerm;
                
                float3 vRandom = GetRandom(NoiseTex1, _id.x, MAX_COUNT);
                float3 vNoiseForce = normalize(vRandom.xyz - 0.5f);

                Particle.NoiseForceDir = vNoiseForce;
            }
                        
            Particle.vForce += Particle.NoiseForceDir * Module[0].NoiseForceScale;
            Particle.NoiseForceAccTime += g_DT_Engine;
        }
        
        // Particle 에 주어진 힘에 따른 가속도 계산
        float3 vAccel = Particle.vForce / Particle.Mass;
        
        // 가속도에 따른 속도의 변화
        Particle.vVelocity += vAccel * g_DT_Engine;
                        
        // Velocity 에 따른 이동 구현        
        if (0 == Module[0].SpaceType)
        {
            // Space 가 Local 이라면
            Particle.vLocalPos += Particle.vVelocity * g_DT_Engine;
            Particle.vWorldPos = Particle.vLocalPos + ParticleObjectPos.xyz;
        }
        else
        {
            Particle.vLocalPos += Particle.vVelocity * g_DT_Engine;
            Particle.vWorldPos += Particle.vVelocity * g_DT_Engine;
        }
        
        // Scale 모듈에 따른 현재 크기 계산
        Particle.vWorldCurrentScale = Particle.vWorldInitScale;
        if (ScaleModule)
            Particle.vWorldCurrentScale = ((Module[0].EndScale - Module[0].StartScale) * Particle.NormalizedAge + Module[0].StartScale) * Particle.vWorldInitScale;
        
        
        if (DragModule)
        {
            if (Particle.NormalizedAge < Module[0].DestNormalizedAge)
            {
                // 기울기
                // (목적 속력 - 현재 속력) / (목적 NA - 현재 NA)
                float Gradient = (Module[0].LimitSpeed - length(Particle.vVelocity)) / (Module[0].DestNormalizedAge - Particle.NormalizedAge);
                float NADT = g_DT_Engine / Particle.Life;
            
                float NewSpeed = length(Particle.vVelocity) + (Gradient * NADT);
                Particle.vVelocity = normalize(Particle.vVelocity) * NewSpeed;
            }
        }
        
        if (Render)
        {
            Particle.vColor.rgb = (Module[0].EndColor - Module[0].vSpawnColor.rgb) * Particle.NormalizedAge + Module[0].vSpawnColor.rgb;

            if (Module[0].FadeOut)
            {
                float fRatio = saturate(1.f - (Particle.NormalizedAge - Module[0].StartRatio) / (1.f - Module[0].StartRatio));
                Particle.vColor.a = fRatio;
            }
        }
        
        Particle.Age += g_DT_Engine;
        Particle.NormalizedAge = Particle.Age / Particle.Life;
        if (Particle.Life <= Particle.Age)
        {
            Particle.Active = 0;
        }
    }
}

#endif