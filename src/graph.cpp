#include <bits/stdc++.h>
using namespace std;

template <typename TE, typename TV>
struct Edge;

template <typename TE, typename TV>
struct Node {
  TV data;
  bool visited;
  TE g;
  TE h;
  TE f;
  TE distance;
  list<Edge<TE, TV> *> edges;
  Node<TE, TV> *parent;
  pair<int, int> position;

  Node(TV data, pair<int, int> pos) : data(data), visited(false), distance(numeric_limits<TE>::max()), g(numeric_limits<TE>::max()), h(0), f(0), parent(nullptr), position(pos) {}
};

template <typename TE, typename TV>
struct Edge {
  Node<TE, TV> *startVertex;
  Node<TE, TV> *endVertex;
  TE weight;

  Edge(Node<TE, TV> *start, Node<TE, TV> *end, TE w) : startVertex(start), endVertex(end), weight(w) {}
};