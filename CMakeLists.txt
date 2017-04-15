cmake_minimum_required(VERSION 3.3)
project(CApp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Data.cpp Data.h Datas.h ConcurQueue.h TFRingBuffer.h SigRingBuffer.h ArrLstStk.h Misc.h)
add_executable(CApp ${SOURCE_FILES})