#include <iostream>
#include <exception>
#include <boost/filesystem.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/thread.hpp>
#include "Hasher.h"

using namespace std;

int main(int argc, char** argv)
{
    try {
        if (argc < 3 || argc > 4)
            throw invalid_argument("Incorrect input");

        boost::filesystem::path inputFileName, outputFileName;
        inputFileName = argv[1];
        outputFileName = argv[2];

        CheckPaths(inputFileName, outputFileName);

        unsigned long chunkSize = 1024 * 1024; //1 mb
        const unsigned long totalSize = boost::filesystem::file_size(argv[1]);

        if (argc == 4) {
            chunkSize = stoi(argv[3]);
            if (chunkSize <= 0)
                throw invalid_argument("Chunk size must be > 0");
            if (chunkSize > totalSize)
                throw invalid_argument("Chunk size is too big");
        }

        const size_t nThreads = boost::thread::hardware_concurrency();
        vector<vector<unsigned short>> outputResult(nThreads);
        boost::asio::io_service ioService;

        for (size_t i = 0; i < nThreads; ++i) {
            ioService.post(boost::bind(&singleThreadHasherTask, inputFileName, i, nThreads, chunkSize, boost::ref(outputResult[i])));
        }

        boost::thread_group threadPool;

        for (size_t i = 0; i < nThreads; ++i) {
            threadPool.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
        }

        threadPool.join_all();
        ioService.stop();

        writeHashSum(outputFileName, outputResult);
    }

    catch (const invalid_argument& errArg) {
        cerr << "Wrong argument. " << errArg.what() << endl;
    }
    catch (const runtime_error& errRun) {
        cerr << "Runtime error. " << errRun.what() << endl;
    }
    catch (const exception& errAny) {
        cerr << errAny.what() << endl;
    }
    return 0;
}