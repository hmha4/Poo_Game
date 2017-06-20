#include "stdafx.h"
#include "CMesh.h"
#include "CUtility.h"

CMesh::CMesh()
{
	m_pMesh = NULL;
	m_numMaterials = 0;
	m_pMeshMaterials = NULL;
	m_ppMeshTextures = NULL;
}


void CMesh::Release()
{
	//Delete the materials
	SAFE_DELETE_ARRAY(m_pMeshMaterials);

	//Delete the textures
	if (m_ppMeshTextures)
	{
		for (DWORD i = 0; i < m_numMaterials; i++)
		{
			SAFE_RELEASE(m_ppMeshTextures[i]);
		}
	}
	SAFE_DELETE_ARRAY(m_ppMeshTextures);
	SAFE_RELEASE(m_pMesh);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Load an X file mesh with no animation.
Parameters:
[in] pDevice – D3D Device
[in] file – File name
Returns: TRUE if load was successful, FALSE otherwise
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BOOL CMesh::Load(LPDIRECT3DDEVICE9 pDevice, char* file)
{
	Release();
	LPD3DXBUFFER pMaterialBuffer;
	char path[MAX_PATH];
	CUtility::GetMediaFile(file, path);
	HRESULT hr = D3DXLoadMeshFromX(path, D3DXMESH_MANAGED, pDevice, NULL, &pMaterialBuffer,
		NULL, &m_numMaterials, &m_pMesh);
	
	if (FAILED(hr))
	{
		SHOWERROR("D3DXLoadMeshFromX - FAILED", __FILE__, __LINE__);
		return FALSE;
	}
	
	//Stor material and texture information
	D3DXMATERIAL* pMaterials = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();

	m_pMeshMaterials = new D3DMATERIAL9[m_numMaterials];
	m_ppMeshTextures = new LPDIRECT3DTEXTURE9[m_numMaterials];

	//Copy the materials and textures form the buffer to the member arrays
	for (DWORD i = 0; i < m_numMaterials; i++)
	{
		m_pMeshMaterials[i] = pMaterials[i].MatD3D;
		m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

		//Create the texture if it exists
		m_ppMeshTextures[i] = NULL;
		if (pMaterials[i].pTextureFilename)
		{
			CUtility::GetMediaFile(pMaterials[i].pTextureFilename, path);
			if (FAILED(D3DXCreateTextureFromFile(pDevice, path, &m_ppMeshTextures[i])))
			{
				SHOWERROR("Failed to load mesh texture", __FILE__, __LINE__);
				return FALSE;
			}
		}
	}
	//don't need this no more!
	pMaterialBuffer->Release();

	return TRUE;
}

CMeshInstance::CMeshInstance()
{
	m_pMesh = NULL;
	m_boundingRadius = 0.0f;
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

	//Compute bounding sphere
	if (m_pMesh)
	{
		D3DXVECTOR3 center;
		LPD3DXMESH pD3DXMesh = m_pMesh->GetMesh();
		DWORD numVertices = pD3DXMesh->GetNumVertices();
		DWORD fvfSize = D3DXGetFVFVertexSize(pD3DXMesh->GetFVF());
		char* pData = NULL;
		if (FAILED(pD3DXMesh->LockVertexBuffer(0, (void**)&pData)))
		{
			SHOWERROR("Failed to lock mesh vertex buffer.", __FILE__, __LINE__);
			return;
		}
		D3DXComputeBoundingSphere((D3DXVECTOR3*)pData, numVertices, fvfSize, &center, &m_boundingRadius);
		if (FAILED(pD3DXMesh->UnlockVertexBuffer()))
		{
			SHOWERROR("Failed to unlock mesh vertex buffer.", __FILE__, __LINE__);
			return;
		}
	}
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