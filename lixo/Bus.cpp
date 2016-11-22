#include "Bus.h"


Bus::Bus(){
    this->position = glm::vec3(0,0,0);
    this->direction = glm::vec3(0,0,-1);
    this->velocity = glm::vec3(0,0,0);
    this->acceleration = glm::vec3(0,0,0);
    this->acFactor = 0.6;
    this->busAngle = 0.0f;
}

Bus::~Bus(){};

/*
bool Bus::compare(glm::vec3 velocity, float least, float most){ //check if velocity >0 and <4

    if(velocity.x > least && velocity.y > least && velocity.z > least && velocity.x < most && velocity.y <most && velocity.z <most)
        return true;

    return false;
}*/

void Bus::updatePosition(){
        this->position = (this->position + (this->velocity *0.1f) + acceleration*0.005f); //p= p' + v*dt
        this->velocity = this->velocity + this->acceleration * 0.1f;


        if(glm::length(this->velocity) > 0.06f) this->velocity = glm::normalize(this->velocity) * 0.06f;
        //if(this->velocity.z > 0.03f) this->velocity = glm::vec3(0,0, 0.03f);
        //if(this->velocity.z < -0.03f) this->velocity = glm::vec3(0,0, -0.03f);

        printInfo();


}

void Bus::regulate(){
    if(glm::length(this->velocity) > 0.0)
        this->acceleration = glm::normalize(this->velocity) * -0.003f;
        //this->acceleration = glm::vec3(0.0f, 0.0f, -0.003f);
    //if(glm::length(this->velocity) < 0.0)
        //this->acceleration = glm::vec3(0.0f, 0.0f, 0.003f);

    if(glm::length(this->velocity) <= 0.0003f){
    //if(this->velocity.z <= 0.0003f && this->velocity.z >= -0.0003f){
        this->velocity = glm::vec3(0,0,0);
        this->acceleration = glm::vec3(0,0,0);
    }
}

void Bus::PrintVec3(glm::vec3 v3){

    for (int i = 0; i < 3; ++i)
        {
            std::cout<<v3[i]<<" ";
        }

        std::cout<<std::endl;
}
void Bus::printInfo(){
    std::clog<<"Bus info:"<<std::endl;
    std::clog<<"position ";
    this->PrintVec3(this->position);
    std::clog<<"velocity ";
    this->PrintVec3(this->velocity);
    std::clog<<"direction ";
    this->PrintVec3(this->direction);

}
