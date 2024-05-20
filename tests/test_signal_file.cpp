/**
|====================================== test_signal_file.cpp ======================================|
|                                                                                                  |
|   Property of Daniel Sturdivant. Unauthorized copying of this file via any medium is would be    |
|   super sad and unfortunate for me. Proprietary and confidential.                                |
|                                                                                                  |
|--------------------------------------------------------------------------------------------------|
|                                                                                                  |
|   @file     examples/test_signal_file.cpp                                                        |
|   @brief    File testing the binary signal_file methods.                                         |
|   @author   Daniel Sturdivant <sturdivant20@gmail.com>                                           |
|   @date     April 2024                                                                           |
|                                                                                                  |
|==================================================================================================|
*/

#include <spdlog/spdlog.h>

#include "file_operations/binary_file.hpp"

int main() {
    std::shared_ptr<spdlog::logger> _log = spdlog::default_logger();

    int8_t initial_arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    file_operations::BinaryFile<int8_t> writer("temp.bin", 'w');
    writer.fwrite(initial_arr, 10);
    writer.fclose();
    _log->info("Array written = [{},{},{},{},{},{},{},{},{},{}]", initial_arr[0], initial_arr[1],
               initial_arr[2], initial_arr[3], initial_arr[4], initial_arr[5], initial_arr[6],
               initial_arr[7], initial_arr[8], initial_arr[9]);

    int8_t final_arr[10];
    file_operations::BinaryFile<int8_t> reader("temp.bin", 'r');
    reader.fread(final_arr, 10);
    reader.fclose();
    _log->info("Array read = [{},{},{},{},{},{},{},{},{},{}]", final_arr[0], final_arr[1],
               final_arr[2], final_arr[3], final_arr[4], final_arr[5], final_arr[6], final_arr[7],
               final_arr[8], final_arr[9]);

    std::remove("temp.bin");
    return 0;
}
