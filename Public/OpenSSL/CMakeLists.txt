cmake_minimum_required(VERSION 3.10)
project(OpenSSL C ASM)

find_program(MAKE_EXE NAMES make)

if (WIN32)
    if (NOT DEFINED MSYS2_HOME)
        message(STATUS "Please specify MSYS2_HOME !")
    else ()
        set(PERL_EXE ${MSYS2_HOME}/usr/bin/perl)
        set(MAKE_PATH "${MSYS2_HOME}/usr/bin;$ENV{PATH}")
    endif ()
else ()
    find_program(PERL_EXE NAMES perl)
endif ()

if(NOT EXISTS PERL_EXE)
    find_program(PERL_EXE NAMES perl)
endif ()

message(STATUS "[${PROJECT_NAME}] PERL_EXE: ${PERL_EXE}")
message(STATUS "[${PROJECT_NAME}] ENV{PATH}: $ENV{PATH}")


include(ExternalProject)
ExternalProject_Add(OpenSSL_Library
    URL https://www.openssl.org/source/openssl-3.3.0.tar.gz
    URL_HASH SHA256=53e66b043322a606abf0087e7699a0e033a37fa13feb9742df35c3a33b18fb02
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS FALSE
    CONFIGURE_COMMAND ${PERL_EXE} <SOURCE_DIR>/Configure no-shared no-asm no-docs no-tests no-legacy mingw64 --prefix=${CMAKE_CURRENT_BINARY_DIR}/OpenSSL
    BUILD_COMMAND ${MAKE_EXE} -e PERL=${PERL_EXE}
    INSTALL_COMMAND ""
    LOG_CONFIGURE TRUE
    LOG_BUILD TRUE
    BUILD_ALWAYS TRUE
)

ExternalProject_Get_Property(OpenSSL_Library BINARY_DIR)
message(STATUS "[${PROJECT_NAME}] BUILD_DIR:${BINARY_DIR}")
set(OPENSSL_INCLUDE_DIRECTORIES ${BINARY_DIR}/include)
set(OPENSSL_LINK_DIRECTORIES ${BINARY_DIR})

add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
set_property(TARGET OpenSSL::Crypto PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/libcrypto.a)
set_property(TARGET OpenSSL::Crypto PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BINARY_DIR}/include)

add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
set_property(TARGET OpenSSL::SSL PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/libssl.a)
set_property(TARGET OpenSSL::SSL PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${BINARY_DIR}/include)