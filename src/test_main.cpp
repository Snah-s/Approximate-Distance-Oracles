#include <iostream>
#include <vector>
#include "ADO.cpp"

int main() {
  srand(time(NULL));

  vector<int> numVertexes = {100, 1000, 10000, 100000};

  for(auto numVertex : numVertexes) {
    ADO<int, int> ado;
    int randomStartIdx = rand() % numVertex;
    int randomGoalIdx = rand() % numVertex;
    ado.compareAlgorithms(numVertex, randomStartIdx, randomGoalIdx);
  }

  return 0;
}