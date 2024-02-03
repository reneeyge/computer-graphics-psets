//
//  SimpleBase.hpp
//  
//
//  Created by Renee Ge on 10/30/23.
//

#ifndef SimpleBase_hpp
#define SimpleBase_hpp

#include <stdio.h>
#include "ParticleState.hpp"
#include "ParticleSystemBase.hpp"

namespace GLOO {
class SimpleBase : public ParticleSystemBase {
public:
    ParticleState ComputeTimeDerivative(const ParticleState& state, float time) const {
        ParticleState f;
        f.positions.push_back(glm::vec3(0, 0, 0)); // set to arbitrary values
        f.velocities.push_back(glm::vec3(0, 0, 0));
        f.positions[0].x = -1 * state.positions[0].y;
        f.positions[0].y = state.positions[0].x;
        f.positions[0].z = 0;
        return f; // derivative for circular motion
    }
};
}

#endif /* SimpleBase_hpp */
