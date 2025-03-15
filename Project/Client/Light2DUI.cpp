#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CLight2D.h>
#include <Engine/CTransform.h>


Light2DUI::Light2DUI()
	: ComponentUI("Light2DUI", COMPONENT_TYPE::LIGHT2D)
{
}

Light2DUI::~Light2DUI()
{

}

void Light2DUI::Render_Update()
{
	CLight2D* pLight2D = GetTargetObject()->Light2D();

	ComponentTitle("Light2D");

	tLight2DInfo Light2DInfo = pLight2D->GetLight2DInfo();

	//==================
	// PROJ_TYPE
	ImGui::Text("Light2D Type");
	ImGui::SameLine(120);

	const char* szProjType[3] = { "DIRECTIONAL", "POINT" , "SPOT" };
	int ProjType = Light2DInfo.Type;

	ImGui::Combo("##combo", &ProjType, szProjType, 3);
	pLight2D->SetLightType((LIGHT_TYPE)ProjType);
	//==================

	//=======================
	// Position
	ImGui::Text("Position");
	ImGui::SameLine(120);

	Vec3 LightPos = pLight2D->Transform()->GetRelativePos();
	ImGui::DragFloat3("##Position", LightPos);
	pLight2D->Transform()->SetRelativePos(LightPos);
	// =======================

	//=======================
	// Color
	ImGui::Text("Color");
	ImGui::SameLine(120);

	if (ImGui::ColorEdit3("##Color", Light2DInfo.vColor, ImGuiColorEditFlags_DisplayHSV))
	{
		pLight2D->SetLightColor(Light2DInfo.vColor);
	}
	//=======================

	//=======================
	// Radius
	ImGui::Text("Radius");
	ImGui::SameLine(120);

	ImGui::DragFloat("##Radius", &Light2DInfo.Radius);
	pLight2D->SetRadius(Light2DInfo.Radius);
	// =======================

	//======================================
	// Angle
	// ���� Ÿ����SPOT�� �ƴϸ� ��Ȱ��ȭ
	ImGui::BeginDisabled(ProjType != (int)LIGHT_TYPE::SPOT);

	ImGui::Text("Angle");
	ImGui::SameLine(120);

	ImGui::DragFloat("##Angle", &Light2DInfo.Angle);
	pLight2D->SetAngle(Light2DInfo.Angle);

	ImGui::EndDisabled();
	//======================================

	//======================================
	// Dir
	// ���� Ÿ����SPOT�� �ƴϸ� ��Ȱ��ȭ
	ImGui::BeginDisabled(ProjType != (int)LIGHT_TYPE::SPOT);

	ImGui::Text("Dir");
	ImGui::SameLine(120);

	// ���� ������ ������ ��ȯ
	float angle = atan2(Light2DInfo.vDir.y, Light2DInfo.vDir.x);

	angle = angle * 180.0f / XM_PI; // ���ȿ��� ���� ��ȯ

	if (ImGui::DragFloat("##Dir", &angle, 1.f, -180.0f, 180.0f))
	{
		// ������ �ٽ� �������� ��ȯ
		float radians = angle * XM_PI / 180.0f;

		// �� ���� ���� ���
		Vec3 newDir(cos(radians), sin(radians), Light2DInfo.vDir.z);

		// ����ȭ�� �� ���� ����
		pLight2D->SetDir(newDir.Normalize());
	}

	ImGui::EndDisabled();
	//=======================================




}