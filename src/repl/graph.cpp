#include "graph.hpp"

variant<Error,monostate> FileSystem::crear_dir(string nombre){
  
  variant<Error,monostate> v = _crear_dir(nombre);
  bool b = canStartVM();
  if (b || holds_alternative<Error>(v)) return v;
  auto p = getLowestAncesterWithVM();
  if (holds_alternative<Error>(p)) return get<Error>(p);
    
  auto [fs,path] = get<pair<FileSystem*,vector<string>>>(p);
  string pathstr = concatPaths(path) + nombre;
  createInVM(operationObj::folder,pathstr,fs->vm.value());
  
  return monostate{};
}


variant<Error,monostate> FileSystem::crear_archivo(string nombre){
  variant<Error,monostate> v = _crear_archivo(nombre);
  if (canStartVM() || holds_alternative<Error>(v)) return v;
  auto p = getLowestAncesterWithVM();
  if (holds_alternative<Error>(p)) return get<Error>(p);

  auto [fs,path] = get<pair<FileSystem*,vector<string>>>(p);

  createInVM(operationObj::file,concatPaths(path) + nombre,fs->vm.value());

  return monostate{};
}

variant<Error,monostate> FileSystem::eliminar(string nombre){
  variant<Error,monostate> v = _eliminar(nombre);
  if (canStartVM() || holds_alternative<Error>(v)) return v;
  auto p = getLowestAncesterWithVM();
  if (holds_alternative<Error>(p)) return get<Error>(p);

  auto [fs,path] = get<pair<FileSystem*,vector<string>>>(p);

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
  auto p = getLowestAncesterWithVM();
  if (holds_alternative<Error>(p)) return get<Error>(p);

  auto [fs,path] = get<pair<FileSystem*,vector<string>>>(p);

  editInVM(concatPaths(path) + nombre,contenido,fs->vm.value());

  return monostate{};
}

variant<Error,FileSystem*> FileSystem::ir(string nombre){
  return _ir(nombre);
}



string readFile(filesystem::path path)
{
    // Open the stream to 'lock' the file.
    ifstream f(path, ios::in | ios::binary);

    // Obtain the size of the file.
    const auto sz = filesystem::file_size(path);

    // Create a buffer.
    string result(sz, '\0');

    // Read the whole file into the buffer.
    f.read(result.data(), sz);

    return result;
}

void FileSystem::celv_importar(string dir, string acc){
  //filesystem::current_path(filesystem::path{dir});
  if (acc == "") acc = concatPaths(path);
  for (auto const& dir_entry : filesystem::directory_iterator(dir))
    {
      string name = dir_entry.path().filename();
      if (filesystem::is_regular_file(dir_entry)){
        succs[name] = readFile(dir_entry.path());
        continue;
      }

      string nextFolder = acc + name + "/";
      FileSystem* fs = new FileSystem(nullopt,this,nextFolder);
      fs->celv_importar(dir + name + "/",nextFolder);
      succs[name] = fs;
    }
}