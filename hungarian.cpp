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
// The assignment itself is given by the vectors mate_V and mate_U.
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
// While some of the vector names here match those given in the
// pseudo-code in [1], there are major differences in this implementation:
//
// 1) There is no auxiliary initial graph A;
// 2) Function search() in this code corresponds to
//    procedure "modify" and the "searches" (dashed-boxes) in [1];
// 3) Labelling of vertices in V and U in this code is made explicit
//    by the vectors label_V and label_U;
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <limits>
#include <vector>

typedef long long ll;

using namespace std;

int N;
vector<vector<ll>> c;
vector<int> mate_V,mate_U,nhbor,parent;
vector<ll> alpha,beta,slack;
vector<bool> label_V,label_U;

void augment( int v, int exposed ) {

  int aux = mate_V[v];

  mate_V[v] = exposed;
  mate_U[exposed] = v;

  if ( parent[v] != -1 )
    augment( parent[v], aux );

}

void update_slack( int v ) {
  
  for ( int u = 0; u < N; u++ ) {
    if ( not label_U[u] ) {
      ll bound = c[v][u]-alpha[v]-beta[u];
      if ( (bound >= 0LL) and (bound < slack[u]) ) {
	slack[u] = bound;
	nhbor[u] = v;
      }
    }
  }
  
}

void search() {

  for ( int i = 0; i < N; i++ ) {
    
    ll theta = numeric_limits<ll>::max();

    for ( int u = 0; u < N; u++ )
      if ( not label_U[u] )
	theta = min( theta, slack[u] );

    // theta will be even before the division
    // as the input is multiplied by 2
    theta /= 2LL;

    if ( theta > 0LL ) {
      for ( int k = 0; k < N; k++ ) {
	if ( label_V[k] ) alpha[k] += theta;
	else alpha[k] -= theta;
	if ( label_U[k] ) beta[k] -= theta;
	else beta[k] += theta;
      }
    }
    
    vector<int> admissibles = vector<int>();

    for ( int u = 0; u < N; u++ ) {
      if ( not label_U[u] ) {
	slack[u] -= 2LL*theta;
	if ( slack[u] == 0LL ) {
	  if ( mate_U[u] == -1 ) {
	    augment( nhbor[u], u );
	    return;
	  } else { 
	    admissibles.push_back( u );
	  }
	}
      }
    }
    
    for ( int u: admissibles ) {
      label_U[u] = label_V[mate_U[u]] = true;
      parent[mate_U[u]] = nhbor[u];
      update_slack( mate_U[u] );
    }
    
  }
  
}

int main() {

  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  cin >> N;
  
  c = vector<vector<ll>>(N,vector<ll>(N));
  
  vector<ll> min_col = vector<ll>(N,numeric_limits<ll>::max());
  
  // input
  for ( int v = 0; v < N; v++ )
    for ( int u = 0; u < N; u++ ) {
      cin >> c[v][u];
      // multiply by 2
      c[v][u] *= 2LL;
      min_col[u] = min( min_col[u], c[v][u] );
    }
  
  mate_V = vector<int>(N,-1);
  mate_U = vector<int>(N,-1);
  alpha = vector<ll>(N,0LL);
  beta = vector<ll>(N);
  for ( int u = 0; u < N; u++ )
    beta[u] = min_col[u];

  ///////////////////////////////////////////  
  // main cycle of the Hungarian Algorithm //
  ///////////////////////////////////////////  

  for ( int s = 0; s < N; s++ ) {
    nhbor = vector<int>(N,-1);
    parent = vector<int>(N,-1);
    slack = vector<ll>(N,numeric_limits<ll>::max());
    label_V = vector<bool>(N,false);
    label_U = vector<bool>(N,false);

    for ( int v = 0; v < N; v++ )
      if ( mate_V[v] == -1 ){
        label_V[v] = true;
	update_slack( v );
      }

    search();
  }
  
  //////////////////////////////////////////////  
  
  ll min_cost = 0LL;
  for ( int v = 0; v < N; v++ ) {
    min_cost += c[v][mate_V[v]];
  }
  cout << min_cost/2LL << endl;
  
  return 0;
  
}
