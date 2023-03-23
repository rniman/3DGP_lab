#pragma once

class CVertex
{
public:
	CVertex();
	CVertex(float x, float y, float z);
	~CVertex();

	XMFLOAT3 Getxmf3Position() const;

private:
	XMFLOAT3 m_xmf3Position{};
};

/// <CVertex>
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// <CPolygon>

class CPolygon
{
public:
	CPolygon();
	CPolygon(int nVertices);
	~CPolygon();

	int GetnVertices() const;
	CVertex* GetpVertices();

	void SetVertex(int nIndex, CVertex& vertex);

private:
	int	m_nVertices{};
	CVertex* m_pVertices{ nullptr };
};

/// <CPolygon>
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// <CMesh>

class CMesh
{
public:
	CMesh();
	CMesh(int nPolygons);
	virtual ~CMesh();

	void SetPolygon(int nIndex, CPolygon* pPolygon);

	void AddRef();
	void Release();

	virtual void Render(HDC hDCFrameBuffer);

protected:
	int	m_nPolygons{ 0 };
	CPolygon** m_ppPolygons{ nullptr };

private:
	int	m_nReferences{ 1 };

};

/// <CMesh>
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// <CCubeMesh>

class CCubeMesh : public CMesh
{
public:
	CCubeMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);
	virtual ~CCubeMesh();
};

/// <CCubeMesh>
//////////////////////////////////////////////////////////////////////////////////////////////////////
/// <CAirplaneMesh>

class CAirplaneMesh : public CMesh
{
public:
	CAirplaneMesh(float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);
	virtual ~CAirplaneMesh();
};

