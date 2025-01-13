cmake_minimum_required(VERSION 3.16)

project(PositionBasedDynamic VERSION 0.1 LANGUAGES CXX)

enable_testing()

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets OpenGLWidgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets Test)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

set(SHARED_SOURCES
    drawarea.h drawarea.cpp
        particle.h
        Vec2.h
        context.h context.cpp
        collider.h collider.cpp
        constraint.h constraint.cpp
        object.h object.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PositionBasedDynamic
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${SHARED_SOURCES}
    )
else()
    add_executable(PositionBasedDynamic
        ${PROJECT_SOURCES}
        ${SHARED_SOURCES}
    )
endif()

target_link_libraries(PositionBasedDynamic PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGLWidgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PositionBasedDynamic)
endif()

# Unit test executable
add_executable(PositionBasedDynamic_Tests
    tst_testgui.h tst_testgui.cpp
    ${SHARED_SOURCES}
)

target_link_libraries(PositionBasedDynamic_Tests
    Qt::Core
    Qt::Test
    Qt::OpenGL
    Qt::OpenGLWidgets
)

add_test(NAME PositionBasedDynamic_Tests COMMAND PositionBasedDynamic_Tests)
