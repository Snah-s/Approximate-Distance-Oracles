#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <chrono>
#include <cmath>
#include "VE.cpp"
#include "Astar.cpp"
#include "Dijkstra.cpp"


using namespace std;
using namespace std::chrono;

template <typename TE, typename TV>
class ADO {
  private:
    vector<Vertex<TE, TV>> vertexes;
    unordered_map<TV, unordered_map<TV, int>> landmarksDistances;
    vector<int> landmarksIdx;
    Astar<TE, TV> astar;
    Dijkstra<TE, TV> dijkstra;

    void randomizeLandmarks(int numLandmarks) {
      srand(time(NULL));
      for (int i = 0; i < numLandmarks; ++i) {
        int randomIdx = rand() % vertexes.size();

        while (find(landmarksIdx.begin(), landmarksIdx.end(), randomIdx) != landmarksIdx.end()) {
          randomIdx = rand() % vertexes.size();
        }

        landmarksIdx.push_back(randomIdx);
      }
    }

    void calcLandmarkDistances() {
      for (int i = 0; i < landmarksIdx.size(); ++i) {
        // Calculate distances from landmark[i] to all other landmarks
        for (int j = i + 1; j < landmarksIdx.size(); ++j) {
          TV distance = astar.run(vertexes, landmarksIdx[i], landmarksIdx[j]);
          landmarksDistances[vertexes[landmarksIdx[i]].data][vertexes[landmarksIdx[j]].data] = distance;
          landmarksDistances[vertexes[landmarksIdx[j]].data][vertexes[landmarksIdx[i]].data] = distance;
        }
      }
    }

    int findNearestLandMark(int vertexIdx){
      Vertex<TE, TV> &vertex = vertexes[vertexIdx];
      vector<pair<int, int>> landmarksDistance;

      for (auto& landmarkIdx : landmarksIdx) {
        pair<int, int> landmarkPosition = vertexes[landmarkIdx].position;
        int distance = abs(vertex.position.first - landmarkPosition.first) + abs(vertex.position.second - landmarkPosition.second);
        landmarksDistance.push_back({distance, landmarkIdx});
      }

      sort(landmarksDistance.begin(), landmarksDistance.end(),  [](const pair<int, int> &a, const pair<int, int> &b) {
        return a.first < b.first;
      });

      return landmarksDistance[0].second;
    }

    void generateRandomGraph(int numVertexes) {
      vertexes.clear();

      // Generar vértices con posiciones aleatorias en el plano cartesiano
      int sideLength = static_cast<int>(sqrt(numVertexes));
      int count = 0;

      for (int i = 0; i < sideLength; ++i) {
        for (int j = 0; j < sideLength; ++j) {
          int x = j;
          int y = i;
          addVertex(count++, {x, y});
        }
      }

      cout << "Vertexes generated" << endl;

      // Generar aristas aleatorias
      for(int i = 0; i < vertexes.size(); ++i) {
        int x = vertexes[i].position.first;
        int y = vertexes[i].position.second;

        // Arriba
        if (y > 0) {
          int upIdx = (y - 1) * sideLength + x;
          int randomWeight = rand() % 10 + 1;
          addEdge(i, upIdx, randomWeight);
        }

        // Abajo
        if (y < sideLength - 1) {
          int downIdx = (y + 1) * sideLength + x;
          int randomWeight = rand() % 10 + 1;
          addEdge(i, downIdx, randomWeight);
        }

        // Izquierda
        if (x > 0) {
          int leftIdx = y * sideLength + (x - 1);
          int randomWeight = rand() % 10 + 1;
          addEdge(i, leftIdx, randomWeight);
        }

        // Derecha
        if (x < sideLength - 1) {
          int rightIdx = y * sideLength + (x + 1);
          int randomWeight = rand() % 10 + 1;
          addEdge(i, rightIdx, randomWeight);
        }
      }

      cout << "Edges generated" << endl;
    }

  public:
    ADO() {}
    ~ADO() {
      landmarksDistances.clear();
      vertexes.clear();
      landmarksIdx.clear();
    }
    void addVertex(TV data, pair<int, int> pos) {
      vertexes.emplace_back(data, pos);
    }

    void addEdge(int startIdx, int endIdx, TE weight) {
      if (startIdx >= 0 && startIdx < vertexes.size() && endIdx >= 0 && endIdx < vertexes.size()) {
        Edge<TE, TV> *newEdge1 = new Edge<TE, TV>(&vertexes[startIdx], &vertexes[endIdx], weight);
        Edge<TE, TV> *newEdge2 = new Edge<TE, TV>(&vertexes[endIdx], &vertexes[startIdx], weight);

        // check if edge already exists
        if(find(vertexes[startIdx].edges.begin(), vertexes[startIdx].edges.end(), newEdge1) != vertexes[startIdx].edges.end()){
          return;
        }

        vertexes[startIdx].edges.push_back(newEdge1);
        vertexes[endIdx].edges.push_back(newEdge2); 
      }
    }

    void preprocess(int numLandmarks) {
      randomizeLandmarks(numLandmarks);
      calcLandmarkDistances();
    }

    TE query(int startIdx, int endIdx) {
      // Validate if start and end indexes are landmarks
      // if are landmarks, return landmarksDistances[startIdx][endIdx]
      // else if startIdx is landmark
      //   find nearest landmark from endIdx
      //   return landmarksDistances[startIdx][endIdx] + landmarksDistances[startIdx][endLandmarkIdx]
      // else if endIdx is landmark
      //   find nearest landmark from startIdx
      //   return landmarksDistances[startIdx][endIdx] + landmarksDistances[startLandmarkIdx][endIdx]
      // else
      //   find nearest landmark from startIdx
      //   find nearest landmark from endIdx
      //   return landmarksDistances[startIdx][startLandmarkIdx] + landmarksDistances[startLandmarkIdx][endLandmarkIdx] + landmarksDistances[endLandmarkIdx][endIdx]
      
      if(find(landmarksIdx.begin(), landmarksIdx.end(), startIdx) != landmarksIdx.end() && find(landmarksIdx.begin(), landmarksIdx.end(), endIdx) != landmarksIdx.end()){
        return landmarksDistances[vertexes[startIdx].data][vertexes[endIdx].data];
      }else if(find(landmarksIdx.begin(), landmarksIdx.end(), startIdx) != landmarksIdx.end()){
        int endLandmarkIdx = findNearestLandMark(endIdx);
        TV endDistance = astar.run(vertexes, endIdx, endLandmarkIdx);
        return landmarksDistances[vertexes[startIdx].data][vertexes[endLandmarkIdx].data] + endDistance;
      }else if(find(landmarksIdx.begin(), landmarksIdx.end(), endIdx) != landmarksIdx.end()){
        int startLandmarkIdx = findNearestLandMark(startIdx);
        TV startDistance = astar.run(vertexes, startIdx, startLandmarkIdx);
        return landmarksDistances[vertexes[startLandmarkIdx].data][vertexes[endIdx].data] + startDistance;
      }else{
        int startLandmarkIdx = findNearestLandMark(startIdx);
        int endLandmarkIdx = findNearestLandMark(endIdx);

        TE startDistance = astar.run(vertexes, startIdx, startLandmarkIdx);
        TE endDistance = astar.run(vertexes, endIdx, endLandmarkIdx);

        return landmarksDistances[vertexes[startLandmarkIdx].data][vertexes[endLandmarkIdx].data] + startDistance + endDistance;
      }
    }

    TE queryDijkstra(int startIdx, int endIdx) {
      return dijkstra.run(vertexes, startIdx, endIdx);
    }

    TE queryAstar(int startIdx, int endIdx) {
      return astar.run(vertexes, startIdx, endIdx);
    }

    void compareAlgorithms(int numVertexes, int startIdx, int endIdx) {
      cout << "--------------------------" << endl;
      generateRandomGraph(numVertexes);
      cout << "Number of vertexes: " << numVertexes << endl;

      // // Medir el tiempo de ejecución de Dijkstra
      auto dijkstraStartTime = high_resolution_clock::now();
      TE dijkstraDistance = queryDijkstra(startIdx, endIdx);
      auto dijkstraEndTime = high_resolution_clock::now();
      auto dijkstraDuration = duration_cast<milliseconds>(dijkstraEndTime - dijkstraStartTime);
      cout << "Dijkstra: " << dijkstraDistance << " in " << dijkstraDuration.count() << " ms" << endl;

      // // Medir el tiempo de ejecución de A*
      auto astarStartTime = high_resolution_clock::now();
      TE astarDistance = queryAstar(startIdx, endIdx);
      auto astarEndTime = high_resolution_clock::now();
      auto astarDuration = duration_cast<milliseconds>(astarEndTime - astarStartTime);
      cout << "Astar: " << astarDistance << " in " << astarDuration.count() << " ms" << endl;

      // // Medir el tiempo de ejecución de ODA
      preprocess(ceil(numVertexes * 0.01)+1);
      auto adoStartTime = high_resolution_clock::now();
      TE adoDistance = query(startIdx, endIdx);
      auto adoEndTime = high_resolution_clock::now();
      auto adoDuration = duration_cast<milliseconds>(adoEndTime - adoStartTime);
      cout << "ADO: " << adoDistance << " in " << adoDuration.count() << " ms" << endl;

      cout << "--------------------------" << endl;
    }
};