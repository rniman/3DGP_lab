#pragma once

#include "GameObject.h"
#include "Camera.h"

class CPlayer : public CGameObject
{
public:
	CPlayer();
	virtual ~CPlayer();
	
	XMFLOAT3 Getxmf3Position() const;
	XMFLOAT3 Getxmf3Right() const;
	XMFLOAT3 Getxmf3Up() const;
	XMFLOAT3 Getxmf3Look() const;

	XMFLOAT3* Getpxmf3Position();
	XMFLOAT3* Getpxmf3CameraOffset();

	//위치 회전 설정, 상속받은 월드 행렬에도 적용한다.
	void SetPosition(float x, float y, float z);
	//void SetRotation(float x, float y, float z);
	//플레이어 위치에 따른 카메라와의 위치 설정
	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);

	//이동
	void Move(DWORD dwDirection, float fDistance);
	void Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(float x, float y, float z);
	
	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

	void LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);


	void Update(float fTimeElapsed = 0.016f);

	virtual void OnUpdateTransform() override;
	virtual void Animate(float fElapsedTime) override;
	//virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }
	CCamera* GetCamera() { return(m_pCamera); }

private:
	//위치와 local축 벡터들
	XMFLOAT3 m_xmf3Position{ XMFLOAT3(0.0f, 0.0f, 0.0f) };
	XMFLOAT3 m_xmf3Right{ XMFLOAT3(1.0f, 0.0f, 0.0f) };
	XMFLOAT3 m_xmf3Up{ XMFLOAT3(0.0f, 1.0f, 0.0f) };
	XMFLOAT3 m_xmf3Look{ XMFLOAT3(0.0f, 0.0f, 1.0f) };

	//카메라 offset하고 속력
	XMFLOAT3 m_xmf3CameraOffset{ XMFLOAT3(0.0f, 0.0f, 0.0f) };

	//속력이 방향?
	XMFLOAT3 m_xmf3Velocity{ XMFLOAT3(0.0f, 0.0f, 0.0f) };

	//마찰 계수
	float m_fFriction{ 5.0f };

	//회전량
	float m_fPitch{ 0.0f };
	float m_fYaw{ 0.0f };
	float m_fRoll{ 0.0f };

	//카메라
	CCamera* m_pCamera{ nullptr };
};

class CAirplanePlayer : public CPlayer
{
public:
	CAirplanePlayer();
	virtual ~CAirplanePlayer();

	virtual void OnUpdateTransform();
	//	virtual void Animate(float fElapsedTime);
	//	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

