#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include "VE.cpp"

using namespace std;

template <typename TE, typename TV>
class Astar{
  private:
    int manhattanDistance(pair<int, int> start, pair<int, int> end) {
      return abs(start.first - end.first) + abs(start.second - end.second);
    }

  public:
    Astar() {}
    unordered_map<TE, TE> run(vector<Vertex<TE, TV>> vertexes, int startIdx, int goalIdx) {
      for (auto& vertex : vertexes) {
        vertex.visited = false;
        vertex.parent = nullptr;
        vertex.g = numeric_limits<TE>::max();
        vertex.h = 0;
        vertex.f = 0;
      }

      auto compare = [](Vertex<TE, TV> *a, Vertex<TE, TV> *b){ return a->f > b->f; };
      priority_queue<Vertex<TE, TV> *, vector<Vertex<TE, TV> *>, decltype(compare)> openSet(compare);
      Vertex<TE, TV> *startVertex = &vertexes[startIdx];
      startVertex->g = 0;
      startVertex->h = manhattanDistance(startVertex->data, vertexes[goalIdx].data);
      startVertex->f = startVertex->g + startVertex->h;
      openSet.push(startVertex);

      while (!openSet.empty()) {
        Vertex<TE, TV> *currentVertex = openSet.top();
        openSet.pop();

        if (currentVertex->data == vertexes[goalIdx].data) {
          printPath(currentVertex);
          return;
        }

        currentVertex->visited = true;

        for (Edge<TE, TV> *edge : currentVertex->edges) {
          Vertex<TE, TV> *neighbor = edge->endVertex;

          if (neighbor->visited) {continue;}

          TE tentative_g = currentVertex->g + edge->weight;

          if (tentative_g < neighbor->g) {
            neighbor->parent = currentVertex;
            neighbor->g = tentative_g;
            neighbor->h = manhattanDistance(neighbor->data, vertexes[goalIdx].data);
            neighbor->f = neighbor->g + neighbor->h;

            if (!neighbor->visited) {
              openSet.push(neighbor);
            }
          }
        }
      }
      unordered_map<TE, int> distances;
      for (auto& vertex : vertexes) {
        distances[vertex.data] = vertex.distance;
      } 
      return distances;
    }
};

