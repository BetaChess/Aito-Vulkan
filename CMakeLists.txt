
cmake_minimum_required (VERSION 3.8)

project ("Aito_Vulkan")

# Include sub-projects.
add_subdirectory ("src")
add_subdirectory ("vendor/glfw")
add_subdirectory ("vendor/glm")
add_subdirectory ("vendor/spdlog")