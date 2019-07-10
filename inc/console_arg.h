#ifndef __CONSOLE_ARGS_H__
#define __CONSOLE_ARGS_H__

#include <string>
#include <getopt.h>
#include <hglib.h>

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>


class ConArgs {
    public:

        ConArgs(void)
            : verbose_flag ( 0 )
            , export_flag(0)
            , pFile ( nullptr )
            , bReady ( false )
        {

        }

        std::string szInputFileName;
        std::string szOutputFileName;
        std::string szApplicationName;
        std::mutex              mutex;
        std::condition_variable condi;
        bool       bReady;
        int verbose_flag;
        int export_flag;
        HGFileInfo* pFile;

        /**
         * Creates the basename of an passed path.
         *
         * @author Daniel Kotschate
         * @date 1/18/2016
         *
         * @tparam T	Generic type parameter.
         * @param path	Full (absolute) path of the file.
         *
         * @return A T.
         */

        template<class T>
        static T base_name(T const & path, T const & delims = "/\\")
        {
            return path.substr(path.find_last_of(delims) + 1);
        }

        /**
         * Path name.
         *
         * @author Daniel Kotschate
         * @date 1/18/2016
         *
         * @tparam T Generic type parameter.
         * @param path Full pathname of the file.
         *
         * @return A T.
         */

        template<class T>
        static T path_name(T const & path, T const & delims = "/\\")
        {
            if (path.find_last_of(delims) != T::npos)
                return path.substr(0, path.find_last_of(delims));
            else
                return T("/");
        }

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


        void usage(void);
        int parse_arguments_long( int argc, char* argv[], const char* cOptions, const struct option* strOption );

};

#endif // __CONSOLE_ARGS_H__
