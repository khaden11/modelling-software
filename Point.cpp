#include "Point.h"
#include "Vector.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

Point::Point() // default constructor
{
    x = 0; y = 0; z = 0; w = 1;
}

Point::Point(float X, float Y, float Z, float W) // initialise constructor
{
    x = X; y = Y; z = Z; w = W;
}

// overload subtraction for Point - Point
Vector operator-(Point p, Point q) {
    return Vector(p[0]-q[0], p[1]-q[1], p[2]-q[2], p[3]-q[3]); }    

// overload addition for Point + Vector
Point operator+ (Point p, Vector v) {
    return Point(p[0]+v[0], p[1]+v[1], p[2]+v[2], p[3]+v[3]); }

// overload subtraction for Point - Vector
Point operator-(Point p, Vector v) {
    return Point(p[0]-v[0], p[1]-v[1], p[2]-v[2], p[3]-v[3]); }
                            
// overload index operator to access indexed coordinates
float & Point::operator[](int i)
{
    switch(i) // with specified index i, return the right value
    {
        case 0: return x;  
        case 1: return y; 
        case 2: return z;
        case 3: return w;
        default:
            std::cout << "Error...";
            return x; // default return value for invalid i value
    }
}

// overload assignment operator to set Point coordinates equal to another
Point& Point::operator=(const Point &p) {
    if (this != &p) {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
        this->w = p.w;
    }
    return *this;
}

// overload ostream operator to print point coordinates to screen
std::ostream &operator<<(std::ostream &output, const Point &p)
{
    output << "(" << p.x << ", " << p.y << ", " << p.z << ", " << p.w << ")";
    return output;
}

// overload ostream operator to read in values to a Point object
std::istream &operator>>(std::istream &input, Point &p)
{
    input >> p.x >> p.y >> p.z;
    return input;
}

bool operator== (const Point &p, const Point &q)
{
    if (p.x == q.x && p.y == q.y && p.z == q.z) {
	return true; }
    else {
	return false; }
}

// function to print out multiple points side by side
void printPoints(Point points[], int spacing, int size, int dimension)
{
    for (int i = 0; i < dimension; i++) { // for number of variables in vector
        for (int j = 0; j < size; j++) { // for the number of vectors provided
            Point point = points[j];
            cout << '|' << setw(4) << point[i] << setw(2) << '|' 
		 << string(spacing,' '); // print the relevant variable for 
					 // each vector
            if (j == size - 1)
	        cout << "\n";
        }
    }
}

// function to work out and print the normal equation of a 2d line using two
// points on the line
void normalEquation(Point a, Point b, string name1, string name2)
{
    // work out the vector of the line using the two given points
    Vector lineVector = b - a;
    // work out the normal to the line vector
    Vector normalVector = normal(b - a);
    // work out the vector p by subtracting the origin from a point on the line
    Vector p = a - Point(0,0,0);
    // the constant c is the dot product of the normal and the vector p
    float constant = dot(normalVector, p);
    
    // print the workings for the normal equation
    cout << "Vector "<< name1 << name2 << " = "<< name2 << " - "<< name1 
	 << " =     Normal of " << name1 << name2 << " =     Constant = "
	 << "Normal dot "<< name1 <<"\n     ";
    cout << "|" << setw(4) << lineVector[0] << setw(2) << "|                "
	 << "|" << setw(4) << normalVector[0] << setw(2) << "|           "
	 << "|" << setw(4) << normalVector[0] << setw(2) << "| dot |"
	 << setw(4) << a[0] << setw(2) << "| = " << constant << "\n     "
         << "|" << setw(4) << lineVector[1] << setw(2) << "|                "
	 << "|" << setw(4) << normalVector[1] << setw(2) << "|           "
	 << "|" << setw(4) << normalVector[1] << setw(2) << "|     |"
	 << setw(4) << a[1] << setw(2) << "|\n\n";
    // print the normal equation to the screen
    cout << "Normal equation = \n"
	 << "|" << setw(4) << normalVector[0] << setw(2) << "| dot p - "
	 << constant << " = 0\n"
	 << "|" << setw(4) << normalVector[0] << setw(2) << "|\n";
}

// function to work out the parametric equation of a line using two points on
// the line
void parametricEquation(Point A, Point B, string name1, string name2)
{
    // work out the vector of the line using two points on the line
    Vector lineVector = B - A; 	
    // print the working out of the vector and the parametric equation to the
    // screen
    cout << "Vector "<< name1 << name2 << " = "<< name2 << " - "<< name1 
	 << " =     Using point " << name1 << ", parametric equation =\n"
	 << "|" << setw(4) << lineVector[0] << setw(2) << "|                    "
         << "| x(t) | = |" << setw(4) << A[0] << setw(2) << "| + |"
         << setw(4) << lineVector[0] << setw(2) << "| t\n|" << setw(4)
	 << lineVector[1] << setw(2) << "|                    "
         << "| y(t) |   |" << setw(4) << A[1] << setw(2) << "|   |"
         << setw(4) << lineVector[1] << setw(2) << "|  \n";
}


// function to work out whether a point P lies on, to the left or to the right
// of the line AB, given the parameters A, B and P
void leftOrRight(Point A, Point B, Point P)
{
    // calculate the normal vector of the line using the normal function
    Vector normalVector = normal(B - A);
    // calculate the vector of the line from the line to the third point P
    Vector offLine = P - A;
    // use the equation v.w/(||v||*||w||) to get the cosine of the angle
    float cosTheta = dot(normalVector,offLine)/
		(length(normalVector)*length(offLine));
    cout << cosTheta;
    if (cosTheta < 0)
        cout << "\nCosine of Theta > 0 so the point is in the direction of "
	     << "the normal\nThis point is to the right of the line\n\n";
    if (cosTheta > 0)
        cout << "\nCosine of Theta < 0 so the point is in the direction away "
	     << "from the normal\nThis point is to the left of the line\n\n";
    if (cosTheta == 0)
        cout << "\nCosine of Theta = 0 so the point is on the line\n\n";
    
}
