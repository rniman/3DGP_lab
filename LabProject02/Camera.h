#pragma once

//#include "Mesh.h"

// ȭ������ ��ȯ�ϱ����� ����
class CViewport
{
public:
	CViewport();
	virtual ~CViewport();

	int GetnLeft() const;
	int Getntop() const;
	int GetnWidth() const;
	int GetnHeight() const;

	//����Ʈ ����
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

// ī�޶� Ŭ����
class CCamera
{
public:
	CCamera();
	virtual ~CCamera();

	XMFLOAT4X4* GetpViewPerspectiveProject();
	CViewport* GetpViewport();

	//ī�޶� ��� ����
	void GenerateViewMatrix();	
	
	//�躤�� ����
	void SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);
	void SetLookAt(XMFLOAT3& xmf3Position, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	//����Ʈ ����
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight);
	
	//FOV ����
	void SetFOVAngle(float fFOVAngle);

	//���� ��� ����
	void GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance);

	void Move(XMFLOAT3& xmf3Shift);
	void Move(float x, float y, float z);
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void Update(XMFLOAT3& xmf3NewPosition, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up, float fTimeElapsed = 0.016f);

private:
	XMFLOAT3 m_xmf3Position{ XMFLOAT3(0.0f, 0.0f, 0.0f) };	//ī�޶� ��ġ ����
	XMFLOAT3 m_xmf3Right{ XMFLOAT3(1.0f, 0.0f, 0.0f) };		//ī�޶� �����ʹ��� ����
	XMFLOAT3 m_xmf3Up{ XMFLOAT3(0.0f, 1.0f, 0.0f) };		//ī�޶� ������ ����
	XMFLOAT3 m_xmf3Look{ XMFLOAT3(0.0f, 0.0f, 1.0f) };		//ī�޶� ����� ����

	float m_fFOVAngle{ 90.0f };				//FOV
	float m_fProjectRectDistance{ 1.0f };	//ī�޶󿡼� ������������ �Ÿ�
	float m_fAspectRatio{ float(FRAMEBUFFER_WIDTH) / float(FRAMEBUFFER_HEIGHT) };	//��Ⱦ��

	XMFLOAT4X4 m_xmf4x4View{ Matrix4x4::Identity() };						//ī�޶� ���
	XMFLOAT4X4 m_xmf4x4PerspectiveProject{ Matrix4x4::Identity() };			//���� ���
	XMFLOAT4X4 m_xmf4x4ViewPerspectiveProject{ Matrix4x4::Identity() };		//ī�޶�*���� ���

	CViewport m_Viewport;
};
