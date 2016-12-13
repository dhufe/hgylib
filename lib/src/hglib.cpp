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
        }catch ( const std::bad_alloc& e ) {
            throw HLibException ( "Bad allocation exception of FileInfo!" );
        }
    }

    if ( hconfig.getStringValue ( "DataType" ).compare ( "V" ) == 0 ) {
        (*ppFileInfo)->eType = HGYVolumeScan;
    } else {
        (*ppFileInfo)->eType = HGYScan;
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

#if _DEBUG
    std::cout << "number of coordinates : " << (*ppFileInfo)->nCoordinates;
#endif

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
            (*ppFileInfo)->pdScale[i] = hconfig.getFloatValue(ss.str());
            ss.clear(); ss.str(""); ss << "Coord" << i + 1 << ".Start";
            (*ppFileInfo)->pdStart[i] = hconfig.getFloatValue(ss.str());
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
}

void HGParser::getData( int16_t *piAmplitude, HGFileInfo** ppFileInfo ) {

    if (piAmplitude == nullptr)
        return;

    float fScaling = 0.0f;
    try {
        fScaling = hconfig.getFloatValue ( "Measure1.ScaleFactor" );
        std::cout << "scaling : " << fScaling << std::endl;
        (*ppFileInfo)->dAmpScaling = (double)(fScaling);
    } catch ( float e ) {
        if ( e == 0.0 ) {
            throw HLibException("key isn't avaiable!");
        } else if ( e == 1.0 ) {
            throw HLibException("eof reached and no suitable key was found!");
        }
    }
    // read data
    std::ifstream g( szHFileName.c_str() , std::ifstream::binary );
    g.exceptions ( std::ifstream::badbit );
    try {
        g.seekg ( (*ppFileInfo)->nDataOffset);
        g.read((char*)(piAmplitude), (*ppFileInfo)->nSamples * sizeof (int16_t) );
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

std::string HGParser::data::getStringValue(const std::string& key) {
    if (!iskey(key))
        throw std::string::npos;

    std::istringstream ss(this->operator [] (key));
    return ss.str();
}
