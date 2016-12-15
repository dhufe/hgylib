#ifndef __VERSION_APPLICATION_H__
#define __VERSION_APPLICATION_H__

#include <string>

struct VersionInfo {
    static const std::string GIT_SHA1;
    static const std::string GIT_DATE;
    static const std::string GIT_COMMIT_SUBJECT;
};

#endif 