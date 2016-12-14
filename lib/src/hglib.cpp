#include <hglib.h>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <hlibexeception.h>
#include <iostream>
#include <cstdint>

std::istream& operator >> ( std::istream& ins, HGParser::data& d ) {
    std::string s, key, value;

    // For each (key, value) pair in the file
    while (std::getline( ins, s ))
    {
        std::string::size_type begin = s.find_first_not_of( " \f\t\v" );

        // Skip blank lines
        if (begin == std::string::npos) continue;

        // Skip commentary and groups
        if (std::string( "#;[" ).find( s[ begin ] ) != std::string::npos) continue;

        // Extract the key value
        std::string::size_type end = s.find( '=', begin );
        key = s.substr( begin, end - begin );

        // (No leading or trailing whitespace allowed)
        key.erase( key.find_last_not_of( " \f\t\v" ) + 1 );

        // No blank keys allowed
        if (key.empty()) continue;

        // Extract the value (no leading or trailing whitespace allowed)
        begin = s.find_first_not_of( " \f\n\r\t\v", end + 1 );
        end   = s.find_last_not_of(  " \f\n\r\t\v" ) + 1;

        // catching n_pos > size() out of range exception
        try {
            value = s.substr( begin, end - begin );
        } catch ( const std::out_of_range& e ) {

        }
        // Insert the properly extracted (key, value) pair into the map
        if ( value.empty() )
            continue;

        d[ key ] = value;
    }

    return ins;
}

//---------------------------------------------------------------------------
// The insertion operator writes all configuration::data to stream.
//
std::ostream& operator << ( std::ostream& outs, const HGParser::data& d ) {
    HGParser::data::const_iterator iter;
    for (iter = d.begin(); iter != d.end(); iter++)
        outs << iter->first << " = " << iter->second << std::endl;
    return outs;
}


 HGParser::HGParser ( const std::string& szFileName )
    : szHFileName( szFileName )
    , nDataOffset(0)
{

}

HGParser::~HGParser( void ) {

}

void  HGParser::parseFile( HGFileInfo **ppFileInfo) {
    // parsing text part of tthe file
    try {
        parseTextPart();
    } catch ( HLibException& e) {
        throw HLibException ( "Could not read file!");
    }

    if ( *ppFileInfo == nullptr ) {
        try {
            (*ppFileInfo) = new HGFileInfo();
            (*ppFileInfo)->pDataTypes = nullptr;
        }catch ( const std::bad_alloc& e ) {
            throw HLibException ( "Bad allocation exception of FileInfo!" );
        }
    }

    std::string szDataType = hconfig.getStringValue("DataType");
    std::string::iterator sIter; 

    if ((*ppFileInfo)->pDataTypes == nullptr) {
        (*ppFileInfo)->pDataTypes = new std::vector<HGDataType>();
    }

    for (sIter = szDataType.begin(); sIter != szDataType.end(); ++sIter) {
        switch (*sIter) {
            case 'V':
                (*ppFileInfo)->pDataTypes->push_back(HGDataType(HGDataType::HGYVScan));
                break;
            case 'C':
                (*ppFileInfo)->pDataTypes->push_back(HGDataType(HGDataType::HGYCScan));
                break;
            case 'D':
                (*ppFileInfo)->pDataTypes->push_back(HGDataType(HGDataType::HGYDScan));
                break;
            default:
                break;
        }
    }

    // get data offset
    try {
        (*ppFileInfo)->nDataOffset = hconfig.getIntValue ("DataOffset");
    } catch ( int e ) {
        if ( e == 0 ) {
            throw HLibException("key isn't avaiable!");
        } else if ( e == 1 ) {
            throw HLibException("eof reached and no suitable key was found!");
        }
    }

    // get number of coordinates
    try {
        (*ppFileInfo)->nCoordinates = hconfig.getIntValue ( "CoordCount");
    } catch ( int e) {
        if ( e == 0 ) {
            throw HLibException("key isn't avaiable!");
        } else if ( e == 1 ) {
            throw HLibException("eof reached and no suitable key was found!");
        }

    }

    // adjust file information
    try {
        (*ppFileInfo)->pnDimension = new size_t [ (*ppFileInfo)->nCoordinates ];
        (*ppFileInfo)->pdScale = new double [ (*ppFileInfo)->nCoordinates ];
        (*ppFileInfo)->pdStart = new double [ (*ppFileInfo)->nCoordinates ];
        (*ppFileInfo)->pUnits = new std::string [ (*ppFileInfo)->nCoordinates ];
        (*ppFileInfo)->pcUnits = new char[(*ppFileInfo)->nCoordinates];
        
    } catch ( const std::bad_alloc& e ) {
        throw HLibException ( "Bad allocation exception of FileInfo attributes!" );
    }

    std::stringstream ss;
    ss.clear(); ss.str("");
    for ( size_t i = 0; i < (*ppFileInfo)->nCoordinates; ++i ) {

        (*ppFileInfo)->pnDimension[i] = 0;
        (*ppFileInfo)->pdScale[i] = 0;
        (*ppFileInfo)->pdStart[i] = 0;
        (*ppFileInfo)->pUnits[i] = std::string(" ");
        (*ppFileInfo)->pcUnits[i] = ' ';


        try {
            ss << "Coord" << i + 1 << ".Points";
            (*ppFileInfo)->pnDimension[i] = hconfig.getIntValue   ( ss.str() );
            ss.clear(); ss.str(""); ss << "Coord" << i + 1 << ".Increment";
            (*ppFileInfo)->pdScale[i] = hconfig.getDoubleValue(ss.str());
            ss.clear(); ss.str(""); ss << "Coord" << i + 1 << ".Start";
            (*ppFileInfo)->pdStart[i] = hconfig.getDoubleValue(ss.str());
            ss.clear(); ss.str(""); ss << "Coord" << i + 1 << ".Unit";
            (*ppFileInfo)->pUnits[i] = hconfig.getStringValue(ss.str());

            // scaling for SI units
            if ((*ppFileInfo)->pUnits[i].compare("mm") == 0) {
                (*ppFileInfo)->pcUnits[i] = 'm';
                (*ppFileInfo)->pdScale[i] = (*ppFileInfo)->pdScale[i] * 1e-3;
                (*ppFileInfo)->pdStart[i] = (*ppFileInfo)->pdStart[i] * 1e-3;
            } else if ((*ppFileInfo)->pUnits[i].compare((*ppFileInfo)->pUnits[i].size()-1 , 1, "s") == 0) {
                (*ppFileInfo)->pcUnits[i] = 's';
                (*ppFileInfo)->pdScale[i] = (*ppFileInfo)->pdScale[i] * 1e-6;
                (*ppFileInfo)->pdStart[i] = (*ppFileInfo)->pdStart[i] * 1e-6;
            }

            // clearing stringstream
            ss.clear(); ss.str("");
        } catch ( int e) {
            if ( e == 0 ) {
                throw HLibException("key isn't avaiable!");
            } else if ( e == 1 ) {
                throw HLibException("eof reached and no suitable key was found!");
            }
        }
        // calculating number of samples
        if ( i == 0) {
            (*ppFileInfo)->nSamples  = (*ppFileInfo)->pnDimension[i];
        } else {
            (*ppFileInfo)->nSamples *= (*ppFileInfo)->pnDimension[i];
        }
    }
    ss.clear(); ss.str("");
    unsigned int nByteSize = 0;
    ssize_t nRelDataOffset = 0;
    // get information about different measurement data sets
    for (size_t i = 0; i < (*ppFileInfo)->pDataTypes->size(); i++) {
        try {
            ss << "Measure" << i + 1 << ".DataWordSize";
            nByteSize = hconfig.getIntValue(ss.str());
            ss.clear(); ss.str("");
            ss << "Measure" << i + 1 << ".RelDataOffset";
            // getting relative data offset
            (*ppFileInfo)->pDataTypes->at(i).nDataOffset = hconfig.getIntValue(ss.str());
            // obtain number of bytes by multiplying number of samples with the number of bytes per data word
            (*ppFileInfo)->pDataTypes->at(i).nBytes = (*ppFileInfo)->nSamples * nByteSize;
            // obtain culmulative number of bytes for the whole file
            (*ppFileInfo)->nBytes += (*ppFileInfo)->pDataTypes->at(i).nBytes;
            ss.clear(); ss.str("");
            ss << "Measure" << i + 1 << ".ScaleFactor";
            (*ppFileInfo)->pDataTypes->at(i).dScaling = hconfig.getDoubleValue(ss.str());
            ss.clear(); ss.str("");
        } catch (int e) {
            if (e == 0) {
                throw HLibException("key isn't avaiable!");
            }
            else if (e == 1) {
                throw HLibException("eof reached and no suitable key was found!");
            }
        }
    }
}

void HGParser::getData( char *pcAmplitude, HGFileInfo** ppFileInfo ) {

    if (pcAmplitude == nullptr)
        return;  
    // read data
    std::ifstream g( szHFileName.c_str() , std::ifstream::binary );
    g.exceptions ( std::ifstream::badbit );
    try {
        g.seekg ( (*ppFileInfo)->nDataOffset);
        g.read((char*)(pcAmplitude), (*ppFileInfo)->nBytes * sizeof(char));
        g.close();
    } catch ( const std::ifstream::failure& e) {
        throw HLibException ( "Exception opening/reading/closing file!!" );
    }
}

void HGParser::parseTextPart ( void ) {
    if ( this->szHFileName.empty() ) {
        throw HLibException ( "Filename not set!");
    } else {
        // just read the whole file
        std::ifstream f( szHFileName.c_str() , std::ifstream::in );
        f.exceptions ( std::ifstream::badbit );
        try {
            // and parse it
            f >> hconfig;
            // finished
            f.close();
        } catch (const std::ifstream::failure& e) {
            throw HLibException ( "Exception opening/reading/closing file!!" );
        }
    }
}

bool HGParser::data::iskey(const std::string &s) const {
    return count( s ) != 0;
}

long HGParser::data::getIntValue( const std::string& key ) {
    if ( ! iskey(key) )
        throw 0;

    std::istringstream ss( this->operator [] ( key ) );
    long result = 0;
    ss >> result;
    if (!ss.eof())
        throw 1;
    return result;
}

float HGParser::data::getFloatValue( const std::string& key ) {
    if ( ! iskey(key) ) {
        throw 0.0;
    }

    std::istringstream ss( this->operator [] ( key ) );
    std::size_t nFound = ss.str().find(",");
    std::string tm = ss.str();
    // replacing comma with dots for floating point variables
    if ( nFound != std::string::npos ) {
        tm.replace ( nFound, 1 , "." );
        ss.clear(); ss.str(tm);
    }

    float result;
    ss >> result;
    if (!ss.eof()) {
        throw 1.0;
    }
    return result;
}

double HGParser::data::getDoubleValue(const std::string& key) {
    if (!iskey(key)) {
        throw 0.0;
    }

    std::istringstream ss(this->operator [] (key));
    std::size_t nFound = ss.str().find(",");
    std::string tm = ss.str();
    // replacing comma with dots for floating point variables
    if (nFound != std::string::npos) {
        tm.replace(nFound, 1, ".");
        ss.clear(); ss.str(tm);
    }

    double result;
    ss >> result;
    if (!ss.eof()) {
        throw 1.0;
    }
    return result;
}

std::string HGParser::data::getStringValue(const std::string& key) {
    if (!iskey(key))
        throw std::string::npos;

    std::istringstream ss(this->operator [] (key));
    return ss.str();
}
