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
using std::vector;
using std::list;
using std::string;
using std::istringstream;

void DFS(unsigned int v, vector<list<int>>& adj, vector<bool>& visited, std::stack<int>& Stack);
vector<list<int>> getTranspose(vector<list<int>>& adj);
void DFSUtil(unsigned int v, vector<list<int>>& adj, vector<bool>& visited, vector<int>& scc);
vector<vector<int>> findSCC(int n, vector<list<int>>& adj);
vector<list<int>> Newgraph(istringstream  &iss,int n, int m) ; 
string kosaraju(int n, vector<list<int>>& vecList) ;
void addEdge(int u, int v, vector<list<int>>& vecList) ;
void removeEdge(int u, int v, vector<list<int>>& vecList) ;

#endif




