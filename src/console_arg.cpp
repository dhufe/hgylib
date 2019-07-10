#include <console_arg.h>

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <version.h>


int ConArgs::parse_arguments_long ( int argc, char* argv[], const char* cOptions, const struct option* strOption ) {
    int c;
    int option_idx = 0;
    std::stringstream argument;
    std::string szMode;
    szApplicationName = remove_extension(base_name(std::string(argv[0])));

    while ( 1 ) {

        c = getopt_long ( argc, argv, cOptions, strOption, &option_idx );

        if ( c == -1 ) {
            break;
        }

        switch ( c ) {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (strOption[option_idx].flag != 0)
                    break;
                std::cout << "option " << strOption[option_idx].name;
                if (optarg) {
                    std::cout << " with args " << optarg;
                }
                std::cout << std::endl;
                break;
            case 'i':
                szInputFileName = std::string ( optarg );
                break;
            case 'o':
                szOutputFileName = std::string(optarg);
                break;
            default:
                usage();
                return -1;
        }
    }

    if (szInputFileName.empty())
        return -1;

    if (szOutputFileName.empty()) {
        szOutputFileName = remove_extension(szInputFileName) + ".mat";
    }

    return 0;
}

/**
 * Usage information on prompt.
 *
 * @author Daniel Kotschate
 * @date 15/12/2015
 */

void ConArgs::usage ( void ) {
    std::cout << std::endl;
    std::cout << szApplicationName << std::endl;
    std::cout << "Copyright © Daniel Kotschate (dkotscha)" << std::endl;
    std::cout << std::endl;
    std::cout << "Version info:" << std::endl;
    std::cout << "  last commit: " << VersionInfo::GIT_DATE << std::endl;
    std::cout << "  commit rev: " << VersionInfo::GIT_SHA1 << std::endl;
    std::cout << "  commig msg: " << VersionInfo::GIT_COMMIT_SUBJECT << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "Usage: "<< szApplicationName << " [-cil]" << std::endl << std::endl;
    std::cout << "  -i (--infile)         input filename (HGY format)" << std::endl;
    std::cout << "  -c (--config)         output filename (MATLAB format)" << std::endl;
    std::cout << "                        time, frequency, vrms" << std::endl;
    std::cout << std::endl;
    //std::cout << "  -l (--logfile)        logfile" << std::endl;
    //std::cout << std::endl;
    std::cout << "     --verbose          verbose output information" << std::endl;
    std::cout << std::endl;
}
