#include "stdafx.h"
#include "GameObject.h"
#include "GraphicsPipeline.h"

CGameObject::CGameObject() {}

CGameObject::~CGameObject(void)
{
	if (m_pMesh) m_pMesh->Release();
}

void CGameObject::SetActive(bool bActive) 
{
	m_bActive = bActive; 
}

void CGameObject::SetMesh(CMesh* pMesh)
{
	m_pMesh = pMesh;
	if (pMesh)
	{
		//매쉬 참조 1증가
		pMesh->AddRef();
	}
}

void CGameObject::SetColor(DWORD dwColor)
{
	m_dwColor = dwColor;
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4World._41 = x;
	m_xmf4x4World._42 = y;
	m_xmf4x4World._43 = z;
}

void CGameObject::SetPosition(XMFLOAT3& xmf3Position)
{
	m_xmf4x4World._41 = xmf3Position.x;
	m_xmf4x4World._42 = xmf3Position.y;
	m_xmf4x4World._43 = xmf3Position.z;
}

void CGameObject::SetMovingDirection(XMFLOAT3& xmf3MovingDirection)
{
	XMStoreFloat3(&m_xmf3MovingDirection, XMVector3Normalize(XMLoadFloat3(&xmf3MovingDirection)));
}

void CGameObject::SetMovingSpeed(float fSpeed)
{
	m_fMovingSpeed = fSpeed;
}

//void CGameObject::SetMovingRange(float fRange)
//{
//	m_fMovingRange = fRange;
//}

void CGameObject::SetRotationAxis(XMFLOAT3& xmf3RotationAxis)
{
	XMStoreFloat3(&m_xmf3RotationAxis, XMVector3Normalize(XMLoadFloat3(&xmf3RotationAxis))); 
}

void CGameObject::SetRotationSpeed(float fSpeed)
{
	m_fRotationSpeed = fSpeed; 
}


/*
XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

XMFLOAT3 CGameObject::GetRight()
{
	XMFLOAT3 xmf3Right;
	XMStoreFloat3(&xmf3Right, XMVector3Normalize(XMVectorSet(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13, 0.0f)));

	return(xmf3Right);
}

XMFLOAT3 CGameObject::GetUp()
{
	XMFLOAT3 xmf3Up;
	XMStoreFloat3(&xmf3Up, XMVector3Normalize(XMVectorSet(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23, 0.0f)));

	return(xmf3Up);
}

XMFLOAT3 CGameObject::GetLook()
{
	XMFLOAT3 xmf3LookAt;
	XMStoreFloat3(&xmf3LookAt, XMVector3Normalize(XMVectorSet(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33, 0.0f)));

	return(xmf3LookAt);
}

void CGameObject::SetRotationTransform(XMFLOAT4X4* pmxf4x4Transform)
{
	m_xmf4x4World._11 = pmxf4x4Transform->_11; m_xmf4x4World._12 = pmxf4x4Transform->_12; m_xmf4x4World._13 = pmxf4x4Transform->_13;
	m_xmf4x4World._21 = pmxf4x4Transform->_21; m_xmf4x4World._22 = pmxf4x4Transform->_22; m_xmf4x4World._23 = pmxf4x4Transform->_23;
	m_xmf4x4World._31 = pmxf4x4Transform->_31; m_xmf4x4World._32 = pmxf4x4Transform->_32; m_xmf4x4World._33 = pmxf4x4Transform->_33;
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position;
	XMStoreFloat3(&xmf3Position, XMVectorAdd(XMVectorSet(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43, 0.0f), XMVectorScale(XMVector3Normalize(XMVectorSet(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13, 0.0f)), fDistance)));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position;
	XMStoreFloat3(&xmf3Position, XMVectorAdd(XMVectorSet(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43, 0.0f), XMVectorScale(XMVector3Normalize(XMVectorSet(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23, 0.0f)), fDistance)));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position;
	XMStoreFloat3(&xmf3Position, XMVectorAdd(XMVectorSet(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43, 0.0f), XMVectorScale(XMVector3Normalize(XMVectorSet(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33, 0.0f)), fDistance)));

	CGameObject::SetPosition(xmf3Position);
}
*/

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	//회전 행렬을 만들어서 월드 변환 행렬과 곱
	XMMATRIX xmmtxRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fPitch), XMConvertToRadians(fYaw), XMConvertToRadians(fRoll));
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
}

void CGameObject::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	//회전 행렬을 만들어서 월드 변환 행렬과 곱
	XMMATRIX xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3RotationAxis), XMConvertToRadians(fAngle));
	XMStoreFloat4x4(&m_xmf4x4World, XMMatrixMultiply(xmmtxRotate, XMLoadFloat4x4(&m_xmf4x4World)));
}

void CGameObject::Move(XMFLOAT3& vDirection, float fSpeed)
{
	//vDirection 방향으로 이동
	SetPosition(m_xmf4x4World._41 + vDirection.x * fSpeed, m_xmf4x4World._42 + vDirection.y * fSpeed, m_xmf4x4World._43 + vDirection.z * fSpeed);
}

/*
void CGameObject::LookTo(XMFLOAT3& xmf3LookTo, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixLookToLH(XMVectorSet(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43, 0.0f), XMLoadFloat3(&xmf3LookTo), XMLoadFloat3(&xmf3Up)));

	m_xmf4x4World._11 = xmf4x4View._11; m_xmf4x4World._12 = xmf4x4View._21; m_xmf4x4World._13 = xmf4x4View._31;
	m_xmf4x4World._21 = xmf4x4View._12; m_xmf4x4World._22 = xmf4x4View._22; m_xmf4x4World._23 = xmf4x4View._32;
	m_xmf4x4World._31 = xmf4x4View._13; m_xmf4x4World._32 = xmf4x4View._23; m_xmf4x4World._33 = xmf4x4View._33;
}

void CGameObject::LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View;
	XMStoreFloat4x4(&xmf4x4View, XMMatrixLookAtLH(XMVectorSet(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43, 0.0f), XMLoadFloat3(&xmf3LookAt), XMLoadFloat3(&xmf3Up)));

	m_xmf4x4World._11 = xmf4x4View._11; m_xmf4x4World._12 = xmf4x4View._21; m_xmf4x4World._13 = xmf4x4View._31;
	m_xmf4x4World._21 = xmf4x4View._12; m_xmf4x4World._22 = xmf4x4View._22; m_xmf4x4World._23 = xmf4x4View._32;
	m_xmf4x4World._31 = xmf4x4View._13; m_xmf4x4World._32 = xmf4x4View._23; m_xmf4x4World._33 = xmf4x4View._33;
}
*/

void CGameObject::OnUpdateTransform() {}

void CGameObject::Animate(float fElapsedTime)
{
	if (m_fRotationSpeed != 0.0f)
	{
		Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);
	}
	if (m_fMovingSpeed != 0.0f)
	{
		Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);
	}
}

void CGameObject::Render(HDC hDCFrameBuffer)
{
	if (m_pMesh)
	{
		//오브젝트의 월드변환을 가져온다.
		CGraphicsPipeline::SetWorldTransform(&m_xmf4x4World);

		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);

		m_pMesh->Render(hDCFrameBuffer);

		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

