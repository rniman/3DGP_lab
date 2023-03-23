#pragma once

#include "Mesh.h"
#include "Camera.h"

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	//Ȱ��ȭ
	void SetActive(bool bActive);
	//������Ʈ�� �Ž� ����
	void SetMesh(CMesh* pMesh);
	//������
	void SetColor(DWORD dwColor);

	//��ġ ����
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& xmf3Position);

	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection);
	void SetMovingSpeed(float fSpeed);
	void SetMovingRange(float fRange);

	//ȸ���� ����
	void SetRotationAxis(XMFLOAT3& xmf3RotationAxis);
	void SetRotationSpeed(float fSpeed);

	//ȸ���� �̵�
	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);
	void Move(XMFLOAT3& xmf3Direction, float fSpeed);

	virtual void OnUpdateTransform();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer);

protected:
	XMFLOAT4X4 m_xmf4x4World{ Matrix4x4::Identity() };

private:
	bool m_bActive{ true };

	//�Ž�
	CMesh* m_pMesh{ nullptr };

	//���庯ȯ���(��ġ, ȸ�� ������ ����)

	DWORD m_dwColor{ RGB(255, 0, 0) };

	//�̵� ����� �ӵ��� Range?
	XMFLOAT3 m_xmf3MovingDirection{ XMFLOAT3(0.0f, 0.0f, 1.0f) };
	float m_fMovingSpeed{ 0.0f };
	float m_fMovingRange{ 0.0f };

	//ȸ����� �ӵ�
	XMFLOAT3 m_xmf3RotationAxis{ XMFLOAT3(0.0f, 1.0f, 0.0f) };
	float m_fRotationSpeed{ 0.0f };

};

