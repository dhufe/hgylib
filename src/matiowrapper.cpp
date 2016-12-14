#include <matiowrapper.h>

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

    double* pSig = NULL;
    size_t pos = 0;
    for (std::vector<HGDataType>::iterator pIter = pDataInfo->pDataTypes->begin(); pIter != pDataInfo->pDataTypes->end(); ++pIter) {

        if (pSig == NULL) {
            pSig = new double[pDataInfo->nSamples]();

            if ((*pIter).DataType != HGDataType::HGYVScan) {
                for (auto y = 0; y < pDataInfo->pnDimension[1]; y++) {
                    for (auto x = 0; x < pDataInfo->pnDimension[0]; x++) {
                        pSig  [ pDataInfo->pnDimension[0] * y + x ] = (double) ( pcData [ (*pIter).nDataOffset + pDataInfo->pnDimension[0] * y + x ] ) ;
                    }
                }
            } else if ( (*pIter).DataType == HGDataType::HGYVScan ) {
                for ( auto z = 0; z < pDataInfo->pnDimension[2]; z++ ) {
                    for (auto y = 0; y < pDataInfo->pnDimension[1]; y++) {
                        for (auto x = 0; x < pDataInfo->pnDimension[0]; x++) {
                            pSig  [ pDataInfo->pnDimension[0] * pDataInfo->pnDimension[1] * z +  pDataInfo->pnDimension[0] * y + x ] = (double) ( pcData [ (*pIter).nDataOffset + pDataInfo->pnDimension[0] * pDataInfo->pnDimension[1] * z + pDataInfo->pnDimension[0] * y + x ] ) ;
                        }
                    }
                }
            }

            pMatVar = Mat_VarCreate( (*pIter).toString().c_str(), MAT_C_DOUBLE, MAT_T_DOUBLE, pDataInfo->nCoordinates, pSizes, pSig, MAT_F_DONT_COPY_DATA);
            Mat_VarWrite(pMat, pMatVar, MAT_COMPRESSION_NONE);
            Mat_VarFree(pMatVar);

            delete[] pSig;
            pSig = NULL;
        }
    }


}
