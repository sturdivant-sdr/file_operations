/**
|====================================== test_yaml_parser.cpp ======================================|
|                                                                                                  |
|   Property of Daniel Sturdivant. Unauthorized copying of this file via any medium is would be    |
|   super sad and unfortunate for me. Proprietary and confidential.                                |
|                                                                                                  |
|--------------------------------------------------------------------------------------------------|
|                                                                                                  |
|   @file     examples/test_yaml_parser.cpp                                                        |
|   @brief    An example useage of the yaml parser system.                                         |
|   @author   Daniel Sturdivant <sturdivant20@gmail.com>                                           |
|   @date     April 2024                                                                           |
|                                                                                                  |
|==================================================================================================|
*/

#include <iostream>

#include "file_operations/yaml_parser.hpp"

int main() {
    // initialize logger
    std::shared_ptr<spdlog::logger> logger = spdlog::default_logger();
    logger->set_level(spdlog::level::trace);
    // logger->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%l] %v %$");

    // intialize sdr struct and generate settings
    file_operations::YamlParser yp("config/default.yaml");
    logger->info("yaml file parsed");
    int mode = yp.GetVariable<int>("mode");
    std::string scenario = yp.GetVariable<std::string>("scenario");
    std::string in_file = yp.GetVariable<std::string>("file_name");

    // print out some variables
    logger->info("mode: {}", mode);
    logger->info("scenario: {}", scenario);
    logger->info("input file: {}", in_file);

    return 0;
}