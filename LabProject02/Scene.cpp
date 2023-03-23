#include "stdafx.h"
#include "Scene.h"
#include "GraphicsPipeline.h"

CScene::CScene(CPlayer* pPlayer)	//씬 생성하면서 플레이어 설정
	: m_pPlayer{ pPlayer }
{}

CScene::~CScene() {}

void CScene::BuildObjects()
{
	// 큐브 매쉬 생성(공유된다)
	CCubeMesh* pCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);

	m_nObjects = 5;
	m_vpObjects.resize(m_nObjects, nullptr);
	//m_vpObjects = new CGameObject * [m_nObjects];

	m_vpObjects[0] = new CGameObject();
	m_vpObjects[0]->SetMesh(pCubeMesh);
	m_vpObjects[0]->SetColor(RGB(255, 0, 0));
	m_vpObjects[0]->SetPosition(-13.5f, 0.0f, +14.0f);
	m_vpObjects[0]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));	//회전축 설정
	m_vpObjects[0]->SetRotationSpeed(90.0f);
	m_vpObjects[0]->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));	//이동 방향 설정
	m_vpObjects[0]->SetMovingSpeed(0.0f);	//이동 속도

	m_vpObjects[1] = new CGameObject();
	m_vpObjects[1]->SetMesh(pCubeMesh);
	m_vpObjects[1]->SetColor(RGB(0, 0, 255));
	m_vpObjects[1]->SetPosition(+13.5f, 0.0f, +14.0f);
	m_vpObjects[1]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_vpObjects[1]->SetRotationSpeed(180.0f);
	m_vpObjects[1]->SetMovingDirection(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	m_vpObjects[1]->SetMovingSpeed(0.0f);

	m_vpObjects[2] = new CGameObject();
	m_vpObjects[2]->SetMesh(pCubeMesh);
	m_vpObjects[2]->SetColor(RGB(0, 255, 0));
	m_vpObjects[2]->SetPosition(0.0f, +5.0f, 20.0f);
	m_vpObjects[2]->SetRotationAxis(XMFLOAT3(1.0f, 0.0f, 1.0f));
	m_vpObjects[2]->SetRotationSpeed(30.15f);
	m_vpObjects[2]->SetMovingDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	m_vpObjects[2]->SetMovingSpeed(0.0f);

	m_vpObjects[3] = new CGameObject();
	m_vpObjects[3]->SetMesh(pCubeMesh);
	m_vpObjects[3]->SetColor(RGB(0, 255, 255));
	m_vpObjects[3]->SetPosition(0.0f, 0.0f, 40.0f);
	m_vpObjects[3]->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_vpObjects[3]->SetRotationSpeed(40.6f);
	m_vpObjects[3]->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_vpObjects[3]->SetMovingSpeed(0.0f);

	m_vpObjects[4] = new CGameObject();
	m_vpObjects[4]->SetMesh(pCubeMesh);
	m_vpObjects[4]->SetColor(RGB(128, 0, 255));
	m_vpObjects[4]->SetPosition(10.0f, 10.0f, 50.0f);
	m_vpObjects[4]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_vpObjects[4]->SetRotationSpeed(50.06f);
	m_vpObjects[4]->SetMovingDirection(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_vpObjects[4]->SetMovingSpeed(0.0f);
}

void CScene::ReleaseObjects()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_vpObjects[i])
		{
			delete m_vpObjects[i];
		}
	}
}

void CScene::Animate(float fElapsedTime)
{
	for (int i = 0; i < m_nObjects; i++)
	{
		m_vpObjects[i]->Animate(fElapsedTime);
	}
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	//카메라의 뷰포트를 가져온다.
	CGraphicsPipeline::SetViewport(pCamera->GetpViewport());
	//카메라의 카메라 * 투영 행렬을 가져온다.
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(pCamera->GetpViewPerspectiveProject());

	for (int i = 0; i < m_nObjects; i++)
	{
		m_vpObjects[i]->Render(hDCFrameBuffer);
	}
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			break;
		}
		case 'A':
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}