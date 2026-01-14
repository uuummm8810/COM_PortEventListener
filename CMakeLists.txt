cmake_minimum_required(VERSION 3.10.0)
project(COM_PortEventListener VERSION 0.1.0 LANGUAGES C)

file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_executable(EventListener ${SOURCES})
target_link_libraries(EventListener PRIVATE shlwapi)
target_link_libraries(EventListener PRIVATE wininet)
target_link_libraries(EventListener PRIVATE bluetoothapis)
set_target_properties(EventListener PROPERTIES LINK_FLAGS "-static")

set(CMAKE_C_STANDARD 11)