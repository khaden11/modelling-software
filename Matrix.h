#ifndef MATRIXH
#define MATRIXH

#include "Vector.h"

class Point;
class Vector;

class Matrix
{
    private:
	//initialise matrix coordinates array

    public:

    float coords[4][4];

	// default matrix constructor
        Matrix(); 
        
	// initialise matrix using four vectors,one for each row
        Matrix(Vector v, Vector w, Vector x, Vector y);

	// initialise matrix using an array of values
	Matrix(float values[4][4]);

	// function to return the identity matrix        
	static Matrix Identity();

	// returns a row as a vector
	Vector operator[](int i);       

	// returns a column as vector     
	Vector column(int j);

	// overload + operator to add matrices together
	friend Matrix operator+ (Matrix m, Matrix n);

	// overload - to get the negative inverse matrix
	friend Matrix operator- (Matrix m);

	// overload * to multiply a matrix * matrix
	friend Matrix operator* (Matrix m, Matrix n);

	// overload * to multiply a matrix * point
	friend Point operator* (Matrix m, Point &p);

	// overload * to multiply a matrix * vector
	friend Vector operator* (Matrix m, Vector v);
        
        // overload ostream operator to print point coordinates to screen
        friend std::ostream &operator<<(std::ostream &output, const Matrix &m);
             
};

// function to print out multiple matrices
void printMatrices(Matrix matrices[], int spacing, int size, int dimension);

#endif
