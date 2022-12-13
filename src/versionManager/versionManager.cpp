#include "versionManager.hpp"

versionManager createVM(){
	versionManager vm;
	vm.versions = 0;
	vm.currentVersion = 0;
	vm.history = {};
	vm.ancestor = { {0} };
	vm.created = {};
	vm.depth = { 0 };
	return vm;
}

int genericOperation( operationType type, operationObj obj, string path, versionManager &vm, string content="" ){	
	int prevCurrentVersion = vm.currentVersion;
	vm.depth.push_back( vm.depth[prevCurrentVersion]+1 );
	vm.currentVersion = ++vm.versions;
	vm.history.push_back({vm.currentVersion,type,obj,content});

	int ancestor = prevCurrentVersion, pow2 = 0;
	vector<int> cuAncestors = { ancestor };
	while( vm.ancestor[ancestor].size() >= pow2+1 ){		
		cuAncestors.push_back( vm.ancestor[ancestor][pow2] );
		ancestor = vm.ancestor[ancestor][pow2];
		pow2++;
	}
	cout <<"Ancestros de "<< vm.currentVersion << " ( padre "<< prevCurrentVersion << " ) ( depth "<< vm.depth.back() << " ) " << endl;
	for(int e: cuAncestors) cout << e << " ";
	cout << endl;
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
	vm.created.erase( vm.created.find(path) );
	return genericOperation(dele,obj,path,vm);
}

// return 1 if the file don't exist
int editInVM( operationObj obj, string path, string content, versionManager &vm ){
	if( !vm.created.count(path) ) return 1;
	return genericOperation(edit,obj,path,vm,content);
}

bool validVersion(versionManager &vm, int v){
	return vm.versions >= v;
}

// return 1 if the version don't exist
int changeVersion( versionManager &vm, int newVersion ){
	if( !validVersion(vm,newVersion) ) return 1;
	vm.currentVersion = newVersion;
	return 0;
}

int getParent( versionManager &vm, int version, int pow2 ){
	if( pow2+1 >= vm.ancestor[version].size() ) return 0;
	return vm.ancestor[version][pow2];
}

// return -1 if the versions aren't valid
int lcaVersions( versionManager vm, int u, int v) {
	if( !validVersion(vm,u) ) return -1;
	if( !validVersion(vm,v) ) return -1;

	if (vm.depth[v] < vm.depth[u]) swap(u, v);
	int diff = vm.depth[v] - vm.depth[u];

	int level = logMAXN;
	for (int i=0; i<level; i++)
		if ((diff>>i)&1) 
			v = getParent(vm,v,i);

	if (u == v) return u;

	for (int i=level-1; i>=0; i--)
		if ( getParent(vm,u,i) != getParent(vm,v,i)){
			u = getParent(vm,u,i);
			v = getParent(vm,v,i);
		}

	return getParent(vm,u,0);
}