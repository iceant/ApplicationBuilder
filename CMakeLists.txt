cmake_minimum_required(VERSION 3.10)
project(ApplicationBuilder C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(Public/OpenSSL)
add_subdirectory(GameAI)
add_subdirectory(WindowsUI)
add_subdirectory(Applications/SocketMonitor)
