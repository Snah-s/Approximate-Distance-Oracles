#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <chrono>

const int INF = std::numeric_limits<int>::max();

template <typename TE>
struct Edge {
  TE dest;
  TE weight;
  Edge(TE d, TE w) : dest(d), weight(w) {}
};

template <typename TV>
struct Node {
  TV id;
  std::vector<Edge<TV>> edges;
  Node(TV id) : id(id) {}
};

template <typename TV>
class Graph {
public:
  std::unordered_map<TV, Node<TV> *> nodes;
  Graph() {
    for (auto &node : nodes) {
      delete node.second;
    }
  }

  ~Graph() {
    for (auto &node : nodes) {
      delete node.second;
    }
  }

  void addNode(TV id) {
    if (nodes.find(id) == nodes.end()) {
      nodes[id] = new Node<TV>(id);
    }
  }

  void addEdge(TV src, TV dest, TV weight) {
    Node<TV> *source = nodes[src];
    source->edges.push_back(Edge<TV>(dest, weight));
  }

  std::unordered_map<int, std::unordered_map<int, int>> floydWarshall() {
    std::unordered_map<int, std::unordered_map<int, int>> dist;
    for (const auto &node : nodes) {
      for (const auto &node2 : nodes) {
        dist[node.first][node2.first] = (node.first == node2.first) ? 0 : INF;
      }
    }

    for (const auto &node : nodes) {
      TV nodeID = node.first;
      for (const auto &edge : node.second->edges) {
        dist[nodeID][edge.dest] = edge.weight;
      }
    }

    for (const auto &k : nodes) {
      int kID = k.first;
      for (const auto &i : nodes) {
        int iID = i.first;
        for (const auto &j : nodes) {
          int jID = j.first;
          if (dist[iID][kID] < INF && dist[kID][jID] < INF) {
            dist[iID][jID] = std::min(dist[iID][jID], dist[iID][kID] + dist[kID][jID]);
          }
        }
      }
    }
    return dist;
  }
};

template <typename TV>
class ADO {
private:
  Graph<TV> *graph;
  std::unordered_map<TV, std::unordered_map<TV, TV>> distances;
  std::vector<TV> landmarks;

  void DijkstraCalcDistance(TV src) {
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    std::unordered_map<TV, TV> &uDist = distances[src];

    for (const auto &node : graph->nodes) {
      uDist[node.first] = INF;
    }

    uDist[src] = 0;
    pq.push(std::make_pair(0, src));

    while (!pq.empty()) {
      int actualDist = pq.top().first;
      TV actualNode = pq.top().second;
      pq.pop();

      if (actualDist > uDist[actualNode]) {
        continue;
      }

      Node<TV> *node = graph->nodes[actualNode];
      for (const auto &edge : node->edges) {
        TV neighbor = edge.dest;
        int weight = edge.weight;
        int distance = actualDist + weight;

        if (distance < uDist[neighbor]) {
          uDist[neighbor] = distance;
          pq.push(std::make_pair(distance, neighbor));
        }
      }
    }
  }

  int randomLandmarks() {
    int index = std::rand() % graph->nodes.size();
    auto it = graph->nodes.begin();
    std::advance(it, index);
    return it->first;
  }

public:
  ADO(Graph<TV> *g) : graph(g) {}

  void preprocess(int k) {
    std::srand(std::time(0));
    for (int i = 0; i < k; ++i) {
      int landmark = randomLandmarks();
      if (std::find(landmarks.begin(), landmarks.end(), landmark) == landmarks.end()) {
        landmarks.push_back(landmark);
        DijkstraCalcDistance(landmark);
      }
      else {
        --i;
      }
    }
  }

  int DistCalc(TV src, TV dest) {
    if (src == dest) {
      return 0;
    }
    int minDist = INF;
    for (TV landmark : landmarks) {
      int distFromSrc = distances[landmark][src];
      int distToDest = distances[landmark][dest];
      if (distFromSrc < INF && distToDest < INF) {
        int dist = distFromSrc + distToDest;
        minDist = std::min(minDist, dist);
        }
      }
      return minDist == INF ? -1 : minDist;
  }
};

int main() {
  Graph<int> g;
  ADO<int> *ado = nullptr;
  std::unordered_map<int, std::unordered_map<int, int>> fwDistances;
  int choise, id, idU, idDest, weight, k;

  while (true) {
    std::cout << "Menú:" << std::endl;
    std::cout << "1. Añadir nodo" << std::endl;
    std::cout << "2. Añadir arista" << std::endl;
    std::cout << "3. Realizar preprocesamiento ODA" << std::endl;
    std::cout << "4. Comparar tiempos de ejecución" << std::endl;
    std::cout << "5. Salir" << std::endl;
    std::cout << "Seleccione una opción: ";
    std::cin >> choise;

    switch (choise) {
      case 1: {
        std::cout << "Introduzca el ID del nodo: ";
        std::cin >> id;
        g.addNode(id);
        std::cout << "Nodo añadido." << std::endl;
        break;
      }
      case 2: {
        std::cout << "Introduzca el ID del nodo origen: ";
        std::cin >> idU;
        std::cout << "Introduzca el ID del nodo destino: ";
        std::cin >> idDest;
        std::cout << "Introduzca el weight de la arista: ";
        std::cin >> weight;

        if (g.nodes.find(idU) == g.nodes.end() || g.nodes.find(idDest) == g.nodes.end()) {
          std::cout << "Nodo origen o destino no existe." << std::endl;
          break;
        }

        g.addEdge(idU, idDest, weight);
        std::cout << "Arista añadida." << std::endl;
        break;
      }
      case 3: {
        std::cout << "Introduzca el número de landmarks para ODA: ";
        std::cin >> k;

        if (ado != nullptr) {
          delete ado;
        }
        ado = new ADO<int>(&g);
        ado->preprocess(k);
        std::cout << "Preprocesamiento ODA completado." << std::endl;
        break;
      }
      case 4: {
        {
          auto startADO = std::chrono::high_resolution_clock::now();
          auto endADO = std::chrono::high_resolution_clock::now();

          auto startFW = std::chrono::high_resolution_clock::now();
          fwDistances = g.floydWarshall();
          auto endFW = std::chrono::high_resolution_clock::now();

          auto durationADO = std::chrono::duration_cast<std::chrono::milliseconds>(endADO - startADO).count();
          auto durationFW = std::chrono::duration_cast<std::chrono::milliseconds>(endFW - startFW).count();

          std::cout << "Tiempo de ejecución de ODA: " << durationADO << " ms" << std::endl;
          std::cout << "Tiempo de ejecución de Floyd-Warshall: " << durationFW << " ms" << std::endl;
        }
        break;
      }
      case 5: {
        if (ado != nullptr) {
          delete ado;
        }
        return 0;
      }
      default: {
        std::cout << "Opción no válida. Intente de nuevo." << std::endl;
        break;
      }
    }
  }
}
