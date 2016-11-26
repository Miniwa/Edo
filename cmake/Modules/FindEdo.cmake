# - Try to find the edo framework
# Once finished this script will define
#   Edo_FOUND - System has edo
#   Edo_INCLUDE_DIR - The include directory
#   Edo_BASE_LIBRARY - The base library
#   Edo_EXT_LIBRARY - The external library

# Will be set to false if problem occurs
set(Edo_FOUND TRUE)
set(Edo_BASE "edo-base")
set(Edo_EXT "edo-ext")

message("Scanning for edo dependencies..")
find_package(Boost 1.58.0 REQUIRED)

set(FIND_Edo_PATHS
    ${Edo_ROOT}
    $ENV{Edo_ROOT}
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

# Try to find the include directory
find_path(Edo_INCLUDE_DIR edo/base/types.hpp
            PATH_SUFFIXES include
            PATHS ${FIND_Edo_PATHS})

if(NOT Edo_INCLUDE_DIR)
    set(Edo_FOUND FALSE)
    set(FIND_Edo_ERROR "Edo include directory could not be located")
endif()


# Find base library
# Debug
find_library(Edo_BASE_LIBRARY_DEBUG
                NAMES ${Edo_BASE}-d
                PATH_SUFFIXES lib
                PATHS ${FIND_Edo_PATHS})

# Release
find_library(Edo_BASE_LIBRARY_RELEASE
                NAMES ${Edo_BASE}
                PATH_SUFFIXES lib
                PATHS ${FIND_Edo_PATHS})

# Handle base library flag
if(Edo_BASE_LIBRARY_DEBUG AND Edo_BASE_LIBRARY_RELEASE)
    set(Edo_BASE_LIBRARY ${Edo_BASE_LIBRARY_DEBUG} ${Edo_BASE_LIBRARY_RELEASE})
# Only debug
elseif(Edo_BASE_LIBRARY_DEBUG)
    set(Edo_BASE_LIBRARY ${Edo_BASE_LIBRARY_DEBUG})
# Only release
elseif(Edo_BASE_LIBRARY_RELEASE)
    set(Edo_BASE_LIBRARY ${Edo_BASE_LIBRARY_RELEASE})
# Neither
else()
    set(Edo_FOUND FALSE)
    set(FIND_Edo_ERROR "Edo base library could not be located")
endif()


# Find ext library
# Debug
find_library(Edo_EXT_LIBRARY_DEBUG
                NAMES ${Edo_EXT}-d
                PATH_SUFFIXES lib
                PATHS ${FIND_Edo_PATHS})

# Release
find_library(Edo_EXT_LIBRARY_RELEASE
                NAMES ${Edo_EXT}
                PATH_SUFFIXES lib
                PATHS ${FIND_Edo_PATHS})


# Handle ext library flag
if(Edo_EXT_LIBRARY_DEBUG AND Edo_EXT_LIBRARY_RELEASE)
    set(Edo_EXT_LIBRARY ${Edo_EXT_LIBRARY_DEBUG} ${Edo_EXT_LIBRARY_RELEASE})
# Only debug
elseif(Edo_EXT_LIBRARY_DEBUG)
    set(Edo_EXT_LIBRARY ${Edo_EXT_LIBRARY_DEBUG})
# Only release
elseif(Edo_EXT_LIBRARY_RELEASE)
    set(Edo_EXT_LIBRARY ${Edo_EXT_LIBRARY_RELEASE})
# Neither
else()
    set(Edo_FOUND FALSE)
    set(FIND_Edo_ERROR "Edo external library could not be located")
endif()


# Finally, handle exit status
if(Edo_FOUND)
    message("Edo successfully located!")
else()
    message(FATAL_ERROR ${FIND_Edo_ERROR})
endif()
