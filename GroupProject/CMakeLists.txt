cmake_minimum_required(VERSION 3.21)

project(GroupProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(VTK REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ModelPart.cpp
        ModelPart.h
        ModelPartList.cpp
        ModelPartList.h
        icons.qrc
        optiondialog.h
        optiondialog.cpp
        optiondialog.ui
        VRRenderThread.h
        VRRenderThread.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GroupProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        icons.qrc
        optiondialog.h optiondialog.cpp optiondialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GroupProject APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GroupProject SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GroupProject
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(GroupProject PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})

vtk_module_autoinit( TARGETS GroupProject MODULES ${VTK_LIBRARIES} )


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GroupProject)
endif()
set_target_properties(GroupProject PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GroupProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES "C:/OpenVR/bin/win64/openvr_api.dll"
        DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES "C:/Qt/6.8.2/msvc2022_64/plugins/platforms/qwindows.dll"
        DESTINATION ${CMAKE_INSTALL_BINDIR}/platforms)


install(FILES $<TARGET_RUNTIME_DLLS:GroupProject>
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(InstallRequiredSystemLibraries)

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
else()
    set(CPACK_GENERATOR "ZIP")
endif()

set(CPACK_NSIS_MENU_LINKS
    "${CMAKE_INSTALL_BINDIR}/GroupProject.exe" "GroupProject"
    "Uninstall.exe" "Uninstall GroupProject"
)

include(CPack)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GroupProject)
endif()

add_custom_target(	VRBindings )
add_custom_command(	TARGET VRBindings PRE_BUILD
COMMAND ${CMAKE_COMMAND} -E
              copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/ )

