cmake_minimum_required(VERSION 2.8.11)

project(openalpr_tagger)

SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 0)
SET(VERSION_PATCH 0)

SET (VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
add_definitions(-DVERSION_MAJOR=${VERSION_MAJOR})
add_definitions(-DVERSION_MINOR=${VERSION_MINOR})
add_definitions(-DVERSION_PATCH=${VERSION_PATCH})
add_definitions(-DVERSION_STRING="${VERSION}")


add_definitions(-DTARGET="openalpr_tagger")
add_definitions(-DTARGET_STRING="openalpr_tagger")
add_definitions(-DTARGET_UPPER_STRING="OPENALPR_TAGGER")
add_definitions(-DTARGET_HUMAN_STRING="openalpr_tagger")

ADD_DEFINITIONS(
    -std=c++11 
)


# Find the QtWidgets library
find_package(Qt5Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Tell CMake to create the helloworld executable
add_executable(openalpr_tagger 
about.cpp
directoryloader.cpp
dot.cpp
imagefile.cpp
imageview.cpp
imageviewerbase.cpp
imageviewerobserver.cpp
imageviewerplateselector.cpp
main.cpp
mainwindow.cpp
options.cpp
platefile.cpp
plateselector.cpp
polygons.cpp
selection.cpp
settings.cpp
squeezedlabel.cpp
threadedimageloader.cpp
utils.cpp
qprogressindicator/QProgressIndicator.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/qprogressindicator)
# Use the Widgets module from Qt 5.
target_link_libraries(openalpr_tagger Qt5::Widgets)
