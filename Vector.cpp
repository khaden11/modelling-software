#include "Vector.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <math.h>

#define PI 3.14159265

using namespace std;

Vector::Vector() // default constructor
{
    x = 0; y = 0; z = 0; w = 0;
}

Vector::Vector(float X, float Y, float Z, float W) // initialise constructor
{
    x = X; y = Y; z = Z; w = W;
}

// overload unary minus operator for -Vector
Vector operator- (Vector v) {
    return Vector(-v[0],-v[1],-v[2]); }

// overload addition for Vector + Vector
Point operator+ (Vector v, Vector w) {
    return Point(v[0]+w[0], v[1]+w[1], v[2]+w[2]); }

// overload subtraction for Vector - Vector
Vector operator- (Vector v, Vector w) {
    return Vector(v[0]-w[0], v[1]-w[1], v[2]-w[2]); }

// overload multiplication for Vector *  scalar
Vector operator* (Vector v, float s) {
    return Vector(v[0]*s, v[1]*s, v[2]*s); }

// overload multiplication for scalar * Vector
Vector operator* (float s, Vector v) {
    return Vector(s*v[0], s*v[1], s*v[2]); }


// overload indexing operator to return the specified value
float Vector::operator[](int i) 
{
    switch(i)
    {
        case 0: return x; // for the int specified, return the right case
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: // else return a default value
            std::cout << "Error...";
            return x;
    }
}

// overload assignment operator to set Vector coordinates equal to another
Vector& Vector::operator=(const Vector &v) {
    if (this != &v) {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = v.w;
    }
    return *this;
}

// overload ostream operator to print point coordinates to screen
std::ostream &operator<<(std::ostream &output, const Vector &v)
{
    output << "| " << v.x << " |\n| " << v.y << " |\n| " << v.z << " |\n| " 
                                                         << v.w << " |\n\n";
    return output;
}

// overload ostream operator to read in values to a Point object
std::istream &operator>>(std::istream &input, Vector &v)
{
    input >> v.x >> v.y >> v.z >> v.w;
    return input;
}

// return length of a given vector
float length(Vector v) {
    return sqrt(dot(v,v)); }

Vector normalize(Vector v) {
    float lng = length(v);
    return Vector(v[0]/lng, v[1]/lng, v[2]/lng);
}

// return normal of a given 2d vector
Vector normal(Vector v) {
    return Vector(-v[1],v[0],v[2]); }

// function to perform a dot product of two vectors
float dot(Vector v, Vector w) {
    return (v[0]*w[0] + v[1]*w[1] + v[2]*w[2] + v[3]*w[3]); }

// function to print calculations for a dot product of two vectors
void dotPrint(Vector v, Vector w, string name1, string name2)
{
    cout << v[0] << "(" << w[0] << ") + " << v[1] << "(" << w[1] << ") + "
	 << v[2] << "(" << w[2] << ") + " << v[3] << "(" << w[3] << ") = "
         << (v[0]*w[0] + v[1]*w[1] + v[2]*w[2] + v[3]*w[3]) << "\n";
}

// function to perform a cross product of two vectors                          
Vector cross (Vector v, Vector w) {
    return Vector((v[1]*w[2]) - (v[2]*w[1]),  // return vector with the
		  (v[2]*w[0]) - (v[0]*w[2]),  // appropriate values
		  (v[0]*w[1]) - (v[1]*w[0]));
}

// function to calculate and print the angle between two vectors
void angleBetweenVectors(Vector v, Vector w, string name1, string name2)
{
    // calculate cos theta using the equation V.W/(||V||*||W||)
    float cosTheta = (dot(v,w))/(length(v)*length(w));
    // acos the result and convert to degrees
    float result = acos(cosTheta) * (180.0 / PI);
    
    cout << name1 << " and " << name2 << ": acos(" << name1 << " dot " 
	 << name2 << " / ||" << name1 << "||*||" << name2 << "||) = acos(" 
	 << dot(v,w) << "/(" << length(v) << "*" << length(w) << ")) = "
	 << result << "\n";
}

// Print Function - used in main.cpp to print multiple vectors side by side
void printVectors(Vector vectors[], int spacing, int size, int dimension)
{
    for (int i = 0; i < dimension; i++) { // for number of variables in vector
        for (int j = 0; j < size; j++) { // for the number of vectors provided
            Vector vector = vectors[j];
            cout << '|' << setw(4) << vector[i] << setw(2) << '|' 
		 << string(spacing,' '); // print the relevant variable for 
					 // each vector
            if (j == size - 1)
	        cout << "\n";
        }
    }
}
