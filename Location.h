#ifndef LOCATION_H
#define LOCATION_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

struct Location  {
	friend std::ostream &operator <<(std::ostream &os, const Location &location) {
		os << "(" << location.x << ", " << location.y << ": " << location.symbol << ", " << location.isPath << ")";
		return os;
	}
	bool operator ==(const Location &rhs) const {return x == rhs.x && y == rhs.y;}
	bool operator !=(const Location &rhs) const {return !(*this == rhs);}
	operator bool() const {return x >= 0;}
	void setCoordinates(int yc, int xc){ y = yc; x = xc; }

	void setBool(bool val){
        isPath = val;
	}

	Location(int x=-1, int y=-1) : x(x), y(y) {}

    int x, y;
	bool isPath;
    char symbol;
};

#endif
