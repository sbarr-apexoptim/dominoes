#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char** argv) {
  ifstream in("mary.dat");
  string line;
  stringstream ss;
  int **data;

  int imax = 40;
  int jmax = 66;
  data = new int*[imax];
  for(int i=0;i<imax;i++) {
    data[i] = new int[jmax];
    getline(in, line);
    ss.clear();
    ss.str(std::string());
    ss << line;
    for(int j=0;j<jmax;j++) {
      ss >> data[i][j];
    }
    cout << data[i][0] << endl;
  }

  return 0;
}
