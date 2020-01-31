// High level design note
// Our matrix should match the behavior of the glm library.
#ifndef MATRIX4F_H
#define MATRIX4F_H

#include <cmath>
#include <iostream>

// We need to Vector4f header in order to multiply a matrix
// by a vector.
#include "Vector4f.h"

// Matrix 4f represents 4x4 matrices in Math
struct Matrix4f{
private:
    float n[4][4];  // Store each value of the matrix

public:
    Matrix4f() = default;

    // Matrix constructor with 9 scalar values.
    // Values in the constructor represent each row in the matrix
    Matrix4f( float n00, float n01, float n02, float n03,
              float n10, float n11, float n12, float n13,
              float n20, float n21, float n22, float n23,
              float n30, float n31, float n32, float n33){
        
        n[0][0] = n00; n[0][1] = n01; n[0][2] = n02; n[0][3] = n03;
        n[1][0] = n10; n[1][1] = n11; n[1][2] = n12; n[1][3] = n13;
        n[2][0] = n20; n[2][1] = n21; n[2][2] = n22; n[2][3] = n23;
        n[3][0] = n30; n[3][1] = n31; n[3][2] = n32; n[3][3] = n33;
    }

    // Matrix constructor from four vectors.
    // Each vector makes up a column in the matrix
    // Note: 'd' will almost always be 0,0,0,1
    Matrix4f(const Vector4f& a, const Vector4f& b, const Vector4f& c, const Vector4f& d){
      n[0][0] = a.x; n[0][1] = a.y; n[0][2] = a.z; n[0][3] = a.w;
      n[1][0] = b.x; n[1][1] = b.y; n[1][2] = b.z; n[1][3] = b.w;
      n[2][0] = c.x; n[2][1] = c.y; n[2][2] = c.z; n[2][3] = c.w;
      n[3][0] = d.x; n[3][1] = d.y; n[3][2] = d.z; n[3][3] = d.w;
    }

    // Makes the matrix an identity matrix
    void identity() {
      for(int x = 0; x <= 3; ++x) {
        for(int y = 0; y <= 3; ++y) {
          if(x == y) {
            n[x][y] = 1.0f;
          }
          else {
            n[x][y] = 0.0f;
          }
        }
      }
    }

    // Prints the matrix to cout, mainly for debugging
    void printMatrix() {
      std::cout << "Printing matrix: " << std::endl;
      std::cout << "[ " << n[0][0] << " " << n[1][0] << " " << n[2][0] << " " << n[3][0] << " ]" << std::endl;
      std::cout << "[ " << n[0][1] << " " << n[1][1] << " " << n[2][1] << " " << n[3][1] << " ]" << std::endl;
      std::cout << "[ " << n[0][2] << " " << n[1][2] << " " << n[2][2] << " " << n[3][2] << " ]" << std::endl;
      std::cout << "[ " << n[0][3] << " " << n[1][3] << " " << n[2][3] << " " << n[3][3] << " ]" << std::endl;
    }

    // Index operator with two dimensions
    // Example: M(1,2) returns row 2 and column 1 of matrix M.
    float& operator ()(int i, int j){
      return (n[i][j]);
    }

    // Index operator with two dimensions
    // Example: M(1,2) returns row 2 and column 1 of matrix M.
    const float& operator ()(int i, int j) const{
      return (n[i][j]);
    }

    // Return a single  vector from the matrix (column)
    Vector4f& operator [](int j){
      return (*reinterpret_cast<Vector4f *>(n[j]));
    }

    // Return a single  vector from the matrix (column)
    const Vector4f& operator [](int j) const{
      return (*reinterpret_cast<const Vector4f *>(n[j]));
    }

    // Make a matrix rotate about various axis
    Matrix4f MakeRotationX(float t){
        return(Matrix4f(1.0f,0,       0,      0,
                        0,   cos(t),  sin(t), 0,
                        0,   -(sin(t)),cos(t),0,
                        0,   0,        0,     1.0f));
    }
    Matrix4f MakeRotationY(float t){
        return(Matrix4f(cos(t),   0,    -(sin(t)), 0,
                        0,        1.0f, 0,      0,
                        sin(t),   0,    cos(t), 0,
                        0,        0,    0,      1.0f));
    }
    Matrix4f MakeRotationZ(float t){
        return(Matrix4f(cos(t),   sin(t),   0,    0,
                        -(sin(t)),cos(t),   0,    0,
                        0,        0,        1.0f, 0,
                        0,        0,        0,    1.0f)); 
    }
    Matrix4f MakeScale(float sx,float sy, float sz){
      n[0][0] *= sx;
      n[0][1] *= sx;
      n[0][2] *= sx;
      n[0][3] *= sx;

      n[1][0] *= sy;
      n[1][1] *= sy;
      n[1][2] *= sy;
      n[1][3] *= sy;

      n[2][0] *= sz;
      n[2][1] *= sz;
      n[2][2] *= sz;
      n[2][3] *= sz;

    }
};

Vector4f row(Matrix4f m, int i) {
  Vector4f vec(m(0, i), m(1, i), m(2, i), m(3, i));
  return vec;
}

// // Matrix Multiplication
// Matrix4f operator *(const Matrix4f& A, const Matrix4f& B){
//   Matrix4f mat4;
//   Vector4f r;
//   for(int x = 0; x <=3; ++x) {
//     for(int y = 0; y <=3; ++y) {
//       r = row(B, y);
//       mat4[x][y] = Dot(A[y], r);
//     }
//   }

//   return mat4;
// }

// Matrix Multiplication // ALTERNATE VERSION OF MULTIPLICATION BASED ON GLM
Matrix4f operator *(const Matrix4f& A, const Matrix4f& B){
  Matrix4f mat4;
  Vector4f r;
  for(int x = 0; x <=3; ++x) {
    for(int y = 0; y <=3; ++y) {
      mat4[x][y] = A[x][y] * B[x][y];
    }
  }

  return mat4;
}

// Matrix multiply by a vector
Vector4f operator *(const Matrix4f& M, const Vector4f& v){

  Vector4f vec;
  for(int x = 0; x <=3; ++x) {
    vec[x] = Dot(M[x], v);
  }
  return vec;
}

#endif
