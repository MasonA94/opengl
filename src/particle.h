#ifndef PARTICLE_H
#define PARTICLE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float radius;
    float mass;

    // stores initial values. used for reset function
    glm::vec3 iPosition;
    glm::vec3 iVelocity;
    glm::vec3 iAcceleration;


    void updateParticle(Particle& p, float dt) {
        p.velocity += p.acceleration * dt;
        p.position += p.velocity * dt;

        glm::vec3 boxMin(-3.0f, -3.0f, -3.0f);
        glm::vec3 boxMax( 3.0f,  3.0f, 3.0f);

        // X walls
        if (p.position.x - p.radius < boxMin.x) {
            p.position.x = boxMin.x + p.radius;
            p.velocity.x *= -1.0f;
        }
        if (p.position.x + p.radius > boxMax.x) {
            p.position.x = boxMax.x - p.radius;
            p.velocity.x *= -1.0f;
        }

        // Y walls
        if (p.position.y - p.radius < boxMin.y) {
            p.position.y = boxMin.y + p.radius;
            p.velocity.y *= -1.0f;
        }
        if (p.position.y + p.radius > boxMax.y) {
            p.position.y = boxMax.y - p.radius;
            p.velocity.y *= -1.0f;
        }

        // Z walls
        if (p.position.z - p.radius < boxMin.z) {
            p.position.z = boxMin.z + p.radius;
            p.velocity.z *= -1.0f;
        }
        if (p.position.z + p.radius > boxMax.z) {
            p.position.z = boxMax.z - p.radius;
            p.velocity.z *= -1.0f;
        }
    }

    /*
    void ResetParticle(Particle& p) {
        p.position = iPosition;
        p.velocity = iVelocity;
        p.acceleration = iAcceleration;
    } */

};

#endif
