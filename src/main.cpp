#include <iostream>
#include "versionManager/versionManager.hpp"
#include "repl/graph.hpp"
#include "repl/repl.hpp"
#include <fstream>
using namespace std;

int main(){
	string testFolder = "./testscripts";
	string testFiles[] {
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
		"root_celv_init",
		"root_celv",
		"crear_archivo_celv",
		"eliminar_dir_celv",
		"eliminar_archivo_celv",
		"ir_celv",
		"ir_celv_with_deletion",
		"ir_y_volver_celv",
		"ir_y_volver_celv_with_deletion",
	};

	for (auto tfile : testFiles){
		cout << "================" << endl;
		cout << "Test: " << tfile << endl;
		cout << "================" << endl;
		string fp = testFolder + "/" + tfile;
		ifstream ifFile(fp,ifstream::in);
		REPL().runREPL(ifFile);
	}
	return 0;
}