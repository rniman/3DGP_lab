#include "stdafx.h"
#include "GraphicsPipeline.h"

XMFLOAT4X4* CGraphicsPipeline::m_pxmf4x4World{ nullptr };
XMFLOAT4X4* CGraphicsPipeline::m_pxmf4x4ViewProject{ nullptr };
CViewport* CGraphicsPipeline::m_pViewport{ nullptr };

void CGraphicsPipeline::SetWorldTransform(XMFLOAT4X4* pxmf4x4World)
{
	m_pxmf4x4World = pxmf4x4World;
}

void CGraphicsPipeline::SetViewPerspectiveProjectTransform(XMFLOAT4X4* pxmf4x4ViewPerspectiveProject)
{ 
	m_pxmf4x4ViewProject = pxmf4x4ViewPerspectiveProject;
}

//void CGraphicsPipeline::SetViewOrthographicProjectTransform(XMFLOAT4X4* pxmf4x4OrthographicProject)
//{
//	m_pxmf4x4ViewProject = pxmf4x4OrthographicProject;
//}

void CGraphicsPipeline::SetViewport(CViewport* pViewport)
{
	m_pViewport = pViewport;
}

//XMFLOAT3 CGraphicsPipeline::Transform(XMFLOAT3& xmf3Model)
//{
//	XMFLOAT3 xmf3Project = Project(xmf3Model);
//	XMFLOAT3 f3Screen = ScreenTransform(xmf3Project);
//
//	return(f3Screen);
//}

XMFLOAT3 CGraphicsPipeline::Project(XMFLOAT3& xmf3Model)
{
	//CScene::Render과 CGameObject::Render에서 가져온 변환 행렬들을 곱해 최종 변환 행렬을 만들어 낸다.
	XMMATRIX xmmtxModelToProject = XMMatrixMultiply(XMLoadFloat4x4(m_pxmf4x4World), XMLoadFloat4x4(m_pxmf4x4ViewProject));
	XMFLOAT3 xmf3Project;
	XMStoreFloat3(&xmf3Project, XMVector3TransformCoord(XMLoadFloat3(&xmf3Model), xmmtxModelToProject));

	return(xmf3Project);
}

XMFLOAT3 CGraphicsPipeline::ScreenTransform(XMFLOAT3& xmf3Project)
{
	XMFLOAT3 f3Screen = xmf3Project;

	float fHalfWidth = m_pViewport->GetnWidth() * 0.5f;
	float fHalfHeight = m_pViewport->GetnHeight() * 0.5f;
	f3Screen.x = m_pViewport->GetnLeft() + (xmf3Project.x * fHalfWidth) + fHalfWidth;
	f3Screen.y = m_pViewport->Getntop() + (-xmf3Project.y * fHalfHeight) + fHalfHeight;

	return(f3Screen);
}
