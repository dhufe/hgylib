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

    std::cout << "Dimensions : " << pFile->pnDimension[0] << "x"        << pFile->pnDimension[1] << "x" << pFile->pnDimension[2] << std::endl;
    std::cout << "Start      : " << pFile->pdStart[0]     << "x"        << pFile->pdStart[1]     << "x" << pFile->pdStart[2]     << std::endl;
    std::cout << "Scale      : " << pFile->pdScale[0]     << "x"        << pFile->pdScale[1]     << "x" << pFile->pdScale[2]     << std::endl;
    std::cout << "nSamples   : " << pFile->pnDimension[0] * pFile->pnDimension[1] * pFile->pnDimension[2] << std::endl;

    try {
        pcData = new char [ pFile->nSamples ] ;
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
