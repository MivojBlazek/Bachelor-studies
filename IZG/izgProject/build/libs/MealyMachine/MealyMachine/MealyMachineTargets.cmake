# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.6)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6...3.20)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget MealyMachine::MealyMachine)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  unset(_targetsDefined)
  unset(_targetsNotDefined)
  unset(_expectedTargets)
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Create imported target MealyMachine::MealyMachine
add_library(MealyMachine::MealyMachine STATIC IMPORTED)

set_target_properties(MealyMachine::MealyMachine PROPERTIES
  COMPATIBLE_INTERFACE_STRING "MealyMachine_MAJOR_VERSION"
  INTERFACE_INCLUDE_DIRECTORIES "/home/michal/Coding/IZG/izgProject/libs/MealyMachine/src;/home/michal/Coding/IZG/izgProject/build/libs/MealyMachine;/home/michal/Coding/IZG/izgProject/libs/MealyMachine"
  INTERFACE_MealyMachine_MAJOR_VERSION "1"
)

# Import target "MealyMachine::MealyMachine" for configuration ""
set_property(TARGET MealyMachine::MealyMachine APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(MealyMachine::MealyMachine PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "/home/michal/Coding/IZG/izgProject/build/libs/MealyMachine/libMealyMachine.a"
  )

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
