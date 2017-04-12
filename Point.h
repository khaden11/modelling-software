#ifndef POINTH
#define POINTH
#include "Vector.h"
#include <iostream>
#include <string>

// use forward declaration to use Vector in Point class functions
class Vector; 

class Point
{
    private:
    
        float x, y, z, w; // initialise variables x,y,z,w

    public:
        
	// default point constructor
        Point(); 

        //initialising constructor (default value for W = 1
        Point(float X, float Y, float Z, float W = 1);

	// OVERLOADING ARITHMETIC OPERATORS ON POINTS
        
        // overload addition for point - point          
        friend Vector operator-(Point p, Point q); 
                  
	// overload addition to allow point + vector           
	friend Point operator+(Point p, Vector v);
                                     
	// overload subtraction to allow point - vector objects         
        friend Point operator-(Point p, Vector v); 
        
        // overload index operator to access indexed coordinates
        float & operator[](int i);
        
        // overload assignment operator
        Point& operator=(const Point &p);

	// OVERLOAD IO STREAMS
        
        // overload ostream operator to print point coordinates to screen
        friend std::ostream &operator<<(std::ostream &output, const Point &p);
        
        // overload ostream operator to read in values to a Point object
        friend std::istream &operator>>(std::istream &input, Point &p);

	friend bool operator== (const Point &p, const Point &q);
};

// function to print out multiple points side by side
void printPoints(Point points[], int spacing, int size, int dimension);

// function to work out whether a point P lies on, to the left or to the right
// of the line AB, given the parameters A, B and P
void leftOrRight(Point A, Point B, Point P);

// function to work out/print the normal equation of a 2d line using two
// points on the line
void normalEquation(Point a, Point b, std::string name1, std::string name2);

// function to work out/print the parametric equation of a line using two
// points on the line
void parametricEquation(Point A, Point B, std::string name1, std::string name2);

#endif
