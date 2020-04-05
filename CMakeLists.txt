cmake_minimum_required(VERSION 3.10.0)
project (apebrain)
add_executable(apebrain main.cpp)

include_directories(.)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CURSES ncurses)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(apebrain ${CURSES_LIBRARIES})

pkg_check_modules(JSONCPP jsoncpp)
include_directories(${JSONCPP_INCLUDE_DIR})
target_link_libraries(apebrain ${JSONCPP_LIBRARIES})
