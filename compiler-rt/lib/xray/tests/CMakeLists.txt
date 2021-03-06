include_directories(..)

add_custom_target(XRayUnitTests)
set_target_properties(XRayUnitTests PROPERTIES FOLDER "XRay unittests")

# Create an XRAY_IMPL_FILES variable which will include all the implementation
# files that are in the lib directory. Unfortunately, when new files are added
# to the implementation, CMake must be run so that this variable is
# re-generated.
file(GLOB XRAY_IMPL_FILES "../*.cc" "../*.h")

set(XRAY_UNITTEST_CFLAGS
  ${XRAY_CFLAGS}
  ${COMPILER_RT_UNITTEST_CFLAGS}
  ${COMPILER_RT_GTEST_CFLAGS}
  -I${COMPILER_RT_SOURCE_DIR}/include
  -I${COMPILER_RT_SOURCE_DIR}/lib/xray
  -I${COMPILER_RT_SOURCE_DIR}/lib)

set(XRAY_TEST_ARCH ${XRAY_SUPPORTED_ARCH})
macro(add_xray_unittest testname)
  cmake_parse_arguments(TEST "" "" "SOURCES;HEADERS" ${ARGN})
  if(UNIX AND NOT APPLE)
    set(CMAKE_DL_LIBS_INIT "")
    foreach(lib ${CMAKE_DL_LIBS})
      list(APPEND CMAKE_DL_LIBS_INIT -l${lib})
    endforeach()
    foreach(arch ${XRAY_TEST_ARCH})
      set(TEST_OBJECTS)
      generate_compiler_rt_tests(TEST_OBJECTS
        XRayUnitTests "${testname}-${arch}-Test" "${arch}"
        SOURCES ${TEST_SOURCES} ${COMPILER_RT_GTEST_SOURCE}
        # Note that any change in the implementations will cause all the unit
        # tests to be re-built. This is by design, but may be cumbersome during
        # the build/test cycle.
        COMPILE_DEPS ${TEST_SOURCES} ${COMPILER_RT_GTEST_SOURCE}
        ${XRAY_HEADERS} ${XRAY_IMPL_FILES}
        DEPS gtest xray llvm-xray
        CFLAGS ${XRAY_UNITTEST_CFLAGS}
        LINK_FLAGS -fxray-instrument
          ${TARGET_LINK_FLAGS}
          -lstdc++ -lm ${CMAKE_THREAD_LIBS_INIT}
          ${CMAKE_DL_LIBS_INIT} -lrt)
      set_target_properties(XRayUnitTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endforeach()
  endif()
endmacro()

if(COMPILER_RT_CAN_EXECUTE_TESTS)
  add_subdirectory(unit)
endif()
