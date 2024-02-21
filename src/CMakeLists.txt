# Adds convenience methods, see cmake/cleanCppExtensions.cmake
include(cleanCppExtensions)

# Creates options to turn on sanitizers, see cmake/sanitizers.cmake
include(sanitizers)

# Enable helpfull warnings and C++20 for all files
if(MSVC)
    add_compile_options(/std:c++20 /W3 /WX)
else()
    add_compile_options(-std=c++20 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wunused -pedantic)
endif()

# configure version.cpp.in with selected version
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.cc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.cc"
)

# If you want to have date and time of the build, your targets has to depend on this.
# This will force recompilation of version.o and thus forcing gcc to update __DATE__ macro.
add_custom_target(versionFileTouchForRebuild
    COMMAND ${CMAKE_COMMAND} -E touch "${CMAKE_CURRENT_BINARY_DIR}/version.cc")

# ------------------------------------------------------------------------------
# Libraries dependencies
find_package(Threads REQUIRED) # threading

# ------------------------------------------------------------------------------
# By using macro to add common dependencies you can avoid repetition when you have
# multiple binaries.
# Use with `addCommonDependencies(planetplus)`
macro(addCommonDependencies TARGET)
    add_dependencies(${TARGET} versionFileTouchForRebuild) # We want precise time of build in version
    target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${TARGET} PRIVATE Threads::Threads)
    target_compile_definitions(${TARGET} PRIVATE -DSOME_DEFINE)
endmacro()

# -------------------------------------------------------------------------------
# planetplus executable setup
set(ICON_RESOURCE "")

# if(WIN32) # windows icon resource file, provided from /packaging directory
#     set(ICON_RESOURCE "${PROJECT_BINARY_DIR}/planetplus.icon.rc")
# elseif(APPLE) # osx icon ressource file, provided from /packaging directory
#     set(ICON_RESOURCE "${PROJECT_SOURCE_DIR}/packaging/planetplus.icns")
# endif()

add_executable(planetplus MACOSX_BUNDLE
    ${ICON_RESOURCE}
    ${CMAKE_CURRENT_BINARY_DIR}/version.cc

    cli/commands/commands.h
    cli/commands/chelp.cc
    cli/commands/cversion.cc
    cli/commands/csetup.cc
    cli/commands/cconfig.cc

    cli/tools.h
    cli/tools.cc

    utils/utils.h
    utils/utils.cc
    utils/config.h
    utils/config.cc

    main.cc)

# Copy the config.ini file to the build directory
# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/config.ini ${CMAKE_CURRENT_BINARY_DIR}/data/config.ini COPYONLY)

# Install the config.ini file to the data directory
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/data/config.ini DESTINATION data)

add_dependencies(planetplus versionFileTouchForRebuild) # We want precise time of build in version
target_include_directories(planetplus PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(planetplus PRIVATE Threads::Threads)

if(APPLE)
    set_target_properties(planetplus PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "planetplus")
    set_target_properties(planetplus PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.planetplus.planetplus")
    set_target_properties(planetplus PROPERTIES MACOSX_BUNDLE_ICON_FILE planetplus.icns)
    set_target_properties(planetplus PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/packaging/MacOSXBundleInfo.plist.in)
    set_source_files_properties(${PROJECT_SOURCE_DIR}/packaging/planetplus.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

install(TARGETS planetplus
    BUNDLE DESTINATION "."
    RUNTIME DESTINATION bin)

# Using macro from cmake/cleanCpppExtensions.cpp included on the top
# Provides planetplus-run and planetplus-dbg
addRunAndDebugTargets(planetplus)

# short convenience target from cmake/cleanCpppExtensions.cpp
add_custom_target(run DEPENDS planetplus-run)

# Manually specified convenience target for running the binary, we use macro
# add_custom_target(planetplus-run
# WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin
# USES_TERMINAL
# COMMAND ./planetplus
# DEPENDS Install_)

# And this would add -DBAR_ENABLE_FEATURE or equivalent to build command
# target_compile_definitions(planetplus PRIVATE BAR_ENABLE_FEATURE)

# Add compile time definition based on option
# option(OPTION_NAME "description for end user" OFF)
# if(OPTION_NAME)
# # you can do anything here, eg. setup new optional executable
# target_compile_definitions(planetplus PRIVATE DEFINE_NAME)
# endif()

# Add building new shared library
# add_library(exampleLibrary SHARED ${LIBRARY_SOURCES})
# target_link_libraries (exampleLibrary PRIVATE ${CMAKE_THREAD_LIBS_INIT})
# set_property(TARGET exampleLibrary PROPERTY CXX_STANDARD 14) # we want C++14

# How to include and link OpenGL and GLUT libraries
# find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
# target_include_directories(planetplus PRIVATE SYSTEM ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )
# target_link_libraries(planetplus PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

# How to use some other standard packages
# find_package(ZLIB REQUIRED)
# target_link_libraries (planetplus PRIVATE ZLIB::ZLIB)
#
# find_package(OpenSSL REQUIRED)
# target_link_libraries (planetplus PRIVATE OpenSSL::SSL OpenSSL::Crypto)
#
# find_package(CURL REQUIRED)
# target_link_libraries (planetplus PRIVATE CURL::libcurl)

# ------------------------------------------------------------------------------
# Unit tests
add_subdirectory(unittest)

# -------------------------------------------------------------------------------
# Copy MINGW needed libraries for building on windows
if(MINGW)
    message(STATUS "MinGW detected")
    get_filename_component(GCC_PATH ${CMAKE_C_COMPILER} PATH)

    if(${GCC_PATH} MATCHES "mingw64/bin")
        set(libgcc "libgcc_s_seh-1.dll") # 64bit
        message(STATUS "  64bit dlls is building")
    else()
        set(libgcc "libgcc_s_dw2-1.dll") # 32bit
        message(STATUS "  32bit dlls is building")
    endif()

    install(FILES ${GCC_PATH}/${libgcc}
        ${GCC_PATH}/libwinpthread-1.dll
        ${GCC_PATH}/libstdc++-6.dll
        DESTINATION ./bin/
    )
endif(MINGW)

# ------------------------------------------------------------------------------
# Other MISC targets

# Adds misc targets: format, cppcheck, tidy, see cmake/cleanCppExtensions.cmake
addMiscTargets()
