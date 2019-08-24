cmake_minimum_required(VERSION 3.13)
project(zad2)

set(CMAKE_CXX_STANDARD 11)

add_executable(test_1 main.cpp Process.cpp Process.h Task.h TasksContainer.h Communication.h Parsers.h TimesCostsMatrix.h Node.h)