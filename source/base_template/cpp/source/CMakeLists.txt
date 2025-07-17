CMAKE_MINIMUM_REQUIRED(VERSION 3.6 FATAL_ERROR)

foreach(policy CMP0068)
  if(POLICY ${policy})
    cmake_policy(SET ${policy} NEW)
  endif()
endforeach()

PROJECT(AddInNative CXX)

IF(WIN32)
    ENABLE_LANGUAGE(RC)
ENDIF()
#----------------------------------------------------------------------------------------------------------------#
#--source files-------------------
SET(AddInNative_SRC 
    AddInNative.cpp
    dllmain.cpp
    stdafx.cpp
    stdafx.h
    ../lib/common.cpp
    impl/test.cpp
    #ВставкаCPPФайлов
    )
if (WIN32)
    LIST(APPEND AddInNative_SRC AddInNative.rc AddInNative.def)
endif()
#--source files end---------------
set(ADN_NAME ${PROJECT_NAME})
set(MECONPANY "MyCompany")
#----------------------------------------------------------------------------------------------------------------#
include_directories(${CMAKE_SOURCE_DIR}/../include)

SET (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_SOURCE_DIR})

if (UNIX)
else()
    add_definitions(-DUNICODE -DWIN32 -D_USRDLL -D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -DADDINCPP_EXPORTS)
endif()

add_library(${PROJECT_NAME} SHARED ${AddInNative_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
    target_precompile_headers(${PROJECT_NAME} PUBLIC stdafx.h)
endif()

set_target_properties( ${PROJECT_NAME} PROPERTIES
       POSITION_INDEPENDENT_CODE ON 
       CXX_STANDARD 11
       CXX_STANDARD_REQUIRED ON
   )
IF (UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,--no-undefined -Xlinker --version-script -Xlinker ${CMAKE_CURRENT_SOURCE_DIR}/version.script" )
ENDIF()

if (WIN32)
    target_link_libraries(AddInNative PRIVATE ${CMAKE_SOURCE_DIR}/../../rust/target/debug/rust_1c.lib
        ntdll 
        Ws2_32
        Crypt32
        Secur32
        Bcrypt
        Userenv
    )
else()
    target_link_libraries(AddInNative PRIVATE ${CMAKE_SOURCE_DIR}/../../rust/target/debug/librust_1c.a -static-libstdc++)
endif()

# Integrate Rust build into the CMake build system
add_custom_target(RustBuild ALL
    COMMAND cargo build
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/../../rust/
    COMMENT "Building Rust project with Cargo"
)

# Ensure that the Rust library is built before AddInNative
add_dependencies(${PROJECT_NAME} RustBuild)