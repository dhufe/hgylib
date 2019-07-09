#include <iostream>
#include <cmath>
#include <matio.h>

const long nNUM_VALUES = 10000000;

int main(int argc, const char *argv[])
{
    mat_t       *matfp;
    matvar_t    *matvar;
    size_t      dims[2] = {1,1};
    double      m_pi    = M_PI;

    // pointer to data == NULL !?
    matfp = Mat_CreateVer ( argv[1], NULL, MAT_FT_MAT73 );
    if ( matfp == nullptr ) {
        std::cerr << "Error creating MAT file " << argv[1] << std::endl;
    }

    matvar  = Mat_VarCreate ( "M_PI", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, &m_pi, 0 );

    if ( matvar != nullptr ) {
        int dim = 1;
        // Mat_VarWrite ( matfp, matvar, MAT_COMPRESSION_ZLIB );
        for (auto idx = 0; idx < nNUM_VALUES; ++idx) {
            Mat_VarWriteAppend ( matfp, matvar, MAT_COMPRESSION_ZLIB, dim );
        }
//        Mat_VarWriteAppend ( matfp, matvar, MAT_COMPRESSION_ZLIB, dim );
        Mat_VarFree (matvar );
    }

    Mat_Close (matfp);
    return 0;
}
