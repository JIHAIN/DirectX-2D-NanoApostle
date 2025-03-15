#include "pch.h"
#include "TransformUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI("TransformUI", COMPONENT_TYPE::TRANSFORM)	
{
	
}

TransformUI::~TransformUI()
{

}

void TransformUI::Render_Update()
{
	ComponentTitle("Transform");

	CTransform* pTransform = GetTargetObject()->Transform();

	Vec3 vPos = pTransform->GetRelativePos();
	Vec3 vScale = pTransform->GetRelativeScale();
	Vec3 vRot  = pTransform->GetRelativeRotation();

	ImGui::Text("Position");
	ImGui::SameLine(100);	
	if (ImGui::DragFloat3("##Pos", vPos))
	{
		pTransform->SetRelativePos(vPos);
	}

	ImGui::Text("Scale");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##Scale", vScale))
	{
		pTransform->SetRelativeScale(vScale);
	}

	ImGui::Text("Rotation");
	ImGui::SameLine(100);	

	vRot = (vRot / XM_PI) * 180.f;

	if (ImGui::DragFloat3("##Rotation", vRot, 0.1f))
	{
		vRot = (vRot / 180) * XM_PI;
		pTransform->SetRelativeRotation(vRot);
	}
}