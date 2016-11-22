#ifndef ARMA_H
#define ARMA_H

//#define GLM_FORCE_RADIANS
// Header file
#include <glm/fwd.hpp>
// Source file
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <stdlib.h>


class Arma
{
public:
     Arma();
    ~Arma();
    //void updatePosition();
    //void regulate();
    void PrintVec3(glm::vec3 v3);
    void printInfo();
    void moveArma(bool flagUp, bool flagDown, bool flagLeft, bool flagRight);
	void rodaArma(bool flagLeft, bool flagRight);
	bool verificaColisao(glm::vec3 menor, glm::vec3 maior);
	
    

//private:   
	glm::vec3 position;
	glm::vec3 rotation;
};

#endif
