#include "versionManager.hpp"

versionManager createVM(){
	versionManager vm;
	vm.versions = 0;
	vm.currentVersion = 0;
	vm.history = { {} };
	vm.ancestor = { {0} };
	vm.created = {};
	vm.depth = { 0 };
	vm.content = {};
	return vm;
}

int genericOperation( operationType type, operationObj obj, string path, versionManager &vm, string content="" ){	
	int prevCurrentVersion = vm.currentVersion;
	vm.depth.push_back( vm.depth[prevCurrentVersion]+1 );
	vm.history.push_back({vm.currentVersion,vm.versions+1,type,obj,path,content});
	vm.currentVersion = ++vm.versions;

	int ancestor = prevCurrentVersion, pow2 = 0;
	vector<int> cuAncestors = { ancestor };
	while( vm.ancestor[ancestor].size() >= pow2+1 ){		
		cuAncestors.push_back( vm.ancestor[ancestor][pow2] );
		ancestor = vm.ancestor[ancestor][pow2];
		pow2++;
	}
	// cout <<"Ancestros de "<< vm.currentVersion << " ( padre "<< prevCurrentVersion << " ) ( depth "<< vm.depth.back() << " ) " << endl;
	// for(int e: cuAncestors) cout << e << " ";
	// cout << endl;
	vm.ancestor.push_back(cuAncestors);
	return 0;
}

// return 1 if the file/folder already exist
int createInVM( operationObj obj, string path, versionManager &vm ){	
	if(obj==folder) path+="/";
	if( vm.created.count(path) ) return 1;
	vm.created.insert(path);
	return genericOperation(crea,obj,path,vm);
}

// return 1 if the file/folder already don't exist
int deleteInVM( operationObj obj, string path, versionManager &vm ){
	if(obj==folder) path+="/";
	if( !vm.created.count(path) ) return 1;
	vm.content.erase(path);
	vm.created.erase( vm.created.find(path) );
	return genericOperation(dele,obj,path,vm);
}

// return 1 if the file don't exist
int editInVM( string path, string content, versionManager &vm ){
	if( !vm.created.count(path) ) return 1;
	vm.content[path] = content;
	return genericOperation(edit,file,path,vm,content);
}

bool validVersion(versionManager &vm, int v){
	return vm.versions >= v;
}

// return {{},{}} if the version don't exist
fileSystemVM changeVersion( versionManager &vm, int newVersion ){
	if( !validVersion(vm,newVersion) ) return {{},{}};
	vm.currentVersion = newVersion;
	
	// update the files in the new version
	vm.content.clear();
	vm.created.clear();

	fileSystemVM fs = getFilesVM(vm,newVersion);
	for(auto e: fs.files){
		vm.content[e.second.name] = e.second.content;
		vm.created.insert(e.second.name);
	}

	for(auto e: fs.folders){
		vm.created.insert(e);
	}

	return fs;
}

int getParent( versionManager &vm, int version, int pow2 ){
	if( pow2 >= vm.ancestor[version].size() ) return 0;
	return vm.ancestor[version][pow2];
}

// return -1 if the versions aren't valid
int lcaVersions( versionManager vm, int u, int v) {
	if( !validVersion(vm,u) ) return -1;
	if( !validVersion(vm,v) ) return -1;

	if(vm.depth[v] < vm.depth[u]) swap(u, v);
	int diff = vm.depth[v] - vm.depth[u];

	int level = logMAXN;
	for(int i=0; i<level; i++)
		if((diff>>i)&1)
			v = getParent(vm,v,i);		

	if(u == v) return u;

	for(int i=level-1; i>=0; i--)
		if( getParent(vm,u,i) != getParent(vm,v,i)){
			u = getParent(vm,u,i);
			v = getParent(vm,v,i);
		}

	return getParent(vm,u,0);
}

// return {{},{}}; if the versions isn't valid
fileSystemVM getFilesVM( versionManager vm, int version  ){
	if( !validVersion(vm,version) ) return {{},{}};
	
	vector<operation> operationsInVersion;
	int current = version;
	while( current ){
		operation op = vm.history[current]; 
		// we will use this variable to save the version of the file
		op.parent_version = current;
		operationsInVersion.push_back( op );
		current = vm.ancestor[current][0];
	}
	reverse( operationsInVersion.begin(), operationsInVersion.end() );

	fileSystemVM fs = { {}, {} };
	for(auto op: operationsInVersion){
		if( op.opObj==folder ){
			if( op.opType==crea ){
				fs.folders.insert(op.path);
			}else{
				fs.folders.erase(fs.folders.find(op.path));
			}
		}else{ 
			if( op.opType==crea || op.opType==edit ){
				fs.files[op.path] = {op.path,op.parent_version,op.content};
			}else if( op.opType==dele ){
				// should exist
				fs.files.erase(op.path);
			}
		}
	}
	return fs;
}

// return -1 if the versions aren't valid
variant<Error,monostate> mergeVersions( versionManager &vm, int v1, int v2 ){
	if( !validVersion(vm,v1) ) return Error("Version: '" + to_string(v1) + "' no valida");
	if( !validVersion(vm,v2) ) return Error("Version: '" + to_string(v2) + "' no valida");

	variant<Error,monostate> merror = monostate{};

	// the version 1 will be our base version
	// where we will add/edit new files
	fileSystemVM fsv1 = getFilesVM(vm,v1);
	fileSystemVM fsv2 = getFilesVM(vm,v2);

	// alias to the tuples
	map<string,tupleFile> &tuplesV1 = fsv1.files;	
	map<string,tupleFile> &tuplesV2 = fsv2.files;	

	// change to the LCA version
	int lca = lcaVersions(vm,v1,v2);
	changeVersion(vm,lca);

	// for each file in v1, compare it to v2
	for(auto fv1 : fsv1.files){
		string path = fv1.first;
		tupleFile tfv1 = fv1.second;

		if( !tuplesV2.count(path) ){
			// cout << "esta en v1 "<< path << ", PERO no esta en v2\n";
			// file in v1 but not in v2
			createInVM(file,path,vm);
			editInVM(path,tfv1.content,vm);
		}else{
			// the file is in both versions
			tupleFile tfv2 = tuplesV2[path];
			int lcaConflic = lcaVersions(vm,tfv1.version,tfv2.version);

			createInVM(file,path,vm);
			if( lcaConflic==lca ){
				// both version have modify the same file
				// both can be the same
				variant<Error,string> mconflictFusion = myers(tfv1.content,tfv2.content);
				if (holds_alternative<Error>(mconflictFusion)){
					merror = Error("Conflicto fusionando V-" + to_string(v1) 
					+ " y V-" + to_string(v2) + "\n" + 
					"archivos: '" + tfv1.name + "' y '" + tfv2.name + "'\n" +
					"diff log\n" + get<Error>(mconflictFusion).description);
				}
				//editInVM(path,conflictFusion,vm);
				

			}else{
				// this file was edit in 1 version
				if( tfv1.version>tfv2.version ){
					editInVM(path,tfv1.content,vm);
				}else{
					editInVM(path,tfv2.content,vm);
				}
			}
		}
	}

	// for each file in v1, compare it to v2
	for(auto fv2 : fsv2.files){
		string path = fv2.first;
		tupleFile tfv2 = fv2.second;

		if( !tuplesV1.count(path) ){
			// cout << "esta en v2 "<< path << ", PERO no esta en v1\n";
			// file in v2 but not in v1
			createInVM(file,path,vm);
			editInVM(path,tfv2.content,vm);
		}else{
			//cout << "El archivo "<< path << " esta en ambos\n";
			// this is intentionally empty
		}
	}

	return merror;
}

string operation::toString(){
	string result;
	result += "Version: " + to_string(parent_version) + "\n";
	result += "Operacion realizada: ";
	switch (opType)
	{
	case operationType::crea:
		result += "\u001b[92mCREAR ";
		break;
	
	case operationType::dele:
		result += "\u001b[91mELIMINAR ";
		break;
	
	default:
		result += "\u001b[95mEDITAR ";
		break;
	}

	switch (opObj)
	{
	case operationObj::file:
		result += "ARCHIVO";
		break;
	
	default:
		result += "DIRECTORIO";
		break;
	}

	result += "\u001b[0m\n";
	result += "Argumentos: " + path + ", " + content + "\n";
	result += "Version anterior: " + to_string(parent_version) + "\n";

	return result;


}