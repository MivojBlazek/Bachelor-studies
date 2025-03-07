# Install script for directory: /home/michal/Coding/IZG/izgProject/libs/ArgumentViewer

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/michal/Coding/IZG/izgProject/build/libs/ArgumentViewer/libArgumentViewer.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ArgumentViewer" TYPE FILE FILES "/home/michal/Coding/IZG/izgProject/build/libs/ArgumentViewer/ArgumentViewer/argumentviewer_export.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ArgumentViewer" TYPE FILE FILES
    "/home/michal/Coding/IZG/izgProject/libs/ArgumentViewer/src/ArgumentViewer/ArgumentViewer.h"
    "/home/michal/Coding/IZG/izgProject/libs/ArgumentViewer/src/ArgumentViewer/Fwd.h"
    "/home/michal/Coding/IZG/izgProject/libs/ArgumentViewer/src/ArgumentViewer/Exception.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer" TYPE FILE FILES
    "/home/michal/Coding/IZG/izgProject/build/libs/ArgumentViewer/ArgumentViewer/ArgumentViewerConfig.cmake"
    "/home/michal/Coding/IZG/izgProject/build/libs/ArgumentViewer/ArgumentViewer/ArgumentViewerConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake"
         "/home/michal/Coding/IZG/izgProject/build/libs/ArgumentViewer/CMakeFiles/Export/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer/ArgumentViewerTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer" TYPE FILE FILES "/home/michal/Coding/IZG/izgProject/build/libs/ArgumentViewer/CMakeFiles/Export/lib/cmake/ArgumentViewer/ArgumentViewerTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/ArgumentViewer" TYPE FILE FILES "/home/michal/Coding/IZG/izgProject/build/libs/ArgumentViewer/CMakeFiles/Export/lib/cmake/ArgumentViewer/ArgumentViewerTargets-noconfig.cmake")
  endif()
endif()

