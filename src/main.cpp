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

int main ( int argc, const char* argv[] ) {

    std::string szFileName;
    if ( argc < 2 )
        szFileName = std::string("test2.ini");
    else
        szFileName = std::string(argv[1]);

    HGParser hp ( szFileName );
    char* pcData  = nullptr;
    HGFileInfo* pFile = nullptr;

    try {
        hp.parseFile(&pFile);
    } catch ( const HLibException& e ) {
        std::cerr << "An error was reported : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Dimensions : ";
    for ( auto i = 0; i < pFile->nCoordinates; i++ ) {
        if ( i <  pFile->nCoordinates - 1  )
            std::cout <<  pFile->pnDimension[i] << " x ";
        else
            std::cout << pFile->pnDimension[i] << std::endl;
    }

    std::cout << "Start : ";
    for (auto i = 0; i < pFile->nCoordinates; i++) {
        if (i <  pFile->nCoordinates - 1)
            std::cout << pFile->pdStart[i] << " x ";
        else
            std::cout << pFile->pdStart[i] << std::endl;
    }

    std::cout << "Scaling : ";
    for (auto i = 0; i < pFile->nCoordinates; i++) {
        if (i <  pFile->nCoordinates - 1)
            std::cout << pFile->pdScale[i] << " x ";
        else
            std::cout << pFile->pdScale[i] << std::endl;
    }


    if (pFile->pDataTypes) {

        std::cout << std::endl << pFile->pDataTypes->size() << " different types of measurement data." << std::endl;
        int i = 0;
        for (std::vector<HGDataType>::iterator pIter = pFile->pDataTypes->begin(); pIter != pFile->pDataTypes->end(); ++pIter ) {
            i++;
            std::cout << "data set #" << i << " Datatype : " << (*pIter).toString() << " DataOffset : " << (*pIter).nDataOffset << " Size : " << (*pIter).nBytes << " bytes" << std::endl;
        }
    }

    std::cout << "total size of data region : " << pFile->nBytes << " Bytes" << std::endl;

    try {
        pcData = new char [ pFile->nBytes ] ;
    } catch (std::bad_alloc) {
        std::cerr << "unable to allocate memory!" << std::endl;
    }

    try {
        hp.getData(pcData, &pFile);
    } catch ( const HLibException& e ) {
        std::cerr << "An error was reported : " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
