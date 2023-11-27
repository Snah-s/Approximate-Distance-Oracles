#include <iostream>
#include "main.cpp"

int main() {
  ADO<int, int> ado;

  // Add vertices
  ado.addVertex(1, {0, 0});
  ado.addVertex(2, {1, 1});
  ado.addVertex(3, {2, 2});
  ado.addVertex(4, {3, 3});

  // Add edges
  ado.addEdge(0, 1, 1);
  ado.addEdge(1, 2, 2);
  ado.addEdge(2, 3, 3);
  ado.addEdge(3, 0, 4);

  // Preprocess with 2 landmarks
  ado.preprocess(2);

  // Query distances
  int distance1 = ado.query(0, 2);
  int distance2 = ado.query(1, 3);

  std::cout << "Distance from vertex 0 to vertex 2: " << distance1 << std::endl;
  std::cout << "Distance from vertex 1 to vertex 3: " << distance2 << std::endl;

  return 0;
}