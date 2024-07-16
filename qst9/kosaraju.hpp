#ifndef KOSARAJU_HPP
#define KOSARAJU_HPP
#include <algorithm>  // Include for std::find
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>
#include <list>
#include <stack>
#include <sstream>

#include <vector>
using namespace std;

void DFS(int v, vector<list<int>>& adj, vector<bool>& visited, stack<int>& Stack);
vector<list<int>> getTranspose(vector<list<int>>& adj);
void DFSUtil(int v, vector<list<int>>& adj, vector<bool>& visited, vector<int>& scc);
vector<vector<int>> findSCC(int n, vector<list<int>>& adj);
vector<list<int>> Newgraph(istringstream  &iss,int n, int m) ; 
string kosaraju(int n, vector<list<int>>& vecList) ;
void addEdge(int u, int v, vector<list<int>>& vecList) ;
void removeEdge(int u, int v, vector<list<int>>& vecList) ;

#endif




