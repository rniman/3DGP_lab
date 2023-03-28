#pragma once

#include "Player.h"
#include "Scene.h"
#include "Timer.h"

class CGameFramework
{
public:
	CGameFramework();
	~CGameFramework();

	// 생성
	void OnCreate(HINSTANCE hInstance, HWND hMainWnd);
	// 소멸
	void OnDestroy();

	// 프레임버퍼 생성
	void BuildFrameBuffer();
	// 프레임버퍼 청소
	void ClearFrameBuffer(DWORD dwColor);
	// 고속복사
	void PresentFrameBuffer();

	// 오브젝트 생성
	void BuildObjects();
	void ReleaseObjects();

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	// 핵심 프레임 워크
	void ProcessInput();
	void AnimateObjects();
	void FrameAdvance();

	//void SetActive(bool bActive);

private:
	HINSTANCE m_hInstance{ nullptr };
	HWND m_hWnd{ nullptr };

	bool m_bActive = true;

	RECT m_rcClient{};

    HDC	m_hDCFrameBuffer{ nullptr };
    HBITMAP	m_hBitmapFrameBuffer{ nullptr };
    HBITMAP	m_hBitmapSelect{ nullptr };

	CPlayer* m_pPlayer{ nullptr };
	CScene*	m_pScene{ nullptr };

	CGameTimer m_GameTimer{};

	POINT m_ptOldCursorPos{};

	_TCHAR m_pszFrameRate[50];
};

