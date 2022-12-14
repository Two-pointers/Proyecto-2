#pragma once 
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <optional>
#include <variant>
#include <iostream>
#include <sstream>
#include "../versionManager/versionManager.hpp"
#include "../versionManager/fileSystemVM.hpp"
#include "../utils/coloring/coloring.hpp"

using namespace std;

using File = string;

string concatPaths(vector<string> paths);

struct Error{
  string description;

  Error()
    : description ("")
    {};
  
  Error (string description)
    : description (description)
    {};
  
  string toString();
};

class FileSystem
{

  private:
    variant<Error,monostate> _crear_dir(string nombre);

    variant<Error,monostate> _crear_archivo(string nombre);

    variant<Error,monostate> _eliminar(string nombre);

    variant<Error,string> _leer(string nombre);

    variant<Error,monostate> _escribir(string nombre, string contenido);

    variant<Error,FileSystem*> _ir(string nombre);

    variant<Error,FileSystem*> _ir(){
      return _ir("..");
    };

    FileSystem* createObject(vector<string> traversal,variant<string,tupleFile> object);
  
    bool childrenHaveVM();

    bool canStartVM(){
      return !childrenHaveVM() && holds_alternative<Error>(getLowestAncesterWithVM());
    };

    variant<Error,pair<FileSystem*,vector<string>>> getLowestAncesterWithVM();
  public:

    // should be something like: ["","dir","subdir"]
    // meaning /dir/subdir
    vector<string> path;

    optional<versionManager> vm;

    map<string,variant<FileSystem*,File>> succs;

    FileSystem() 
      : vm (nullopt)
      , succs ({})
      {
        path.push_back("");
      };

    FileSystem(optional<versionManager> vm, FileSystem* parent, string nextPath)
      : vm (vm)
      {
        succs[".."] = parent;
        path = parent->path;
        path.push_back(nextPath);
      };
    

    string getFolderName(){
      return path[path.size()-1];
    }

    string typeOf(variant<FileSystem*,File> t);

    bool objectExists(string name);

    variant<Error,monostate> crear_dir(string nombre);

    variant<Error,monostate> crear_archivo(string nombre);

    variant<Error,monostate> eliminar(string nombre);

    variant<Error,string> leer(string nombre);

    variant<Error,monostate> escribir(string nombre, string contenido);

    variant<Error,FileSystem* > ir(string nombre);

    variant<Error,FileSystem* > ir(){
      return ir("..");
    };

    variant<Error,monostate> celv_iniciar();
    variant<Error,string> celv_historia();

    string toString(bool withContent = false, bool showDotDot = false, int tabs = 1);

    variant<Error,FileSystem*> celv_vamos(int version);

    variant<Error,FileSystem*> celv_fusion(int version1, int version2);

    bool isRoot();



    

};
