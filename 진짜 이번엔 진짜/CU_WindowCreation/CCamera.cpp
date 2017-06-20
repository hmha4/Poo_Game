#include "stdafx.h"
#include "CCamera.h"


CCamera::CCamera()
{
	m_maxPitch = D3DXToRadian(89.0f);
	m_maxVelocity = 1.0f;
	m_invertY = FALSE;
	m_enableYMovement = TRUE;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_look = D3DXVECTOR3(0.0, 0.0, 1.0f);
	CreateProjectionMatrix(D3DX_PI / 3.0f, 1.3f, 0.1f, 1000.0f);
	Update();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Creates the projection matrix.
Parameters:
[in] fov - Field of view
[in] aspect - Aspect ratio
[in] near - Near plane
[in] far - Far plane
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::CreateProjectionMatrix(float fov, float aspect, float nearPlane, float farPlane)
{
	m_fov = fov;
	m_aspect = aspect;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	D3DXMatrixPerspectiveFovLH(&m_projection, m_fov, m_aspect, m_nearPlane, m_farPlane);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Moves the camera forward and backward
Parameters:
[in] units - Amount to move
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::MoveForward(float units)
{
	if (m_enableYMovement)
	{
		m_velocity += m_look * units;
	}
	else
	{
		D3DXVECTOR3 moveVector(m_look.x, 0.0f, m_look.z);
		D3DXVec3Normalize(&moveVector, &moveVector);
		moveVector *= units;
		m_velocity += moveVector;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Moves the camera left and right
Parameters:
[in] units - Amount to move
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::Strafe(float units)
{
	m_velocity += m_right * units;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Moves the camera up and down
Parameters:
[in] units - Amount to move
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::MoveUp(float units)
{
	if (m_enableYMovement)
	{
		m_velocity.y += units;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Yaw the camera around its Y-axis.
Parameters:
[in] radians - Radians to yaw.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::Yaw(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &m_up, radians);
	D3DXVec3TransformNormal(&m_right, &m_right, &rotation);
	D3DXVec3TransformNormal(&m_look, &m_look, &rotation);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Pitch the camera around its X-axis.
Parameters:
[in] radians - Radians to pitch.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::Pitch(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}

	radians = (m_invertY) ? -radians : radians;
	m_pitch -= radians;
	if (m_pitch > m_maxPitch)
	{
		radians += m_pitch - m_maxPitch;
	}
	else if (m_pitch < -m_maxPitch)
	{
		radians += m_pitch + m_maxPitch;
	}

	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &m_right, radians);
	D3DXVec3TransformNormal(&m_up, &m_up, &rotation);
	D3DXVec3TransformNormal(&m_look, &m_look, &rotation);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Roll the camera around its Z-axis.
Parameters:
[in] radians - Radians to roll.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::Roll(float radians)
{
	if (radians == 0.0f)
	{
		return;
	}
	D3DXMATRIX rotation;
	D3DXMatrixRotationAxis(&rotation, &m_look, radians);
	D3DXVec3TransformNormal(&m_right, &m_right, &rotation);
	D3DXVec3TransformNormal(&m_up, &m_up, &rotation);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the camera and creates a new view matrix.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::Update()
{
	// Cap velocity to max velocity
	if (D3DXVec3Length(&m_velocity) > m_maxVelocity)
	{
		m_velocity = *(D3DXVec3Normalize(&m_velocity, &m_velocity)) * m_maxVelocity;
	}

	// Move the camera
	m_position += m_velocity;
	// Could decelerate here. I'll just stop completely.
	m_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_lookAt = m_position + m_look;

	// Calculate the new view matrix
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_view, &m_position, &m_lookAt, &up);

	//Calculate new View frustum
	BuildViewFrustum();

	// Set the camera axes from the view matrix
	m_right.x = m_view._11;
	m_right.y = m_view._21;
	m_right.z = m_view._31;
	m_up.x = m_view._12;
	m_up.y = m_view._22;
	m_up.z = m_view._32;
	m_look.x = m_view._13;
	m_look.y = m_view._23;
	m_look.z = m_view._33;

	// Calculate yaw and pitch
	float lookLengthOnXZ = sqrtf(m_look.z * m_look.z + m_look.x * m_look.x);
	m_pitch = atan2f(m_look.y, lookLengthOnXZ);
	m_yaw = atan2f(m_look.x, m_look.z);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Build the view frustum planes using the current view/projection matrices
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::BuildViewFrustum()
{
	D3DXMATRIX viewProjection;
	D3DXMatrixMultiply(&viewProjection, &m_view, &m_projection);
	//D3DXMatrixMultiply( &viewProjection, &m_mView, &m_mProjection );
	// Left plane
	m_frustum[0].a = viewProjection._14 + viewProjection._11;
	m_frustum[0].b = viewProjection._24 + viewProjection._21;
	m_frustum[0].c = viewProjection._34 + viewProjection._31;
	m_frustum[0].d = viewProjection._44 + viewProjection._41;

	// Right plane
	m_frustum[1].a = viewProjection._14 - viewProjection._11;
	m_frustum[1].b = viewProjection._24 - viewProjection._21;
	m_frustum[1].c = viewProjection._34 - viewProjection._31;
	m_frustum[1].d = viewProjection._44 - viewProjection._41;

	// Top plane
	m_frustum[2].a = viewProjection._14 - viewProjection._12;
	m_frustum[2].b = viewProjection._24 - viewProjection._22;
	m_frustum[2].c = viewProjection._34 - viewProjection._32;
	m_frustum[2].d = viewProjection._44 - viewProjection._42;

	// Bottom plane
	m_frustum[3].a = viewProjection._14 + viewProjection._12;
	m_frustum[3].b = viewProjection._24 + viewProjection._22;
	m_frustum[3].c = viewProjection._34 + viewProjection._32;
	m_frustum[3].d = viewProjection._44 + viewProjection._42;

	// Near plane
	m_frustum[4].a = viewProjection._13;
	m_frustum[4].b = viewProjection._23;
	m_frustum[4].c = viewProjection._33;
	m_frustum[4].d = viewProjection._43;

	// Far plane
	m_frustum[5].a = viewProjection._14 - viewProjection._13;
	m_frustum[5].b = viewProjection._24 - viewProjection._23;
	m_frustum[5].c = viewProjection._34 - viewProjection._33;
	m_frustum[5].d = viewProjection._44 - viewProjection._43;

	// Normalize planes
	for (int i = 0; i < 6; i++)
	{
		D3DXPlaneNormalize(&m_frustum[i], &m_frustum[i]);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Checks whether a sphere is inside the camera's view frustum.
Parameters:
[in] pPosition - Position of the sphere.
[in] radius - Radius of the sphere.
Returns: TRUE if the sphere is in the frustum, FALSE otherwise
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
BOOL CCamera::SphereInFrustum(D3DXVECTOR3* pPosition, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		if (D3DXPlaneDotCoord(&m_frustum[i], pPosition) + radius < 0)
		{
			// Outside the frustum, reject it!
			return FALSE;
		}
	}
	return TRUE;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the camera and creates a new view matrix.
Parameters:
[in] pPosition - New position
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::SetPosition(D3DXVECTOR3* pPosition)
{
	m_position.x = pPosition->x;
	m_position.y = pPosition->y;
	m_position.z = pPosition->z;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Updates the camera and creates a new view matrix.
Parameters:
[in] pPosition - New target
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CCamera::SetLookAt(D3DXVECTOR3* pLookAt)
{
	m_lookAt.x = pLookAt->x;
	m_lookAt.y = pLookAt->y;
	m_lookAt.z = pLookAt->z;
	D3DXVec3Normalize(&m_look, &(m_lookAt - m_position));
}