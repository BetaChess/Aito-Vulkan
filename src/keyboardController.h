#ifndef AITO_KEYBOARDCONTROLLER_H
#define AITO_KEYBOARDCONTROLLER_H

#include "object.h"
#include "window.h"


namespace aito
{
	class KeyboardController
	{
	public:
		struct KeyMappings
		{
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_SPACE;
			int moveDown = GLFW_KEY_LEFT_SHIFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookLeft = GLFW_KEY_LEFT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		void moveInPlaneXZ(GLFWwindow* window, float dt, Object& object);

		KeyMappings keys{};
		float moveSpeed{ 3.0f };
		float lookSpeed{ 1.5f };
	};
}



#endif /* AITO_KEYBOARDCONTROLLER_H */
