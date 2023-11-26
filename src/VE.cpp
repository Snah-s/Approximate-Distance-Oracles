template <typename TE, typename TV>
struct Edge;

template <typename TE, typename TV>
struct Vertex {
  TV data;
  bool visited;
  TE distance;
  list<Edge<TE, TV> *> edges;
  Vertex<TE, TV> *parent;
  pair<int, int> position;

  Vertex(TV data, pair<int, int> pos) : data(data), visited(false), distance(numeric_limits<TE>::max()), parent(nullptr), position(pos) {}
};

template <typename TE, typename TV>
struct Edge {
  Vertex<TE, TV> *startVertex;
  Vertex<TE, TV> *endVertex;
  TE weight;

  Edge(Vertex<TE, TV> *start, Vertex<TE, TV> *end, TE w) : startVertex(start), endVertex(end), weight(w) {}
};