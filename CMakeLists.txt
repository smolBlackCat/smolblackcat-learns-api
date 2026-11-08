cmake_minimum_required(VERSION 3.20)
project(gov-id-validator-api VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_TEST "Build API test scripts" ON)

include(FetchContent)

# Crow 
FetchContent_Declare(
	crow
	GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
	GIT_TAG v1.3.5
)
FetchContent_MakeAvailable(crow)

if(ENABLE_TEST)
	include(CTest)
	FetchContent_Declare(
		catch2
		GIT_REPOSITORY https://github.com/catchorg/Catch2.git
		GIT_TAG v3.16.1)
	FetchContent_MakeAvailable(catch2)

	add_executable(id-check-test test/id-check-test.cpp src/services.cpp)
	target_link_libraries(id-check-test PRIVATE Catch2::Catch2WithMain)
	target_include_directories(id-check-test PRIVATE include/)
	add_test(NAME id-check-test COMMAND ${PROJECT_BINARY_DIR}/id-check-test)
endif()

# Main API
add_executable(gov-api src/main.cpp src/services.cpp)
target_include_directories(gov-api PRIVATE include/)
target_link_libraries(gov-api PRIVATE Crow::Crow)