cmake_minimum_required(VERSION 3.10)

project(gorgon)

add_executable(
	${PROJECT_NAME}
	main.cpp
	rendering.cpp
	settings.cpp
	assets.cpp
	timing.cpp
)

add_library(
	imgui
	STATIC
	imgui_demo.cpp
	imgui_draw.cpp
	imgui_freetype.cpp
	imgui_impl_glfw.cpp
	imgui_impl_opengl3.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	imgui.cpp
)

add_library(
	nanovg
	STATIC
	nanovg.c
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories(${PROJECT_NAME} PUBLIC .)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui nanovg freetype uv sfml-audio glfw3 fmt simdjson glew32 opengl32)
