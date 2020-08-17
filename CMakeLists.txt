cmake_minimum_required(VERSION 3.17)
project(test_shell)

set(CMAKE_CXX_STANDARD 14)

add_executable(test_shell main.cpp PCB.cpp PCB.h RCB.cpp RCB.h util/linklist.cpp util/linklist.h util/queue.cpp util/queue.h shell.cpp shell.h)