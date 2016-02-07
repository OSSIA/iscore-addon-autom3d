cmake_minimum_required(VERSION 3.1)
project(iscore_plugin_autom3d LANGUAGES CXX)

###  General initialization of always necessary stuff (AUTOMOC, etc) ###
iscore_common_setup()

### Requirements ###
if(NOT TARGET iscore_plugin_ossia)
    return()
endif()

find_package(Qt5 5.3 REQUIRED COMPONENTS Core Widgets)

find_package(VTK COMPONENTS
  vtkCommonCore
  vtkFiltersSources
  vtkInteractionWidgets
  vtkGUISupportQt
)
if(NOT ${VTK_FOUND})
    return()
endif()

### Files & main target ###
file(GLOB_RECURSE HDRS
    "${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/*.hpp"
)
set(SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Autom3DSerialization.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Autom3DLayerSerialization.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Autom3DModel.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Autom3DLayerModel.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Autom3DPanelProxy.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Commands/ChangeAddress.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Commands/Autom3DCommandFactory.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Executor/Executor.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Executor/Component.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Executor/ComponentFactory.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/State/Autom3DState.cpp"

"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Panel/AutomWidget.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Panel/AutomWidget.hpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Panel/AutomWidget.ui"

"${CMAKE_CURRENT_SOURCE_DIR}/iscore_plugin_autom3d.cpp"
)

if(TARGET iscore_lib_inspector)
    set(INSPECTOR_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Inspector/Autom3DInspectorFactory.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Inspector/Autom3DStateInspector.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Inspector/Autom3DStateInspectorFactory.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Inspector/Autom3DInspectorWidget.cpp"
    )

    set(SRCS ${SRCS} ${INSPECTOR_SRCS})

    file(GLOB_RECURSE INSPECTOR_HDRS
        "${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Inspector/*.hpp")

    set(HDRS ${HDRS} ${INSPECTOR_HDRS})
endif()



qt5_add_resources(QRCS "${CMAKE_CURRENT_SOURCE_DIR}/Autom3D/Resources/Autom3DResources.qrc")

add_library(${PROJECT_NAME} ${SRCS} ${HDRS} ${QRCS})

### If there are commands this will generate the files showcased in iscore_plugin_autom3d.cpp ###
iscore_generate_command_list_file(${PROJECT_NAME} "${HDRS}")

target_link_libraries(${PROJECT_NAME} PUBLIC
                     Qt5::Core Qt5::Widgets
                     iscore_lib_base iscore_lib_device iscore_plugin_deviceexplorer iscore_lib_process iscore_lib_state  iscore_plugin_ossia iscore_lib_dummyprocess
                     )

if(TARGET iscore_lib_inspector)
    target_link_libraries(${PROJECT_NAME} PUBLIC iscore_lib_inspector)
endif()

### Vtk set-up ###
target_compile_definitions(${PROJECT_NAME} PRIVATE "${VTK_DEFINITIONS}")
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE  ${VTK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

### Setup of the target ###
setup_iscore_plugin(${PROJECT_NAME})
