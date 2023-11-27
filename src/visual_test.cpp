#include <iostream>
#include "ADO.cpp"

using namespace std;

int main(){
  srand(time(NULL));
  int choice;
  while(true){
    cout << "Menú:" << endl;
    cout << "1. Comparar tiempos de ejecución con 100 nodos" << endl;
    cout << "2. Comparar tiempos de ejecución con 1000 nodos" << endl;
    cout << "3. Comparar tiempos de ejecución con 10000 nodos" << endl;
    cout << "4. Comparar tiempos de ejecución con 100000 nodos" << endl;
    cout << "5. Comparar tiempos de ejecución con n nodos" << endl;
    cout << "6. Salir" << endl;
    cout << "Seleccione una opción: ";
    cin >> choice;
    switch (choice){
      case 1:{
        ADO<int, int> ado;
        int randomStartIdx = rand() % 100;
        int randomGoalIdx = rand() % 100;
        ado.compareAlgorithms(100, randomStartIdx, randomGoalIdx);
        break;
      }
      case 2:{
        ADO<int, int> ado;
        int randomStartIdx = rand() % 1000;
        int randomGoalIdx = rand() % 1000;
        ado.compareAlgorithms(1000, randomStartIdx, randomGoalIdx);
        break;
      }
      case 3:{
        ADO<int, int> ado;
        int randomStartIdx = rand() % 10000;
        int randomGoalIdx = rand() % 10000;
        ado.compareAlgorithms(10000, randomStartIdx, randomGoalIdx);
        break;
      }
      case 4:{
        ADO<int, int> ado;
        int randomStartIdx = rand() % 100000;
        int randomGoalIdx = rand() % 100000;
        ado.compareAlgorithms(100000, randomStartIdx, randomGoalIdx);
        break;
      }
      case 5:{
        int n;
        cout << "Ingrese el número de nodos: ";
        cin >> n;
          ADO<int, int> ado;
          int randomStartIdx = rand() % n;
          int randomGoalIdx = rand() % n;
          ado.compareAlgorithms(n, randomStartIdx, randomGoalIdx);
        }
      case 6:{
        return 0;
      }
      default:{
        cout << "Opción inválida intente de nuevo" << endl;
        break;
      }
    }
  }
}