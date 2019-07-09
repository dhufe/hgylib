#include <iostream>
#include <cmath>
#include <matio.h>

const long nNUM_VALUES = 1000;

int main(int argc, const char *argv[])
{
    mat_t       *matfp;
    matvar_t    *matvar;
    size_t      dims[2] = {nNUM_VALUES,1};
    double      *m_pi    = new double [nNUM_VALUES];

    // pointer to data == NULL !?
    matfp = Mat_CreateVer ( argv[1], NULL, MAT_FT_MAT73 );
    if ( matfp == nullptr ) {
        std::cerr << "Error creating MAT file " << argv[1] << std::endl;
    }

    for ( auto idx = 0; idx < nNUM_VALUES; ++idx )
        m_pi[ idx  ] = M_PI;


    matvar  = Mat_VarCreate ( "M_PI", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dims, &m_pi, 0 );

    if ( matvar != nullptr ) {
        int dim = 2;
        // Mat_VarWrite ( matfp, matvar, MAT_COMPRESSION_ZLIB );
        for (auto idx = 0; idx < 1000*nNUM_VALUES; ++idx) {
            Mat_VarWriteAppend ( matfp, matvar, MAT_COMPRESSION_ZLIB, dim );
        }
//        Mat_VarWriteAppend ( matfp, matvar, MAT_COMPRESSION_ZLIB, dim );
        Mat_VarFree (matvar );
    }

    Mat_Close (matfp);
    delete [] m_pi;
    return 0;
}
