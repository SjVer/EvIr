#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include <vector>

using namespace std;

namespace tools {

    // string ops
    std::vector<string> split_string(const string& str, const string& delimiter);
    string fstr(string format, ...);
    string replacestr(string source, string oldstring, string newstring);
    char escchr(char ogchar);
    const char* unescchr(char escchar);
    string escstr(string str);
    string unescstr(string str, bool ign_s_quotes = false, bool ign_d_quotes = false);

    void ltrim(std::string &s);
    void rtrim(std::string &s);
    void trim(std::string &s);
    std::string ltrim_copy(std::string s);
    std::string rtrim_copy(std::string s);
    std::string trim_copy(std::string s);

    string readf(string path);
    void writef(string path, string text);
    
}
#endif