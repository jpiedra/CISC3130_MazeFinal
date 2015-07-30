#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include "Location.h"
#include "Maze.h"


using namespace std;

int main(int argc, char **argv){

    if (argc != 2) {
            cerr << "NamUsage: maze_solver <maze file>" << endl;
            exit(1);
    }

    string mazeFileName = string(argv[1]);
    Maze maze = load(mazeFileName);
    cout << maze << endl;

    maze.solve();

    return 0;
}
