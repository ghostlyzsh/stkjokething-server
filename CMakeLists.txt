SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR "x86-64")

if (NOT DEFINED CMAKE_C_COMPILER)
	find_program(CMAKE_C_COMPILER NAMES
		"clang"
		"clang-4.0"
		"clang-6.0"
		"clang-7"
		"clang-9"
		"clang-10"
		"clang-11"
		"clang-12"
		)
endif(NOT DEFINED CMAKE_C_COMPILER)

if (NOT DEFINED CMAKE_CXX_COMPILER)
	find_program(CMAKE_CXX_COMPILER NAMES
		"clang++"
		"clang++-4.0"
		"clang++-6.0"
		"clang++-7"
		"clang++-9"
		"clang++-10"
		"clang++-11"
		"clang++-12"
		)
endif(NOT DEFINED CMAKE_CXX_COMPILER)

cmake_minimum_required(VERSION 3.10)

project(starkingdoms 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(starkingdoms main.cpp)
