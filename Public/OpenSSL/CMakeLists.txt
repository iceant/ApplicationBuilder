cmake_minimum_required(VERSION 3.10)
project(OpenSSL C ASM)

find_program(MAKE_EXE NAMES mingw32-make make)


set(PERL_EXE D:/Install/msys2/usr/bin/perl)

if(NOT EXISTS PERL_EXE)
    find_program(PERL_EXE NAMES perl)
endif ()

include(ExternalProject)
ExternalProject_Add(OpenSSL_Library PREFIX openssl
    URL https://www.openssl.org/source/openssl-3.3.0.tar.gz
    URL_HASH SHA256=53e66b043322a606abf0087e7699a0e033a37fa13feb9742df35c3a33b18fb02
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    DOWNLOAD_NO_PROGRESS FALSE
    CONFIGURE_COMMAND ${PERL_EXE} <SOURCE_DIR>/Configure no-shared no-asm no-docs no-tests no-legacy mingw
    BUILD_COMMAND "${MAKE_EXE}"
    LOG_CONFIGURE TRUE
    LOG_BUILD TRUE
    BUILD_ALWAYS TRUE
)

ExternalProject_Get_Property(OpenSSL_Library INSTALL_DIR)
set(OPENSSL_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)
set(OPENSSL_LINK_DIRECTORIES ${INSTALL_DIR}/lib)
file(MAKE_DIRECTORY ${INSTALL_DIR}/include)

add_library(openssl-crypto-static STATIC IMPORTED GLOBAL)
set_property(TARGET openssl-crypto-static PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/lib/libcrypto.a)
set_property(TARGET openssl-crypto-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)

add_library(openssl-ssl-static STATIC IMPORTED GLOBAL)
set_property(TARGET openssl-ssl-static PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/lib/libssl.a)
set_property(TARGET openssl-ssl-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)