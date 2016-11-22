#include "Arma.h"
#include "include.h"


Arma::Arma(){
    this->position = glm::vec3(0.0,0.0,0.0);
	this->rotation = glm::vec3(0.0, 0.0, 0.0);
	//this->rotated = false;
}

Arma::~Arma(){};
/*
bool Arma::verificaColisao(glm::vec3 menor, glm::vec3 maior){
	if( (position.x >= menor.x) && (position.x <= maior.x) && (position.y >= menor.y) && (position.y <= maior.y) ){
		std::clog<<" ACERTOU O ALVO NA POSIÇÃO: "<<std::endl;
		std::clog<<" X =  "<< position.x <<std::endl;
		std::clog<<" Y =  "<< position.y <<std::endl;
		std::clog<<"OS LIMITES DO ALVO ERAM: " <<std::endl;
		std::clog<<" Xs =  "<< menor.x <<std::endl;
		std::clog<<" Ys =  "<< menor.y <<std::endl;
		std::clog<<" Xl =  "<< maior.x <<std::endl;
		std::clog<<" Yl =  "<< maior.y <<std::endl;
		std::clog<<" FIM "<<std::endl;
		return true;
	}

	return false;
}*/

void Arma::rodaArma(bool flagLeft, bool flagRight){
	if(flagLeft && rotation.y <= 15.0){
    	rotation.y += 0.3;
    }

    if(flagRight && rotation.y >= -15.0){
        rotation.y -= 0.3;
    }
}

void Arma::moveArma(bool flagUp, bool flagDown, bool flagLeft, bool flagRight){
	
	
	if(flagLeft && position.x > -5.0){
    	position.x -= 0.1;
    }

    if(flagRight && position.x < 5.0){
        position.x += 0.1;
    }
}

void Arma::PrintVec3(glm::vec3 v3){

    for (int i = 0; i < 3; ++i)
        {
            std::cout<<v3[i]<<" ";
        }

        std::cout<<std::endl;
}
void Arma::printInfo(){
    std::clog<<"Arma info:"<<std::endl;
    std::clog<<"position: " ;
    this->PrintVec3(this->position);
}
