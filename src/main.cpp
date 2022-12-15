#include <iostream>
#include "versionManager/versionManager.hpp"
#include "repl/graph.hpp"
#include "repl/repl.hpp"
#include <fstream>
#include <vector>
using namespace std;

enum class TestMode { TREE, ROOTCELV, NONROOTCELV };

int main(){
	string testFolder = "./testscripts";
	vector<string> treeTests {
		"crear_dir",
		"crear_archivo",
		"llenar_archivo",
		"eliminar_dir",
		"eliminar_archivo",
		"ir",
		"ir_y_volver",
		"dup_dir1",
		"dup_dir2",
		"dup_archivo1",
		"dup_archivo2",
		"cannot_mkdir_dotdot_root",
		"cannot_mkdir_dotdot",
		"cannot_mkfile_dotdot_root",
		"cannot_mkfile_dotdot",
		"cannot_delete_dotdot_root",
		"cannot_delete_dotdot",
		"cannot_delete_inexistent",
		"cannot_read_inexistent",
		"cannot_ir_inexistent",
		"cannot_ir_solo_root",
		"help",
		
	};

	vector<string> rootCELVTests{
		"root_celv_init",
		"root_celv",
		"crear_archivo_celv",
		"eliminar_dir_celv",
		"eliminar_archivo_celv",
		"ir_celv",
		"ir_celv_with_deletion",
		"ir_y_volver_celv",
		"ir_y_volver_celv_with_deletion",
		"celv_jump_backwards",
		"celv_jump_backwards_then_forwards",
		"llenar_archivo_celv",
		"celv_jump_with_edition",
		"celv_deletion",
		"celv_with_existing_tree",
		"celv_conflict",
		"celv_fusion_success",
	};

	vector<string> nonRootCELVTests{
		"root_celv_init_nr",
		"root_celv_nr",
		"crear_archivo_celv_nr",
		"eliminar_dir_celv_nr",
		"eliminar_archivo_celv_nr",
		"ir_celv_nr",
		"ir_celv_with_deletion_nr",
		"ir_y_volver_celv_nr",
		"ir_y_volver_celv_with_deletion_nr",
		"celv_jump_backwards_nr",
		"celv_jump_backwards_then_forwards_nr",
		"llenar_archivo_celv_nr",
		"celv_jump_with_edition_nr",
		"celv_deletion_nr",
		"celv_with_existing_tree_nr",
	};


	TestMode mode = TestMode::ROOTCELV;
	vector<string> testsToRun;
	switch (mode)
	{
	case TestMode::TREE:
		testsToRun = treeTests;
		break;

	case TestMode::ROOTCELV:
		testsToRun = rootCELVTests;
		break;
	
	case TestMode::NONROOTCELV:
		testsToRun = nonRootCELVTests;

	default:
		break;
	}

	for (auto tfile : testsToRun){
		cout << "================" << endl;
		cout << "Test: " << tfile << endl;
		cout << "================" << endl;
		string fp = testFolder + "/" + tfile;
		ifstream ifFile(fp,ifstream::in);
		REPL().runREPL(ifFile);
	}

	return 0;
}