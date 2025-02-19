cmake_minimum_required(VERSION 3.15...3.31)

project(NDB VERSION 0.0.1 LANGUAGES C)
set(NDB_CMAKE_BUILD_TESTING ON)

include(CTest)

add_custom_target(
	doc
	COMMAND cd .. && jupyter-book build .
	#	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
	COMMENT "Generating API documentation with Doxygen"
	VERBATIM
)

add_subdirectory(src)

if(BUILD_TESTING)
	add_subdirectory(tests)
endif()

# FreeRTOS

# NuttX
option(WITH_NUTTX "Build with NuttX" OFF)
if(WITH_NUTTX)
  include(ExternalProject)
  ExternalProject_Add(external-nuttx
    URL https://github.com/apache/nuttx/tarball/master/nuttx.tar.gz
    UPDATE_COMMAND ""
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/nuttx/nuttx
      -DCMAKE_TOOLCHAIN_FILE:PATH=${TOOLCHAIN_FILE}
      -DCMAKE_BUILD_TYPE:STRING=Debug
      -DBOARD_CONFIG=stm32f4discovery:nsh
      -DENABLE_TESTING:BOOL=OFF
      -DENABLE_PROGRAMS:BOOL=OFF
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    )
    set(NUTTX_PREFIX ${PROJECT_BINARY_DIR}/third_party/nuttx PARENT_SCOPE)
endif(WITH_NUTTX)

# NanoPB
option(WITH_NANOPB "Build with nanopb" OFF)
if(WITH_NANOPB)
  include(ExternalProject)
  ExternalProject_Add(external-nanopb
    URL https://github.com/nanopb/nanopb/archive/refs/tags/nanopb-0.4.9.zip
    UPDATE_COMMAND ""
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/nanopb
      -DCMAKE_TOOLCHAIN_FILE:PATH=${TOOLCHAIN_FILE}
      -DCMAKE_BUILD_TYPE:STRING=Debug
      -DENABLE_TESTING:BOOL=OFF
      -DENABLE_PROGRAMS:BOOL=OFF
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    )
    set(NANOPB_PREFIX ${PROJECT_BINARY_DIR}/third_party/nanopb PARENT_SCOPE)
endif(WITH_NANOPB)

# LWIP
option(WITH_LWIP "Build with lwip" OFF)
if(WITH_LWIP)
  include(ExternalProject)
  ExternalProject_Add(external-lwip
    URL https://download.savannah.nongnu.org/releases/lwip/lwip-2.2.0.zip
    UPDATE_COMMAND ""
    PATCH_COMMAND patch -p0 < ../../../../patches/patch.traps &&
                  patch -p0 < ../../../../patches/patch.main &&
		  cd contrib/examples/example_app && ln -s ../../../src/include/lwip/opt.h lwipcfg.h
		  # cd contrib/examples/example_app && ln -s test_configs/opt_default.h lwipcfg.h
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/lwip
      -DCMAKE_TOOLCHAIN_FILE:PATH=${TOOLCHAIN_FILE}
      -DCMAKE_BUILD_TYPE:STRING=Debug
      -DENABLE_TESTING:BOOL=OFF
      -DENABLE_PROGRAMS:BOOL=OFF
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    )
    set(LWIP_PREFIX ${PROJECT_BINARY_DIR}/third_party/lwip PARENT_SCOPE)
endif(WITH_LWIP)

# MBEDTLS
option(WITH_MBEDTLS "Build with mbedtls" OFF)
if(WITH_MBEDTLS)
  include(ExternalProject)
  ExternalProject_Add(external-mbedtls
    URL https://github.com/Mbed-TLS/mbedtls/releases/download/mbedtls-3.6.2/mbedtls-3.6.2.tar.bz2
    UPDATE_COMMAND ""
    PATCH_COMMAND ./scripts/config.py set MBEDTLS_THREADING_C &&
                  ./scripts/config.py set MBEDTLS_THREADING_PTHREAD
    CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}/third_party/mbedtls
      -DCMAKE_TOOLCHAIN_FILE:PATH=${TOOLCHAIN_FILE}
      -DCMAKE_BUILD_TYPE:STRING=Debug
      -DENABLE_TESTING:BOOL=OFF
      -DENABLE_PROGRAMS:BOOL=ON
    TEST_COMMAND ""
    )
    set(MBEDTLS_PREFIX ${PROJECT_BINARY_DIR}/third_party/mbedtls PARENT_SCOPE)
endif(WITH_MBEDTLS)
