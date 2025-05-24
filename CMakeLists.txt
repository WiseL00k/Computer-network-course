cmake_minimum_required(VERSION 3.10)
project(MySniffer VERSION 0.1.0 LANGUAGES C)

add_definitions(-DWIN32 -finput-charset=UTF-8 -fexec-charset=GBK)

include_directories(inc)

aux_source_directory(src DIR_SRCS)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
