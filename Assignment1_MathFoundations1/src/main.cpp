// Includes for the assignment
#include "Vector4f.h"
#include "Matrix4f.h"
#include <iostream>

// Tests for comparing our library
// You may compare your operations against the glm library
// which is a well tested library.
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

void printGlmMatrix(glm::mat4 n) {
      std::cout << "Printing matrix: " << std::endl;
      std::cout << "[ " << n[0][0] << " " << n[1][0] << " " << n[2][0] << " " << n[3][0] << " ]" << std::endl;
      std::cout << "[ " << n[0][1] << " " << n[1][1] << " " << n[2][1] << " " << n[3][1] << " ]" << std::endl;
      std::cout << "[ " << n[0][2] << " " << n[1][2] << " " << n[2][2] << " " << n[3][2] << " ]" << std::endl;
      std::cout << "[ " << n[0][3] << " " << n[1][3] << " " << n[2][3] << " " << n[3][3] << " ]" << std::endl;
    }

void printGlmVector(glm::vec4 n){
    std::cout << "Printing Vector:  " << std::endl;
    std::cout << "[ " << n[0] <<  " ]" << std::endl;
    std::cout << "[ " << n[1] <<  " ]" << std::endl;
    std::cout << "[ " << n[2] <<  " ]" << std::endl;
    std::cout << "[ " << n[3] <<  " ]" << std::endl;

}

bool areMatricesEqual(glm::mat4 glmMatrix, Matrix4f myMatrix){
    return glmMatrix[0][0]==myMatrix[0][0] &&
           glmMatrix[0][1]==myMatrix[0][1] &&
           glmMatrix[0][2]==myMatrix[0][2] &&
           glmMatrix[0][3]==myMatrix[0][3] &&
           glmMatrix[1][0]==myMatrix[1][0] &&
           glmMatrix[1][1]==myMatrix[1][1] &&
           glmMatrix[1][2]==myMatrix[1][2] &&
           glmMatrix[1][3]==myMatrix[1][3] &&
           glmMatrix[2][0]==myMatrix[2][0] &&
           glmMatrix[2][1]==myMatrix[2][1] &&
           glmMatrix[2][2]==myMatrix[2][2] &&
           glmMatrix[2][3]==myMatrix[2][3] &&
           glmMatrix[3][0]==myMatrix[3][0] &&
           glmMatrix[3][1]==myMatrix[3][1] &&
           glmMatrix[3][2]==myMatrix[3][2] &&
           glmMatrix[3][3]==myMatrix[3][3];
}

bool areVectorsEqual(glm::vec4 glmVector, Vector4f myVector){
    return glmVector[0] == myVector[0] &&
           glmVector[1] == myVector[1] &&
           glmVector[2] == myVector[2] &&
           glmVector[3] == myVector[3];
}

// ============ MATRIX UNIT TESTS ======================
// Sample unit test comparing against GLM.
bool unitTest0(){
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Matrix4f myIdentity( 1.0f,0,0,0,
        			     0,1.0f,0,0,
		        	     0,0,1.0f,0,
			             0,0,0,1.0f);

    return areMatricesEqual(glmIdentityMatrix, myIdentity);
}

// Tests Matrix4f.identity() against glm
bool unitTest1(){
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Matrix4f myIdentity( 0,0,0,0,
        			     0,0,0,0,
		        	     0,0,0,0,
			             0,0,0,0);
    myIdentity.identity();
    return areMatricesEqual(glmIdentityMatrix, myIdentity);
}

// Tests constructor using 4 vectors
bool unitTest2(){
	glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);
	Vector4f a(1.0f ,0.0f,  0.0f,   0.0f);
	Vector4f b(0.0f ,1.0f,  0.0f,   0.0f);
	Vector4f c(0.0f ,0.0f,  1.0f,   0.0f);
	Vector4f d(0.0f ,0.0f,  0.0f,   1.0f);
	Matrix4f myIdentity(a,b,c,d);
    return areMatricesEqual(glmIdentityMatrix, myIdentity);

}

// Sample unit test comparing against GLM.
bool unitTest3(){
	glm::mat4 glmScale = glm::mat4(1.0f, 1.0f, 2.0f, 1.0f,
                                    0.0f, 2.0f, 0.0f, 3.0f,
                                    1.0f, 2.0f, 3.0f, 0.0f,
                                    0.0f, 0.0f, 2.0f, 1.0f);
    glmScale = glm::scale(glmScale, glm::vec3(2.0f, 2.0f, 2.0f));
	Matrix4f myScaled(1.0f, 1.0f, 2.0f, 1.0f,
                      0.0f, 2.0f, 0.0f, 3.0f,
                      1.0f, 2.0f, 3.0f, 0.0f,
                      0.0f, 0.0f, 2.0f, 1.0f);

    myScaled.MakeScale(2.0f,2.0f,2.0f);

    return areMatricesEqual(glmScale, myScaled);
}

// Sample unit test comparing against GLM.
// Testing operator
bool unitTest4(){
	glm::mat4 glmTest = glm::mat4(1.0f);
    glmTest[1][3] = 72.0f;
    glmTest[2][3] = 2.1f;

	Matrix4f myMatrix(0,0,0,0,
                      0,0,0,0,
                      0,0,0,0,
                      0,0,0,0);

    myMatrix[1][3] = 72.0f;
    myMatrix[2][3] = 2.1f;

    return glmTest[1][3]==myMatrix[1][3] &&
           glmTest[2][3]==myMatrix[2][3];
}

// Sample unit test testing your library
bool unitTest5(){
  Vector4f a(1,1,1,1);
  Vector4f b(0,0,0,0);
  Vector4f c = a + b;

  if(c.x == 1 && c.y == 1 && c.z ==1 && c.w==1){
    return true;
  }
    return false;
}

// Tests glm vec4 against our library
bool unitTest6(){
    Vector4f a(1, 2, 3, 4);
    glm::vec4 b(1, 2, 3,4);

    return areVectorsEqual(b, a);

}

// Tests the [] operator against a glm vector
bool unitTest7(){
    glm::vec4 glmVec(0.0f, 0.0f, 1.0f, 0.0f);

    Vector4f a(1.0f ,0.0f,  0.0f,   0.0f);
	Vector4f b(0.0f ,1.0f,  0.0f,   0.0f);
	Vector4f c(0.0f ,0.0f,  1.0f,   0.0f);
	Vector4f d(0.0f ,0.0f,  0.0f,   1.0f);
	Matrix4f myIdentity(a,b,c,d);

    Vector4f myVector = myIdentity[2];
    return areVectorsEqual(glmVec, myVector);
}

// Tests the rotation matrices X, Y, and Z against glm
bool unitTest8(){
    glm::mat4 glmMatrixX(1.0f);
    glm::mat4 glmMatrixY(1.0f);
    glm::mat4 glmMatrixZ(1.0f);
    glmMatrixX = glm::rotate(glmMatrixX, glm::radians(180.0f), glm::vec3(1, 0, 0));
    glmMatrixY = glm::rotate(glmMatrixY, glm::radians(40.0f), glm::vec3(0, 1, 0));
    glmMatrixZ = glm::rotate(glmMatrixZ, glm::radians(10.0f), glm::vec3(0, 0, 1));

    Vector4f a(1.0f ,0.0f,  0.0f,   0.0f);
	Vector4f b(0.0f ,1.0f,  0.0f,   0.0f);
	Vector4f c(0.0f ,0.0f,  1.0f,   0.0f);
	Vector4f d(0.0f ,0.0f,  0.0f,   1.0f);
	Matrix4f myMatrixX(a,b,c,d);
    Matrix4f myMatrixY(a,b,c,d);
    Matrix4f myMatrixZ(a,b,c,d);

    myMatrixX = myMatrixX.MakeRotationX(glm::radians(180.0f));
    myMatrixY = myMatrixY.MakeRotationY(glm::radians(40.0f));
    myMatrixZ = myMatrixZ.MakeRotationZ(glm::radians(10.0f));

    return areMatricesEqual(glmMatrixX, myMatrixX) && areMatricesEqual(glmMatrixY, myMatrixY) && areMatricesEqual(glmMatrixZ, myMatrixZ);
} 

// TODO: Test a matrix multiplied by another matrix
bool unitTest9(){
    Matrix4f myA(1.0f, 0.0f, 0.0f, 0.0f, 
                0.0f, 1.0f, 0.0f, 0.0f, 
                0.0f, 0.0f, 1.0f, 0.0f, 
                0.0f, 0.0f, 0.0f, 1.0f);

    Matrix4f myB(3.0f, 0.0f, 0.0f, 0.0f, 
                0.0f, 3.0f, 0.0f, 0.0f, 
                0.0f, 0.0f, 3.0f, 0.0f, 
                0.0f, 0.0f, 0.0f, 3.0f);

    glm::mat4 glmA(1.0f);
    glm::mat4 glmB(3.0f);

    myA = myA * myB;
    glmA = glm::matrixCompMult(glmA, glmB);

    return areMatricesEqual(glmA, myA);
}

// Test a matrix multiplied by a vector
bool unitTest10(){

    glm::mat4 glmMatrix(1.0f);
    glm::vec4 vec(4.0, 1.0f, 1.0f, 1.0f);
    Vector4f a(4.0f , 1.0f, 1.0f, 1.0f);

	Matrix4f myMatrix(1.0f, 0.0f, 0.0f, 0.0f, 
                      0.0f, 1.0f, 0.0f, 0.0f, 
                      0.0f, 0.0f, 1.0f, 0.0f, 
                      0.0f, 0.0f, 0.0f, 1.0f);

    Vector4f myVector;
    glm::vec4 glmVector;

    myVector = myMatrix * a;
    glmVector = glmMatrix * vec;

    return areVectorsEqual(glmVector, myVector);

}

// ============ VECTOR UNIT TESTS ======================

// Test our constructor
bool unitTest11(){
    Vector4f myVector(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmVector(1.0f, 2.0f, 3.0f, 4.0f);

    return areVectorsEqual(glmVector, myVector);

}

// Test scalar vector *=
bool unitTest12(){
    Vector4f myVector(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmVector(1.0f, 2.0f, 3.0f, 4.0f);

    myVector *= 5.0f;
    glmVector *= 5.0f;

    return areVectorsEqual(glmVector, myVector);
}

// Test scalar vector /=
bool unitTest13(){
    Vector4f myVector(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmVector(1.0f, 2.0f, 3.0f, 4.0f);

    myVector /= 5.0f;
    glmVector /= 5.0f;

    return areVectorsEqual(glmVector, myVector);
}

// Test vector +=
bool unitTest14(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4f myB(2.0f, 3.0f, 4.0f, 1.0f);
    glm::vec4 glmA(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmB(2.0f, 3.0f, 4.0f, 1.0f);

    myA += myB;
    glmA += glmB;

    return areVectorsEqual(glmA, myA);
}

// Test vector -=
bool unitTest15(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4f myB(2.0f, 3.0f, 4.0f, 1.0f);
    glm::vec4 glmA(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmB(2.0f, 3.0f, 4.0f, 1.0f);

    myA -= myB;
    glmA -= glmB;

    return areVectorsEqual(glmA, myA);
}

// Test vector scalar multiplication
bool unitTest16(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4f myB;
    glm::vec4 glmA(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmB;

    myB = myA * 5.0f;
    glmB = glmA * 5.0f;

    return areVectorsEqual(glmB, myB);
}

// Test vector scalar division
bool unitTest17(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4f myB;
    glm::vec4 glmA(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmB;

    myB = myA / 5.0f;
    glmB = glmA / 5.0f;

    return areVectorsEqual(glmB, myB);
}

// Test vector negation
bool unitTest18(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4f myB;
    glm::vec4 glmA(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmB;

    myB = -myA;
    glmB = -glmA;

    return areVectorsEqual(glmB, myB);
}

// Test vector addition
bool unitTest19(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4f myB(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmA(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmB(1.0f, 2.0f, 3.0f, 4.0f);

    Vector4f myC;
    glm::vec4 glmC;

    myC = myA + myB;
    glmC = glmA + glmB;

    return areVectorsEqual(glmC, myC);
}

// Test vector subtraction
bool unitTest20(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 4.0f);
    Vector4f myB(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmA(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmB(1.0f, 2.0f, 3.0f, 4.0f);

    Vector4f myC;
    glm::vec4 glmC;

    myC = myA - myB;
    glmC = glmA - glmB;

    return areVectorsEqual(glmC, myC);
}

// Test vector magnitude
bool unitTest21(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 4.0f);
    glm::vec4 glmA(1.0f, 2.0f, 3.0f, 4.0f);

    return Magnitude(myA) == glm::length(glmA);
}

// Test normalizing a vector
bool unitTest22(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 4.0f);
    myA = Normalize(myA);
    float mag = Magnitude(myA);

    return (abs(mag - 1.0f)) < 0.0000001f; // The magnitude doesn't evaluate exactly to 1, so we check the delta is in an acceptable range
}

// Test dot product of vector a and vector b
bool unitTest23(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 0.0f);
    Vector4f myB(2.0f, 4.0f, 3.0f, 0.0f);

    glm::vec4 glmA(1.0f, 2.0f, 3.0f, 0.0f);
    glm::vec4 glmB(2.0f, 4.0f, 3.0f, 0.0f);

    float myDot = Dot(myA, myB);
    float glmDot = glm::dot(glmA, glmB);

    return glmDot == myDot;

}

bool unitTest24(){
    Vector4f myA(1.0f, 2.0f, 3.0f, 0.0f);
    Vector4f myB(2.0f, 4.0f, 3.0f, 0.0f);

    myA = CrossProduct(myA, myB);

    glm::vec3 glmA(1.0f, 2.0f, 3.0f);
    glm::vec3 glmB(2.0f, 4.0f, 3.0f);

    glmA = glm::cross(glmA, glmB);

    return glmA[0] == myA[0] &&
            glmA[1] == myA[1] &&
            glmA[2] == myA[2];

}

int main(){
    // TODO:
    // Keep track of the tests passed
    unsigned int testsPassed = 0;
    unsigned int testsRun = 0;

    // Run 'unit tests'
    std::cout << "Passed 0: " << unitTest0() << " \n";
    std::cout << "Passed 1: " << unitTest1() << " \n";
    std::cout << "Passed 2: " << unitTest2() << " \n";
    std::cout << "Passed 3: " << unitTest3() << " \n";
    std::cout << "Passed 4: " << unitTest4() << " \n";
    std::cout << "Passed 5: " << unitTest5() << " \n";
    std::cout << "Passed 6: " << unitTest6() << " \n";
    std::cout << "Passed 7: " << unitTest7() << " \n";
    std::cout << "Passed 8: " << unitTest8() << " \n";
    std::cout << "Passed 9: " << unitTest9() << " \n";
    std::cout << "Passed 10: " << unitTest10() << " \n";
    std::cout << "Passed 11: " << unitTest11() << " \n";
    std::cout << "Passed 12: " << unitTest12() << " \n";
    std::cout << "Passed 13: " << unitTest13() << " \n";
    std::cout << "Passed 14: " << unitTest14() << " \n";
    std::cout << "Passed 15: " << unitTest15() << " \n";
    std::cout << "Passed 16: " << unitTest16() << " \n";
    std::cout << "Passed 17: " << unitTest17() << " \n";
    std::cout << "Passed 18: " << unitTest18() << " \n";
    std::cout << "Passed 19: " << unitTest19() << " \n";
    std::cout << "Passed 20: " << unitTest20() << " \n";
    std::cout << "Passed 21: " << unitTest21() << " \n";
    std::cout << "Passed 22: " << unitTest22() << " \n";
    std::cout << "Passed 23: " << unitTest23() << " \n";
    std::cout << "Passed 24: " << unitTest24() << " \n";




    return 0;
}
