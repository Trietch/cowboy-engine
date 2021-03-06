cmake_minimum_required(VERSION 3.11)

project(cowboy-engine VERSION "0.1.0")

string(TIMESTAMP COMPILATION_DATE "%d/%m/%Y")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_CXX_FLAGS "-w")
add_subdirectory("externals/OpenMesh")
file(MAKE_DIRECTORY report)


set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -pedantic -fopenmp -pthread -DGL_GLEXT_PROTOTYPES")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_CXX_FLAGS} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS} -O3")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)

find_package(Qt5 COMPONENTS OpenGL REQUIRED)
if (Qt5OpenGL_FOUND)
	message(STATUS "Found Qt ${Qt5OpenGL_VERSION}")
else (Qt5OpenGL_FOUND)
	message(EROR "Unable to find QTOpenGL library, you should probably change line 24 of CMakeLists.txt")
endif(Qt5OpenGL_FOUND)
find_package(OpenGL REQUIRED)

set(CMAKE_AUTOMOC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTORCC on)

configure_file("src/config.h.in" "config.h")

add_executable(${PROJECT_NAME}
	src/config.h
	src/utils.h

 	src/main.cpp
	src/shapes.h

	src/InputManager.h
	src/InputManager.cpp

	src/Shader.h
	src/Scene.h

	src/ECS.h
	src/ECS.cpp


	src/DrawableComponent.h
	src/CameraComponent.h
	src/TransformComponent.h
	src/LightComponent.h
	src/SubdivideComponent.h
	src/MetaballComponent.h
	src/MarchingCubeComponent.h
	src/BoneComponent.h
	src/SkeletonComponent.h

	src/ui/MainWindow.h
	src/ui/MainWindow.cpp
	src/ui/MainWindow.ui

	src/Renderer.h
	src/Renderer.cpp

	src/GlWidget.h
	src/GlWidget.cpp

	src/BSpline.h
	src/BSplineTensor.h
)
set(EXT_LIBS Qt5::OpenGL ${OPENGL_LIBRARIES})


target_include_directories(${PROJECT_NAME}
	PUBLIC "externals/${OPENMESH_INCLUDE_DIRS}"
	PUBLIC "externals/tinygltf"
	PUBLIC "externals/glm"
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE "${OPENMESH_LIBRARIES}"
	${EXT_LIBS}
)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build)
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${EXECUTABLE_OUTPUT_PATH})
message("-- Shaders copied to the build directory")
file(COPY ${CMAKE_SOURCE_DIR}/models DESTINATION ${EXECUTABLE_OUTPUT_PATH})
message("-- Models copied to the build directory")

execute_process(
	COMMAND bash -c "cppcheck --enable=all --inconclusive --library=posix --xml --xml-version=2 --std=c++17 ../src 2>../report/report.xml && cppcheck-htmlreport --source-dir=../src --file=../report/report.xml --report-dir=../report"
)

