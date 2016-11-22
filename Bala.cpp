#include "Arma.h"
#include "include.h"


Bala::Bala(Arma* arma){
    this->position_bala = glm::vec3(arma->position.x, arma->position.y + 0.9, arma->position.z + 3.0);
}

Bala::~Bala(){};

void Bala::movimentaBala(){
		std::clog<<"posicao z bala: " << position_bala.z <<std::endl;
		position_bala.z -= 0.0001;
		glTranslatef(position_bala.x, position_bala.y, position_bala.z);		
}

bool Bala::checarMovimento(){
	if(position_bala.z > -5)
		return true;
	if(position_bala.z <= -5)
		return false;
}

void Bala::PrintVec3(glm::vec3 v3){

    for (int i = 0; i < 3; ++i)
        {
            std::cout<<v3[i]<<" ";
        }

        std::cout<<std::endl;
}
void Bala::printInfo(){
    std::clog<<"Arma info:"<<std::endl;
    std::clog<<"position: " ;
    this->PrintVec3(this->position_bala);
}
