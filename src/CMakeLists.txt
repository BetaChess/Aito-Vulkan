
project ("Aito_Vulkan")

set(SOURCES
	"main.cpp"
    "aito.h"
    "logger.h"
    "logger.cpp"
    "app.cpp" 
    "app.h" 
    "camera.cpp" 
    "camera.h" 
    "descriptor.cpp" 
    "descriptor.h" 
    "device.cpp" 
    "device.h" 
    "pipeline.cpp" 
    "pipeline.h" 
    "renderer.cpp" 
    "renderer.h" 
    "swapchain.cpp" 
    "swapchain.h" 
    "window.cpp" 
    "window.h"
 "pch.h" "transform.h" "transform.cpp" "object.h" "shape.h" "shape.cpp" "utils.h" "buffer.cpp" "buffer.h"  "frame_info.h" "keyboardController.cpp" "keyboardController.h" "point_light_system.cpp" "point_light_system.h" "simple_render_system.cpp" "simple_render_system.h" "time.cpp" "time.h" "vecmath.h")

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} 
    PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

target_include_directories(${PROJECT_NAME} 
    PUBLIC $ENV{VULKAN_SDK}/Include/ 
	PUBLIC ../vendor/glfw/include/
	PUBLIC ../vendor/spdlog/include
    PUBLIC ../vendor/glm
    PUBLIC ../vendor/tinyobjloader/
)

target_link_directories(${PROJECT_NAME}
	PRIVATE ../vendor/glfw/src
    PRIVATE $ENV{VULKAN_SDK}/Lib
)
 
target_link_libraries(${PROJECT_NAME}
	glfw
    vulkan-1
)

# Command to copy models to output folder
add_custom_command(
         TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_CURRENT_SOURCE_DIR}/../models $<TARGET_FILE_DIR:${PROJECT_NAME}>/models
         COMMENT "Copying models" VERBATIM 
         )

# Adding debug PDB
set_property(
    TARGET ${PROJECT_NAME}
    APPEND PROPERTY
        CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)

# Precompiled header
target_precompile_headers(${PROJECT_NAME}
    PUBLIC "pch.h"
)
# TODO: Add tests and install targets if needed.

############## Build SHADERS #######################
 
# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslangValidator HINTS 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)
 
# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/../shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/../shaders/*.vert"
)

message("${GLSL_SOURCE_FILES}")
 
foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${PROJECT_SOURCE_DIR}/../shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)
 
add_custom_target(
    ${PROJECT_NAME}_Shaders
    SOURCES ${GLSL_SOURCE_FILES}
    DEPENDS ${SPIRV_BINARY_FILES}
)

add_custom_command(
    TARGET ${PROJECT_NAME}_Shaders POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory              
        ${CMAKE_CURRENT_SOURCE_DIR}/../shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
    COMMENT "Copying shaders" VERBATIM 
    )

ADD_DEPENDENCIES(${PROJECT_NAME} ${PROJECT_NAME}_Shaders)