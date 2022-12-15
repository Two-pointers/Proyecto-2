#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <math.h>
#include <algorithm>
#include <map>
#include "fileSystemVM.hpp"
#include <variant>
#include "../utils/types/error.hpp"
#include "../diff/diff.hpp"
using namespace std;

typedef long long ll;

#define FOR(i,n,m) for(int i = n; i < (int)m; i++)

const int MAXN = 1e6;
const int logMAXN = 18;

enum operationType { crea, dele, edit };
enum operationObj { file, folder };

struct operation{
	int parent_version;
	operationType opType;
	operationObj opObj;
	string path;
	string content;

	string toString();
};

struct versionManager{
	int versions;
	int currentVersion;
  vector<operation> history;	
	// lca
	vector<vector<int>> ancestor;
	vector<int> depth;
	// for file managment
	set<string> created;
	map<string,string> content;
};

// to get the history vm.history
// to get the content of a file vm.content[path]
// the last created version vm.versions
versionManager createVM();
int createInVM( operationObj obj, string path, versionManager &vm );
int deleteInVM( operationObj obj, string path, versionManager &vm );
int editInVM( string path, string content, versionManager &vm );
fileSystemVM changeVersion( versionManager &vm, int newVersion );
int lcaVersions( versionManager vm, int v1, int v2 );
fileSystemVM getFilesVM( versionManager vm, int version  );
variant<Error,monostate> mergeVersions( versionManager &vm, int v1, int v2 );