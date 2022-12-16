#include <iostream>
#include "versionManager/versionManager.hpp"
#include "repl/graph.hpp"
#include "repl/repl.hpp"
#include <fstream>
#include <vector>
using namespace std;

enum class TestMode { TREE, ROOTCELV, NONROOTCELV };

int main(){	
	REPL().runREPL(cin,false);
	return 0;
}