#ifndef AITO_OBJECT_H
#define AITO_OBJECT_H

#include "transform.h"
#include "shape.h"

namespace aito
{

// Something that can be rendered
class Object
{
public:
	// NOTE: change to shape
	std::shared_ptr<Model> model{};
	Vec3f color{};
	Transform transform{};

};

}


#endif // AITO_OBJECT_H