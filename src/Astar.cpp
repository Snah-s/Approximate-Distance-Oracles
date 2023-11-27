#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include "VE.cpp"

using namespace std;

template <typename TE, typename TV>
struct CompareVertex {
  bool operator()(Vertex<TE, TV>* a, Vertex<TE, TV>* b) {
    return a->f > b->f;
  }
};

template <typename TE, typename TV>
class Astar {
  private:
    int manhattanDistance(pair<int, int> start, pair<int, int> end) {
      return abs(start.first - end.first) + abs(start.second - end.second);
    }

  public:
    Astar() {}

    TE run(vector<Vertex<TE, TV>>& vertices, int startIdx, int goalIdx) {
      for (auto& vertex : vertices) {
        vertex.visited = false;
        vertex.parent = nullptr;
        vertex.g = numeric_limits<TE>::max();
        vertex.h = 0;
        vertex.f = 0;
      }

      unordered_set<Vertex<TE, TV>*> inQueue;
      priority_queue<Vertex<TE, TV>*, vector<Vertex<TE, TV>*>, CompareVertex<TE, TV>> openSet;

      Vertex<TE, TV>* startVertex = &vertices[startIdx];
      startVertex->g = 0;
      startVertex->h = manhattanDistance(startVertex->position, vertices[goalIdx].position);
      startVertex->f = startVertex->g + startVertex->h;
      openSet.push(startVertex);
      inQueue.insert(startVertex);

      while (!openSet.empty()) {
        Vertex<TE, TV>* currentVertex = openSet.top();
        openSet.pop();
        inQueue.erase(currentVertex);

        if (currentVertex == &vertices[goalIdx]) {
          break; // Goal reached
        }

        currentVertex->visited = true;

        for (Edge<TE, TV>* edge : currentVertex->edges) {
          Vertex<TE, TV>* neighbor = edge->endVertex;

          if (neighbor->visited) continue;

          TE tentative_g = currentVertex->g + edge->weight;

          if (tentative_g < neighbor->g) {
            neighbor->parent = currentVertex;
            neighbor->g = tentative_g;
            neighbor->h = manhattanDistance(neighbor->position, vertices[goalIdx].position);
            neighbor->f = neighbor->g + neighbor->h;

            if (inQueue.find(neighbor) == inQueue.end()) {
              openSet.push(neighbor);
              inQueue.insert(neighbor);
            }
          }
        }
      }

      return vertices[goalIdx].g;
    }
};
