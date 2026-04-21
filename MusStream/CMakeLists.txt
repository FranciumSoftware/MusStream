cmake_minimum_required(VERSION 3.16)

project(MusStream VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Sql Multimedia)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appMusStream
    main.cpp
)

qt_add_qml_module(appMusStream
    URI MusStream
    QML_FILES
        Main.qml
        RESOURCES Resources.qrc
        QML_FILES interface/Home.qml
        QML_FILES interface/Search.qml
        SOURCES database.h database.cpp
        QML_FILES interface/Sources.qml
        SOURCES sourcemanager.h sourcemanager.cpp
        SOURCES backend.h backend.cpp
)
qt6_add_resources(QRC_SOURCES Resources.qrc)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
target_sources(appMusStream PRIVATE ${QRC_SOURCES})

set_target_properties(appMusStream PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMusStream
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
if(WIN32)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources/app_icon.rc")
    # Ajouter le fichier .rc aux sources de l'exécutable
    target_sources(appMusStream PRIVATE ${app_icon_resource_windows})
endif()

target_link_libraries(appMusStream
    PRIVATE Qt6::Quick Qt6::Sql Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS appMusStream
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
#add_subdirectory(curl-8.19.0)