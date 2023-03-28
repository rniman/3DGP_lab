#pragma once

//#include "Mesh.h"

// 화면으로 변환하기위한 정보
class CViewport
{
public:
	CViewport();
	virtual ~CViewport();

	int GetnLeft() const;
	int Getntop() const;
	int GetnWidth() const;
	int GetnHeight() const;

	//뷰포트 설정
	void SetViewport(int nLeft, int nTop, int nWidth, int nHeight);

private:
	int	m_nLeft{ 0 };
	int	m_nTop{ 0 };
	int	m_nWidth{ 0 };
	int	m_nHeight{ 0 };

};

/// <CViewport>
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// <CCamera>

// 카메라 클래스
class CCamera
{
public:
	CCamera();
	virtual ~CCamera();

	XMFLOAT4X4* GetpViewPerspectiveProject();
	CViewport* GetpViewport();

	//카메라 행렬 생성
	void GenerateViewMatrix();	
	
	//룩벡터 설정
	void SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);
	void SetLookAt(XMFLOAT3& xmf3Position, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	//뷰포트 설정
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight);
	
	//FOV 설정
	void SetFOVAngle(float fFOVAngle);

	//투영 행렬 생성
	void GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance);

	void Move(XMFLOAT3& xmf3Shift);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void Update(XMFLOAT3& xmf3NewPosition, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up, float fTimeElapsed = 0.016f);

private:
	XMFLOAT3 m_xmf3Position{ XMFLOAT3(0.0f, 0.0f, 0.0f) };	//카메라 위치 벡터
	XMFLOAT3 m_xmf3Right{ XMFLOAT3(1.0f, 0.0f, 0.0f) };		//카메라 오른쪽방향 벡터
	XMFLOAT3 m_xmf3Up{ XMFLOAT3(0.0f, 1.0f, 0.0f) };		//카메라 위방향 벡터
	XMFLOAT3 m_xmf3Look{ XMFLOAT3(0.0f, 0.0f, 1.0f) };		//카메라 룩방향 벡터

	float m_fFOVAngle{ 90.0f };				//FOV
	float m_fProjectRectDistance{ 1.0f };	//카메라에서 투영평면까지의 거리
	float m_fAspectRatio{ float(FRAMEBUFFER_WIDTH) / float(FRAMEBUFFER_HEIGHT) };	//종횡비

	XMFLOAT4X4 m_xmf4x4View{ Matrix4x4::Identity() };						//카메라 행렬
	XMFLOAT4X4 m_xmf4x4PerspectiveProject{ Matrix4x4::Identity() };			//투영 행렬
	XMFLOAT4X4 m_xmf4x4ViewPerspectiveProject{ Matrix4x4::Identity() };		//카메라*투영 행렬

	CViewport m_Viewport;
};
