#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <math.h>

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
	string content;
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
};

versionManager createVM();
int createInVM( operationObj obj, string path, versionManager &vm );
int deleteInVM( operationObj obj, string path, versionManager &vm );
int editInVM( operationObj obj, string path, string content, versionManager &vm );
int changeVersion( versionManager &vm, int newVersion );
int lcaVersions( versionManager vm, int v1, int v2 );