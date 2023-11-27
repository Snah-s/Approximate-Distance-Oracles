#include <iostream>
#include <vector>
#include <unordered_map>
#include "VE.cpp"

using namespace std;

template <typename TE, typename TV>
class Dijkstra {
  private:
    int minDistance(vector<Vertex<TE, TV>> &vertexes) {
      int minIdx = -1;
      TE minDistance = numeric_limits<TE>::max();

      for (int i = 0; i < vertexes.size(); ++i) {
        if (!vertexes[i].visited && vertexes[i].distance <= minDistance) {
          minDistance = vertexes[i].distance;
          minIdx = i;
        }
      }

      return minIdx;
    }
  public:
    Dijkstra () {}
    TE run(vector<Vertex<TE, TV>> &vertexes, int startIdx, int endIdx) {
      for (auto& vertex : vertexes) {
        vertex.visited = false;
        vertex.distance = numeric_limits<TE>::max();
        vertex.parent = nullptr;
      }

      vertexes[startIdx].distance = 0;

      while(true){
        int currentIdx = minDistance(vertexes);

        if(currentIdx == -1) break;

        Vertex<TE, TV>& currentVertex = vertexes[currentIdx];
        currentVertex.visited = true;

        for (auto& edge : currentVertex.edges) {
          TE newDistance = currentVertex.distance + edge->weight;
          if (newDistance < edge->endVertex->distance) {
            edge->endVertex->distance = newDistance;
            edge->endVertex->parent = &currentVertex;
          }
        }
      }

      return vertexes[endIdx].distance;
    }
};
