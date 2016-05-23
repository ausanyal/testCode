#include <iostream>
#include <list>

using namespace std;

typedef list<int>::iterator lit;

class Graph {
public:
  int num_vertices;
  list<int> *adj;
  Graph(int v);
  void addEdge(int v1, int v2);
  void doBfs(int s);
  void doDfs(int s);
  void runDfs(int s, bool *visited);
  void printGraph();
};

Graph::Graph(int v) {
  this->num_vertices = v;
  this->adj = new list<int>[v];
}

void Graph::addEdge(int v1, int v2) { this->adj[v1].push_back(v2); }

void Graph::doBfs(int s) {
  bool *visited = new bool[this->num_vertices];
  for (int i = 0; i < this->num_vertices; i++) {
    visited[i] = false;
  }

  list<int> queue;
  queue.push_front(s);
  visited[s] = true;

  while (!queue.empty()) {
    int x = queue.back();
    cout << " retrieved: " << x << endl;
    queue.pop_back();

    for (lit it = adj[x].begin(); it != adj[x].end(); it++) {
      if (!visited[*it]) {
        queue.push_front(*it);
        // cout << "Inserting into queue: " << *it << endl;
        visited[*it] = true;
      } else {
        cout << "already visited: " << *it << endl;
      }
    }
  }
  delete[] visited;
}

void Graph::runDfs(int s, bool *visited) {

  list<int> l = this->adj[s];
  for (lit it = l.begin(); it != l.end(); it++) {
    if (!visited[*it]) {
      visited[*it] = true;
      cout << "..Reached: " << *it << endl;
      runDfs(*it, visited);
    }
  }
}

void Graph::doDfs(int s) {

  bool *visited = new bool[this->num_vertices];
  for (int i = 0; i < this->num_vertices; i++) {
    visited[i] = false;
  }

  runDfs(s, visited);
}

int main() {

  Graph g(4);

  g.addEdge(0, 1);
  g.addEdge(0, 2);
  g.addEdge(1, 2);
  g.addEdge(2, 0);
  g.addEdge(2, 3);
  g.addEdge(3, 3);
  g.printGraph();

  g.doBfs(2);
  g.doDfs(2);

  return 0;
}

void Graph::printGraph() {
  cout << "************************************\n";
  for (int i = 0; i < this->num_vertices; i++) {
    cout << "i: " << i << endl;
    for (lit it = this->adj[i].begin(); it != this->adj[i].end(); it++) {
      cout << " .. " << i << *it;
    }
    cout << endl;
  }
}
