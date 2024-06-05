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
    INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/OpenSSL
    CONFIGURE_COMMAND ${PERL_EXE} <SOURCE_DIR>/Configure no-shared no-asm no-docs no-tests no-legacy mingw64 --prefix=<INSTALL_DIR>
    BUILD_COMMAND ${MAKE_EXE} -e PERL=${PERL_EXE}
    LOG_CONFIGURE TRUE
    LOG_BUILD TRUE
    BUILD_ALWAYS FALSE
)

ExternalProject_Get_Property(OpenSSL_Library INSTALL_DIR)
message(STATUS "[${PROJECT_NAME}] INSTALL_DIR: ${INSTALL_DIR}")
set(OPENSSL_USE_STATIC_LIBS ON)

#if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#    include(${INSTALL_DIR}/lib64/cmake/OpenSSL/OpenSSLConfigVersion.cmake)
#    include(${INSTALL_DIR}/lib64/cmake/OpenSSL/OpenSSLConfig.cmake)
#else()
#    include(${INSTALL_DIR}/lib/cmake/OpenSSL/OpenSSLConfigVersion.cmake)
#    include(${INSTALL_DIR}/lib/cmake/OpenSSL/OpenSSLConfig.cmake)
#endif ()
#message(STATUS "[${PROJECT_NAME}] OPENSSL_SSL_LIBRARY: ${OPENSSL_SSL_LIBRARY}")
#message(STATUS "[${PROJECT_NAME}] OPENSSL_CRYPTO_LIBRARY: ${OPENSSL_CRYPTO_LIBRARY}")

set(OPENSSL_ROOT_DIR ${INSTALL_DIR})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(LIB_PATH ${INSTALL_DIR}/lib64)
else()
    set(LIB_PATH ${INSTALL_DIR}/lib)
endif ()

set(OPENSSL_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)
set(OPENSSL_LINK_DIRECTORIES ${LIB_PATH})
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
set_property(TARGET OpenSSL::Crypto PROPERTY IMPORTED_LOCATION ${LIB_PATH}/libcrypto.a)
set_property(TARGET OpenSSL::Crypto PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)

add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
set_property(TARGET OpenSSL::SSL PROPERTY IMPORTED_LOCATION ${LIB_PATH}/libssl.a)
set_property(TARGET OpenSSL::SSL PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)

if (WIN32)
    set(OPENSSL_LIBCRYPTO_DEPENDENCIES -lws2_32 -lgdi32 -lcrypt32)
    set_property(TARGET OpenSSL::Crypto
            PROPERTY INTERFACE_LINK_LIBRARIES ${OPENSSL_LIBCRYPTO_DEPENDENCIES})
endif ()

