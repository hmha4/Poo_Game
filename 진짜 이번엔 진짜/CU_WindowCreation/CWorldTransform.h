#pragma once
#ifndef CWORLDMATRIX_H
#define CWORLDMATRIX_H

#include "stdafx.h"

class CWorldTransform
{
protected:
	D3DXMATRIX m_translate;
	D3DXMATRIX m_rotate;
	D3DXMATRIX m_scale;
	D3DXMATRIX m_transform;
	float m_rotationX, m_rotationY, m_rotationZ;
public:
	CWorldTransform();
	~CWorldTransform() { Reset(); };

	void Reset();
	void TranslateAbs(float x, float y, float z);
	void TranslateRel(float x, float y, float z);
	void RotateAbs(float x, float y, float z);
	void RotateRel(float x, float y, float z);
	void ScaleAbs(float x, float y, float z);
	void ScaleRel(float x, float y, float z);

	D3DXMATRIX* GetTransform();

	float GetXPosition() { return m_translate._41; }
	float GetYPosition() { return m_translate._42; }
	float GetZPosition() { return m_translate._43; }
	float GetXRotation() { return m_rotationX; }
	float GetYRotation() { return m_rotationY; }
	float GetZRotation() { return m_rotationZ; }
	float GetXScale() { return m_scale._11; }
	float GetYScale() { return m_scale._22; }
	float GetZScale() { return m_scale._33; }
};

#endif // !CWORLDMATRIX_H