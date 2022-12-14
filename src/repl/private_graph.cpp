#include "graph.hpp"


string Error::toString(){
  string errString = "[ERROR] " + this->description;
  return colorRED(errString);
}

FileSystem generateRoot(){
  FileSystem fs;
  return fs; 
}

bool FileSystem::objectExists(string name){
  return succs.count(name) > 0;
}

string FileSystem::typeOf(variant<FileSystem*,File> t){
    if (holds_alternative<File>(t)) return "archivo";
    //if (t.index() == 1) return "archivo";
    return "directorio";
}


variant<Error,monostate> FileSystem::_crear_dir(string nombre){
  if (objectExists(nombre)) return Error("El " + typeOf(succs[nombre]) + ": '" + nombre + "' ya existe");
  if (nombre=="..") return Error("Nombre: '..' reservado para directorios padres");
  FileSystem* s = new FileSystem{nullopt,this,nombre};
  succs[nombre] = s;
  return monostate{};
}


variant<Error,monostate> FileSystem::_crear_archivo(string nombre){
  if (objectExists(nombre)) return Error("El " + typeOf(succs[nombre]) + ": '" + nombre + "' ya existe");
  if (nombre=="..") return Error("Nombre: '..' reservado para directorios padres");
  succs[nombre] = "";
  return monostate{};
}

variant<Error,monostate> FileSystem::_eliminar(string nombre){
  if (!objectExists(nombre)) return Error("'" + nombre + "' NO existe");
  if (nombre=="..") return Error("NO se puede eliminar '..'");

  succs.erase(nombre);
  return monostate{};
}

variant<Error,string> FileSystem::_leer(string nombre){
  if (!objectExists(nombre)) return Error("'" + nombre + "' NO existe");
  if (typeOf(succs[nombre]) != "archivo") return Error("El objeto: '" + nombre + "' es un directorio");

  return get<string>(succs[nombre]);
}

variant<Error,monostate> FileSystem::_escribir(string nombre, string contenido){
  if (!objectExists(nombre)) return Error("'" + nombre + "' NO existe");
  if (typeOf(succs[nombre]) != "archivo") return Error("El objeto: '" + nombre + "' es un directorio");

  
  succs[nombre] = contenido;
  return monostate{};
}

variant<Error,FileSystem*> FileSystem::_ir(string nombre){
  if (!objectExists(nombre)) return Error("'" + nombre + "' NO existe"); 
  if (typeOf(succs[nombre]) != "directorio") return Error("El objeto: '" + nombre + "' es un archivo");
  return get<FileSystem*>(succs[nombre]);
}

string tabulate(int tabs, string s){
  string acc;

  for (int i = 0; i < tabs; i++)
    acc.push_back('\t');
  
  acc += s;

  return acc;
}

string concatPaths(vector<string> paths){
  string acc;

  for(auto path : paths)
    acc +=  path + "/"; 
  
  return acc;

}

string FileSystem::toString(bool withContent, bool showDotDot, int tabs){
  string acc;

  if (tabs == 1) acc += "Directorio 'Raiz' absoluto: " + concatPaths(path) + "\n";

  for(auto [name,value] : succs)
  {
    if  (typeOf(value) == "archivo")
    {
      acc += tabulate(tabs,"Archivo: " + name + "\n");
      acc += withContent ? tabulate(tabs,"Contenido: '" + get<string>(value) + "'\n") : "";
    } else{
      if (name == ".."){
        if (showDotDot) acc += tabulate(tabs,"Directorio: " + name + "\n");
      } else
      {
        acc += tabulate(tabs,"Directorio: " + name + "\n");
        acc += get<FileSystem*>(value)->toString(withContent,showDotDot,tabs+1);
      }
      
      
    }
  }

  return acc;

}