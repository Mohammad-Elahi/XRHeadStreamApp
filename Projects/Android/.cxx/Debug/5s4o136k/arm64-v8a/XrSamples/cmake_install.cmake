# Install script for directory: A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/MetaOpenXRSDK")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
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
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrBodyFaceEyeSocial/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrColorSpaceFB/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrCompositor_NativeActivity/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrControllers/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrHandDataSource/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrHandTrackingWideMotionMode/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrHandsAndControllers/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrHandsFB/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrInput/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrKeyboard/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrPassthrough/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrPassthroughOcclusion/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrSceneModel/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrSpaceWarp/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrSpatialAnchor/cmake_install.cmake")
  include("A:/Mohammad/Android/XRHeadtracking/XRproject/Samples/XrSamples/XrBodyFaceEyeSocial/Projects/Android/.cxx/Debug/5s4o136k/arm64-v8a/XrSamples/XrVirtualKeyboard/cmake_install.cmake")

endif()

