cmake_minimum_required(VERSION 3.16)

project(MusStream VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Sql Multimedia)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(MusStream
    src/main.cpp
)

qt_add_qml_module(MusStream
    URI MusStream
    QML_FILES
        src/Main.qml
        RESOURCES src/Resources.qrc
        QML_FILES src/interface/Home.qml
        QML_FILES src/interface/Search.qml
        SOURCES src/database.h src/database.cpp
        QML_FILES src/interface/Sources.qml
        QML_FILES src/interface/compoments/MusicRow.qml
        SOURCES src/sourcemanager.h src/sourcemanager.cpp
        SOURCES src/backend.h src/backend.cpp
        QML_FILES src/interface/Settings.qml
)
qt6_add_resources(QRC_SOURCES src/Resources.qrc)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
target_sources(MusStream PRIVATE ${QRC_SOURCES})

set_target_properties(MusStream PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MusStream
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
if(WIN32)
    set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/src/resources/app_icon.rc")
    # Ajouter le fichier .rc aux sources de l'exécutable
    target_sources(MusStream PRIVATE ${app_icon_resource_windows})
endif()

target_link_libraries(MusStream
    PRIVATE Qt6::Quick Qt6::Sql Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS MusStream
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
#add_subdirectory(curl-8.19.0)
