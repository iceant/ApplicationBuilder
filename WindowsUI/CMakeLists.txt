cmake_minimum_required(VERSION 3.10)
project(WindowsUI C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

foreach (item IN LISTS SOURCES)
    message(STATUS "[${PROJECT_NAME}] SOURCE: ${item}")
endforeach ()

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC SDK)


