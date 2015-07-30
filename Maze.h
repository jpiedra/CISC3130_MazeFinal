#ifndef MAZE_H
#define MAZE_H

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <cstring>
#include <vector>
#include "Location.h"

using namespace std;

class Maze{

static const int initRowCount = 0;

friend std::ostream &operator <<(std::ostream &os, const Maze &maze){

    int count = 0;
    do{
        for(int i = 0; i < maze.columns; i++){
            cout << maze.mazeLoc[count][i] << (i == maze.columns-1 ? "\n" : " | ");
        }
        count++;
    } while (count < maze.row);

    return os;
}

//Maze() constructor will call load. Constructor should accept a parameter, string, for filename


friend Maze load(std::string mazeName);

public:
    //INstart and INfinish must be created first, in the driver program,
    //then passed as arguments to respected Maze members...)
   Maze(const std::vector<std::vector<bool> > &INspec, const Location &INstart, const Location &INfinish, int INcols) :
       spec(INspec), start(INstart), finish(INfinish), columns(INcols), iRC(0), mazeLoc(INspec.size(), vector<Location>()) {

            row = spec.size();
            cout << "set no. of rows to " << row << endl;
            do {
                for(int i = 0; i < columns; i++){
                    spec[iRC].push_back(false);
                }
                iRC++;
            } while (iRC < row);

            iRC = 0;
            do {
                for(int i = 0; i < columns; i++){
                    spec[iRC][i] = INspec[iRC][i];
                }
                iRC++;
            } while (iRC < row);

            iRC = 0;
            do {
                for(int i = 0; i < columns; i++){
                    mazeLoc[iRC].push_back(Location(i, iRC));
                    mazeLoc[iRC][i].isPath = spec[iRC][i];
                    mazeLoc[iRC][i].symbol = '-';
                    //cout << mazeLoc[iRC][i].x << ", " << mazeLoc[iRC][i].y << ": " << mazeLoc[iRC][i].symbol << "," << mazeLoc[iRC][i].isPath << endl;
                }
                iRC++;
            } while (iRC < row);
    }

    void setmazeLocSymbol(int y, int x, const char &val){
        mazeLoc[y][x].symbol = val;
    }

    void setSpecBool(int y, int x, bool val){
        spec[y][x] = val;
    }

    void printBoolMaze(){
        int k = 0;
        do {
            for (int j = 0; j < columns; j++){
                cout << spec[k][j] << (j == columns-1 ? "\n" : "");
            }
        k++;
        } while (k < row);
    }

    void printCharMaze(){
        int k = 0;
        do {
            for (int j = 0; j < columns; j++){
                cout << mazeLoc[k][j].symbol << (j == columns-1 ? "\n" : "");
            }
            k++;
        } while (k < row);

    }

    int height() const {return spec.size();}
	int width() const {return spec[0].size();}

        Location moveUp (const Location &current) {
        if (current.y < 2) {
            return current;
        }

        return mazeLoc[current.y-1][current.x];
    }

    Location moveDown (const Location &current) {
        if (current.y > mazeLoc.size()-2) {
            return current;
        }

        return mazeLoc[current.y+1][current.x];
    }

    Location moveLeft (const Location &current) {
        if (current.x < 2) {
            return current;
        }

        return mazeLoc[current.y][current.x-1];
    }

    Location moveRight (const Location &current) {
        if (current.x > width()-2) {
            return current;
        }

        return mazeLoc[current.y][current.x+1];
    }

    bool solve(){

    stack <Location> leStack;
    leStack.push(start);

    while (!leStack.empty()) {

        printCharMaze();

        current = leStack.top();
        leStack.pop();



        if (current == finish) {
            std::cout << "Solved the maze!" << endl;
            return true;
        }
//rewrite
        if (moveUp(current) != previous && moveUp(current).isPath) {
                leStack.push(moveUp(current));
        }
        else if (moveDown(current) != previous && moveDown(current).isPath) {
                leStack.push(moveDown(current));
        }
        else if (moveLeft(current) != previous && moveLeft(current).isPath) {
                leStack.push(moveLeft(current));
        }
        else if (moveRight(current) != previous && moveRight(current).isPath) {
                leStack.push(moveRight(current));
        }
//...
        std::cout << "Press enter to continue..." << endl;
        std::cin.ignore();
        previous = current;
        mazeLoc[previous.y][previous.x].symbol = 'O';




    }
    cout << "Could not solve maze." << endl;
    return false;



    }

    void run(){}

private:
    bool contains(const Location &location) const {
        //return ()
    };
	bool isPath(const Location &location) const {
        return location.isPath;
	};

    int row, columns;
    int iRC;

    std::vector<std::vector<bool> > spec;
    std::vector<std::vector<Location> > mazeLoc;

	const Location start, finish;
	Location current, previous;
};

Maze load(std::string mazeName){

//this reads in a string, and loads a file with ifstream using that string as the name

    string buffer;

    ifstream readin( mazeName.c_str() );

//this generates count, the number of times lines are reads in.
//count is used to make an array of strings.
//those strings contain every line of the maze file, later converted to cstrings.

    int count = 0;

    while ( !readin.eof() ){
        getline(readin, buffer);
        count++;
    }

    readin.close();
    readin.open( mazeName.c_str() );

//use count to create array of strings.
//closed the file, we now open it again to create this string array.
//every time we read a string for the string array, print it out.
//step to the next string array element, using i as subscript

    string bufArr[count];
    int i = 0;

    while ( !readin.eof() ) {
        getline(readin, bufArr[i]);
        //cout << bufArr[i] << endl;
        i++;
    }

//start over again. get the length of the first string of the txt file,
//which we will use to compare every subsequent string - need to maze sure
//all lines are the same! if not, valid set to false, we terminate/throw exception

    bool valid = true;

    i = 0;
    int sizeRequired = bufArr[0].length();

    do {
        if (bufArr[i].length() != sizeRequired)
            valid = false;
        i++;
    } while (i < count);

    if (valid == false){
        cout << "Your maze is invalid: every line in file must be same length!" << endl;
        //return 1;
    }

    cout << "Maze is valid! Every row is the same length (forms a square/rectangle)" << endl;

//if all the line are the same length, then we pass the length value to charsAmount
//which we will use to generate a 2d array of cstrings.

    int charsAmount = sizeRequired;

//this assigns all of the chars in the 2d array of chars, mzrow, to a corresponding value
//culled from a cstring. using count from before, which is the amount of strings (or max boundary of y!)

    char mzrow[count][charsAmount];

    for (int i = 0; i < count; i++){
        strcpy(mzrow[i], bufArr[i].c_str() );
        //cout << mzrow[i] << endl;
    }

    int j;
    int k = 0;

//this initializes specs, the 2d array of bools, so it's all false

    vector < vector<bool> > specs;

    for (int i = 0; i < count; i++){
        vector <bool> row;
        specs.push_back(row);
    }

    for (int i = 0; i < count; i++){
        for (j = 0; j < charsAmount; j++){
            specs[i][j] = false;
        }
    }

//this sets the boolean parameter for each element in the parallel 2d vect of bools

    do {
        for (int j = 0; j < charsAmount; j++){
            if (mzrow[k][j] == '*' || mzrow[k][j] == 's' || mzrow[k][j] == 'S' || mzrow[k][j] == 'f' || mzrow[k][j] == 'F'){
                    specs[k][j] = true;}
        }
    k++;
    } while (k < count);

//this prints maze in boolean format, 1 for path area and 0 for non-path area

    k = 0;
    do {
        for (int j = 0; j < charsAmount; j++){
        //cout << specs[k][j] << (j == charsAmount-1 ? "\n" : "");
            }
        k++;
    } while (k < count);

//this gets start and finish locations to pass to Maze constructor

    Location start(-1, -1);
    Location finish(-1, -1);

    int yCount = 0, xCount = 0;

    bool foundStart = false;

    k = 0;
    do {
        for (int j = 0; j < charsAmount; j++){
            if (mzrow[k][j] == 'S' || mzrow[k][j] == 's')
                {
                    foundStart = true;
                    start.y = k;
                    start.x = j;
                    start.symbol = mzrow[k][j];
                    start.isPath = true;
                }
            }
        k++;
    } while (k < count && foundStart == false);



    bool foundFinish = false;

    k = 0;
     do {
        for (int j = 0; j < charsAmount; j++){
            if (mzrow[k][j] == 'f' || mzrow[k][j] == 'F')
                {
                    foundFinish = true;
                    finish.y = k;
                    finish.x = j;
                    finish.symbol = mzrow[k][j];
                    finish.isPath = true;
                }
            }
        k++;
    } while (k < count && foundFinish == false);

    cout << start << endl;
    cout << finish << endl;

    Maze myMaze(specs, start, finish, charsAmount);
    cout << myMaze.height() << endl;
    cout << myMaze.width() << endl;

    int b = 0;
    do {
        for(int a = 0; a < charsAmount; a++){
            myMaze.setmazeLocSymbol(b, a, mzrow[b][a]);
        }
    b++;
    } while (b < count);

    //myMaze.printBoolMaze();
    //myMaze.printCharMaze();

    return myMaze;

    }


#endif
