#if !defined(COMPILING) && __INCLUDE_LEVEL__ == 1
#	error "including this file is not allowed."
#endif

#ifndef EVIR__TOOLS_PCH_H
#define EVIR__TOOLS_PCH_H

#include <string>
#include <vector>

#ifndef COMPILING
#   define ATTR [[deprecated]]
#else 
#   define ATTR 
#endif

namespace evir {
namespace tools {

    using namespace std;

    ATTR char escchr(char ogchar);
    ATTR const char* unescchr(char escchar);

    // string ops
    ATTR std::vector<string> split_string(const string& str, const string& delimiter);
    ATTR string fstr(string format, ...);
    ATTR string replacestr(string source, string oldstring, string newstring);
    ATTR string escstr(string str);
    ATTR string unescstr(string str, bool ign_s_quotes = false, bool ign_d_quotes = false);

    ATTR void ltrim(string &s, char delim = ' ');
    ATTR void rtrim(string &s, char delim = ' ');
    ATTR void trim(string &s, char delim = ' ');
    ATTR string ltrimc(string s, char delim = ' ');
    ATTR string rtrimc(string s, char delim = ' ');
    ATTR string trimc(string s, char delim = ' ');

    ATTR string readf(string path);
    ATTR void writef(string path, string text);

    ATTR bool is_ident(string str);
}
}
#endif