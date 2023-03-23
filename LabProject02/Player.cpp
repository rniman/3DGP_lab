#include "stdafx.h"
#include "Player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CPlayer::CPlayer() {}

CPlayer::~CPlayer()
{
	if (m_pCamera)
	{
		delete m_pCamera;
	}
}

XMFLOAT3 CPlayer::Getxmf3Position() const
{
	return m_xmf3Position;
}

XMFLOAT3 CPlayer::Getxmf3Right() const
{
	return m_xmf3Right;
}

XMFLOAT3 CPlayer::Getxmf3Up() const
{
	return m_xmf3Up;
}

XMFLOAT3 CPlayer::Getxmf3Look() const
{
	return m_xmf3Look;
}

XMFLOAT3* CPlayer::Getpxmf3Position()
{
	return &m_xmf3Position;
}

XMFLOAT3* CPlayer::Getpxmf3CameraOffset()
{
	return &m_xmf3CameraOffset;
}

void CPlayer::SetPosition(float x, float y, float z)
{
	m_xmf3Position = XMFLOAT3(x, y, z);

	CGameObject::SetPosition(x, y, z);
}

void CPlayer::SetCameraOffset(XMFLOAT3& xmf3CameraOffset)
{
	//카메라 위치를 해당 오프셋 만큼 이동시킨다.
	m_xmf3CameraOffset = xmf3CameraOffset;
	XMFLOAT3 xmf3CameraPosition;
	XMStoreFloat3(&xmf3CameraPosition, XMVectorAdd(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&m_xmf3CameraOffset)));
	
	//해당 위치에서의 행렬 생성
	m_pCamera->SetLookAt(xmf3CameraPosition, m_xmf3Position, m_xmf3Up);

	//카메라 행렬 생성
	m_pCamera->GenerateViewMatrix();
}

void CPlayer::Move(DWORD dwDirection, float fDistance)
{
	if (dwDirection)
	{
		XMFLOAT3 xmf3Shift = XMFLOAT3(0, 0, 0);
		if (dwDirection & DIR_FORWARD) XMStoreFloat3(&xmf3Shift, XMVectorAdd(XMLoadFloat3(&xmf3Shift), XMVectorScale(XMLoadFloat3(&m_xmf3Look), fDistance)));
		if (dwDirection & DIR_BACKWARD) XMStoreFloat3(&xmf3Shift, XMVectorAdd(XMLoadFloat3(&xmf3Shift), XMVectorScale(XMLoadFloat3(&m_xmf3Look), -fDistance)));
		if (dwDirection & DIR_RIGHT) XMStoreFloat3(&xmf3Shift, XMVectorAdd(XMLoadFloat3(&xmf3Shift), XMVectorScale(XMLoadFloat3(&m_xmf3Right), fDistance)));
		if (dwDirection & DIR_LEFT) XMStoreFloat3(&xmf3Shift, XMVectorAdd(XMLoadFloat3(&xmf3Shift), XMVectorScale(XMLoadFloat3(&m_xmf3Right), -fDistance)));
		if (dwDirection & DIR_UP) XMStoreFloat3(&xmf3Shift, XMVectorAdd(XMLoadFloat3(&xmf3Shift), XMVectorScale(XMLoadFloat3(&m_xmf3Up), fDistance)));
		if (dwDirection & DIR_DOWN) XMStoreFloat3(&xmf3Shift, XMVectorAdd(XMLoadFloat3(&xmf3Shift), XMVectorScale(XMLoadFloat3(&m_xmf3Up), -fDistance)));

		Move(xmf3Shift, true);
	}
}

void CPlayer::Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
	if (bUpdateVelocity)
	{
		XMStoreFloat3(&m_xmf3Velocity, XMVectorAdd(XMLoadFloat3(&m_xmf3Velocity), XMLoadFloat3(&xmf3Shift)));
	}
	else //이동 안함
	{
		XMStoreFloat3(&m_xmf3Position, XMVectorAdd(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&xmf3Shift)));
		m_pCamera->Move(xmf3Shift);
	}
}

void CPlayer::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z), false);
}

void CPlayer::Rotate(float fPitch, float fYaw, float fRoll)
{
	m_pCamera->Rotate(fPitch, fYaw, fRoll);
	if (fRoll != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		//XMVector3TransformNormal -> 벡터와 행렬의 곱 (동차 좌표 0) -> 방향 벡터이므로
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
	}
	if (fPitch != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
	}

	// local 좌표축 다시 만들기
	XMVECTOR xmvLook = XMVector3Normalize(XMLoadFloat3(&m_xmf3Look));
	XMVECTOR xmvUp = XMVector3Normalize(XMLoadFloat3(&m_xmf3Up));
	XMVECTOR xmvRight = XMVector3Normalize(XMVector3Cross(xmvUp, xmvLook));
	xmvUp = XMVector3Normalize(XMVector3Cross(xmvLook, xmvRight));

	XMStoreFloat3(&m_xmf3Right, xmvRight);
	XMStoreFloat3(&m_xmf3Up, xmvUp);
	XMStoreFloat3(&m_xmf3Look, xmvLook);
}

void CPlayer::LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	//카메라 행렬 생성
	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixLookAtLH(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&xmf3LookAt), XMLoadFloat3(&xmf3Up)));

	//XMMatrixLookAtLH을 생성
	// R U L 0 
	// R U L 0
	// R U L 0
	// 0 0 0 1
	XMVECTORF32 xmf32vRight = { xmf4x4View._11, xmf4x4View._21, xmf4x4View._31, 0.0f };
	XMVECTORF32 xmf32vUp = { xmf4x4View._12, xmf4x4View._22, xmf4x4View._32, 0.0f };
	XMVECTORF32 xmf32vLook = { xmf4x4View._13, xmf4x4View._23, xmf4x4View._33, 0.0f };

	XMStoreFloat3(&m_xmf3Right, XMVector3Normalize(xmf32vRight));
	XMStoreFloat3(&m_xmf3Up, XMVector3Normalize(xmf32vUp));
	XMStoreFloat3(&m_xmf3Look, XMVector3Normalize(xmf32vLook));
}

void CPlayer::Update(float fTimeElapsed)
{
	//이동
	Move(m_xmf3Velocity, false);

	XMMATRIX xmmtx4Rotate;
	xmmtx4Rotate.r[0] = XMVectorSet(m_xmf3Right.x, m_xmf3Right.y, m_xmf3Right.z, 0.0f);	//Right
	xmmtx4Rotate.r[1] = XMVectorSet(m_xmf3Up.x, m_xmf3Up.y, m_xmf3Up.z, 0.0f);			//Up
	xmmtx4Rotate.r[2] = XMVectorSet(m_xmf3Look.x, m_xmf3Look.y, m_xmf3Look.z, 0.0f);	//Look
	xmmtx4Rotate.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR xmvOffset = XMVector3TransformCoord(XMLoadFloat3(&m_xmf3CameraOffset), xmmtx4Rotate);
	XMVECTOR xmvNewPosition = XMVectorAdd(XMLoadFloat3(&m_xmf3Position), xmvOffset);
	XMFLOAT3 xmf3NewPosition;
	XMStoreFloat3(&xmf3NewPosition, xmvNewPosition);

	m_pCamera->Update(xmf3NewPosition, m_xmf3Position, m_xmf3Up, fTimeElapsed);
	m_pCamera->GenerateViewMatrix();
		
	
	XMVECTOR xmvVelocity = XMLoadFloat3(&m_xmf3Velocity);	//속력 방향
	XMVECTOR xmvDeceleration = XMVector3Normalize(XMVectorScale(xmvVelocity, -1.0f));	//속력 반대
	float fLength = XMVectorGetX(XMVector3Length(xmvVelocity));	//벡터의 크기
	float fDeceleration = m_fFriction * fTimeElapsed;			//감속
	if (fDeceleration > fLength)
	{
		fDeceleration = fLength;
	}
	XMStoreFloat3(&m_xmf3Velocity, XMVectorAdd(xmvVelocity, XMVectorScale(xmvDeceleration, fDeceleration)));	//감속 적용
}

void CPlayer::Animate(float fElapsedTime)
{
	OnUpdateTransform();

	CGameObject::Animate(fElapsedTime);
}

void CPlayer::OnUpdateTransform()
{
	m_xmf4x4World._11 = m_xmf3Right.x; m_xmf4x4World._12 = m_xmf3Right.y; m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x; m_xmf4x4World._22 = m_xmf3Up.y; m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x; m_xmf4x4World._32 = m_xmf3Look.y; m_xmf4x4World._33 = m_xmf3Look.z;
	m_xmf4x4World._41 = m_xmf3Position.x; m_xmf4x4World._42 = m_xmf3Position.y; m_xmf4x4World._43 = m_xmf3Position.z;
}

//void CPlayer::Render(HDC hDCFrameBuffer, CCamera* pCamera)
//{
//	CGameObject::Render(hDCFrameBuffer, pCamera);
//}

/// <CPlayer>
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// <CAirplanePlayer>

CAirplanePlayer::CAirplanePlayer()
{
}

CAirplanePlayer::~CAirplanePlayer()
{
}

//void CAirplanePlayer::Animate(float fElapsedTime)
//{
//	CPlayer::Animate(fElapsedTime);
//}

void CAirplanePlayer::OnUpdateTransform()
{
	CPlayer::OnUpdateTransform();

	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f), XMLoadFloat4x4(&m_xmf4x4World)));
}

//void CAirplanePlayer::Render(HDC hDCFrameBuffer, CCamera* pCamera)
//{
//	CPlayer::Render(hDCFrameBuffer, pCamera);
//}

