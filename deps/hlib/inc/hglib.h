#ifndef __HG_LIB_H__
#define __HG_LIB_H__

#include <map>
#include <string>
#include <cstdint>
#include <vector>
#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

static const char *HGDataTypeStr[] = {
    "HGYVScan",
    "HGYCScan",
    "HGYDScan"
};

struct HGDataType {

    enum eDataType {
        HGYVScan = 0x00,
        HGYCScan = 0x01,
        HGYDScan = 0x02
    };

    HGDataType( eDataType _type )
        : DataType( _type )
        , nBytes( 0 )
        , nDataWordSize(0)
        , nDataOffset( 0 )
        , dScaling (0)
    {

    }

    std::string toString( void )  const {
        return HGDataTypeStr[ DataType ];
    }

    std::string toString( eDataType _type )  const {
        return HGDataTypeStr[_type];
    }

    ssize_t nBytes;
    size_t nDataWordSize;
    ssize_t nDataOffset;
    double  dScaling;
    std::string szUnit;
    eDataType  DataType;
};

struct HGFileInfo {
    size_t                  *pnDimension;
    double                  *pdScale;
    double                  *pdStart;
    std::string             *pUnits;
    char	            	*pcUnits;
    ssize_t                 nSamples;
    ssize_t                 nBytes;
    size_t                  nCoordinates;
    ssize_t		            nDataOffset;
    std::vector<HGDataType>* pDataTypes;
};

class HGParser {
    public:
        explicit HGParser       ( const std::string& szFileName, uint8_t exportFlag = 0 );
        void     parseFile      ( HGFileInfo** ppFileInfo );
        void     openFileBinary ( void );
        void     closeFileBinary( void );
        void     getData	    ( char *pcAmplitude, HGFileInfo**);
        ssize_t  getDataChunk   ( char* pcAmplitude, ssize_t, ssize_t, ssize_t );
        virtual ~HGParser   ( void );

    private:
        std::string         szHFileName;

        struct data: std::map <std::string, std::string> {
            public:
                bool    iskey           ( const std::string& s ) const;
                long    getIntValue     ( const std::string& key );
                float   getFloatValue   ( const std::string& key );
                double  getDoubleValue  ( const std::string& key );
                std::string getStringValue(const std::string& key );
                ssize_t nDataOffset;
        };

        friend std::istream& operator >> ( std::istream& ins, data& d );
        friend std::ostream& operator << ( std::ostream& outs, const data& d );

        data            hconfig;
        ssize_t         nDataOffset;
        uint8_t         nExport;
        std::ifstream*  pFileBinaryHd;

        // Chunk pointer
        ssize_t     nCurrentDataPointer;

    private:

        /**
        * Removes the extension described by filename.
        *
        * @author Daniel Kotschate
        * @date 1/18/2016
        *
        * @tparam T Generic type parameter.
        * @param filename Filename of the file.
        *
        * @return A T.
        */

        template<class T>
        static T remove_extension(T const & filename)
        {
            typename T::size_type const p(filename.find_last_of('.'));
            return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
        }

        void parseTextPart ( void );
        void printExportTable( HGFileInfo** );

};



#endif // __HG_LIB_H__
