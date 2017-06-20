#include "stdafx.h"
#include "CMeshInstance.h"


CMeshInstance::CMeshInstance()
{
	m_pMesh = NULL;
}

void CMeshInstance::Release()
{
	//Mesh data is Released in CMesh
	m_pMesh = NULL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Set the CMesh reference
Parameters:
[in] pMesh – Pointer to a CMesh
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMeshInstance::SetMesh(CMesh* pMesh)
{
	Release();
	m_pMesh = pMesh;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Renders the mesh
Parameters:
[in] pDevice – D3D Device
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CMeshInstance::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (pDevice && m_pMesh)
	{
		pDevice->SetTransform(D3DTS_WORLD, GetTransform());
		DWORD numMaterials = m_pMesh->GetNumMaterials();
		for (DWORD i = 0; i < numMaterials; i++)
		{
			pDevice->SetMaterial(m_pMesh->GetMeshMaterial(i));
			pDevice->SetTexture(0, m_pMesh->GetMeshTexture(i));
			m_pMesh->GetMesh()->DrawSubset(i);
		}
	}
}