#pragma once

// ConstBuffer
enum class CB_TYPE
{
	TRANSFORM,
	MATERIAL,
	SPRITE,
	GLOBAL,
	MOUSE,
	END,
};


// RasterizerState Type
enum class RS_TYPE
{
	CULL_BACK,	// 뒷면 컬링 모드
	CULL_FRONT, // 앞면 컬링 모드
	CULL_NONE,  // 컬링하지 않음
	WIRE_FRAME, // WireFrame 모드
	END,
};


// BlendState
enum class BS_TYPE
{
	DEFAULT,				// 강제출력
	ALPHABLEND,				// 알파값에 따른 블렌딩
	ALPHABLEND_COVERAGE,	// ALPHABLEND + AlphaToCoverage
	ONE_ONE,				// 1:1 블렌딩

	END,
};

// DepthStencilState
enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,

	GREATER,			// Decal, Volumesh, 겹쳐있는 영역

	NO_TEST,			// 깊이판정 X
	NO_WRITE,			// 깊이판정 Less O, 깊이 기록 X
	NO_TEST_NO_WRITE,	// 깊이판정 X, 깊이기록 X

	END,
};




// 쉐이더 동작 분류
enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,			// 불투명
	DOMAIN_MASK,			// 불투명 or 투명
	DOMAIN_TRANSPARENT,		// 반투명 + 투명
	DOMAIN_EFFECT,          // 이펙트
	DOMAIN_PARTICLE,		// 반투명 + 투명
	DOMAIN_POSTPROCESS,		// 후처리
	DOMAIN_NONE,
};

enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
};

enum ASSET_TYPE
{
	MESH,
	MESH_DATA,
	TEXTURE,
	SOUND,
	PREFAB,
	FLIPBOOK,
	SPRITE,
	MATERIAL,
	GRAPHIC_SHADER,
	COMPUTE_SHADER,
	END,
};

extern const char* ASSET_TYPE_STRING[];

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// 방향성 광원		태양광, 달
	POINT,			// 점광원			전구, 형광등, 스킬 이펙트, 횃불
	SPOT,			// 스포트 라이트		손전등, 
};


enum class COMPONENT_TYPE
{
	TRANSFORM,			// 위치, 크기, 회전, 부모자식 계층구조
	COLLIDER2D,			// 2D 충돌체
	COLLIDER3D,			// 3D 충돌체
	FLIPBOOKPLAYER,		// 2D Animation(FlipBook) 관리 및 재생
	ANIMATOR3D,			// 3D Animation 관리 및 재생
	CAMERA,				// 카메라 기능
	LIGHT2D,			// 2D 광원
	LIGHT3D,			// 3D 광원
	STATEMACINE,		// 상태 관리		

	RIGIDBODY,			// 리지드 바디
	MESHRENDER,			// 기본적인 렌더링 기능
	TILEMAP,			// 2D 타일기반 렌더링
	REPEATSPRITE,		// 단순 스프라이트 반복
	PARTICLE_SYSTEM,	// 입자(Particle) 시뮬레이션 및 렌더링
	SKYBOX,				// 3D 배경
	DECAL,				// 깊이 체크, 문양 렌더링
	LANDSCALE,			// 3D 지형 시뮬레이션 및 렌더링

	END,

	SCRIPT,				// 대본
};



// 카메라 투영 (Projection)
enum PROJ_TYPE
{
	PERSPECTIVE,	// 원근 투영
	ORTHOGRAPHIC,	// 직교 투영
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,
	CROSS,
	LINE,
	MOUSE,
};


enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEX_ARR_0,
	TEX_ARR_1,
	TEX_ARR_2,
	TEX_ARR_3,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_END,
};



// Task
enum class TASK_TYPE
{
	// 0 : Parent Address, 1 : Child Address
	ADD_CHILD,

	// 0 : Object Address, 1 : Layer Index, 2 : ChildMove
	CREATE_OBJECT, 

	// 0 : Object Address
	DELETE_OBJECT,

	// 0 : Level Address
	CHANGE_LEVEL,

	// 0 : NextState
	CHANGE_LEVEL_STATE,

	// 0 : Asset Adress
	DELETE_ASSET,
};


enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
	NONE,
};

enum class LEVEL_TYPE
{
	MAIN,
	TRAP,
	BOSS,

	END, 
};

enum class PARTICLE_MODULE
{
	SPAWN,
	SPAWN_BURST,
	ADD_VELOCITY,
	SCALE,
	DRAG,
	NOISE_FORCE,
	RENDER,

	END,
};

enum class CPlayer_Dash_State
{
	Left,
	Right,
	Up,
	Down,
	LeftUP,
	LeftDOWN,
	RightUP,
	RightDOWN,
};

enum class Player_Ani_State
{
	Player_Intro1,
	Player_Intro2,
	Player_Intro3,

	Idle_Down,
	Idle_Up,
	Idle_Right,

	Move_Down,
	Move_Up,
	Move_Right,

	Dash_Down,
	Dash_Right,
	Dash_Up,

	Atk_Down1,
	Atk_Down2,
	Atk_Down3,

	Atk_Up1,
	Atk_Up2,
	Atk_Up3,

	Atk_Right1,
	Atk_Right2,
	Atk_Right3,

	Shield_Right,
	Shield_Up,
	Shield_Down,

	Death,
	EndBoss,
	appeared,
	Warp,
	End,
};

enum class SCRIPT_PARAM
{
	INT,
	FLOAT,
	VEC2,
	VEC4,

	TEXTURE,
	PREFAB,
};

enum class Boss_Ani_State
{
	Boss_Bottom_Atk,
	Boss_Charge_Atk,
	Boss_Groggy,
	Boss_Groggying,
	Boss_Jump_Atk,
	Boss_Jump_Atk_landing,
	Boss_Nor_Atk1,
	Boss_Nor_Atk2,
	Boss_Nor_Atk3,
	Boss_Idle,
	Boss_Dash,
	Boss_Execute,
};