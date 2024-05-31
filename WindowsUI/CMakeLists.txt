cmake_minimum_required(VERSION 3.10)
project(WindowsUI C)

########################################################################################################################
#### SETTINGS
set(CMAKE_C_STANDARD 11)

########################################################################################################################
#### PROJECT SOURCES

file(GLOB SOURCES *.c)
list(FILTER SOURCES EXCLUDE REGEX \\.t\\.c)
foreach (item IN LISTS SOURCES)
    message(STATUS "[${PROJECT_NAME}] SOURCE: ${item}")
endforeach ()

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC SDK)

########################################################################################################################
#### TEST
file(GLOB TEST_SOURCES *.t.c)

foreach (item IN LISTS TEST_SOURCES)
    # 设置为GUI应用程序
    set(CMAKE_WIN32_EXECUTABLE ON)

    get_filename_component(TestUnit ${item} NAME_WLE)
    message(STATUS "[${PROJECT_NAME}] TEST: ${TestUnit} - ${item}")
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PUBLIC ${PROJECT_NAME})

    # 不链接CMT.LIB库，这是Windows控制台应用程序的一个链接库
    if(CMAKE_WIN32_EXECUTABLE)
        set_target_properties(${TestUnit} PROPERTIES WIN32_EXECUTABLE TRUE)
    endif()
endforeach ()

