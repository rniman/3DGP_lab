#pragma once

#include "Mesh.h"
#include "Camera.h"

class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject();

	//활성화
	void SetActive(bool bActive);
	//오브젝트의 매쉬 설정
	void SetMesh(CMesh* pMesh);
	//색설정
	void SetColor(DWORD dwColor);

	//위치 지정
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& xmf3Position);

	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection);
	void SetMovingSpeed(float fSpeed);
	void SetMovingRange(float fRange);

	//회전축 설정
	void SetRotationAxis(XMFLOAT3& xmf3RotationAxis);
	void SetRotationSpeed(float fSpeed);

	//회전과 이동
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

	//매쉬
	CMesh* m_pMesh{ nullptr };

	//월드변환행렬(위치, 회전 정보가 담긴다)

	DWORD m_dwColor{ RGB(255, 0, 0) };

	//이동 방향과 속도와 Range?
	XMFLOAT3 m_xmf3MovingDirection{ XMFLOAT3(0.0f, 0.0f, 1.0f) };
	float m_fMovingSpeed{ 0.0f };
	float m_fMovingRange{ 0.0f };

	//회전축과 속도
	XMFLOAT3 m_xmf3RotationAxis{ XMFLOAT3(0.0f, 1.0f, 0.0f) };
	float m_fRotationSpeed{ 0.0f };

};

