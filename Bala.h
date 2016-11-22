#ifndef BALA_H
#define BALA_H

//#define GLM_FORCE_RADIANS
// Header file
#include <glm/fwd.hpp>
// Source file
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>


class Bala
{
public:
    Bala(Arma* arma);
    ~Bala();
    void PrintVec3(glm::vec3 v3);
    void printInfo();
    void movimentaBala();
	bool checarMovimento();
    

//private:   
	glm::vec3 position_bala;
};

#endif
