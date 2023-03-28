#include "stdafx.h"
#include "Camera.h"
#include "Mesh.h"
#include "Player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
CViewport::CViewport() {}

CViewport::~CViewport() {}

int CViewport::GetnLeft() const
{
	return m_nLeft;
}

int CViewport::Getntop() const
{
	return m_nTop;
}

int CViewport::GetnWidth() const
{
	return m_nWidth;
}

int CViewport::GetnHeight() const
{
	return m_nHeight;
}

void CViewport::SetViewport(int nLeft, int nTop, int nWidth, int nHeight)
{
	m_nLeft = nLeft;
	m_nTop = nTop;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

/// <CViewport>
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// <CCamer>

CCamera::CCamera() {}

CCamera::~CCamera() {}

XMFLOAT4X4* CCamera::GetpViewPerspectiveProject()
{
	return &m_xmf4x4ViewPerspectiveProject;;
}

CViewport* CCamera::GetpViewport()
{
	return &m_Viewport;
}

void CCamera::GenerateViewMatrix()
{
	// 정규화된 룩, 업, 라이트 XMVECTOR 생성하기위한 과정
	XMVECTOR xmvLook = XMVector3Normalize(XMLoadFloat3(&m_xmf3Look));		//룩벡터를 가져온다
	XMVECTOR xmvUp = XMVector3Normalize(XMLoadFloat3(&m_xmf3Up));			//업벡터를 가져온다.
	XMVECTOR xmvRight = XMVector3Normalize(XMVector3Cross(xmvUp, xmvLook));	//업벡터와 룩벡터를 외적하여 라이트 벡터 생성
	xmvUp = XMVector3Normalize(XMVector3Cross(xmvLook, xmvRight));			//제대로된 업벡터를 외적해 생성한다.

	// 벡터를 멤버변수에 저장
	XMStoreFloat3(&m_xmf3Look, xmvLook);
	XMStoreFloat3(&m_xmf3Right, xmvRight);
	XMStoreFloat3(&m_xmf3Up, xmvUp);

	// 카메라 행렬 직접 만든다.
	m_xmf4x4View._11 = m_xmf3Right.x; m_xmf4x4View._12 = m_xmf3Up.x; m_xmf4x4View._13 = m_xmf3Look.x;
	m_xmf4x4View._21 = m_xmf3Right.y; m_xmf4x4View._22 = m_xmf3Up.y; m_xmf4x4View._23 = m_xmf3Look.y;
	m_xmf4x4View._31 = m_xmf3Right.z; m_xmf4x4View._32 = m_xmf3Up.z; m_xmf4x4View._33 = m_xmf3Look.z;

	// 위치 벡터를 이용해 월드 중심으로 옮기기 위한 벡터를 행렬의 4행에 추가
	// 위치 벡터와 각각의 카메라 local축을 내적한 값 -> 역변환 행렬을 하면서 곱해진다. -> 겜수 ppt.2.34p 참고
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	m_xmf4x4View._41 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvRight));
	m_xmf4x4View._42 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvUp));
	m_xmf4x4View._43 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvLook));

	//XMMatrixMultiply -> 행렬곱
	//카메라*투영 변환 행렬에 저장
	XMStoreFloat4x4(&m_xmf4x4ViewPerspectiveProject, XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4View), XMLoadFloat4x4(&m_xmf4x4PerspectiveProject)));
}

void CCamera::SetLookAt(XMFLOAT3& xmf3Position, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	m_xmf3Position = xmf3Position;
	//위치 초점 위쪽벡터를 이용해 카메라 행렬 생성 
	//													EYE							LOOK(위치 벡터)						UP
	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixLookAtLH(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&xmf3LookAt), XMLoadFloat3(&xmf3Up)));

	//XMMatrixLookAtLH을 생성
	// R U L 0 
	// R U L 0
	// R U L 0
	// 0 0 0 1
	XMVECTORF32 xmf32vRight = { m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31, 0.0f };
	XMVECTORF32 xmf32vUp = { m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32, 0.0f };
	XMVECTORF32 xmf32vLook = { m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33, 0.0f };

	//정규화
	XMStoreFloat3(&m_xmf3Right, XMVector3Normalize(xmf32vRight));
	XMStoreFloat3(&m_xmf3Up, XMVector3Normalize(xmf32vUp));
	XMStoreFloat3(&m_xmf3Look, XMVector3Normalize(xmf32vLook));
}

void CCamera::SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	SetLookAt(m_xmf3Position, xmf3LookAt, xmf3Up);
}

void CCamera::SetViewport(int nLeft, int nTop, int nWidth, int nHeight)
{
	m_Viewport.SetViewport(nLeft, nTop, nWidth, nHeight);
	m_fAspectRatio = float(nWidth) / float(nHeight);		//종횡비 설정
}

void CCamera::SetFOVAngle(float fFOVAngle)
{
	m_fFOVAngle = fFOVAngle;
	//m_fProjectRectDistance = float(1.0f / tan(XMConvertToRadians(fFOVAngle * 0.5f)));	// d = 1 / tan(세타)
}

void CCamera::GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance)
{
	//투영 변환 행렬 생성
	XMStoreFloat4x4(&m_xmf4x4PerspectiveProject, XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fFOVAngle), m_fAspectRatio, fNearPlaneDistance, fFarPlaneDistance));
}

void CCamera::Move(XMFLOAT3& xmf3Shift)
{
	//이동
	XMStoreFloat3(&m_xmf3Position, XMVectorAdd(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&xmf3Shift)));
}

void CCamera::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z));
}

void CCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (fRoll != 0.0f)	//z축 회전
	{
		//룩벡터를 축으로 하는 회전 행렬 생성
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		//XMVector3TransformNormal -> 벡터와 행렬의 곱 (동차 좌표 0) -> 방향 벡터이므로
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
	}
	if (fPitch != 0.0f)	//x축 회전
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
	}
	if (fYaw != 0.0f)	//y축 회전
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
	}
}

void CCamera::Update(XMFLOAT3& xmf3NewPosition, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up, float fTimeElapsed)
{
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	XMVECTOR xmvNewPosition = XMLoadFloat3(&xmf3NewPosition);
	XMVECTOR xmvDirection = XMVectorSubtract(xmvNewPosition, xmvPosition); //카메라 방향

	float fLength = XMVectorGetX(XMVector3Length(xmvDirection));	//거리 반환
	xmvDirection = XMVector3Normalize(xmvDirection);	//방향 정규화

	// 시간 지연 크기 -> 커지면 마우스 회전시 화면이 쫒아가는 속도가 빨라진다.
	float fTimeLagScale = fTimeElapsed * 4.0f; // 0.016 -> 0.08
	float fDistance = fLength * fTimeLagScale; // fDis < fLen
	if (fDistance > fLength)	// 시간 지연이 1보다 크면 지연따위 없다. 약 60이상을 곱해야한다.
	{
		fDistance = fLength;
	}
	if (fLength < 0.01f)	// 거리가 짧다. -> 지연따위는 없다.
	{
		fDistance = fLength;
	}
	if (fDistance > 0)
	{
		XMStoreFloat3(&m_xmf3Position, XMVectorAdd(xmvPosition, XMVectorScale(xmvDirection, fDistance)));
		SetLookAt(xmf3LookAt, xmf3Up);
	}
}
