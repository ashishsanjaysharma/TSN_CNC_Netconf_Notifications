cmake_minimum_required(VERSION 2.8.12)
project(endstationapp)
set(PROJECT_DESC "End station cnc notification using sysrepo and netopeer2")
include(GNUInstallDirs)

# setup version
set(ENDSTAAPP_MAJOR_VERSION 1)
set(ENDSTAAPP_MINOR_VERSION 1)
set(ENDSTAAPP_PATCH_VERSION 1)
set(ENDSTAAPP_VERSION ${ENDSTAAPP_MAJOR_VERSION}.${ENDSTAAPP_MINOR_VERSION}.${ENDSTAAPP_PATCH_VERSION})
set(ENDSTAAPP_SOVERSION ${ENDSTAAPP_MAJOR_VERSION}.${ENDSTAAPP_MINOR_VERSION})

# osx specific
set(CMAKE_MACOSX_RPATH TRUE)

# set default build type if not specified by user
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE debug)
endif()
string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWER)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_C_FLAGS         "${CMAKE_C_FLAGS} -Wall -Wpedantic -std=gnu11 -Wno-language-extension-token")
if(CMAKE_COMPILER_IS_GNUCC)
    # disable strict aliasing in GCC, since it produces false alarams in libev
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-strict-aliasing")
endif()
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -O2")
set(CMAKE_C_FLAGS_DEBUG   "-g -O0")

if(NOT UNIX)
    message(FATAL_ERROR "Only Unix-like systems are supported.")
endif()

if(NOT DEFINED IS_DEVELOPER_CONFIGURATION)
    if(CMAKE_BUILD_TYPE_LOWER MATCHES "debug" AND NOT CMAKE_BUILD_TYPE_LOWER MATCHES "^rel")
        set(IS_DEVELOPER_CONFIGURATION true)
    else()
        set(IS_DEVELOPER_CONFIGURATION false)
    endif()
endif()

# location of system repository
if(${IS_DEVELOPER_CONFIGURATION})
    set(REPOSITORY_LOC "${CMAKE_BINARY_DIR}/repository" CACHE PATH "System repository location, contains configuration schema and data files.")
else()
    set(REPOSITORY_LOC "/etc/sysrepo" CACHE PATH "System repository location, contains configuration schema and data files.")
endif()

# include custom Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# find required libraries
find_package(SYSREPO REQUIRED)
include_directories(${SYSREPO_INCLUDE_DIR})

find_package(YANG REQUIRED)
include_directories(${YANG_INCLUDE_DIR})

# add subdirectories
add_subdirectory(src)

install(CODE "
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/scripts/install_yang.sh)")



