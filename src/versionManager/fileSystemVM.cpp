#include "fileSystemVM.hpp"

string fileSystemToString( fileSystemVM fs ){
    string ans = "";
    ans += "=== Carpetas ===\n";
    for(auto e: fs.folders) ans += e + " ";
    ans += "\n=== Archivos ===\n";
    for(auto e: fs.files){        
        ans += "< " + e.second.name;
        ans += " , ";
        ans += to_string(e.second.version) + " , ";
        ans += e.second.content + " >\n";
    }
    return ans;
}