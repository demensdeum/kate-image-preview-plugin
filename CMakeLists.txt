cmake_minimum_required(VERSION 3.16)
project(imagepreviewplugin)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
find_package(KF6 REQUIRED COMPONENTS CoreAddons I18n TextEditor)

add_library(imagepreviewplugin MODULE
    ImagePreviewPlugin.cpp
    ImagePreviewPlugin.h
)

target_link_libraries(imagepreviewplugin
    PRIVATE
        Qt6::Widgets
        Qt6::Gui
        KF6::TextEditor
        KF6::CoreAddons
        KF6::I18n
)

install(TARGETS imagepreviewplugin
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf6/ktexteditor
)

install(FILES plugin.json
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf6/ktexteditor/imagepreviewplugin
)
