////////////////////////////////////////////////////////////////////////
//
// Code written for UNIVESP, Univ. Virtual do Estado de Sao Paulo, 2019
//
// Author: Guilherme A. Pinto (guilherme.pinto@gmail.com)
//
// MIT License, Copyright (c) 2019 Guilherme A. Pinto
//
// This is an (as plain C++ as possible, competitive programming style)
// implementation of the alpha-beta O(N^3) version of the Hungarian
// Algorithm for the Assigment Problem as presented in section 11.2 of:
//
// [1] C. H. Papadimitriou, K. Steiglitz:
// Combinatorial Optimization: Algorithms and Complexity, Dover, 1998
//
// The code reads the instance from the standard input and
// writes the value of the optimal assignment to the standard output.
// To output the assignment itself use "-m" or "--match"
//
// The input should describe the cost matrix like this example from [1]:
//
// 5
// 7 2 1 9 4
// 9 6 9 5 5
// 3 8 3 1 8
// 7 9 4 2 2
// 8 4 7 4 8
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <limits>
#include <vector>

typedef long long ll;
using namespace std;

int N;
vector<vector<ll>> c;
vector<int> mate_V,mate_U,nhbor,parent;
vector<ll> alpha,beta,slack,min_col;
vector<bool> label_V,label_U;

bool unlabelled_U( int u ) { return not label_U[u]; }
bool unmatched_V( int v ) { return mate_V[v] == -1; }
bool unmatched_U( int u ) { return mate_U[u] == -1; }
bool admissible_U( int u ) { return slack[u] == 0LL; }

void augment( int v, int exposed_u ) {

  int aux = mate_V[v];

  mate_V[v] = exposed_u;
  mate_U[exposed_u] = v;

  if ( parent[v] != -1 )
    augment( parent[v], aux );

}

void update_slack( int v ) {

  // for unlabelled u in U
  for ( int u = 0; u < N; u++ )
    if ( unlabelled_U( u ) ) {
      ll bound = c[v][u]-alpha[v]-beta[u];
      if ( 0LL <= bound and bound < slack[u] ) {
	slack[u] = bound;
	nhbor[u] = v;
      }
    }
  
}

ll update_alpha_beta() {
  
  ll theta = numeric_limits<ll>::max();

  // for unlabelled u in U
  for ( int u = 0; u < N; u++ )
    if ( unlabelled_U( u ) )
      theta = min( theta, slack[u] );
  
  // skip if theta == 0 (no update needed)
  if ( theta > 0LL ) {
    
    // integrality is ensured
    theta /= 2LL;
    
    for ( int i = 0; i < N; i++ ) {
      if ( label_V[i] ) alpha[i] += theta;
      else alpha[i] -= theta;
      if ( label_U[i] ) beta[i] -= theta;
      else beta[i] += theta;
    }
  }
  
  return theta;
}

int search_augmenting_alternating_path() {

  while ( true ) {
    ll theta = update_alpha_beta();
    
    vector<int> admissibles = vector<int>();
    
    // for unlabelled u in U
    for ( int u = 0; u < N; u++ )
      if ( unlabelled_U( u ) ) {
	slack[u] -= 2LL*theta;
	if ( admissible_U( u ) ) {
	  // unlabelled, admissible and unmatched => path found
	  if ( unmatched_U( u ) ) return u;
	  else admissibles.push_back( u );
	}
      }
    
    for ( int u: admissibles ) {
      label_U[u] = true;
      label_V[mate_U[u]] = true;
      parent[mate_U[u]] = nhbor[u];
      update_slack( mate_U[u] );
    }    
  }
  
}

void initialize_search() {

  nhbor = vector<int>(N,-1);
  parent = vector<int>(N,-1);
  slack = vector<ll>(N,numeric_limits<ll>::max());
  label_V = vector<bool>(N,false);
  label_U = vector<bool>(N,false);
  
}

void initialize_alpha_beta() {

  mate_V = vector<int>(N,-1);
  mate_U = vector<int>(N,-1);
  alpha = vector<ll>(N,0LL);
  beta = vector<ll>(N);
  for ( int u = 0; u < N; u++ )
    beta[u] = min_col[u];
  
}

void hungarian_algorithm() {
  
  initialize_alpha_beta();
  
  for ( int i = 0; i < N; i++ ) {
    initialize_search();
    
    // start with unmatched v in V
    for ( int v = 0; v < N; v++ )
      if ( unmatched_V( v ) ) {
	label_V[v] = true;
	update_slack( v );
      }
    
    int u = search_augmenting_alternating_path();
    
    augment( nhbor[u], u );
  }
  
}

void read_input() {
  
  cin >> N;
  
  c = vector<vector<ll>>(N,vector<ll>(N));
  min_col = vector<ll>(N,numeric_limits<ll>::max());
  
  for ( int v = 0; v < N; v++ )
    for ( int u = 0; u < N; u++ ) {
      cin >> c[v][u];
      
      // multiply by 2LL to ensure integrality
      c[v][u] *= 2LL;
      min_col[u] = min( min_col[u], c[v][u] );
    }
  
}

int main(int argc, char* argv[]) {
  
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  bool match = false;
  if ( argc > 1 ) {
    string opt = argv[1];
    if ( opt == "-m" or opt == "--match" )
      match = true;
  }

  read_input();

  hungarian_algorithm();

  if ( match ) { // output assignment itself
    for ( int v = 0; v < N; v++ )
      cout << mate_V[v] << endl;
  } else {       // output optimal assignment cost
    ll opt_cost = 0LL;
    for ( int i = 0; i < N; i++ )
      opt_cost += alpha[i]+beta[i];
    cout << opt_cost/2LL << endl;
  }
  
  return 0;
  
}
