#include <iostream>
#include <vector>
#include <cstdlib>
#include <sstream>
// Reference why you should include string: https://stackoverflow.com/questions/33349833/can-stdstring-be-used-without-include-string
#include <string>
void readTable(std::vector<std::vector<int>>&sudokuTable){
  std::string temp;
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      std::cin >> temp;
      if(temp=="#")
        sudokuTable[i][j] = -1;
      else
        sudokuTable[i][j] = atoi(temp.c_str());
    }
  }
}
void printTable(const std::vector<std::vector<int>>sudokuTable){
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      if(sudokuTable[i][j]==-1)
        std::cout << "- ";
      else
        std::cout << sudokuTable[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}
bool judgeVector(std::vector<int> sudokuVector){
  int cnt = 0;
  for(int i=1; i<=9; i++){
    for(int j=1; j<=9; j++){
      if(sudokuVector[j-1]==i)
        cnt++;
    }
    if(cnt!=1)
      return false;
    cnt = 0;
  }
  return true;
}
bool judgeTable(std::vector<std::vector<int>> sudokuTable){
  std::vector<int> sudokuVector(9,-1);
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      sudokuVector[j] = sudokuTable[i][j];
    }
    if(!judgeVector(sudokuVector))
      return false;
  }
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      sudokuVector[j] = sudokuTable[j][i];
    }
    if(!judgeVector(sudokuVector))
      return false;
  }
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      sudokuVector[j] = sudokuTable[3+j%3][i-i%3+j/3];
    }
    if(!judgeVector(sudokuVector))
      return false;
  }
  return true;
}
bool isComplete(const std::vector<std::vector<int>>sudokuTable){
  for(int i=0; i<9; i++){
    for(int j=0; j<9; j++){
      if(sudokuTable[i][j]==-1)
        return false;
    }
  }
  return true;
}
bool getInput(std::vector<std::vector<int>> &sudokuTable){
// x and y are positions in sudoku table and n is the number that we want to insert into that position.
  int temp;
  int x;
  int y;
  int n;
  if(std::cin >> x >> y >> n){
    sudokuTable[y-1][x-1] = n;
    return true;
  }
  return false;
}
int main(int argc, char *argv[]){

  std::vector<std::vector<int>>sudokuTable(9, std::vector<int> (9, -1));
  readTable(sudokuTable);
  printTable(sudokuTable);
  bool endofInput = false;
  while(!isComplete(sudokuTable) && !endofInput){
    endofInput = !getInput(sudokuTable);
    if(!endofInput)
      printTable(sudokuTable);
  }
  bool result;
  result = judgeTable(sudokuTable);
  if(result)
    std::cout << "valid" << std::endl;
  else
    std::cout << "invalid" << std::endl;
  return 0;
}
