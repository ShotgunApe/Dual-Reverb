cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.12 CACHE STRING "")

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(vst3sdk_SOURCE_DIR "../../../../../vstsdk/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(IS-Reverb
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "IS-Reverb VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(IS-Reverb
    source/version.h
    source/myplugincids.h
    source/mypluginprocessor.h
    source/mypluginprocessor.cpp
    source/myplugincontroller.h
    source/myplugincontroller.cpp
    source/mypluginentry.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ENABLE_VSTGUI_SUPPORT)
    target_sources(IS-Reverb
        PRIVATE
            resource/myplugineditor.uidesc
    )
    target_link_libraries(IS-Reverb
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(IS-Reverb
        RESOURCES
            "resource/myplugineditor.uidesc"
    )
endif(SMTG_ENABLE_VSTGUI_SUPPORT)
# -------------------

smtg_target_add_plugin_snapshots (IS-Reverb
    RESOURCES
        resource/099A656FF6B1532DB9389F6F9E80B962_snapshot.png
        resource/099A656FF6B1532DB9389F6F9E80B962_snapshot_2.0x.png
        resource/test-v2.png
)

target_link_libraries(IS-Reverb
    PRIVATE
        sdk
)

smtg_target_configure_version_file(IS-Reverb)

if(SMTG_MAC)
    smtg_target_set_bundle(IS-Reverb
        BUNDLE_IDENTIFIER com.fhw.vst3.reverb
        COMPANY_NAME "fhw"
    )
    smtg_target_set_debug_executable(IS-Reverb
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(IS-Reverb PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT IS-Reverb)

        smtg_target_set_debug_executable(IS-Reverb
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
