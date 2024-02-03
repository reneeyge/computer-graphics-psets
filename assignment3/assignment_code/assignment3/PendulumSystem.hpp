//
//  PendulumSystem.hpp
//  
//
//  Created by Renee Ge on 10/31/23.
//

#ifndef PendulumSystem_hpp
#define PendulumSystem_hpp

#include <stdio.h>
#include "ParticleSystemBase.hpp"
#include "ParticleState.hpp"
#include "Spring.hpp"

namespace GLOO {
class PendulumSystem : public ParticleSystemBase {
public:
    std::vector<float> masses;
    std::vector<Spring> springs;
    std::vector<int> fixed;
    float drag_const;
    
    ParticleState ComputeTimeDerivative(const ParticleState& state, float time) const {
        ParticleState f;
        std::vector<glm::vec3> forces;
        std::vector<glm::vec3> accelerations;
        
        for (int i = 0; i < masses.size(); i++) {
            glm::vec3 weight = glm::vec3(0, -1 * masses[i] * 9.81, 0); // grav force
            glm::vec3 drag = -1 * drag_const * state.velocities[i]; // viscous force
            forces.push_back(weight + drag);
        }
        
        for (int i = 0; i < springs.size(); i++) {
            int sphere_1 = springs[i].sphere_indices[0];
            int sphere_2 = springs[i].sphere_indices[1];
            
            float spring_const = springs[i].spring_const;
            float rest_length = springs[i].rest_length;
            
            glm::vec3 d = state.positions[sphere_1] - state.positions[sphere_2];
            float norm_d = glm::length(d);
            
            glm::vec3 spring_force;
            
            if (norm_d == 0) {
                spring_force = glm::vec3(0.f, 0.f, 0.f);
            }
            else {
                spring_force = -1 * spring_const * (norm_d - rest_length) * (d / norm_d);
            }
            
            forces[sphere_1] += spring_force;
            forces[sphere_2] -= spring_force;
        }
        
        for (int i = 0; i < fixed.size(); i++) {
            int sphere_fixed = fixed[i];
            forces[sphere_fixed] = glm::vec3(0.f, 0.f, 0.f); // remove forces from fixed spheres
        }
        
        for (int i = 0; i < forces.size(); i++) {
            accelerations.push_back(forces[i] / masses[i]); // convert forces to accelerations
        }
        
        f.positions = state.velocities;
        f.velocities = accelerations;
        
        return f;
    }
    
    void AddMass(float mass) {
        masses.push_back(mass);
    }
    
    void AddSpring(int sphere_1, int sphere_2, float spring_const, float rest_length) {
        auto spring = make_unique<Spring>();
        spring->sphere_indices = {sphere_1, sphere_2};
        spring->spring_const = spring_const;
        spring->rest_length = rest_length;
        springs.push_back(*spring);
    }
    
    void FixSphere(int sphere_index) {
        fixed.push_back(sphere_index);
    }
    
    void SetDragConst(float drag) {
        drag_const = drag;
    }
};
} // namespace GLOO

#endif /* PendulumSystem_hpp */
