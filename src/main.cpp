#include <iostream>
#include <unordered_map>
#include <algorithm>
#include "VE.cpp"
#include "Astar.cpp"


using namespace std;

template <typename TE, typename TV>
class ADO {
  private:
    vector<Vertex<TE, TV>> vertexes;
    unordered_map<TE, unordered_map<TE, int>> landmarksDistances;
    vector<int> landmarksIdx;
    Astar<TE, TV> astar;

    void randomizeLandmarks(int numLandmarks) {
      srand(time(NULL));
      for (int i = 0; i < numLandmarks; ++i) {
        int randomIdx = rand() % vertexes.size();
        landmarksIdx.push_back(randomIdx);
      }
    }

    void calcLandmarkDistances() {
      for (int i = 0; i < landmarksIdx.size(); ++i) {
        // Calculate distances from landmark[i] to all other landmarks
        auto distances = astar.run(vertexes, landmarksIdx[i], landmarksIdx[i]);
        for (int j = 0; j < landmarksIdx.size(); ++j) {
          if (i != j) {
            landmarksDistances[vertexes[landmarksIdx[i]].data][vertexes[landmarksIdx[j]].data] = distances[vertexes[landmarksIdx[j]].data];
          }
        }
      }
    }

    int findNearestLandMark(int vertexIdx){
      Vertex<TE, TV> &vertex = vertexes[vertexIdx];
      vector<pair<int, int>> landmarksDistance;

      for (auto& landmarkIdx : landmarksIdx) {
        pair<int, int> landmarkPosition = vertexes[landmarkIdx].position;
        int distance = abs(vertex.position.first - landmarkPosition.first) + abs(vertex.position.second - landmarkPosition.second);
        landmarksDistance.emplace_back(distance, landmarkIdx);
      }

      sort(landmarksDistance.begin(), landmarksDistance.end());

      return landmarksDistance[0].second;
    }

  public:
    void addVertex(TV data, pair<int, int> pos) {
      vertexes.emplace_back(data, pos);
    }

    void addEdge(int startIdx, int endIdx, TE weight) {
      if (startIdx >= 0 && startIdx < vertexes.size() && endIdx >= 0 && endIdx < vertexes.size()) {
        Edge<TE, TV> *newEdge1 = new Edge<TE, TV>(&vertexes[startIdx], &vertexes[endIdx], weight);
        Edge<TE, TV> *newEdge2 = new Edge<TE, TV>(&vertexes[endIdx], &vertexes[startIdx], weight);

        vertexes[startIdx].edges.push_back(newEdge1);
        vertexes[endIdx].edges.push_back(newEdge2); 
      }
    }

    void preprocess(int numLandmarks) {
      randomizeLandmarks(numLandmarks);
      calcLandmarkDistances();
    }

    TE query(int startIdx, int endIdx) {
      int startLandmarkIdx = findNearestLandMark(startIdx);
      int endLandmarkIdx = findNearestLandMark(endIdx);

      unordered_map<TE, int> startDistances = astar.run(vertexes, startIdx, startLandmarkIdx);
      unordered_map<TE, int> endDistances = astar.run(vertexes, endIdx, endLandmarkIdx);

      TE approxDistance = startDistances[startLandmarkIdx] + landmarksDistances[startLandmarkIdx][endLandmarkIdx] + endDistances[endLandmarkIdx];

      return approxDistance;
    }
};