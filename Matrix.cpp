#include "Matrix.h"
#include <iomanip>

using namespace std;

// default constructor
Matrix::Matrix() 
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {     
            coords[i][j] = 0;	//set all coordinates equal to 0 as a default
        }
    }
}

// initialise matrix using vectors for for each row
Matrix::Matrix(Vector v, Vector w, Vector x, Vector y) 
{
    for (int i = 0; i < 4; i++) {  // for each x, y, z and w coordinate
        coords[0][i] = v[i]; 	   // assign the values to the matrix
	coords[1][i] = w[i];
	coords[2][i] = x[i];
	coords[3][i] = y[i];
    }
}

// initialise matrix using an array of values
Matrix::Matrix(float values[4][4])
{
    for (int i = 0; i < 4; ++i)     // set the matrix coordinates to the
        for (int j = 0; j < 4; ++j) // given array
            coords[i][j] = values[i][j]; 
}

// initialise identity matrix
Matrix Matrix::Identity() 
{
    float coords[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
	    if (i == j) {
                coords[i][j] = 1; // set the diagonal to 1
	    }
            if (i != j) {
		coords[i][j] = 0;
 	    }
        }
    }
    return Matrix(coords);
}

// returns a row as a vector with the appropriate coords
Vector Matrix::operator[](int i) {
    return Vector(coords[i][0],coords[i][1],coords[i][2],coords[i][3]); }

// returns a column as vector with the appropriate coords
Vector Matrix::column(int j) {
    return Vector(coords[0][j],coords[1][j],coords[2][j],coords[3][j]); }            

// overload + operator to add matrices together
Matrix operator+ (Matrix m, Matrix n)
{
    float coordArray[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            coordArray[i][j] = m.coords[i][j] + n.coords[i][j];
	}
    }
    return Matrix(coordArray);
}

// overload * to multiply a matrix * matrix
Matrix operator* (Matrix m, Matrix n)
{
    float vals[4][4];
    //Matrix result;
    for (int i = 0; i < 4; i++) {
        // get the current row vector from the matrix
        Vector row = m[i];
        for (int j = 0; j < 4; j++) {
	    // get the current column vector from the matrix
	    Vector column = n.column(j);
	    // get the dot product of that row and the specified point
            vals[i][j] = dot(row,column);
        }
    }
    // Using the calculated values to make the resulting matrix
    Matrix* mat = new Matrix(vals);
    return *mat;
}

// overload * to multiply a matrix * point
Point operator* (Matrix m, Point &p)
{
    float values[4];
    // convert the given point to a vector to run the dot product function
    Vector v = Vector(p[0],p[1],p[2],p[3]);
    for (int i = 0; i < 4; i++) {
	// get the current vector row in the matrix (by the indexing function)
	Vector row = m[i];
	// get the dot product of that row and the specified point
        values[i] = dot(row,v);
    }
    // return the values as a point
    Point* pnt = new Point(values[0],values[1],values[2],values[3]);
    return *pnt;
}

// overload * to multiply a matrix * vector
Vector operator* (Matrix m, Vector v)
{
    float values[4];
    for (int i = 0; i < 4; i++) {
	// get the current vector row in the matrix (using indexing operator)
	Vector row = m[i];
	// get the dot product of that row and the specified point
        values[i] = dot(row,v);
    }
    // return the values as a vector
    Vector* vec = new Vector(values[0],values[1],values[2],values[3]);
    return *vec;
}


// overload - operator to create a negative inverse matrix
Matrix operator- (Matrix m)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m.coords[i][j] = -m.coords[i][j];
	}
    }
    return Matrix(m.coords);
}

// overload ostream operator to print matrix coordinates to screen
std::ostream &operator<<(std::ostream &output, const Matrix &m)
{
    for (int i = 0; i < 4; i++) {
	output << "|";
        for (int j = 0; j < 4; j++) {
            output << setw(5) << m.coords[i][j] << setw(1);
        }
	output << "|\n";
    }
    return output;
}

// function to print out multiple matrices
void printMatrices(Matrix matrices[], int spacing, int size, int dimension)
{
    for (int dimen = 0; dimen < dimension; dimen++) { // size of the matrix
        for (int i = 0; i < size; i++) {    // number of matrices to print
	    cout << "|";
            Matrix matrix = matrices[i];
	    Vector row = matrix[dimen];
	    for (int j = 0; j < 4; j++) {   // print the values of one row
	        cout << setw(5) << row[j] << setw(1);
	    }
	    cout << "|" << string(spacing,' ');
	    if (i == size - 1) cout << "\n";
	}
    }
}
