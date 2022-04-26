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

    void ltrim(string &s, char delim = ' ');
    void rtrim(string &s, char delim = ' ');
    void trim(string &s, char delim = ' ');
    string ltrimc(string s, char delim = ' ');
    string rtrimc(string s, char delim = ' ');
    string trimc(string s, char delim = ' ');

    string readf(string path);
    void writef(string path, string text);

    bool is_ident(string str);
}
#endif