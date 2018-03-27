cmake_minimum_required(VERSION 3.9)
set(PROJECT_NAME ImageAlgorithm)
project(${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(OpenMP REQUIRED)
find_package(Boost REQUIRED)
find_package(TBB REQUIRED tbb)

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE 
	src
	extern/eigen-git-mirror/
	extern/stb/
	${Boost_INCLUDE_DIRS})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_CXX_FLAGS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_EXE_LINKER_FLAGS} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${TBB_IMPORTED_TARGETS})