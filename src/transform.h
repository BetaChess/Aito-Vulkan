#ifndef AITO_TRANSFORM_H
#define AITO_TRANSFORM_H

#include "vecmath.h"

namespace aito
{

struct Transform
{
	Vec3f translation{};
	Vec3f scale{ 1.0f, 1.0f, 1.0f };


	Mat4f mat4() const;
	Mat3f normalMatrix() const;

	Vec3f rotation{};
};

}


#endif // AITO_TRANSFORM_H