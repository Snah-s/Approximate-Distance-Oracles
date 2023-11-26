#include <iostream>
#include <unordered_map>
#include "VE.cpp"
#include "Dijkstra.cpp"
#include "Astar.cpp"


using namespace std;

template <typename TE, typename TV>
  class ADO {
  private:
    vector<Vertex<TE, TV>> vertexes;
    unordered_map<TE, unordered_map<TE, int>> landmarksDistances;
    vector<int> landmarksIdx;
    Dijkstra<TE, TV> dijkstra;
    Astar<TE, TV> astar;

    void randomizeLandmarks(int numLandmarks) {
      srand(time(NULL));
      for (int i = 0; i < numLandmarks; ++i) {
        int randomIdx = rand() % vertexes.size();
        landmarksIdx.push_back(randomIdx);
      }
    }

    void calcLandmarkDistances() {
      vector<Vertex<TE, TV>> landmarks = vector<Vertex<TE, TV>>();
      for(auto& landmarkIdx : landmarksIdx) {
        landmarks.push_back(vertexes[landmarkIdx]);
      }

      for (int i = 0; i < landmarks.size(); ++i) {
        // astar returns a map of distances from the landmark to all other landmarks
        for(int j = i + 1; j < landmarks.size(); ++j) {
          // validate if there is saved distance from landmark i to landmark j
          if(landmarksDistances[landmarks[i].data].find(landmarks[j].data) != landmarksDistances[landmarks[i].data].end()) {
            continue;
          }

          // calculate the distance from landmark i to landmark j
          unordered_map<TE, int> distances = astar.run(vertexes, landmarks[i].data, landmarks[j].data);
          landmarksDistances[landmarks[i].data][landmarks[j].data] = distances[landmarks[j].data];
          landmarksDistances[landmarks[j].data][landmarks[i].data] = distances[landmarks[j].data];
        }
      }
    }

    int findNearestLandMark(int vertexIdx){
      Vertex<TE, TV> vertex = vertexes[vertexIdx];

      // Calculate distance between vertex and landmarks using Manhattan distance
      vector<pair<int, int>> landmarksDistance = vector<int>();

      for (auto& landmarkIdx : landmarksIdx) {
        pair<int, int> landmarkPosition = vertexes[landmarkIdx].position;
        int distance = abs(vertex->position.first - landmarkPosition.first) + abs(vertex->position.second - landmarkPosition.second);
        landmarksDistance.push_back(pair(distance, landmarkIdx));
      }

     // Sort landmarksDistance landmarkId by distance
      sort(landmarksDistance.begin(), landmarksDistance.end(), [](pair<int, int> a, pair<int, int> b) {
        return a.first < b.first;
      });

      return landmarksDistance[0].second;
    }

  public:
    void addVertex(TV data, pair<int, int> pos) {
      vertexes.emplace_back(data, pos);
    }

    void addEdge(int startIdx, int endIdx, TE weight) {
      if (startIdx >= 0 && startIdx < vertexes.size() && endIdx >= 0 && endIdx < vertexes.size()) {
        Edge<TE, TV> *newEdge = new Edge<TE, TV>(&vertexes[startIdx], &vertexes[endIdx], weight);
        Edge<TE, TV> *newEdge = new Edge<TE, TV>(&vertexes[endIdx], &vertexes[startIdx], weight);

        vertexes[startIdx].edges.push_back(newEdge);
      }
    }

    void preprocess(int numLandmarks) {
      randomizeLandmarks(numLandmarks);
      calcLandmarkDistances();
    }

    TE query(int startIdx, int endIdx) {
      // Find the nearest landmark for start and end
      int startLandmarkIdx = findNearestLandMark(&vertexes[startIdx]);
      int endLandmarkIdx = findNearestLandMark(&vertexes[endIdx]);

      // Calculate the distance from start to startLandmark and end to endLandmark using A*
      unordered_map<TE, int> startDistances = astar.run(vertexes, start, startLandmarkIdx);
      unordered_map<TE, int> endDistances = astar.run(vertexes, end, endLandmarkIdx);

      // Calculate the distance from startIdx to endIdx 
      // approxDistance = (A -> landmarkA) + (landmarkA -> landmarkB) + (B->landmarkB) 
      TE approxDistance = startDistances[startLandmarkIdx] + landmarksDistances[startLandmarkIdx][endLandmarkIdx] + endDistances[endLandmarkIdx];

      return approxDistance;
    }
};
