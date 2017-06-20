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
	D3DXVECTOR3 m_position;
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
	D3DXVECTOR3* GetPosition() { return &m_position; }
	float GetXPosition() { return m_translate._41; }
	float GetYPosition() { return m_translate._42; }
	float GetZPosition() { return m_translate._43; }
	float GetXRotation() { return m_rotationX; }
	float GetYRotation() { return m_rotationY; }
	float GetZRotation() { return m_rotationZ; }
	float GetXScale()    { return m_scale._11; }
	float GetYScale()    { return m_scale._22; }
	float GetZScale()    { return m_scale._33; }
	void SetXPosition(float x) { m_translate._41 = x; }
	void SetYPosition(float y) { m_translate._42 = y; }
	void SetZPosition(float z) { m_translate._43 = z; }
	void SetXRotation(float x) { RotateAbs(x, m_rotationY, m_rotationZ); }
	void SetYRotation(float y) { RotateAbs(m_rotationX, y, m_rotationZ); }
	void SetZRotation(float z) { RotateAbs(m_rotationX, m_rotationY, z); }
	void SetXScale(float x)    { m_scale._11 = x; }
	void SetYScale(float y)    { m_scale._22 = y; }
	void SetZScale(float z)    { m_scale._33 = z; }
};

#endif // !CWORLDMATRIX_H