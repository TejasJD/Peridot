# CMakeList.txt : CMake project for Peridot, include source and define
# project specific logic here.
#

set(PERIDOT_SRC_FILES
	"src/Buffer.cpp"
	"src/Context.cpp"
	"src/Core.cpp"
	"src/Renderer.cpp"
	"src/Shader.cpp"
	"src/Texture.cpp"
	"src/TimeTracker.cpp"
	"src/Utils.cpp"
	"src/VertexArray.cpp"
	"src/RenderCalls.cpp"
	"src/Camera.cpp"
 "src/Input.cpp")

set(PERIDOT_PUBLIC_HEADERS
	"include/Peridot/Core.h"
	"include/Peridot/RenderCalls.h"
	"include/Peridot/Camera.h"
 "include/Peridot/Input.h" "include/Peridot/KeyCodes.h" "include/Peridot/MouseCodes.h" "include/Peridot/OrthographicCamController.h" "include/Peridot/PerspectiveCamController.h")

# Add source to this project's executable.
add_library (Peridot
	${PERIDOT_SRC_FILES}
	${PERIDOT_PUBLIC_HEADERS}
)

target_include_directories(Peridot
	PUBLIC "include"
	PRIVATE "src"
)

target_link_libraries(Peridot
	PRIVATE
	glfw
	glad::glad
	spdlog::spdlog
	PUBLIC
	glm::glm-header-only
)

# TODO: Add tests and install targets if needed.