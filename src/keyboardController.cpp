#include "pch.h"

#include "keyboardController.h"

namespace aito
{
	void KeyboardController::moveInPlaneXZ(GLFWwindow* window, float dt, Object& object)
	{
		// ROTATION
		Vec3f rotate{ 0.0f };

		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)
			rotate.y += 1.0f;
		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)
			rotate.y -= 1.0f;
		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)
			rotate.x += 1.0f;
		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)
			rotate.x -= 1.0f;

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
			object.transform.rotation += lookSpeed * dt * glm::normalize(rotate);

		object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -1.5f, 1.5f);
		object.transform.rotation.y = glm::mod(object.transform.rotation.y, glm::two_pi<float>());

		// MOVEMENT
		float yaw = object.transform.rotation.y;
		const Vec3f forward{ sin(yaw), 0.0f, cos(yaw) };
		const Vec3f right{ forward.z, 0.0f, -forward.x };
		const Vec3f up{ 0.0f, -1.0f, 0.0f };

		Vec3f move{ 0.0f };
		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
			move += forward;
		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
			move -= forward;
		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
			move += right;
		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
			move -= right;
		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
			move += up;
		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
			move -= up;

		if (glm::dot(move, move) > std::numeric_limits<float>::epsilon())
			object.transform.translation += moveSpeed * dt * glm::normalize(move);

	}
}
