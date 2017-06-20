#pragma once
#ifndef CMESHINSTANCE_H
#define CMESHINSTANCE_H

#include "stdafx.h"
#include "CWorldTransform.h"
#include "CMesh.h"

class CMeshInstance : public CWorldTransform
{
private:
	CMesh* m_pMesh;
public:
	CMeshInstance();
	~CMeshInstance() { Release(); }

	void Release();
	void SetMesh(CMesh* pMesh);
	void Render(LPDIRECT3DDEVICE9 pDevice);
};

#endif // !CMESHINSTANCE_H