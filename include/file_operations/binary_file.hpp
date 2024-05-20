/**
|======================================== binary_file.hpp =========================================|
|                                                                                                  |
|   Property of Daniel Sturdivant. Unauthorized copying of this file via any medium is would be    |
|   super sad and unfortunate for me. Proprietary and confidential.                                |
|                                                                                                  |
|--------------------------------------------------------------------------------------------------|
|                                                                                                  |
|   @file     utils/file_operations/binary_file.cpp                                                |
|   @brief    File containing the binary signal_file definitions.                                  |
|   @author   Daniel Sturdivant <sturdivant20@gmail.com>                                           |
|   @date     May 2024                                                                             |
|                                                                                                  |
|==================================================================================================|
*/

#ifndef FILE_OPS_BINARY_FILE_HPP
#define FILE_OPS_BINARY_FILE_HPP

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace file_operations {

template <class T>
class BinaryFile {
  public:
    //* === BinaryFile ===
    /// @brief constructor
    /// @param filename string containing signal file name (if path and filename is one string, this
    /// is the only input)
    /// @param filepath string containing path to signal file
    /// @param mode     method of opening file, file type is set to binary (default = 'r')
    ///                   - "r": Read, open file at beginning to read (error if nonexistant file)
    ///                   - "w": Write, open file at end to append (error if nonexistant file)
    BinaryFile(const std::string fname, const std::string fpath, const char mode = 'r')
        : __log(spdlog::default_logger()), mode(mode) {
        std::stringstream tmp;
        tmp << fpath << "/" << fname;
        filename = tmp.str();
        if (fopen()) {
            __log->debug("BinaryFile opened {}", filename);
        }
    };
    //* === BinaryFile ===
    BinaryFile(const std::string fname, const char mode = 'r')
        : __log(spdlog::default_logger()), filename(fname), mode(mode) {
        if (fopen()) {
            __log->debug("BinaryFile opened {} in mode {}.", filename, mode);
        }
    };

    //* === ~BinaryFile ===
    /// @brief destructor
    ~BinaryFile() {
        if (fclose()) {
            __log->debug("BinaryFile closed {}.", filename);
        }
    };

    //* === fread ===
    /// @brief read data from file
    /// @param buffer destination of data parsed from file
    /// @param len    number of samples to read
    /// @return True|False based upon reading/parsing success
    bool fread(T *buffer, const int len) {
        try {
            if (!fid.is_open() || fid.bad()) {
                __log->error("BinaryFile::fread bad signal file.");
                return false;
            }
            // read into standard array
            fid.read(reinterpret_cast<char *>(buffer), len * sizeof(T));
            return true;
        } catch (std::exception const &e) {
            __log->error("BinaryFile::fread unable to read file. ERROR {}.", e.what());
            return false;
        }
    };
    //* === fread ===
    bool fread(std::vector<T> &buffer, const int len) {
        try {
            if (!fid.is_open() || fid.bad()) {
                __log->error("BinaryFile::fread bad signal file.");
                return false;
            }
            // read into standard vector
            fid.read(reinterpret_cast<char *>(&buffer[0]), len * sizeof(T));
            return true;
        } catch (std::exception const &e) {
            __log->error("BinaryFile::fread unable to read file. ERROR {}.", e.what());
            return false;
        }
    };

    //* === fwrite ===
    /// @brief write data to file
    /// @param buffer data to write into file
    /// @param len    number of samples to write
    /// @return True|False based upon writing success
    bool fwrite(T *buffer, const int len) {
        try {
            if (!fid.is_open() || fid.bad()) {
                __log->error("BinaryFile::fwrite bad signal file.");
                return false;
            }
            // write into standard array
            fid.write(reinterpret_cast<char *>(buffer), len * sizeof(T));
            return true;
        } catch (std::exception const &e) {
            __log->error("BinaryFile::fwrite unable to write to file. ERROR {}.", e.what());
            return false;
        }
    };
    //* === fwrite ===
    bool fwrite(std::vector<T> &buffer, const int len) {
        try {
            if (!fid.is_open() || fid.bad()) {
                __log->error("BinaryFile::fwrite bad signal file.");
                return false;
            }
            // write into standard array
            fid.write(reinterpret_cast<char *>(&buffer[0]), len * sizeof(T));
            return true;
        } catch (std::exception const &e) {
            __log->error("BinaryFile::fwrite unable to write to file. ERROR {}.", e.what());
            return false;
        }
    };

    //* === fseek ===
    /// @brief seek to a specific sample from the beginning of the file
    /// @param len    number of samples to skip from beginning of file
    /// @return True|False based upon seeking success
    bool fseek(const int len) {
        try {
            if (!fid.is_open() || fid.bad()) {
                __log->error("BinaryFile::fseek bad signal file.");
                return false;
            }
            // seek to sample
            fid.seekg(len * sizeof(T), fid.beg);
            return true;
        } catch (std::exception const &e) {
            __log->error("BinaryFile::fseek unable to seek in file. ERROR {}.", e.what());
            return false;
        }
    };

    //* === ftell ===
    /// @brief retrieve sample number from the beginning of the file
    /// @param location sample index from beginning of file
    /// @return True|False based upon telling success
    bool ftell(int &location) {
        try {
            if (!fid.is_open() || fid.bad()) {
                __log->error("BinaryFile::ftell bad signal file.");
                return false;
            }
            // tell sample
            location = fid.tellg() / sizeof(T);
            return true;
        } catch (std::exception const &e) {
            __log->error("BinaryFile::ftell unable to tell in file. ERROR {}.", e.what());
            return false;
        }
    };
    //* === ftell ===
    int ftell() {
        int location;
        if (ftell(location)) {
            return location;
        } else {
            return -1;
        }
    };

    //* === fopen ===
    /// @brief attempts to open signal file under specified filename and mode
    /// @return True|False based upon opening success
    bool fopen() {
        try {
            if (mode == 'r') {
                // reading file
                fid = std::fstream(filename, std::ios::in | std::ios::binary);
                return true;
            } else if (mode == 'w') {
                // writing file
                fid = std::fstream(filename, std::ios::out | std::ios::binary);
                return true;
            } else {
                // error
                __log->error(
                    "BinaryFile::fopen unable to open file. ERROR bad mode, "
                    "must be 'r' or 'w'.");
                return false;
            }
        } catch (std::exception const &e) {
            __log->error("BinaryFile::fopen unable to open file. ERROR {}.", e.what());
            return false;
        }
    };

    //* === fclose ===
    /// @brief attempts to close active file identifier
    /// @return True|False based upon closing success
    bool fclose() {
        try {
            fid.close();
            return true;
        } catch (std::exception const &e) {
            __log->error("BinaryFile::fopen unable to close file. ERROR {}.", e.what());
            return false;
        }
    };

    //* === interleaved_to_complex ===

  private:
    /// @brief class variables
    std::shared_ptr<spdlog::logger> __log;
    std::string filename;
    char mode;
    std::fstream fid;

};  // end class BinaryFile
}  // namespace file_operations

#endif