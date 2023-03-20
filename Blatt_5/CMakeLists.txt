cmake_minimum_required(VERSION 3.19)
project(Blatt_5)

set(CMAKE_CXX_STANDARD 14)



add_executable(Blatt_5 main.cpp MiniShell.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(Blatt_5 -pthread -lm -lssl -lcrypto)







