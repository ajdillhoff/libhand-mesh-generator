CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT)
FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)
FIND_PACKAGE(OpenCV PATHS ${OPENCV_PATH} NO_DEFAULT_PATH)
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(OGRE REQUIRED)
FIND_PACKAGE(PythonLibs 3 REQUIRED)
FIND_PACKAGE(pybind11 REQUIRED)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${LibHand_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${pybind11_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

#ADD_LIBRARY(PoseGenerator
    #PoseGenerator.cpp
    #PoseConfig.cpp
    #PosePrototype.cpp
#)

#TARGET_LINK_LIBRARIES(PoseGenerator
    #dot_sceneloader
    #${LibHand_LIBRARIES}
    #${OPENGL_LIBRARIES}
    #${pybind11_LIBRARIES}
#)

#ADD_EXECUTABLE(GeneratePose generate_random.cpp)

#TARGET_LINK_LIBRARIES(GeneratePose
    #PoseGenerator
    #${Boost_FILESYSTEM_LIBRARY}
    #${Boost_SYSTEM_LIBRARY}
    #${OpenCV_LIBS}
    #${TIFF_LIBRARIES}
    #/usr/lib/x86_64-linux-gnu/libtiff.so.5
#)

pybind11_add_module(PoseGenerator
    PoseGenerator.cpp
    PoseConfig.cpp
    PosePrototype.cpp
    ndarray_converter.cpp
)

TARGET_LINK_LIBRARIES(PoseGenerator PRIVATE
    dot_sceneloader
    ${PYTHON_LIBRARIES}
    ${LibHand_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${pybind11_LIBRARIES}
)

message(STATUS "================================================")
message(STATUS "                BUILD DIAGNOSTICS               ")
message(STATUS "")
message(STATUS "PYTHON_INCLUDE_DIRS: ${PYTHON_INCLUDE_DIRS}")
message(STATUS "PYTHON_LIBRARIES: ${PYTHON_LIBRARIES}")
message(STATUS "LIBHAND_PATH: ${LIBHAND_PATH}")
message(STATUS "LibHand_INCLUDE_DIRS: ${LibHand_INCLUDE_DIRS}")
message(STATUS "LibHand_LIBRARIES: ${LibHand_LIBRARIES}")
message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
message(STATUS "================================================")
