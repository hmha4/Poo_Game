#pragma once
#ifndef CMESH_H
#define CMESH_H

#include "stdafx.h"
#include "CWorldTransform.h"

class CMesh
{
private:
	LPD3DXMESH m_pMesh;
	DWORD m_numMaterials;
	D3DMATERIAL9 *m_pMeshMaterials;
	LPDIRECT3DTEXTURE9 *m_ppMeshTextures;
public:
	CMesh();
	~CMesh() { Release(); }
	BOOL Load(LPDIRECT3DDEVICE9 pDevice, char* file);
	void Release();

	LPD3DXMESH GetMesh() { return m_pMesh; }
	DWORD GetNumMaterials() { return m_numMaterials; }
	D3DMATERIAL9* GetMeshMaterial(int i) { return &m_pMeshMaterials[i]; }
	LPDIRECT3DTEXTURE9 GetMeshTexture(int i) { return m_ppMeshTextures[i]; }
};

class CMeshInstance : public CWorldTransform
{
private:
	CMesh* m_pMesh;
	float m_boundingRadius;
public:
	CMeshInstance();
	~CMeshInstance() { Release(); }

	void Release();
	void SetMesh(CMesh* pMesh);
	void Render(LPDIRECT3DDEVICE9 pDevice);

	float GetBoundingRadius() { return m_boundingRadius; }
};


#endif // !CMESH_H