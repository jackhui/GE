// Camera.cpp

#include "Camera.h"
#include <Windows.h> // get keyboard state
#include <sstream>
#include <string>

Camera::Camera(const Vector3& m_vPos, const Vector3& m_vLookAt, const Vector3& m_vUp)
	: m_vPos(m_vPos)
	, m_vLookAt(m_vLookAt)
	, m_vUp(m_vUp)
{

}

Matrix4 Camera::getViewMatrix()
{
	Matrix4 cameraMatrix;
	cameraMatrix.CreateLookAt(m_vPos, m_vLookAt, m_vUp);
	return cameraMatrix;
}

void Camera::rotateVPos(const float thetaX, const float thetaY) {
	Matrix4 transform;
	m_vPos -= m_vLookAt;
	transform.CreateRotationX(thetaY);
	m_vPos.TransformAsVector(transform);
	transform.CreateRotationY(thetaX);
	m_vPos.TransformAsVector(transform);
	m_vPos += m_vLookAt;
}

void Camera::rotateVLookAt(const CameraMove moveType, const float theta)
{
	wchar_t s[64];
	swprintf(s, 64, L"%f, %f, %f\n", m_vLookAt.GetX(), m_vLookAt.GetY(), m_vLookAt.GetZ());
	OutputDebugStringW(s);
	float sign = (moveType == CameraMove::ROTATE_RIGHT || moveType == CameraMove::ROTATE_UP) ? 1.0f : -1.0f;
	Matrix4 transform;
	m_vLookAt -= m_vPos;
	switch (moveType) {
		case CameraMove::ROTATE_LEFT:
		case CameraMove::ROTATE_RIGHT:
			transform.CreateRotationY(sign * theta);
			break;
		case CameraMove::ROTATE_UP:
		case CameraMove::ROTATE_DOWN:
			transform.CreateRotationX(sign * theta);
			break;
	}

	m_vLookAt.TransformAsVector(transform);
	m_vLookAt += m_vPos;

	swprintf(s, 64, L"%f, %f, %f\n", m_vPos.GetX(), m_vPos.GetY(), m_vPos.GetZ());
	OutputDebugStringW(s);
}

void Camera::move(const CameraMove moveType, const float offset)
{
	Vector3 transform;
	float sign = (moveType == CameraMove::BACKWARD || moveType == CameraMove::RIGHT) ? 1.0f : -1.0f;
	if (moveType == CameraMove::FORWARD || moveType == CameraMove::BACKWARD) {
		transform = (m_vLookAt - m_vPos).Normalize() * (sign * offset);
	}
	else if (moveType == CameraMove::LEFT || moveType == CameraMove::RIGHT) {
		Matrix4 rotation;
		rotation.CreateRotationY(PI / 2.0f);
		transform = (m_vLookAt - m_vPos).Normalize();
		transform.TransformAsVector(rotation);
		transform = transform * (sign * offset);
	}

	m_vPos += transform;
	m_vLookAt += transform;
}