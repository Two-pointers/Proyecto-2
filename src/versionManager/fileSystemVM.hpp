#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <math.h>
#include <string>
#include <map>

using namespace std;

typedef long long ll;

#define FOR(i,n,m) for(int i = n; i < (int)m; i++)

struct tupleFile{
	string name;
	int version;
	string content;
	bool operator < (const tupleFile &other) const { return name < other.name; }
};

struct fileSystemVM{
	set<string> folders;
	map<string,tupleFile> files;
};

string fileSystemToString( fileSystemVM fs );