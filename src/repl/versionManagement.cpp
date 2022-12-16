#include "graph.hpp"

vector<string> unconcatPaths(string path){

  vector<string> result;
  char delim = '/';
  string item;

  for(auto c : path)
  {
   if (c == delim){
    result.push_back(item);
    item = "";
    continue;
   } 
   item.push_back(c);
  }

  if (item != "") result.push_back(item);

  return result;

}

// true if node is true
bool FileSystem::isRoot(){
  return !objectExists("..");
}


bool FileSystem::childrenHaveVM(){
  bool b = vm.has_value();
  for(auto [name,child] : succs)
  {
   if (holds_alternative<File>(child) || name == "..") continue;
   b = b || get<FileSystem*>(child)->childrenHaveVM();

  }

  return b;

}


vector<variant<string,pair<File,string>>> FileSystem::getObjects(string path){
  vector<variant<string,pair<File,string>>> res;
  FileSystem* fs;
  for(auto [name,value] : succs)
  {
    if (name == "..") continue;
    if (holds_alternative<File>(value)){
      res.push_back(make_pair(path + name,get<File>(value)));
      continue;
    }
    fs = get<FileSystem*>(value);
    res.push_back(path + name);
    for(auto r : (fs->getObjects(path + name + "/")))
      res.push_back(r);
  }

  return res;
}

variant<Error,monostate> FileSystem::celv_iniciar(){
  if (!canStartVM()) return Error("No se puede inicializar CELV, ya existe uno.");
  vm = createVM();

  for (auto elem: getObjects()){
    if (holds_alternative<string>(elem)){
      createInVM(operationObj::folder,get<string>(elem),vm.value());
    } else{
      auto [name,content] = get<pair<File,string>>(elem);
      createInVM(operationObj::file,name,vm.value());
      editInVM(name,content,vm.value());
    }
  }

  return monostate{};
}

variant<Error,string> FileSystem::celv_historia(){
  variant<Error,pair<FileSystem*,vector<string>>> mfs = getLowestAncesterWithVM();
  if (holds_alternative<Error>(mfs)) return Error("No existe un CELV inicializado, no se puede obtener su historia.");
  FileSystem* fs = get<pair<FileSystem*,vector<string>>>(mfs).first;
  
  vector<operation> operations = fs->vm.value().history;
  string result;
  for(auto op : operations)
   result += op.toString(); 
  
  return result;
  
}

variant<Error,FileSystem*> FileSystem::celv_fusion(int version1, int version2){
  variant<Error,pair<FileSystem*,vector<string>>> mfs = getLowestAncesterWithVM();
  if (holds_alternative<Error>(mfs)) return Error("No existe un CELV inicializado, no se puede obtener su historia.");
  pair<FileSystem*,vector<string>> pfs = get<pair<FileSystem*,vector<string>>>(mfs);
  FileSystem* fs = pfs.first;
  vector<string> path = pfs.second;

  variant<Error,monostate> err = mergeVersions(fs->vm.value(),version1,version2);

  if (holds_alternative<Error>(err)) return get<Error>(err);

  // go down the tree, trying to recover the last
  // dir.
  FileSystem* root = fs;
  variant<Error,FileSystem*> next;
  for(auto elem : path)
  {
    if (elem == "/" || elem == "" || elem == " ") continue;
    next = root->ir(elem);
    if (holds_alternative<Error>(next)) return root;
    root = get<FileSystem*>(next);
  }
  return root;
}

// Given a tree, go up till you find a FS with a VM.
// and return alongside it.
variant<Error,pair<FileSystem*,vector<string>>> FileSystem::getLowestAncesterWithVM(){

  FileSystem* candidate = this;
  vector<string> path;
  pair<FileSystem*,vector<string>> res;

  if (candidate->vm.has_value()) {
      path.insert(path.begin(), "");
      return make_pair(candidate,path);
    }
  while (!candidate->vm.has_value()){
    
    path.insert(path.begin(), candidate->getFolderName());
    auto p = candidate->ir();
    if (holds_alternative<Error>(p)) break;
    candidate = get<FileSystem *>(p);
    if (candidate->vm.has_value()) {
      path.insert(path.begin(), "");
      return make_pair(candidate,path);
    }
  }

  return Error("No existe ningun manejador de versiones activo para: '" + concatPaths(this->path) + "'");

}

// assumes that object only contains the name, not the entire path.
void FileSystem::createObject(vector<string> traversal,variant<string,tupleFile> object){

  if (traversal.empty()){
    if (holds_alternative<string>(object)){
      _crear_dir(get<string>(object));
    } else{
      tupleFile f = get<tupleFile>(object);
      _crear_archivo(f.name);
      get<monostate>(_escribir(f.name,f.content));
    }
    return;
  }

  string next = traversal[0];
  if (!objectExists(next)){
    _crear_dir(next);
  }

  traversal.erase(traversal.begin());
  get<FileSystem*>(ir(next))->createObject(traversal,object);
  return ;
}

// flush everything, and rebuild the tree from the lowest node that has a VM.
// return the closest ancestor from the current node.
variant<Error,FileSystem*> FileSystem::celv_vamos(int version){

  // hold the current path, we shall need it in order to return the
  // closest ancestor.
  vector<string> currentPath;

  // try to get the lowest node with a VM.
  variant<Error,pair<FileSystem*,vector<string>>> mfs = getLowestAncesterWithVM();
  FileSystem* fs;

  // if there are none, return an error
  if (holds_alternative<Error>(mfs)){
    return get<Error>(mfs);
  } 
  // else, get the file system and the path.
  auto [_fs,_currentPath] = get<pair<FileSystem*,vector<string>>>(mfs);
  fs = _fs;
  currentPath = _currentPath;
  

  // before clearing the content of the node
  // that has the VM we must preserve its parent
  // folder
  FileSystem* parent = get<FileSystem*>(fs->succs[".."]);
  // clear the node so we can seconstruct
  fs->succs= {};
  // preserve the parent
  fs->succs[".."] = parent;

  // take the vm from the filesystem
  versionManager workingVM = fs->vm.value();
  // execute the changeversion and get the new (virtual) FS.
  fileSystemVM newFS = changeVersion(workingVM,version);
  fs->vm = workingVM;

  // check for error in the versions
  if (newFS.folders.empty() && newFS.files.empty()) return Error("Version: '" + to_string(version) + "' does not exists");

  vector<string> paths;
  string folder;
  string file;

  
  // for each folder in the VM
  for(auto folder : newFS.folders)
  {
    //get the paths and delete the "root"
    paths = unconcatPaths(folder);
    if (paths[0] == "/" || paths[0] == "")
      paths.erase(paths.begin());
    // get the folder that needs to be created and pop it
    folder = paths[paths.size() - 1];
    paths.pop_back();
    // create the object!
    fs->createObject(paths,folder);    
  }
  
  for(auto [path,tf] : newFS.files)
  {
    
    // get the paths and delete the "root"
    paths = unconcatPaths(path);
    if (paths[0] == "/" || paths[0] == "")
      paths.erase(paths.begin());

    // get the file that needs to be created and pop it
    tf.name = paths[paths.size() - 1];
    paths.pop_back();
    // create the object!
    fs->createObject(paths,tf);
  }

  
  // go down the tree, trying to recover the last
  // dir.
  FileSystem* root = fs;
  variant<Error,FileSystem*> next;
  
  for(auto elem : currentPath)
  {
    if (elem == "/" || elem == "" || elem == " ") continue;
    next = root->ir(elem);
    if (holds_alternative<Error>(next)) return root;
    root = get<FileSystem*>(next);
  }
  return root;
}