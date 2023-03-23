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
	m_ppObjects = new CGameObject * [m_nObjects];

	m_ppObjects[0] = new CGameObject();
	m_ppObjects[0]->SetMesh(pCubeMesh);
	m_ppObjects[0]->SetColor(RGB(255, 0, 0));
	m_ppObjects[0]->SetPosition(-13.5f, 0.0f, +14.0f);
	m_ppObjects[0]->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));	//회전축 설정
	m_ppObjects[0]->SetRotationSpeed(90.0f);
	m_ppObjects[0]->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));	//이동 방향 설정
	m_ppObjects[0]->SetMovingSpeed(0.0f);	//이동 속도

	m_ppObjects[1] = new CGameObject();
	m_ppObjects[1]->SetMesh(pCubeMesh);
	m_ppObjects[1]->SetColor(RGB(0, 0, 255));
	m_ppObjects[1]->SetPosition(+13.5f, 0.0f, +14.0f);
	m_ppObjects[1]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_ppObjects[1]->SetRotationSpeed(180.0f);
	m_ppObjects[1]->SetMovingDirection(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	m_ppObjects[1]->SetMovingSpeed(0.0f);

	m_ppObjects[2] = new CGameObject();
	m_ppObjects[2]->SetMesh(pCubeMesh);
	m_ppObjects[2]->SetColor(RGB(0, 255, 0));
	m_ppObjects[2]->SetPosition(0.0f, +5.0f, 20.0f);
	m_ppObjects[2]->SetRotationAxis(XMFLOAT3(1.0f, 0.0f, 1.0f));
	m_ppObjects[2]->SetRotationSpeed(30.15f);
	m_ppObjects[2]->SetMovingDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	m_ppObjects[2]->SetMovingSpeed(0.0f);

	m_ppObjects[3] = new CGameObject();
	m_ppObjects[3]->SetMesh(pCubeMesh);
	m_ppObjects[3]->SetColor(RGB(0, 255, 255));
	m_ppObjects[3]->SetPosition(0.0f, 0.0f, 40.0f);
	m_ppObjects[3]->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_ppObjects[3]->SetRotationSpeed(40.6f);
	m_ppObjects[3]->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_ppObjects[3]->SetMovingSpeed(0.0f);

	m_ppObjects[4] = new CGameObject();
	m_ppObjects[4]->SetMesh(pCubeMesh);
	m_ppObjects[4]->SetColor(RGB(128, 0, 255));
	m_ppObjects[4]->SetPosition(10.0f, 10.0f, 50.0f);
	m_ppObjects[4]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_ppObjects[4]->SetRotationSpeed(50.06f);
	m_ppObjects[4]->SetMovingDirection(XMFLOAT3(0.0f, 1.0f, 1.0f));
	m_ppObjects[4]->SetMovingSpeed(0.0f);
}

void CScene::ReleaseObjects()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i])
		{
			delete m_ppObjects[i];
		}
	}
	if (m_ppObjects)
	{
		delete[] m_ppObjects;
	}
}

void CScene::Animate(float fElapsedTime)
{
	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i]->Animate(fElapsedTime);
	}
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetViewport(pCamera->GetpViewport());
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(pCamera->GetpViewPerspectiveProject());

	for (int i = 0; i < m_nObjects; i++)
	{
		m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
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