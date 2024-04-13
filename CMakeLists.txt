cmake_minimum_required(VERSION 3.13)
project(SimpleServer)

add_executable(${PROJECT_NAME}
    main.cpp
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

find_package(Boost COMPONENTS program_options system REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options Boost::system)
