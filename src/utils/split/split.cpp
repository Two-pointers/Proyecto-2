#include "split.hpp"

vector<string> split( string s, char c ){
	int n = s.size();
	vector<string> ans;
	string temp = "";
	FOR(i,0,n){
		if( s[i]==c ){
			ans.push_back( temp );
			temp = "";
		}else{
			temp += s[i];
		}
	}
	if(temp.size()) ans.push_back(temp);
	return ans;
}
