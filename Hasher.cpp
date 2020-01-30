//
// Created by Татьяна on 2020-01-29.
//
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/crc.hpp>
#include <boost/filesystem.hpp>
#include "Hasher.h"

using namespace std;
namespace fs = boost::filesystem;

void CheckPaths(const fs::path &inFilePath, const fs::path &outFilePath) {
    if (!fs::exists(inFilePath))
        throw std::runtime_error("source file does not exist");

    if (!fs::is_regular_file(inFilePath))
        throw std::runtime_error(inFilePath.string() + "is not a file");

    if (!fs::exists(outFilePath)) {
        std::ofstream created(outFilePath.string().c_str());
        if (!created)
            throw std::runtime_error("cannot create output file");
    } else if (!fs::is_regular_file(outFilePath))
        throw std::runtime_error(outFilePath.string() + "is not a file");
}

void singleThreadHasherTask(const fs::path &inputFileName, size_t &threadNumber,
                            size_t &nThreads, unsigned long &chunkSize,
                            vector<unsigned short> &resultVector) {

    std::ifstream inputFile(inputFileName.c_str(), std::ios::in | std::ios::binary);
    auto buffer = std::vector<char>(chunkSize);

    boost::crc_32_type crcSum;

    size_t iteration = 0;

    while (!inputFile.eof()) {
        inputFile.seekg((threadNumber + iteration * nThreads) * chunkSize, std::ios::beg);
        inputFile.read(&buffer[0], chunkSize);

        const size_t readBytes = inputFile.gcount();

        if (readBytes < chunkSize)
            buffer.resize(readBytes);

        if (!buffer.empty()) {
            crcSum.process_bytes(&buffer[0], buffer.size());
            resultVector.push_back(crcSum.checksum());
            ++iteration;
        }
    }
    inputFile.close();
}


void writeHashSum(const fs::path& outputFileName, vector<std::vector<unsigned short>>& crcSum) {
    std::ofstream outputFileHandler;
    outputFileHandler.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    try {
        outputFileHandler.open(outputFileName.c_str());
        for (const auto &elem : crcSum)
            for (const auto &sum : elem)
                outputFileHandler << std::hex << sum << ' ';
        outputFileHandler.close();
    }

    catch (std::ofstream::failure &e) {
        std::cerr << "Error making output file." << e.what() << std::endl;
    }
}


