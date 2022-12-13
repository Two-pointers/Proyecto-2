#pragma once 
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <optional>
#include <variant>
#include "../versionManager/versionManager.hpp"
#include "../utils/coloring/coloring.hpp"

using namespace std;

using File = string;

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
  
  public:

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

    string toString(bool withContent = false, bool showDotDot = false, int tabs = 1);

};
