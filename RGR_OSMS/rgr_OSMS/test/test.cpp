#include <iostream>
#include <vector>

int main() {
  std::vector<int> my_vector;
  int corrIndex = 4;
  // Добавляем значения от 1 до 10
  for (int i=1; i<=10; i++) my_vector.push_back(i);



  // Удаляем первые 3 элемента
  my_vector.erase(my_vector.begin(),my_vector.begin()+corrIndex);

  std::cout << "my_vector содержит:";
  for (unsigned i=0; i<my_vector.size(); ++i)
    std::cout << ' ' << my_vector[i];
  std::cout << '\n';

  return 0;
}