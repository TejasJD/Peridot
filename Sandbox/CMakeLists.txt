set(SANDBOX_SRC_FILES
	"Main.cpp"
)

add_executable(Sandbox ${SANDBOX_SRC_FILES})

target_link_libraries(Sandbox
	PRIVATE
	Peridot
	glad::glad
	spdlog::spdlog
	glm::glm-header-only
)

configure_file("shader.vert" "shader.vert" COPYONLY)
configure_file("shader.frag" "shader.frag" COPYONLY)