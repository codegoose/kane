cmake_minimum_required(VERSION 3.10)

project(kane)

add_executable(
	${PROJECT_NAME}
	source/kane/main.cpp
	source/kane/impl.cpp
	source/kane/rendering.cpp
	source/kane/settings.cpp
	source/kane/assets.cpp
	source/kane/timing.cpp
	source/kane/cursor.cpp
	source/kane/input.cpp
	source/kane/local_player.cpp
	source/kane/npc/merchant.cpp
	source/kane/npc/mud_summoner.cpp
	source/kane/npc/mud_minion.cpp
	source/kane/camera.cpp
	source/kane/audio.cpp
	source/kane/game.cpp
)

add_subdirectory(emico)
target_include_directories(${PROJECT_NAME} PUBLIC include emico/include)

add_library(
	imgui
	STATIC
	third-party/source/imgui_demo.cpp
	third-party/source/imgui_draw.cpp
	third-party/source/imgui_freetype.cpp
	third-party/source/imgui_impl_glfw.cpp
	third-party/source/imgui_impl_opengl3.cpp
	third-party/source/imgui_tables.cpp
	third-party/source/imgui_widgets.cpp
	third-party/source/imgui.cpp
)

add_library(
	nanovg
	STATIC
	third-party/source/nanovg.c
)

# mingw-w64-x86_64-freetype has the freetype include files under:
#  /mingw64/include/freetype2/freetype/
# but the headers themselves expect a path like:
#  /mingw64/include/freetype/
find_path(FREETYPE2_FULL_PATH "freetype2\\ft2build.h")
target_include_directories(imgui PUBLIC third-party/include "${FREETYPE2_FULL_PATH}/freetype2/")
target_include_directories(nanovg PUBLIC third-party/include "${FREETYPE2_FULL_PATH}/freetype2/")

target_include_directories(nanovg PUBLIC third-party/include)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_include_directories(${PROJECT_NAME} PUBLIC include third-party/include)
target_link_libraries(${PROJECT_NAME} PUBLIC emico nanovg imgui sfml-audio freetype glfw3 fmt glew32 opengl32)
