cmake_minimum_required(VERSION 3.6)
project(file_operations)

# --- Version Control ---
set(FILE_OPERATIONS_MAJOR_VERSION 0)
set(FILE_OPERATIONS_MINOR_VERSION 0)
set(FILE_OPERATIONS_PATCH_VERSION 1)
set(PROJECT_VERSION ${FILE_OPERATIONS_MAJOR_VERSION}.${FILE_OPERATIONS_MINOR_VERSION}.${FILE_OPERATIONS_PATCH_VERSION})
message(STATUS "${PROJECT_NAME} version ${PROJECT_VERSION}")

# --- Compiler Flags ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Ofast")

# --- Add Dependencies ---
find_package(spdlog REQUIRED)
find_package(yaml-cpp REQUIRED)

set(FILE_OPERATIONS_SRCS
    include/file_operations/binary_file.hpp
    include/file_operations/yaml_parser.hpp
)

# --- Create the C++ Library ---
add_library(${PROJECT_NAME} INTERFACE ${FILE_OPERATIONS_SRCS})
target_include_directories(
    ${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(
    ${PROJECT_NAME} INTERFACE
    spdlog::spdlog
    yaml-cpp::yaml-cpp
)

# --- Add Executables ---
if(NOT DEFINED BUILD_TESTS)
    set(BUILD_TESTS true)
endif()

if(BUILD_TESTS)
    # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)
    add_executable(test_signal_file tests/test_signal_file.cpp)
    add_executable(test_yaml_parser tests/test_yaml_parser.cpp)
    target_link_libraries(test_signal_file PUBLIC ${PROJECT_NAME})
    target_link_libraries(test_yaml_parser PUBLIC ${PROJECT_NAME})
    list(APPEND FILE_OPERATIONS_TESTS
        test_signal_file
        test_yaml_parser
    )
    install(
        TARGETS ${FILE_OPERATIONS_TESTS}
        DESTINATION bin
    )
endif()

# --- Compile and Make Visible to other packages ---
# include(CMakePackageConfigHelpers)

# install(
# DIRECTORY include/
# DESTINATION include
# )

# set(PKG_NAME ${PROJECT_NAME})
# configure_package_config_file(
# "cmake/${PROJECT_NAME}Config.cmake.in"
# "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
# INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
# PATH_VARS PKG_NAME
# NO_SET_AND_CHECK_MACRO
# NO_CHECK_REQUIRED_COMPONENTS_MACRO
# )

# install(
# FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
# "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
# DESTINATION lib/cmake/${PROJECT_NAME}
# )