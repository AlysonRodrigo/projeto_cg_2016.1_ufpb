#ifndef BUS_H
#define BUS_H

#define GLM_FORCE_RADIANS
// Header file
#include <glm/fwd.hpp>
// Source file
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>


class Bus
{
public:
    Bus();
    ~Bus();
    void updatePosition();
    //bool compare(glm::vec3 velocity, float least, float most);
    void regulate();
    void printInfo();
    void PrintVec3(glm::vec3 v3);

//private:
    glm::vec3 acceleration;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 direction;
    float acFactor;
    float busAngle;
};

#endif
