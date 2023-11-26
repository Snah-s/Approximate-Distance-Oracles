#include <iostream>
#include <unordered_map>
#include "VE.cpp"
#include "Dijkstra.cpp"

using namespace std;

template <typename TE, typename TV>
  class ADO {
  private:
    vector<Vertex<TE, TV>> vertexes;
    unordered_map<TE, unordered_map<TE, int>> landmarksDistances;
    vector<int> landmarksIdx;
    Dijkstra<TE, TV> dijkstra;

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
        // dijkstra returns a map of distances from the landmark to all other landmarks
        unordered_map<TE, int> distances = dijkstra.run(landmarks, i);
        landmarksDistances[landmarks[i].data] = distances;
      }

      // CALCULATE THE DISTANCE FROM LANDMARK TO LANDMARK - DIRECTLY, WITHOUT DIJKSTRA BECAUSE IT CALCS THE DISTANCE FROM LANDMARK TO ALL OTHER VERTEXES
    }

    void findNearestLandMark(Vertex<TE, TV> *vertex){
      vector<pair<int, int>> landMarksPositions = vector<pair<int, int>>();

      for (auto& landmarkIdx : landmarksIdx) {
        landMarksPositions.push_back(vertexes[landmarkIdx].position);
      }

      // Calculate distance between vertex and landmarks using Manhattan distance
      vector<int> landmarksDistance = vector<int>();
      for(auto& landmarkPosition : landMarksPositions) {
        int distance = abs(vertex->position.first - landmarkPosition.first) + abs(vertex->position.second - landmarkPosition.second);
        landmarksDistance.push_back(distance);
      }

      // Find the minimum distance and return the landmark
      int minDistance = numeric_limits<int>::max();
      int minIdx = -1;

      for (int i = 0; i < landmarksDistance.size(); ++i) {
        if (landmarksDistance[i] < minDistance) {
          minDistance = landmarksDistance[i];
          minIdx = i;
        }
      }

      // validate if there is valid path between vertex and landmark


      return landmarksIdx[minIdx];
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

    TE query(TE start, TE end) {
      // Find the nearest landmark for start and end
      int startLandmark = findNearestLandMark(&vertexes[start]);
      int endLandmark = findNearestLandMark(&vertexes[end]);

      // VALIDATE IF LANDMARKS ARE CONNECTED

      // Calculate the distance from start to startLandmark and end to endLandmark
      
    }
};
