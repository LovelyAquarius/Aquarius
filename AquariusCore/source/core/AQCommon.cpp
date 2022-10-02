#include "AQPCH.h"
#include "AQCommon.h"

namespace Aquarius
{
    std::string AQ_ExtractFilename(const std::string& filepath)
    {
        //assets/filename.file,我们要抓取filename这个字符串
        auto lastslash = filepath.find_last_of("/\\");
        lastslash = lastslash == std::string::npos ? 0 : lastslash + 1;
        auto lastdot = filepath.rfind('.');
        auto count = lastdot == std::string::npos ? filepath.size() - lastslash : lastdot - lastslash;

        return filepath.substr(lastslash, count);
    }

}
