#ifndef VECTORH
#define VECTORH

#include <iostream> // include for ostream/istream operators
#include "Point.h"

class Point; // use forward declaration to use Point in Vector class overloads

class Vector
{
    private:
        

    public:

    float x, y, z, w;
        
        Vector(); // default Vector constructor
        
	// initialise vector with 4 values (default value of w being 0)
        Vector(float X, float Y, float Z, float W = 0); 

	// OVERLOADING ARITHMETIC OPERATORS ON VECTORS

	// overload unary minus operator for -Vector
        Vector operator- (Vector v); 

        // overload addition for Vector + Vector                   
        friend Point operator+ (Vector v, Vector w);

        // overload subtraction for Vector - Vector
        friend Vector operator- (Vector v, Vector w);
        
        // overload multiplication for Vector * scalar
        friend Vector operator* (Vector v, float s);

	// overload multiplication for scalar * Vector
        friend Vector operator* (float s, Vector v);

	// overload index operator to access indexed coordinates
        float operator[](int i);	

        // overload assignment operator
	Vector& operator=(const Vector &v);

	// OVERLOAD IO STREAMS

	// overload ostream operator to print point coordinates to screen
        friend std::ostream &operator<<(std::ostream &output, const Vector &v);
        
        // overload ostream operator to read in values to a Point object
        friend std::istream &operator>>(std::istream &input, Vector &v);

};

// MATHEMATICAL FUNCTIONS ON VECTORS

// return normal vector of a given 2d vector
Vector normal(Vector v);

// return the length of the current vector
float length(Vector v);

Vector normalize(Vector v);

// function to perform a dot product of two vectors
float dot(Vector v, Vector w); 

// function to print calculations for a dot product of two vectors
void dotPrint(Vector v, Vector w, std::string name1, std::string name2); 

// function to perform a cross product of two vectors
Vector cross (Vector v, Vector w);

// function to calculate and print the angle between two vectors
void angleBetweenVectors(Vector v, Vector w, std::string name1, 
					     std::string name2);

// Print Function - used in main.cpp to print multiple vectors side by side
void printVectors(Vector vectors[], int spacing, int size, int dimension);

#endif
