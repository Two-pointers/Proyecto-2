#pragma once
#include "graph.hpp"
#include <regex>

struct REPL
{
  FileSystem* current;

  string oneArgumentBuilder(string name){
    return "\\s*" + name + "\\s*\\(\\s*[^\\)]+\\s*\\)\\s*";
  }

  
  regex firstArgumentRegex{"\\(\\s*[^\\)]+"};
  regex secondArgumentRegex{",\\s*[^\\)]"};
  smatch m;

  bool crear_dir(string line){
    regex crearDirRegex{oneArgumentBuilder("crear_dir")};
    if (!regex_match(line,crearDirRegex)) return false;

    regex_search(line,m,firstArgumentRegex);
    
    string match = m.str();
    match.erase(match.begin());
    variant<Error, std::monostate> merror = current->crear_dir(match);

    if (holds_alternative<Error>(merror))
      cout << get<Error>(merror).description << endl;

    return true;

  }

  bool crear_archivo(string line){
    regex crearDirRegex{oneArgumentBuilder("crear_archivo")};
    if (!regex_match(line,crearDirRegex)) return false;

    regex_search(line,m,firstArgumentRegex);
    
    string match = m.str();
    match.erase(match.begin());
    variant<Error, std::monostate> merror = current->crear_archivo(match);

    if (holds_alternative<Error>(merror))
      cout << get<Error>(merror).description << endl;

    return true;
  }

  bool eliminar(string line){
    regex crearDirRegex{oneArgumentBuilder("eliminar")};
    if (!regex_match(line,crearDirRegex)) return false;

    regex_search(line,m,firstArgumentRegex);
    
    string match = m.str();
    match.erase(match.begin());
    variant<Error, std::monostate> merror = current->eliminar(match);

    if (holds_alternative<Error>(merror))
      cout << get<Error>(merror).description << endl;

    return true;
  }

  bool leer(string line){
    regex crearDirRegex{oneArgumentBuilder("leer")};
    if (!regex_match(line,crearDirRegex)) return false;

    regex_search(line,m,firstArgumentRegex);
    
    string match = m.str();
    match.erase(match.begin());
    variant<Error, std::string> merror = current->leer(match);

    if (holds_alternative<Error>(merror)){
      cout << get<Error>(merror).description << endl;
    } else {
      cout << get<string>(merror) << endl;
    }
    return true;
  }

  bool escribir(string line){
    regex crearDirRegex{oneArgumentBuilder("leer")};
    if (!regex_match(line,crearDirRegex)) return false;

    regex_search(line,m,firstArgumentRegex);
    
    string match = m.str();
    match.erase(match.begin());
    variant<Error, std::string> merror = current->leer(match);

    if (holds_alternative<Error>(merror)){
      cout << get<Error>(merror).description << endl;
    } else {
      cout << get<string>(merror) << endl;
    }
    return true;
  }

  void test(){
    crear_dir("crear_dir(abc)");
    
  }
};
