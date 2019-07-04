#include <matiowrapper.h>
#include <sstream>

MatioWrapper::MatioWrapper(const std::string& _FileName)
    : szFileName(_FileName)
    , pMat(NULL)
    , pMatVar(NULL)
{

}

bool MatioWrapper::writeData(const HGFileInfo* pDataInfo, const char* pcData) {
    if (szFileName.empty())
        return false;

    if (pDataInfo == nullptr)
        return false;

    if (pcData == nullptr)
        return false;


    pMat = Mat_CreateVer(szFileName.c_str() , NULL, MAT_FT_MAT73);

    if (pMat == NULL)
        return false;

    // size of matrices
    size_t* pSizes = new size_t[pDataInfo->nCoordinates];
    for (auto i = 0; i < pDataInfo->nCoordinates; i++) {
        pSizes[i] = pDataInfo->pnDimension[i];
    }

    double* pSig      = NULL;
    int16_t* pIData   = NULL;
    int32_t* pI32Data = NULL;
    size_t nIndex     = 0;

    bool bIsRotary    = false;

    // reconstruction parameters
    size_t  sDim[2] = { 1, pDataInfo->nCoordinates };
    double* pdStart  = new double[pDataInfo->nCoordinates];
    double* pdScale  = new double[pDataInfo->nCoordinates];
    double* pdSizes  = new double[pDataInfo->nCoordinates];
    double* pnSizes  = new double[pDataInfo->nCoordinates];
    char*   pcUnits  = new char[pDataInfo->nCoordinates];
    int*   piRotary  = new int[pDataInfo->nCoordinates];
    // struct dimensions
    size_t  dims_struct[2] = { 1, 1 };
    std::stringstream ss;
    
    for (std::vector<HGDataType>::iterator pIter = pDataInfo->pDataTypes->begin(); pIter != pDataInfo->pDataTypes->end(); ++pIter) {

        if (pSig == NULL) {
            pSig = new double[pDataInfo->nSamples]();

            switch ((*pIter).nDataWordSize) {
                default:
                case 2:
                    pIData = (int16_t*)(&pcData[(*pIter).nDataOffset]);
                    break;
                case 4:
                    pI32Data = (int32_t*)(&pcData[(*pIter).nDataOffset]);
                    break;
            }

            if ((*pIter).DataType != HGDataType::HGYVScan) {
                for (auto y = 0; y < pDataInfo->pnDimension[1]; y++) {
                    for (auto x = 0; x < pDataInfo->pnDimension[0]; x++) {
                        switch ((*pIter).nDataWordSize) {
                            default:
                            case 2:
                                pSig[pDataInfo->pnDimension[0] * y + x] = (*pIter).dScaling * (double)(pIData[pDataInfo->pnDimension[0] * y + x]);
                                break;
                            case 4:
                                pSig[pDataInfo->pnDimension[0] * y + x] = (*pIter).dScaling * (double)(pI32Data[pDataInfo->pnDimension[0] * y + x]);
                                break;
                        }
                    }
                }
            } else if ( (*pIter).DataType == HGDataType::HGYVScan ) {
                for ( auto z = 0; z < pDataInfo->pnDimension[2]; z++ ) {
                    for (auto y = 0; y < pDataInfo->pnDimension[1]; y++) {
                        for (auto x = 0; x < pDataInfo->pnDimension[0]; x++) {
                            switch ((*pIter).nDataWordSize) {
                                default:
                                case 2:
                                    pSig[pDataInfo->pnDimension[0] * pDataInfo->pnDimension[1] * z + pDataInfo->pnDimension[0] * y + x] = (*pIter).dScaling * (double)(pIData[pDataInfo->pnDimension[0] * pDataInfo->pnDimension[1] * z + pDataInfo->pnDimension[0] * y + x]);
                                    break;
                                case 4:
                                    pSig[pDataInfo->pnDimension[0] * pDataInfo->pnDimension[1] * z + pDataInfo->pnDimension[0] * y + x] = (*pIter).dScaling * (double)(pI32Data[pDataInfo->pnDimension[0] * pDataInfo->pnDimension[1] * z + pDataInfo->pnDimension[0] * y + x]);
                                    break;
                            }
                        }
                    }
                }
            }

            ss << (*pIter).toString().c_str() << "_" << nIndex;

            pMatVar = Mat_VarCreate( ss.str().c_str() , MAT_C_DOUBLE, MAT_T_DOUBLE, (int) (pDataInfo->nCoordinates), pSizes, pSig, MAT_F_DONT_COPY_DATA);
            ss.clear(); ss.str("");
            Mat_VarWrite(pMat, pMatVar, MAT_COMPRESSION_ZLIB);
            Mat_VarFree(pMatVar);

            delete[] pSig;
            pSig = NULL;
            nIndex++;
        }

    }

    for (auto i = 0; i < pDataInfo->nCoordinates; i++) {
        pnSizes[i] = (double) (pDataInfo->pnDimension[i]);
        pdSizes[i] = (double)(pDataInfo->pdStart[i] + pDataInfo->pnDimension[i] * pDataInfo->pdScale[i]);
        pcUnits[i] = pDataInfo->pcUnits[i];
        if (pcUnits[i] == '°')
            piRotary[i] = 1;
        else 
            piRotary[i] = 0;
    }

    pMatVar = Mat_VarCalloc();
    matvar_t** ppMatStructVars = (matvar_t**)calloc(6 , sizeof(matvar_t*));

    ppMatStructVars[0] = Mat_VarCreate("Scale", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, sDim, (void*)(pDataInfo->pdScale), 0);
    ppMatStructVars[1] = Mat_VarCreate("Start", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, sDim, (void*)(pDataInfo->pdStart), 0);
    ppMatStructVars[2] = Mat_VarCreate("Dimensions", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, sDim, (void*)(pnSizes), 0);
    ppMatStructVars[3] = Mat_VarCreate("Sizes", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, sDim, (void*)(pdSizes), 0);
    ppMatStructVars[4] = Mat_VarCreate("Rotary", MAT_C_INT32, MAT_T_INT32, 2, sDim, (void*)(piRotary), 0);
    ppMatStructVars[5] = NULL;

    matvar_t*  pMatStruct = Mat_VarCreate("recparm", MAT_C_STRUCT, MAT_T_STRUCT, 2, dims_struct, ppMatStructVars, 0);
    Mat_VarWrite(pMat, pMatStruct, MAT_COMPRESSION_ZLIB);
    Mat_VarFree(pMatVar);
    Mat_Close(pMat);

    delete[] pdStart;
    delete[] pdScale;
    delete[] pdSizes;
    delete[] pnSizes;
    delete[] pcUnits;
    delete[] piRotary;

    return true;
}
