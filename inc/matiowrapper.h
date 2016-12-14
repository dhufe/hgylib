/**
*       @file  matiowrapper.cpp
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

#ifndef __MATIO_WRAPPER_H__
#define __MATIO_WRAPPER_H__

#include <matio_private.h>
#include <hglib.h>

class MatioWrapper {
    public:

        MatioWrapper( const std::string& );
        bool writeData(const HGFileInfo*, const char*);

    private:
        mat_t*		pMat;
        matvar_t*	pMatVar;
        std::string szFileName;
};

#endif // __MATIO_WRAPPER_H__