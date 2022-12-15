#include "diff.hpp"

// We only have 3 options for the edit script
enum class Action { INSERT, DELETE, NOTHING };

vector<pair<Action,char>> diffMyers(string v1, string v2){
  // Myers algorithm strings starts at 1.

  // it's just easier to manage a map because of the range. After testing
  // we can change this to array and do a traslation.
  map<int,int> V; // V : Array [-MAX..MAX]
  // it's easier to keep track of the traces than reconstructing them using
  // the V vector.
  map<int,vector<pair<Action,char>>> traces;

  V[1]  = 0;
  traces[1] = vector<pair<Action,char>>();

  const int M = v1.length();
  const int N = v2.length();
  const int MAX   = M + N + 1;

  bool chooseInsert;
  int x,y;
  vector<pair<Action,char>> trace;

  for (int D = 0; D < MAX; D++)
  {
    for (int k = -D; k < D+1; k+=2)
    {
      chooseInsert = k == -D || (k != D && V[k-1] < V[k+1]);
      if (chooseInsert){
        x = V[k+1];
        trace = traces[k+1];
      } else{
        x = V[k-1] + 1;
        trace = traces[k-1];
      }

      y = x - k;

      // expand a D-1 path.
      if (1 <= y && y <= N && chooseInsert){
        trace.push_back(make_pair(Action::INSERT,v2[y - 1]));
      } else if (1 <= x && x <= M)
      {
        trace.push_back(make_pair(Action::DELETE,v1[x - 1]));
      }

      // and then expand the SNAKEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE.
      while (x < M && y < N && v1[(x+1) - 1] == v2[(y+1) - 1])
      {
        x++;
        y++;
        trace.push_back(make_pair(Action::NOTHING,v1[x - 1]));
      }

      // did we arrived at the end? return the trace.
      if (x >= M && y >= N){
        return trace;
      } else{
        V[k]  = x;
        traces[k] = trace;
      }
    }
  }
  
}

vector<pair<Action,string>> group(vector<pair<Action,char>> sol){

  vector<pair<Action,string>> groupedSol;

  if (sol.size() == 0) return groupedSol;

  auto [previousAction,firstChar] = sol[0];
  string acc{firstChar};

  for (int i = 1; i < sol.size(); i++)
  {
    auto [nextAction,nextChar] = sol[i];
    if (nextAction == previousAction){
      acc.push_back(nextChar);
    } else{
      groupedSol.push_back(make_pair(previousAction,acc));
      acc = nextChar;
      previousAction = nextAction;
    }

    
  }
  
  groupedSol.push_back(pair<Action,string>(previousAction,acc));

  return groupedSol;
}

string groupedSolToString(vector<pair<Action,string>> groupedSol){
  string acc = "";

  for(auto [action,word] : groupedSol)
  {
    switch (action)
    {
      case Action::INSERT :
        acc += "{{{" + word + "}}}";
        break;
    
      case Action::DELETE :
        acc += "[[[" + word + "]]]";
        break;

      case Action::NOTHING :
        acc += word;
        break;
    }
  }

  return acc;
}


string myers(string v1, string v2){
  vector<pair<Action,char>> sol          = diffMyers(v1,v2);
  vector<pair<Action,string>> groupedSol = group(sol);
  string stringSol = groupedSolToString(groupedSol);
  return stringSol;
}


/*
int main(int argc, char const *argv[])
{
  string v1 = "aabbcc";
  string v2 = "abaccdd";
  vector<pair<Action,char>>  sol = diffMyers(v1,v2);
  vector<pair<Action,string>> groupedSol = group(sol);
  string stringSol = groupedSolToString(groupedSol);

  cout << v1 << endl;
  cout << v2 << endl;
  cout << stringSol << endl;

  return 0;
}
*/