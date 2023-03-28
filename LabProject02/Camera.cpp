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
	// ����ȭ�� ��, ��, ����Ʈ XMVECTOR �����ϱ����� ����
	XMVECTOR xmvLook = XMVector3Normalize(XMLoadFloat3(&m_xmf3Look));		//�躤�͸� �����´�
	XMVECTOR xmvUp = XMVector3Normalize(XMLoadFloat3(&m_xmf3Up));			//�����͸� �����´�.
	XMVECTOR xmvRight = XMVector3Normalize(XMVector3Cross(xmvUp, xmvLook));	//�����Ϳ� �躤�͸� �����Ͽ� ����Ʈ ���� ����
	xmvUp = XMVector3Normalize(XMVector3Cross(xmvLook, xmvRight));			//����ε� �����͸� ������ �����Ѵ�.

	// ���͸� ��������� ����
	XMStoreFloat3(&m_xmf3Look, xmvLook);
	XMStoreFloat3(&m_xmf3Right, xmvRight);
	XMStoreFloat3(&m_xmf3Up, xmvUp);

	// ī�޶� ��� ���� �����.
	m_xmf4x4View._11 = m_xmf3Right.x; m_xmf4x4View._12 = m_xmf3Up.x; m_xmf4x4View._13 = m_xmf3Look.x;
	m_xmf4x4View._21 = m_xmf3Right.y; m_xmf4x4View._22 = m_xmf3Up.y; m_xmf4x4View._23 = m_xmf3Look.y;
	m_xmf4x4View._31 = m_xmf3Right.z; m_xmf4x4View._32 = m_xmf3Up.z; m_xmf4x4View._33 = m_xmf3Look.z;

	// ��ġ ���͸� �̿��� ���� �߽����� �ű�� ���� ���͸� ����� 4�࿡ �߰�
	// ��ġ ���Ϳ� ������ ī�޶� local���� ������ �� -> ����ȯ ����� �ϸ鼭 ��������. -> �׼� ppt.2.34p ����
	XMVECTOR xmvPosition = XMLoadFloat3(&m_xmf3Position);
	m_xmf4x4View._41 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvRight));
	m_xmf4x4View._42 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvUp));
	m_xmf4x4View._43 = -XMVectorGetX(XMVector3Dot(xmvPosition, xmvLook));

	//XMMatrixMultiply -> ��İ�
	//ī�޶�*���� ��ȯ ��Ŀ� ����
	XMStoreFloat4x4(&m_xmf4x4ViewPerspectiveProject, XMMatrixMultiply(XMLoadFloat4x4(&m_xmf4x4View), XMLoadFloat4x4(&m_xmf4x4PerspectiveProject)));
}

void CCamera::SetLookAt(XMFLOAT3& xmf3Position, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	m_xmf3Position = xmf3Position;
	//��ġ ���� ���ʺ��͸� �̿��� ī�޶� ��� ���� 
	//													EYE							LOOK(��ġ ����)						UP
	XMStoreFloat4x4(&m_xmf4x4View, XMMatrixLookAtLH(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&xmf3LookAt), XMLoadFloat3(&xmf3Up)));

	//XMMatrixLookAtLH�� ����
	// R U L 0 
	// R U L 0
	// R U L 0
	// 0 0 0 1
	XMVECTORF32 xmf32vRight = { m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31, 0.0f };
	XMVECTORF32 xmf32vUp = { m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32, 0.0f };
	XMVECTORF32 xmf32vLook = { m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33, 0.0f };

	//����ȭ
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
	m_fAspectRatio = float(nWidth) / float(nHeight);		//��Ⱦ�� ����
}

void CCamera::SetFOVAngle(float fFOVAngle)
{
	m_fFOVAngle = fFOVAngle;
	//m_fProjectRectDistance = float(1.0f / tan(XMConvertToRadians(fFOVAngle * 0.5f)));	// d = 1 / tan(��Ÿ)
}

void CCamera::GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance)
{
	//���� ��ȯ ��� ����
	XMStoreFloat4x4(&m_xmf4x4PerspectiveProject, XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fFOVAngle), m_fAspectRatio, fNearPlaneDistance, fFarPlaneDistance));
}

void CCamera::Move(XMFLOAT3& xmf3Shift)
{
	//�̵�
	XMStoreFloat3(&m_xmf3Position, XMVectorAdd(XMLoadFloat3(&m_xmf3Position), XMLoadFloat3(&xmf3Shift)));
}

void CCamera::Move(float x, float y, float z)
{
	Move(XMFLOAT3(x, y, z));
}

void CCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (fRoll != 0.0f)	//z�� ȸ��
	{
		//�躤�͸� ������ �ϴ� ȸ�� ��� ����
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		//XMVector3TransformNormal -> ���Ϳ� ����� �� (���� ��ǥ 0) -> ���� �����̹Ƿ�
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
	}
	if (fPitch != 0.0f)	//x�� ȸ��
	{
		XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
	}
	if (fYaw != 0.0f)	//y�� ȸ��
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
	XMVECTOR xmvDirection = XMVectorSubtract(xmvNewPosition, xmvPosition); //ī�޶� ����

	float fLength = XMVectorGetX(XMVector3Length(xmvDirection));	//�Ÿ� ��ȯ
	xmvDirection = XMVector3Normalize(xmvDirection);	//���� ����ȭ

	// �ð� ���� ũ�� -> Ŀ���� ���콺 ȸ���� ȭ���� �i�ư��� �ӵ��� ��������.
	float fTimeLagScale = fTimeElapsed * 4.0f; // 0.016 -> 0.08
	float fDistance = fLength * fTimeLagScale; // fDis < fLen
	if (fDistance > fLength)	// �ð� ������ 1���� ũ�� �������� ����. �� 60�̻��� ���ؾ��Ѵ�.
	{
		fDistance = fLength;
	}
	if (fLength < 0.01f)	// �Ÿ��� ª��. -> ���������� ����.
	{
		fDistance = fLength;
	}
	if (fDistance > 0)
	{
		XMStoreFloat3(&m_xmf3Position, XMVectorAdd(xmvPosition, XMVectorScale(xmvDirection, fDistance)));
		SetLookAt(xmf3LookAt, xmf3Up);
	}
}
