/**
|======================================== yaml_parser.hpp =========================================|
|                                                                                                  |
|   Property of Daniel Sturdivant. Unauthorized copying of this file via any medium is would be    |
|   super sad and unfortunate for me. Proprietary and confidential.                                |
|                                                                                                  |
|--------------------------------------------------------------------------------------------------|
|                                                                                                  |
|   @file     utils/file_operations/yaml_parser.hpp                                                |
|   @brief    File containing the yaml_parser definitions.                                         |
|   @author   Daniel Sturdivant <sturdivant20@gmail.com>                                           |
|   @date     May 2024                                                                             |
|                                                                                                  |
|==================================================================================================|
*/

#ifndef FILE_OPS_YAML_PARSER_HPP
#define FILE_OPS_YAML_PARSER_HPP

#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <string>

namespace file_operations {

class YamlParser {
  public:
    //* === YamlParser ===
    /// @brief constructor
    /// @param yamlFileName string containing yaml file name
    /// @param searchpath   string containing yaml file path (folder)
    YamlParser(const std::string yamlFileName, const std::string searchPath)
        : __log(spdlog::default_logger()), search_path(searchPath), filepath(yamlFileName) {
        yaml_node = LoadYaml(filepath, search_path);
    };
    YamlParser(const std::string yamlFileName)
        : __log(spdlog::default_logger()), search_path(""), filepath(yamlFileName) {
        yaml_node = LoadYaml(filepath, search_path);
    };
    YamlParser() : __log(spdlog::default_logger()) {};

    //* === ~YamlParser ===
    /// @brief destructor
    ~YamlParser() {};

    //* === LoadYaml ===
    /// @brief load yaml file
    /// @param yamlFileName string containing yaml file name
    /// @param searchpath   string containing yaml file path (folder)
    /// @return yaml node with yaml key-value pairs
    YAML::Node LoadYaml(const std::string& yamlFileName, const std::string& searchPath) {
        YAML::Node root;
        // create full path variable
        std::stringstream path;
        path << std::filesystem::current_path().c_str() << "/" << searchPath << "/" << yamlFileName;
        std::string pathStr = path.str();
        // __log->debug("YamlParser::LoadYaml {}", pathStr);

        try {
            // load yaml file
            root = YAML::LoadFile(pathStr);
            __log->debug("Default yaml file specified: {}", root["default"].IsDefined());

            // default yaml file does not exist, return
            if (!root["default"].IsDefined()) {
                return root;
            }

            // default file exists, if at default (lowest level yaml file), return
            if (root["default"].IsNull()) {
                return root;
            }
            __log->debug("Default file is not \"Null\"");

            // --- iteratively update default yaml ---
            std::stringstream defaultsfilepath;
            defaultsfilepath << root["default"].as<std::string>();
            // __log->debug(defaultsfilepath.str());
            YAML::Node defaults = LoadYaml(defaultsfilepath.str(), searchPath);

            // update and overwrite default yaml
            defaults = UpdateOverwrite(defaults, root);
            yaml_node = defaults;
            return yaml_node;
        } catch (YAML::Exception const& e) {
            __log->warn("Settings::get_config -> ERROR {}", e.what());
            throw YAML::BadFile(pathStr);
        }
    };

    //* === exists ===
    /// @brief query whether variable exists in yaml node
    /// @returns True|False based on if settings have been generated
    bool exists(const std::string& variableName) {
        return yaml_node[variableName].IsDefined();
    };

    //* === GetVariable ===
    template <typename T>
    bool GetVariable(T& value, const std::string& variableName) {
        bool available = exists(variableName);
        // __log->debug("'{}' available: {}", variableName, available);
        if (available) {
            value = yaml_node[variableName].as<T>();
            __log->trace("YamlParser::get_variable -> '{}' = {} -> successfully read", variableName,
                         value);
        }
        return available;
    }

    //* === GetVariable ===
    /// @brief grab requested variable
    /// @tparam T
    /// @param variableName
    /// @return requested variable
    template <typename T>
    T GetVariable(const std::string& variableName) {
        T value;
        if (!GetVariable<T>(value, variableName)) {
            __log->debug(
                "YamlParser::get_variable -> ERROR variable '{}' must be set in config file",
                variableName);
            throw std::runtime_error(variableName);
        }
        return value;
    }

    //* === AddVariable ===
    template <typename T>
    bool AddVariable(T& value, const std::string& variableName) {
        yaml_node[variableName] = value;
        return exists(variableName);
    }

  private:
    //* === update_overwrite ===
    /// @brief update and overwrite current key-value pairs
    /// @param defaults   yaml key-value node to overwrite
    /// @param overrides  new yaml values
    /// @return updated yaml node
    YAML::Node UpdateOverwrite(YAML::Node defaults, YAML::Node overrides) {
        // loop through list of fieldnames in new yaml file
        for (const auto& kv : overrides) {
            std::string key(kv.first.as<std::string>());
            // __log->debug("{}", key);

            // if current key is a map of other values, first fix those values
            if (overrides[key].IsMap()) {
                defaults[key] = UpdateOverwrite(defaults[key], overrides[key]);
            } else {
                defaults[key] = overrides[key];
            }
        }
        return defaults;
    };

    /// @brief variables
    std::shared_ptr<spdlog::logger> __log;
    YAML::Node yaml_node;
    std::string search_path;
    std::string filepath;

};  // end class YamlParser
};  // namespace file_operations

#endif