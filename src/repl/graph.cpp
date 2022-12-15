#include "graph.hpp"

variant<Error,monostate> FileSystem::crear_dir(string nombre){
  
  variant<Error,monostate> v = _crear_dir(nombre);
  bool b = canStartVM();
  if (b || holds_alternative<Error>(v)) return v;
  auto p = getLowestAncesterWithVM();
  if (holds_alternative<Error>(p)){
    cout << get<Error>(p).toString() << endl;
  }
    
  auto [fs,path] = get<pair<FileSystem*,vector<string>>>(p);
  string pathstr = concatPaths(path) + nombre;
  createInVM(operationObj::folder,pathstr,fs->vm.value());
  
  return monostate{};
}


variant<Error,monostate> FileSystem::crear_archivo(string nombre){
  variant<Error,monostate> v = _crear_archivo(nombre);
  if (canStartVM() || holds_alternative<Error>(v)) return v;

  auto [fs,path] = get<pair<FileSystem*,vector<string>>>(getLowestAncesterWithVM());

  createInVM(operationObj::file,concatPaths(path) + nombre,fs->vm.value());

  return monostate{};
}

variant<Error,monostate> FileSystem::eliminar(string nombre){
  variant<Error,monostate> v = _eliminar(nombre);
  if (canStartVM() || holds_alternative<Error>(v)) return v;

  auto [fs,path] = get<pair<FileSystem*,vector<string>>>(getLowestAncesterWithVM());

  int i = deleteInVM(operationObj::folder,concatPaths(path) + nombre,fs->vm.value());
  if (i == 1)
    deleteInVM(operationObj::file,concatPaths(path) + nombre,fs->vm.value());

  return monostate{};
}

variant<Error,string> FileSystem::leer(string nombre){
  return _leer(nombre);
}

variant<Error,monostate> FileSystem::escribir(string nombre, string contenido){
  variant<Error,monostate> v = _escribir(nombre,contenido);
  if (canStartVM() || holds_alternative<Error>(v)) return v;

  auto [fs,path] = get<pair<FileSystem*,vector<string>>>(getLowestAncesterWithVM());

  editInVM(concatPaths(path) + nombre,contenido,fs->vm.value());

  return monostate{};
}

variant<Error,FileSystem*> FileSystem::ir(string nombre){
  return _ir(nombre);
}