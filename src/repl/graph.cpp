#include "graph.hpp"

variant<Error,monostate> FileSystem::crear_dir(string nombre){
  if (!vm.has_value()) return _crear_dir(nombre);

  // fill implementation with version manager
  return {};
}


variant<Error,monostate> FileSystem::crear_archivo(string nombre){
  if (!vm.has_value()) return _crear_archivo(nombre);
  // fill implementation with version manager
  return {};
}

variant<Error,monostate> FileSystem::eliminar(string nombre){
  if (!vm.has_value()) return _eliminar(nombre);

  // fill implementation with version manager
  return {};
  return {};
}

variant<Error,string> FileSystem::leer(string nombre){
  if (!vm.has_value()) return _leer(nombre);

  // fill implementation with version manager
  return {};
}

variant<Error,monostate> FileSystem::escribir(string nombre, string contenido){
  if (!vm.has_value()) return _escribir(nombre, contenido);

  // fill implementation with version manager
  return {};
}

variant<Error,FileSystem*> FileSystem::ir(string nombre){
  if (!vm.has_value()) return _ir(nombre);

  // fill implementation with version manager
  return {};
}