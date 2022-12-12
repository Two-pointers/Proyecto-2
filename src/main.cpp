#include <iostream>
#include "versionManager/versionManager.hpp"

using namespace std;

int main(){
  
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

	return 0;

}