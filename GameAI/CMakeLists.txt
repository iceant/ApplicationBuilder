cmake_minimum_required(VERSION 3.10)
project(GameAI C)


########################################################################################################################
#### SETTINGS
set(CMAKE_C_STANDARD 11)

########################################################################################################################
#### SUB PROJECTS
add_subdirectory(../SDK ${CMAKE_CURRENT_BINARY_DIR}/SDK)

########################################################################################################################
#### PROJECT SOURCES

file(GLOB SOURCES *.c)
list(FILTER SOURCES EXCLUDE REGEX \\.t\\.c)
foreach (item IN LISTS SOURCES)
    message(STATUS "[${PROJECT_NAME}] SOURCE: ${item}")
endforeach ()

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_link_libraries(${PROJECT_NAME} PUBLIC TLSF)
target_link_libraries(${PROJECT_NAME} PUBLIC SDK)

########################################################################################################################
#### TEST
file(GLOB TEST_SOURCES *.t.c)

foreach (item IN LISTS TEST_SOURCES)
    get_filename_component(TestUnit ${item} NAME_WLE)
    message(STATUS "[${PROJECT_NAME}] TEST: ${TestUnit} - ${item}")
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PUBLIC ${PROJECT_NAME})
endforeach ()

