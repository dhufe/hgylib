/**
 *       @file  main.cpp
 *      @brief
 *
 * Detailed description starts here.
 *
 *     @author  Daniel Kotschate (daniel), daniel@epyx-online.de
 *
 *   @internal
 *     Created  29.04.2016
 *    Revision  ---
 *    Compiler  gcc/g++
 *     Company  daniel@epyx-online.de
 *   Copyright  Copyright (c) 2016, Daniel Kotschate
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License as published by the Free Software Foundation.
 * =====================================================================================
 */

#include <cstdlib>
#include <iostream>
#include <hglib.h>
#include <inttypes.h>
#include <hlibexeception.h>
#include <console_arg.h>
#include <matiowrapper.h>
#include <consumer.h>
#include <producer.h>
#include <threadqueue.h>
#include <version.h>

int main ( int argc, char* argv[] ) {


    ConArgs *pConfigArgs = new ConArgs();

    static struct option long_options[] =
    {
        { "verbose", no_argument, &(pConfigArgs->verbose_flag), 1 },
        { "export", no_argument, &(pConfigArgs->export_flag), 1 },
        { "infile", required_argument, 0, 'i' },
        { "outfile", required_argument, 0, 'o' },
        //{ "logfile", optional_argument, 0, 'l' },
        { 0, 0, 0, 0 }
    };

    if (pConfigArgs->parse_arguments_long(argc, argv, "i:o:", long_options) == -1) {
        pConfigArgs->usage();
        return EXIT_FAILURE;
    }
    else {
        std::cout << std::endl;
        std::cout << pConfigArgs->szApplicationName << std::endl;
        std::cout << std::endl;
        std::cout << "Copyright © Daniel Kotschate (dkotscha)" << std::endl;
        std::cout << std::endl;
        std::cout << "Version info:" << std::endl;
        std::cout << "  last commit: " << VersionInfo::GIT_DATE << std::endl;
        std::cout << "  commit rev: " << VersionInfo::GIT_SHA1 << std::endl;
        std::cout << "  commig msg: " << VersionInfo::GIT_COMMIT_SUBJECT << std::endl;
        std::cout << std::endl << std::endl;

        TQueue<std::vector<char>> queue;
        Producer producer ( &pConfigArgs, queue );
        Consumer consumer ( &pConfigArgs, queue );

        producer.start();
        consumer.start();

        producer.join();
        consumer.join();

//        HGParser hp(pConfigArgs->szInputFileName, pConfigArgs->export_flag );
 // char* pcData = nullptr;
//        HGFileInfo* pFile = pConfigArgs->pFile;

//        std::cout << "Number of coordinates: " << pFile->nCoordinates << std::endl;

 // try {
 // hp.parseFile(&pFile);
 // }
 // catch (const HLibException& e) {
 // std::cerr << "An error was reported : " << e.what() << std::endl;
 // return EXIT_FAILURE;
 // }
 /*
       try {
            pcData = new char[pFile->nBytes];
        }
        catch (std::bad_alloc) {
            std::cerr << "unable to allocate memory!" << std::endl;
        }
        */
/*
        try {
            hp.getData(pcData, &pFile);
        }
        catch (const HLibException& e) {
            std::cerr << "An error was reported : " << e.what() << std::endl;
        }

        MatioWrapper wrapper(pConfigArgs->szOutputFileName);
        wrapper.writeData(pFile, pcData);
*/


//        delete[] pcData;
        delete pConfigArgs;
//        delete pFile;

        return EXIT_SUCCESS;
    }
}
