#include "pch.h"

#include "camera.h"

#include "vecmath.h"

#include <cassert>
#include <limits.h>

namespace aito
{
	void Camera::setOrthographicProjection(
		float left, float right, float top, float bottom, float near, float far) {
		projectionMatrix_ = Mat4f{ 1.0f };
		projectionMatrix_[0][0] = 2.f / (right - left);
		projectionMatrix_[1][1] = 2.f / (bottom - top);
		projectionMatrix_[2][2] = 1.f / (far - near);
		projectionMatrix_[3][0] = -(right + left) / (right - left);
		projectionMatrix_[3][1] = -(bottom + top) / (bottom - top);
		projectionMatrix_[3][2] = -near / (far - near);
	}

	void Camera::setPerspectiveProjection(float fovy, float aspect, float near, float far) {
		assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
		const float tanHalfFovy = tan(fovy / 2.f);
		projectionMatrix_ = Mat4f{ 0.0f };
		projectionMatrix_[0][0] = 1.f / (aspect * tanHalfFovy);
		projectionMatrix_[1][1] = 1.f / (tanHalfFovy);
		projectionMatrix_[2][2] = far / (far - near);
		projectionMatrix_[2][3] = 1.f;
		projectionMatrix_[3][2] = -(far * near) / (far - near);
	}

	void Camera::setViewDirection(Point3f position, Vec3f direction, Vec3f up) {
		assert(direction != Vec3f(0.0f) && "Direction vector cannot be a null vector: setViewDirection");

		// Generate orthonormal basis vectors
		const Vec3f w{ glm::normalize(direction) };
		const Vec3f u{ glm::normalize(glm::cross(w, up)) };
		const Vec3f v{ glm::cross(w, u) };

		// Generate view matrix
		viewMatrix_ = Mat4f{ 1.f };
		viewMatrix_[0][0] = u.x;
		viewMatrix_[1][0] = u.y;
		viewMatrix_[2][0] = u.z;
		viewMatrix_[0][1] = v.x;
		viewMatrix_[1][1] = v.y;
		viewMatrix_[2][1] = v.z;
		viewMatrix_[0][2] = w.x;
		viewMatrix_[1][2] = w.y;
		viewMatrix_[2][2] = w.z;
		viewMatrix_[3][0] = -glm::dot(u, position);
		viewMatrix_[3][1] = -glm::dot(v, position);
		viewMatrix_[3][2] = -glm::dot(w, position);
	}

	void Camera::setViewTarget(Point3f position, Point3f target, Vec3f up) {
		// Calculate direction
		const Vec3f direction = target - position;
		assert(direction != Vec3f(0.0f) && "Direction vector cannot be a null vector: setViewTarget");

		// Set the view direction
		setViewDirection(position, direction, up);
	}

	void Camera::setViewYXZ(Point3f position, Vec3f rotation) {
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);
		const Vec3f u{ (c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1) };
		const Vec3f v{ (c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3) };
		const Vec3f w{ (c2 * s1), (-s2), (c1 * c2) };
		viewMatrix_ = Mat4f{ 1.f };
		viewMatrix_[0][0] = u.x;
		viewMatrix_[1][0] = u.y;
		viewMatrix_[2][0] = u.z;
		viewMatrix_[0][1] = v.x;
		viewMatrix_[1][1] = v.y;
		viewMatrix_[2][1] = v.z;
		viewMatrix_[0][2] = w.x;
		viewMatrix_[1][2] = w.y;
		viewMatrix_[2][2] = w.z;
		viewMatrix_[3][0] = -glm::dot(u, position);
		viewMatrix_[3][1] = -glm::dot(v, position);
		viewMatrix_[3][2] = -glm::dot(w, position);
	}
}