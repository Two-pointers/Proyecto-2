#include <iostream>
#include "versionManager/versionManager.hpp"

using namespace std;

int main(){
  
	versionManager vm = createVM();
	fileSystemVM fs;

	int val = createInVM(file,"src/main",vm);
	val = createInVM(file,"src/f1",vm);
	val = createInVM(file,"src/f20",vm);
	fs = changeVersion( vm, 0 );
	val = createInVM(file,"src/f3",vm);
	val = createInVM(file,"src/f4",vm);
	val = createInVM(file,"src/f5",vm);
	fs = changeVersion( vm, 1 );
	val = createInVM(file,"src/f1",vm);
	val = createInVM(file,"src/f2",vm);
	val = createInVM(file,"src/f3",vm);

	val = editInVM("src/f1","c1",vm);
	val = editInVM("src/main","c2",vm);
	val = editInVM("src/f2","c3",vm);
	val = deleteInVM(file,"src/f3",vm);

	// FOR(i,0,100){
	// 	int v, u; cin>>v>>u;
	// 	cout << "LCA entre "<< v << " "<< u << endl;
	// 	cout << lcaVersions(vm, v,u) << endl;
	// }

	val = mergeVersions(vm,3,12);

	cout << "\n\n\nVersiones "<< vm.versions << endl;
	fs = getFilesVM(vm,vm.versions);
	cout << fileSystemToString(fs) << "\n";


	return 0;

}