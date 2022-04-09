#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>

namespace tools {

    using namespace std;

    char escchr(char ogchar);
    const char* unescchr(char escchar);

    // string ops
    std::vector<string> split_string(const string& str, const string& delimiter);
    string fstr(string format, ...);
    string replacestr(string source, string oldstring, string newstring);
    string escstr(string str);
    string unescstr(string str, bool ign_s_quotes = false, bool ign_d_quotes = false);

    void ltrim(string &s);
    void rtrim(string &s);
    void trim(string &s);
    string ltrim_copy(string s);
    string rtrim_copy(string s);
    string trim_copy(string s);

    string readf(string path);
    void writef(string path, string text);
}
#endif