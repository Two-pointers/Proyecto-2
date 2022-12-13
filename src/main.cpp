#include <iostream>
#include "versionManager/versionManager.hpp"
#include "repl/graph.hpp"
#include "repl/repl.hpp"
#include <fstream>
using namespace std;

int main(){

  /*
	versionManager vm = createVM();

	int val = createInVM(file,"src/main",vm);
	val = createInVM(file,"src/f1",vm);
	val = createInVM(file,"src/f2",vm);
	val = changeVersion( vm, 0 );
	val = createInVM(file,"src/f3",vm);
	val = createInVM(file,"src/f4",vm);
	val = createInVM(file,"src/f5",vm);
	val = changeVersion( vm, 1 );
	val = createInVM(file,"src/f6",vm);
	val = createInVM(file,"src/f7",vm);
	val = createInVM(file,"src/f8",vm);

	FOR(i,0,100){
		int v, u; cin>>v>>u;
		cout << "LCA entre "<< v << " "<< u << endl;
		cout << lcaVersions(vm, v,u) << endl;
	}
	

	FileSystem root;

	root.crear_dir("dir1");
	root.crear_dir("dir2");
	root.crear_archivo("archivo1");
	root.escribir("archivo1","asdf");

	
	FileSystem* dir2 = get<FileSystem*>(root.ir("dir2"));
	
	cout << "ACA" << endl;
	dir2->crear_archivo("archivo2");
	
	dir2->escribir("archivo2","aaaaa");
	
	dir2->crear_archivo("archivo3");
	dir2->escribir("archivo3","bbbb");

	cout << "root: " << endl;
	cout << root.toString(1,true,false) << endl;
	cout << "================" << endl;
	cout << dir2->toString() << endl;
	*/

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