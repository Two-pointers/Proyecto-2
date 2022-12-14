#pragma once
#include "graph.hpp"
#include <iostream> 
#include <string> 
#include <regex>

struct REPL
{
  FileSystem* current = new FileSystem();

  string oneArgumentBuilder(string name){
    return "\\s*" + name + "\\s*\\(\\s*[^\\),]+\\s*\\)\\s*";
  }

  string twoArgumentsBuilder(string name){
    return "\\s*" + name + "\\s*\\(\\s*[^\\),]+\\s*,\\s*[^\\),]+\\s*\\)\\s*";
  }

  
  regex firstArgumentRegex{"\\(\\s*[^\\),]+"};
  regex secondArgumentRegex{",\\s*[^\\)]+"};
  smatch m;

  bool crear_dir(string line){
  
    regex crearDirRegex{oneArgumentBuilder("crear_dir")};
    
    if (!regex_match(line,crearDirRegex)) return false;
    
    regex_search(line,m,firstArgumentRegex);
    
    string match = m.str();
    
    match.erase(match.begin());
    variant<Error, std::monostate> merror = current->crear_dir(match);
    
    if (holds_alternative<Error>(merror)){
      cout << get<Error>(merror).toString() << endl;
    }
      
    
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
      cout << get<Error>(merror).toString() << endl;

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
      cout << get<Error>(merror).toString() << endl;

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
      cout << get<Error>(merror).toString() << endl;
    } else {
      cout << get<string>(merror) << endl;
    }
    return true;
  }

  bool escribir(string line){
    smatch m2;
    regex escribirRegex{twoArgumentsBuilder("escribir")};
    if (!regex_match(line,escribirRegex)) return false;

    regex_search(line,m,firstArgumentRegex);
    regex_search(line,m2,secondArgumentRegex);

    string f1 = m.str();
    f1.erase(f1.begin());

    string f2 = m2.str();
    f2.erase(f2.begin());
    variant<Error, monostate> merror = current->escribir(f1,f2);

    if (holds_alternative<Error>(merror))
      cout << get<Error>(merror).toString() << endl;
    
    return true;
  }

  bool ir1(string line){
    regex irRegex{oneArgumentBuilder("ir")};
    if (!regex_match(line,irRegex)) return false;

    regex_search(line,m,firstArgumentRegex);
    
    string match = m.str();
    match.erase(match.begin());
    variant<Error, FileSystem*> merror = current->ir(match);

    if (holds_alternative<Error>(merror)){
      cout << get<Error>(merror).toString() << endl;
    } else{
      current = get<FileSystem*>(merror);
    }
    return true;
  }

  bool ir0(string line){
    regex ir{"\\s*ir\\s*\\(\\s*\\)\\s*"};
    if (!regex_match(line,ir)) return false;

    variant<Error, FileSystem*> merror = current->ir();

    if (holds_alternative<Error>(merror)){
      cout << get<Error>(merror).toString() << endl;
    } else{
      current = get<FileSystem*>(merror);
    }
  }

  bool celv_iniciar(string line){
    regex iniciarRegex{"\\s*celv_iniciar\\s*\\(\\s*\\)\\s*"};
    if (!regex_match(line,iniciarRegex)) return false;

    variant<Error, monostate> merror = current->celv_iniciar();

    if (holds_alternative<Error>(merror)){
      cout << get<Error>(merror).toString() << endl;
    } 
    return true;
  }

  bool celv_historia(string line){
    regex historiaRegex{"\\s*celv_historia\\s*\\(\\s*\\)\\s*"};
    if (!regex_match(line,historiaRegex)) return false;

    variant<Error, string> merror = current->celv_historia();

    if (holds_alternative<Error>(merror)){
      cout << get<Error>(merror).toString() << endl;
    } else {
      cout << get<string>(merror) << endl;
    }
    return true;
  }

  bool celv_vamos(string line){
    regex vamosRegex{oneArgumentBuilder("celv_vamos")};
    if (!regex_match(line,vamosRegex)) return false;

    regex_search(line,m,firstArgumentRegex);
    
    string match = m.str();
    match.erase(match.begin());
    variant<Error, FileSystem*> merror = current->celv_vamos(stoi(match));

    if (holds_alternative<Error>(merror)){
      cout << get<Error>(merror).toString() << endl;
    } else{
      current = get<FileSystem*>(merror);
    }
    return true;
  }

  bool celv_fusion(string line){
    smatch m2;
    regex celv_fusionRegex{twoArgumentsBuilder("celv_fusion")};
    if (!regex_match(line,celv_fusionRegex)) return false;

    regex_search(line,m,firstArgumentRegex);
    regex_search(line,m2,secondArgumentRegex);

    string f1 = m.str();
    f1.erase(f1.begin());

    string f2 = m2.str();
    f2.erase(f2.begin());
    variant<Error, FileSystem*> merror = current->celv_fusion(stoi(f1),stoi(f2));

    if (holds_alternative<Error>(merror)){
      cout << get<Error>(merror).toString() << endl;
    } else{
      current = get<FileSystem*>(merror); 
    }
      
    
    return true;
  }

  bool comment(string line){
    regex comment{"\\s*//"};
    return !regex_match(line,comment);
  }

  bool print(string line){
    regex print{"\\s*print\\s*\\(\\s*\\)\\s*"};
    if (!regex_match(line,print)) return false;

    cout << current->toString() << endl;

    return true;

  }

  bool salir(string line){
    regex salir{"\\s*salir\\s*$"};
    if (!regex_match(line,salir)) return false;

    cout << "GRACIAS POR VERSIONAR CON NOSOTROS." << endl;

    return true;

  }

  bool ayuda(string line){
    regex ayuda{"\\s*ayuda\\s*"};
    if (!regex_match(line,ayuda)) return false;

    cout << "AYUDA:" << endl;
    cout << "-----" <<  endl;
    cout << "crear_dir(arg)         - Crea un directiorio en la locacion actual" << endl;
    cout << "crear_archivo(arg)     - Crea un archivo en la locacion actual" << endl;
    cout << "eliminar(arg)          - eliminar el objeto hijo" << endl;
    cout << "leer(arg)              - imprime el contenido del archivo hijo" << endl;
    cout << "escribir(file,content) - llena el archivo hijo" << endl;
    cout << "ir(file)               - ir al directorio hijo" << endl;
    cout << "ir()                   - ir al directorio padre" << endl;

    return true;

  }

  void runREPL(istream& c, bool showBanner=false){

    if (showBanner){
      cout << "==========================" << endl;
      cout << "| GLORIOUS CELV REPL     |" << endl;
      cout << "==========================" << endl;
    }
    
    
    bool b = false;
    while(true){
      string line;
      cout << "> ";
      std::getline(c, line);
      b = false;
      b = crear_dir(line);
      if (!b) b = crear_archivo(line);
      if (!b) b = eliminar(line);
      if (!b) b = leer(line);
      if (!b) b = escribir(line);
      if (!b) b = ir1(line);
      if (!b) b = ir0(line);
      if (!b) b = print(line);
      if (!b) b = ayuda(line);
      if (!b) b = celv_iniciar(line);
      if (!b) b = celv_historia(line);
      if (!b) b = celv_vamos(line);
      if (!b) b = celv_fusion(line);
      if (!b) b = comment(line);
      if (line == "salir") return;
      if (!b) (cout << "Bad function: " << line << endl);
      
    }
    

  }

  void test(){
    crear_dir("crear_dir(abc)");
    
  }
};
