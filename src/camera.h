#ifndef AITO_CAMERA_H
#define AITO_CAMERA_H

#include "vecmath.h"


namespace aito
{
	class Camera
	{

	public:

		void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
		void setPerspectiveProjection(float fovy, float aspect, float near, float far);

		void setViewDirection(Point3f position, Vec3f direction, Vec3f up = Vec3f(0.0f, -1.0f, 0.0f));
		void setViewTarget(Point3f position, Point3f target, Vec3f up = Vec3f(0.0f, -1.0f, 0.0f));
		void setViewYXZ(Point3f position, Vec3f rotation);

		inline const Mat4f& getProjection() const { return projectionMatrix_; };
		inline const Mat4f& getView() const { return viewMatrix_; };

	private:
		Mat4f projectionMatrix_{ 1.0f };
		Mat4f viewMatrix_{ 1.0f };
	};
}


#endif /* AITO_CAMERA_H */
