#include <climits>

#include "prqueue.h"  // priority queue min-heap

// Prim's algorithm (better for dense graphs;
// prioriry queue implementation O((v_amnt + e_amnt) * log v_amnt))
int minSpanningTree(int v_amnt, Vector<Pair<int>> adj[]) {
  prqueue pq;  // {weight, vertex}

  // Marks vertices already taken in minimum spanning tree (MST)
  Vector<bool> visited(v_amnt);
  visited.fill(false);

  // Stores current smallest edge weight that can connect
  // vertex v to MST, avoiding pushing worse edges
  Vector<int> best(v_amnt);
  best.fill(INT_MAX);
  // At the beginning we only know how to reach vertex 0
  best[0] = 0;

  // Start from vertex 0, weight 0
  pq.push({0, 0});

  int res = 0;

  // Take the smallest-weight candidate edge from the queue
  while (!pq.empty()) {
    Pair<int> p = pq.top();  // smallest-weight edge
    pq.pop();

    int wt = p[0];  // edge weight
    int u = p[1];   // end vertex of edge

    // Ignore vertex if already in MST (avoids cycles)
    if (visited[u]) {
      continue;
    }

    // If a smaller by weight edge to u was found later,
    // older bigger weights are ignored
    //
    // Ex:
    // pq contains {10, 5} and {7, 5}
    // best[5] = 7
    // If {7, 5} is popped first, it is used
    // Later, when {10, 5} is popped:
    //   wt = 10
    //   best[5] = 7
    //   wt != best[u] => continue
    if (wt != best[u]) {
      continue;
    }

    res += wt;          // add edge to MST (res weight sum)
    visited[u] = true;  // vertex was visited

    // Look at all edges from vertex u (adj[u])
    // Ex: adj[1]: {2, 6}, {4, 5}
    for (int i = 0; i < adj[u].get_size(); ++i) {
      int v = adj[u][i][0];  // neighbor vertex v
      int w = adj[u][i][1];  // edge u-v weight

      // best[v] is smallest edge weight that was found so far
      // that can connect vertex v to MST
      //
      // If v is not already in MST
      if (!visited[v] && w < best[v]) {
        best[v] = w;      // w is smaller => update best[v]
        pq.push({w, v});  // push the edge into pr
      }
    }
  }

  return res;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: input-file\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cout << "Couldn't open input file\n";
    return 2;
  }

  // Amount of vertices and edges of graph
  int v;
  int e;

  input >> v >> e;

  int beg;
  int end;
  int weight;

  Vector<Pair<int>>* adj = new Vector<Pair<int>>[v];  // {vertex, weight}
  for (int i = 0; i < e; ++i) {
    input >> beg >> end >> weight;

    // Read one edge from file
    // Graph is undirected, so store in both directions
    // Ex: 1 2 6 is 1-2 and 2-1
    adj[beg].push_back({end, weight});
    adj[end].push_back({beg, weight});
  }

  std::cout << minSpanningTree(v, adj) << '\n';

  delete[] adj;
  input.close();
  return 0;
}