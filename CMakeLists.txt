#
# This file is part of the KandVST synthesizer.
#
# Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
#                      Sarosh Nasir, Stanisław Zwierzchowski,
#                      Klas Ludvigsson and Andreas Kuszli
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#

cmake_minimum_required(VERSION 3.4)
project(KandVST)
enable_testing()

### Project default configuration
set(ALLOW_PARTIAL_TEST true 
	CACHE BOOL "Allow tester to run a subset of tests if python is not available")
set(AudioPlugin_jucer_FILE "${CMAKE_SOURCE_DIR}//AudioPlugin.jucer" 
    CACHE STRING "Path to project jucere file")
set(TEAMCITY_TEST_MODE false 
	CACHE BOOL "Make tester exe output information to teamcity")
set(VST_SDK_URL "https://download.steinberg.net/sdk_downloads/vstsdk3613_08_04_2019_build_81.zip" 
	CACHE STRING "Url to the Steinberg VST sdk")
set(JUCE_SDK_URL "https://github.com/WeAreROLI/JUCE/archive/5.4.3.zip" 
	CACHE STRING "Url to the JUCE sdk")
set(FORCE_GIT_INFO false
	CACHE BOOL "Is it required that we find the git sha?")

add_definitions(-DJUCE_WEB_BROWSER=0)

find_package(PythonInterp 3)
if(EXISTS ${PYTHON_EXECUTABLE})
    #Build Resources
    execute_process(
      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
      COMMAND ${PYTHON_EXECUTABLE}  "buildResc.py" "Resources"
      )
else()
    message(STATUS "Python not found! Skipping update of resources")
endif()

macro(NO_GIT_INFO_FOUND)
	if(FORCE_GIT_INFO)
		message(FATAL_ERROR "Could not find git info, but cache option forces us to find it")
	else()
		message(WARNING "Could not find source git sha, will build without version information")
		set(GIT_LONG_SHA "unk")
		set(GIT_SHORT_SHA "unk")
	endif()
endmacro(NO_GIT_INFO_FOUND)

find_program(GIT_EXE NAMES "git" "Git" )
if(GIT_EXE)
	execute_process(COMMAND ${GIT_EXE} "log" "--pretty=format:%H" "-n 1" RESULT_VARIABLE LONG_SHA_RES OUTPUT_VARIABLE GIT_LONG_SHA)
	execute_process(COMMAND ${GIT_EXE} "log" "--pretty=format:%h" "-n 1" RESULT_VARIABLE SHORT_SHA_RES OUTPUT_VARIABLE GIT_SHORT_SHA)
	if( (NOT (LONG_SHA_RES EQUAL 0)) OR (NOT (SHORT_SHA_RES EQUAL 0) ) )
		NO_GIT_INFO_FOUND()
	else()
		message(STATUS "Building version ${GIT_LONG_SHA}")
	endif()
else()
	message(WARNING "Could not find the git executable")
	NO_GIT_INFO_FOUND()
endif()




macro(SELECT_VST_SDK_VERSION)
		if(EXISTS "${CMAKE_BINARY_DIR}/Download/VST_SDK/VST3_SDK/")
			if(${VST_SDK_URL} MATCHES "vstsdk([0-9]+)_([0-9]+)_([0-9]+)_([0-9]+)_build_([0-9]+)")
				set(VST_VERSION ${CMAKE_MATCH_1})
				set(VST_BUILD ${CMAKE_MATCH_5})
				set(VST_PATH "${CMAKE_BINARY_DIR}/Download/VST_SDK/VST3_SDK/")
				message(STATUS "Found VST version " ${VST_VERSION} " build " ${VST_BUILD})
				set(VST_SDK_INSTALLED TRUE CACHE INTERNAL "")
			else()
				set(VST_SDK_INSTALLED FALSE CACHE INTERNAL "")
				message(FATAL_ERROR "Can't figure out version of vst sdk")
			endif()
		else()
				set(VST_SDK_INSTALLED FALSE CACHE INTERNAL "")
				message(FATAL_ERROR "Could not find VST_SDK folder in downloads! If you rerun cmake I will try to download it")
		endif()
endmacro()

macro(SELECT_JUCE_SDK_VERSION)
	file(GLOB JUCE_FILES "${CMAKE_BINARY_DIR}/Download/JUCE-*")
	list(LENGTH JUCE_FILES JUCE_FILES_LEN)
	if(JUCE_FILES_LEN EQUAL 1)
		list(GET JUCE_FILES 0 JUCE_PATH)
		if(EXISTS "${JUCE_PATH}/modules" )
			if (${JUCE_PATH} MATCHES "JUCE-([0-9]+\.[0-9]+\.[0-9]+)")
				set(JUCE_VERSION ${CMAKE_MATCH_1})
				set(JUCE_MODULES "${JUCE_PATH}/modules/")
				message(STATUS "Found juce version ${JUCE_VERSION}" )
				set(JUCE_SDK_INSTALLED TRUE CACHE INTERNAL "")
			else()
				set(JUCE_SDK_INSTALLED FALSE CACHE INTERNAL "")
				message(FATAL_ERROR "Failed to determain juce version")
			endif()
		else()
			set(JUCE_SDK_INSTALLED FALSE CACHE INTERNAL "")
			message(FATAL_ERROR "Found more than one juce sdk, can't decide which to use")
		endif()
	else()
		set(JUCE_SDK_INSTALLED FALSE CACHE INTERNAL "")
		message(FATAL_ERROR "Found more than one juce sdk, can't decide which to use")
	endif()
endmacro()

#Download the  vst sdk
if(VST_SDK_INSTALLED)
	#We have already downloaded the sdk :)
	SELECT_VST_SDK_VERSION()
else()
	#Download SDKs
	message(STATUS "Downloading VST sdk...")
	file(DOWNLOAD ${VST_SDK_URL} ${CMAKE_BINARY_DIR}/Download/vst_sdk.zip STATUS VST_SDK_STATUS )
	list(GET VST_SDK_STATUS 0 VST_STATUS)
	if( VST_STATUS EQUAL 0)
		message(STATUS "Unpackning VST sdk...")
		execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_BINARY_DIR}/Download/vst_sdk.zip WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Download)
		file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/Download/VST_SDK/VST2_SDK")
		file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/Download/VST_SDK/my_plugins")
		file(REMOVE "${CMAKE_BINARY_DIR}/Download/vst_sdk.zip")
		file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/Download/VST_SDK/VST3_SDK/doc")
		SELECT_VST_SDK_VERSION()
	else()
		list(GET VST_SDK_STATUS 1 VST_ERROR)
		message(FATAL_ERROR "Could not download vst sdk due to error "  ${VST_ERROR})
	endif()
endif()

#Download the juce sdk
if(JUCE_SDK_INSTALLED)
	#We have already downloaded juce
	SELECT_JUCE_SDK_VERSION()
else()
	message(STATUS "Downloading JUCE sdk...")
	file(DOWNLOAD ${JUCE_SDK_URL} ${CMAKE_BINARY_DIR}/Download/juce_sdk.zip STATUS JUCE_SDK_STATUS )
	list(GET JUCE_SDK_STATUS 0 JUCE_STATUS)
	if( JUCE_STATUS EQUAL 0)
		message(STATUS "Unpackning JUCE sdk...")
		execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_BINARY_DIR}/Download/juce_sdk.zip WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/Download)
		SELECT_JUCE_SDK_VERSION()
		file(REMOVE "${CMAKE_BINARY_DIR}/Download/juce_sdk.zip")
	else()
		list(GET JUCE_SDK_STATUS 1 JUCE_ERROR)
		message(FATAL_ERROR "Could not download juce sdk due to error "  ${JUCE_ERROR})
	endif()
endif()
	
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/third_party/FRUT/cmake")
include(Reprojucer)


if(NOT DEFINED AudioPlugin_jucer_FILE)
  message(FATAL_ERROR "AudioPlugin_jucer_FILE must be defined")
endif()

get_filename_component(AudioPlugin_jucer_FILE
  "${AudioPlugin_jucer_FILE}" ABSOLUTE
  BASE_DIR "${CMAKE_BINARY_DIR}"
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})
include_directories("${CMAKE_SOURCE_DIR}//Source//")
include_directories("${CMAKE_SOURCE_DIR}//Source//Generators//")
include_directories("${CMAKE_SOURCE_DIR}//Source//Interfaces//")
include_directories("${CMAKE_SOURCE_DIR}//Source//Editor//")
include_directories("${CMAKE_SOURCE_DIR}//Source//Util//")
include_directories("${CMAKE_SOURCE_DIR}//Source//Processor//")
include_directories("${CMAKE_SOURCE_DIR}//Source//Effects//")

file(WRITE "${CMAKE_BINARY_DIR}/cmake_versions.cpp" 
	"#include <string>\n"
	"#include <array>\n"
	"namespace CMAKE {\n"
	"std::array<std::string,8> VERSION_INFO= {\"JUCE_VERSION\", \"${JUCE_VERSION}\" , \"VST_VERSION\", \"${VST_VERSION}\", \"GIT_LONG\", \"${GIT_LONG_SHA}\", \"GIT_SHORT\", \"${GIT_SHORT_SHA}\" };\n}\n"
)

jucer_project_begin(
  JUCER_VERSION "5.2.0"
  PROJECT_FILE "${AudioPlugin_jucer_FILE}"
  PROJECT_ID "0nRd9LLGO"
)

jucer_project_settings(
  PROJECT_NAME "KandVST"
  PROJECT_VERSION "1.0.0"
  COMPANY_NAME ""
  COMPANY_COPYRIGHT ""
  COMPANY_WEBSITE ""
  COMPANY_EMAIL ""
  REPORT_JUCE_APP_USAGE OFF # Required for closed source applications without an Indie or Pro JUCE license
  DISPLAY_THE_JUCE_SPLASH_SCREEN OFF # Required for closed source applications without an Indie or Pro JUCE license
  SPLASH_SCREEN_COLOUR "Dark"
  PROJECT_TYPE "Audio Plug-in"
  BUNDLE_IDENTIFIER "KandVST"
)

jucer_audio_plugin_settings(
  BUILD_VST ON
  BUILD_VST3 ON
  BUILD_AUDIOUNIT OFF
  BUILD_AUDIOUNIT_V3 OFF
  BUILD_RTAS OFF
  BUILD_AAX OFF
  BUILD_STANDALONE_PLUGIN ON
  PLUGIN_NAME "KandVST"
  PLUGIN_DESCRIPTION "KandVST"
  PLUGIN_MANUFACTURER "Chalmers studenter"
  PLUGIN_MANUFACTURER_CODE "chas"
  PLUGIN_CODE "ktcs"
  # PLUGIN_CHANNEL_CONFIGURATIONS
  PLUGIN_IS_A_SYNTH ON
  PLUGIN_MIDI_INPUT ON
  PLUGIN_MIDI_OUTPUT OFF
  MIDI_EFFECT_PLUGIN OFF
  KEY_FOCUS ON
  PLUGIN_AU_EXPORT_PREFIX "JuceDemoProjectAU"
  # PLUGIN_AU_MAIN_TYPE
  # VST_CATEGORY
  # PLUGIN_RTAS_CATEGORY
  PLUGIN_AAX_CATEGORY "AAX_ePlugInCategory_Dynamics"
  PLUGIN_AAX_IDENTIFIER "com.yourcompany.JuceDemoPlugin"
)

jucer_project_files("KandVST"
# Compile   Xcode     Binary
#           Resource  Resource
x         .         .         "Source/Editor/PluginGUI.cpp"
.         .         .         "Source/Editor/PluginGUI.h"
  x         .         .         "Source/Generators/EnvelopeGenerator.cpp"
  .         .         .         "Source/Generators/EnvelopeGenerator.h"
  x         .         .         "Source/Processor/PluginProcessor.cpp"
  .         .         .         "Source/Processor/PluginProcessor.h"
  x         .         .         "Source/Processor/PipelineManager.cpp"
  .         .         .         "Source/Processor/PipelineManager.h"
  x         .         .         "Source/Processor/Pipeline.cpp"
  .         .         .         "Source/Processor/Pipeline.h"
  .         .         .         "Source/Interfaces/IGenerator.h"
  x         .         .         "Source/Interfaces/IGenerator.cpp"  
  .         .         .         "Source/Interfaces/IEffect.h"
  x         .         .         "Source/Effects/DelayEffect.cpp"
  .         .         .         "Source/Effects/DelayEffect.h"
  x         .         .         "Source/Effects/BitcrushEffect.cpp"
  .         .         .         "Source/Effects/BitcrushEffect.h"
  x         .         .         "Source/Util/ParameterHandler.cpp"
  .         .         .         "Source/Util/ParameterHandler.h"
  .         .         .         "Source/Interfaces/IWavetable.h"
  .         .         .         "Source/Generators/Wavetable.h"
  x         .         .         "Source/Generators/Wavetable.cpp"
  x         .         .         "Source/Generators/WavetableOsc.cpp"
  .         .         .         "Source/Generators/WavetableOsc.h"
  x         .         .         "Source/Util/Log.cpp"
  .         .         .         "Source/Util/Log.h"
  .         .         .         "Source/Util/Global.h"
  x         .         .         "Source/Util/Global.cpp"
  .         .         .         "Source/Util/TemplateHelper.h"
  x         .         .         "Source/Components/ConsoleComponent.cpp"
  .         .         .         "Source/Components/ConsoleComponent.h"
  .         .         .         "Source/Interfaces/IVSTParameters.h"
  x         .         .         "Source/Components/ParameterSlider.cpp"
  .         .         .         "Source/Components/ParameterSlider.h"
  x         .         .         "Source/Components/EnvelopeComponent.cpp"
  .         .         .         "Source/Components/EnvelopeComponent.h"
  x         .         .         "Source/Generators/LFO.cpp"
  .         .         .         "Source/Generators/LFO.h"
  x         .         .         "Source/Components/InformationComponent.cpp"
  .         .         .         "Source/Components/InformationComponent.h"
  x         .         .         "Source/Components/OscillatorComponent.cpp"
  .         .         .         "Source/Components/OscillatorComponent.h"  
  x         .         .         "Source/Components/MasterPageComponent.cpp"
  .         .         .         "Source/Components/MasterPageComponent.h"
  x         .         .         "Source/Components/OscillatorPageComponent.cpp"
  .         .         .         "Source/Components/OscillatorPageComponent.h"
  x         .         .         "Source/Components/LFOComponent.cpp"
  .         .         .         "Source/Components/LFOComponent.h"
  x         .         .         "Source/Effects/FilterButterworth.cpp"
  .         .         .         "Source/Effects/FilterButterworth.h"
  x         .         .         "Source/Effects/FilterLP.cpp"
  .         .         .         "Source/Effects/FilterLP.h"
  x         .         .         "Source/Effects/FilterHP.cpp"
  .         .         .         "Source/Effects/FilterHP.h"
  x         .         .         "Source/Components/ParameterButton.cpp"
  .         .         .         "Source/Components/ParameterButton.h"
  x         .         .         "Source/Editor/PresetManager.cpp"
  .         .         .         "Source/Editor/PresetManager.h"
  x         .         .         "Source/Components/PresetsComponent.cpp"
  .         .         .         "Source/Components/PresetsComponent.h"
  x         .         .         "Source/Components/PresetRow.cpp"
  .         .         .         "Source/Components/PresetRow.h"
  x         .         .         "Source/Editor/OurLookAndFeel.cpp"
  .         .         .         "Source/Editor/OurLookAndFeel.h"
  x         .         .         "Source/Util/Resources_files.cpp"
  .         .         .         "Source/Util/Resources_files.h"
  x         .         .         "Source/Components/MixerComponent.cpp"
  .         .         .         "Source/Components/MixerComponent.h"
  x         .         .         "Source/Components/FilterComponent.cpp"
  .         .         .         "Source/Components/FilterComponent.h"
  x         .         .         "Source/Util/Constants.cpp"
  x         .         .         "Source/Util/ParameterListener.cpp"
  .         .         .         "Source/Util/ParameterListener.h"
  x         .         .         "Source/Components/SettingsComponent.cpp"
  .         .         .         "Source/Components/SettingsComponent.h"
  x         .         .         "Source/Components/DelayComponent.cpp"
  .         .         .         "Source/Components/DelayComponent.h"
  x         .         .         "Source/Util/Linkable.cpp"
  .         .         .         "Source/Util/Linkable.h"
  x         .         .         "Source/Components/OSCLFOComponent.cpp"
  .         .         .         "Source/Components/OSCLFOComponent.h"
  x         .         .         "Source/Components/MixerSubComponent.cpp"
  .         .         .         "Source/Components/MixerSubComponent.h"
  x         .         .         "Source/Effects/DistEffect.cpp"
  .         .         .         "Source/Effects/DistEffect.h"
  x         .         .         "Source/Components/DistComponent.cpp"
  .         .         .         "Source/Components/DistComponent.h"
  .         .         .         "Source/Util/Swatch.h"
  x         .         .         "Source/Components/PresetRowModel.cpp"
  .         .         .         "Source/Components/PresetRowModel.h"
  x         .         .         "Source/Effects/ConvolutionReverb.cpp"
  .         .         .         "Source/Effects/ConvolutionReverb.h"
  x         .         .         "Source/Components/ReverbComponent.cpp"
  .         .         .         "Source/Components/ReverbComponent.h"
  x         .         .         "Source/Components/AboutPageComponent.cpp"
  .         .         .         "Source/Components/AboutPageComponent.h"
  x         .         .         "Source/Components/MarkdownComponent.cpp"
  .         .         .         "Source/Components/MarkdownComponent.h"
  x         .         .         "Source/Editor/ThemePicker.cpp"
  .         .         .         "Source/Editor/ThemePicker.h"
  x         .         .         "Source/Components/VolumeMeterComponent.cpp"
  .         .         .         "Source/Components/VolumeMeterComponent.h"
)

jucer_project_module(
  juce_audio_basics
  PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_audio_devices
  PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_audio_formats
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_audio_plugin_client
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_audio_processors
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_audio_utils
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_core
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_data_structures
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_events
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_graphics
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_gui_basics
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_gui_extra
	PATH "${JUCE_MODULES}"
)

jucer_project_module(
  juce_dsp
	PATH "${JUCE_MODULES}"
)


jucer_appconfig_header(
  USER_CODE_SECTION
""
)

jucer_export_target(
  "Xcode (MacOSX)"
  VST3_SDK_FOLDER "${VST_PATH}/"
  TARGET_PROJECT_FOLDER "Builds/MacOSX"  # only used by PREBUILD_SHELL_SCRIPT and POSTBUILD_SHELL_SCRIPT
  # POSTBUILD_SHELL_SCRIPT
)

jucer_export_target_configuration(
  "Xcode (MacOSX)"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "KandVST"
  OPTIMISATION "-O0 (no optimisation)"
  OSX_BASE_SDK_VERSION "Use Default"
  OSX_DEPLOYMENT_TARGET "Use Default"
  OSX_ARCHITECTURE "Universal Binary (32/64-bit)"
)

jucer_export_target_configuration(
  "Xcode (MacOSX)"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "KandVST"
  OPTIMISATION "-O3 (fastest with safe optimisations)"
  OSX_BASE_SDK_VERSION "Use Default"
  OSX_DEPLOYMENT_TARGET "Use Default"
  OSX_ARCHITECTURE "Universal Binary (32/64-bit)"
)

jucer_export_target(
  "Visual Studio 2015"
  VST3_SDK_FOLDER "${VST_PATH}/"
)

jucer_export_target_configuration(
  "Visual Studio 2015"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "KandVST"
  OPTIMISATION "No optimisation"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "x64"
)

jucer_export_target_configuration(
  "Visual Studio 2015"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "KandVST"
  OPTIMISATION "Maximise speed"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "x64"
)

jucer_export_target(
  "Visual Studio 2017"
  VST3_SDK_FOLDER "${VST_PATH}/"
)

jucer_export_target_configuration(
  "Visual Studio 2017"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "KandVST"
  OPTIMISATION "No optimisation"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "x64"
)

jucer_export_target_configuration(
  "Visual Studio 2017"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "KandVST"
  OPTIMISATION "Maximise speed"
  WARNING_LEVEL "High"
  GENERATE_MANIFEST ON
  # ARCHITECTURE "x64"
)

jucer_export_target(
  "Linux Makefile"
  VST_SDK_FOLDER "../VST"
  EXTRA_COMPILER_FLAGS "-fPIC"
  CXX_STANDARD_TO_USE "C++14"
)

jucer_export_target_configuration(
  "Linux Makefile"
  NAME "Debug"
  DEBUG_MODE ON
  BINARY_NAME "KandVST"
  EXTRA_LIBRARY_SEARCH_PATHS "/usr/X11R6/lib/"
  OPTIMISATION "-O0 (no optimisation)"
)

jucer_export_target_configuration(
  "Linux Makefile"
  NAME "Release"
  DEBUG_MODE OFF
  BINARY_NAME "KandVST"
  EXTRA_LIBRARY_SEARCH_PATHS "/usr/X11R6/lib/"
  OPTIMISATION "-O3 (fastest with safe optimisations)"
)

jucer_project_end()

#Enable console for debug builds
 target_compile_definitions("KandVST_Shared_Code"
        PRIVATE $<$<CONFIG:Debug>:CONSOLE> )
    
add_subdirectory("./Tester")
