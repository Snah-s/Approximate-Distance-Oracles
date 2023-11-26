#include <iostream>
#include <vector>
#include <unordered_map>
#include "VE.cpp"

using namespace std;

template <typename TE, typename TV>
class Dijkstra {
  private:
  int minDistance() {
      TE minDist = numeric_limits<TE>::max();
      int minIdx = -1;

      for (int i = 0; i < vertexes.size(); ++i) {
        if (!vertexes[i].visited && vertexes[i].distance < minDist) {
          minDist = vertexes[i].distance;
          minIdx = i;
        }
      }

      return minIdx;
    }
  public:
    unordered_map<TE, int> run(vector<Vertex<TE, TV>> vertexes, int vertexIdx){
      for (auto& vertex : vertexes) {
        vertex.visited = false;
        vertex.distance = numeric_limits<TE>::max();
        vertex.parent = nullptr;
      }

      vertexes[vertexIdx].distance = 0;

      while(true){
        int currentIdx = minDistance();

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

      unordered_map<TE, int> distances;
      for (auto& vertex : vertexes) {
        distances[vertex.data] = vertex.distance;
      } 
      
      return distances;
    }
};
