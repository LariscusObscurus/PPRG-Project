cmake_minimum_required(VERSION 3.9)
set(PROJECT_NAME ImageAlgorithm)
project(${PROJECT_NAME})

find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE src extern/eigen-git-mirror/ extern/boost/libs/gil/include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_CXX_FLAGS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenMP_EXE_LINKER_FLAGS})
