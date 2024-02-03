//
//  LennardJones.hpp
//  
//
//  Created by Renee Ge on 12/8/23.
//

#ifndef LennardJones_hpp
#define LennardJones_hpp

#include <stdio.h>

#include <glm/glm.hpp>

#include "ParticleState.hpp"

namespace GLOO {
class LennardJones {
  public:
    LennardJones();
    LennardJones(float epsilon, float r_min);
    float CalcForce(glm::vec3 pos1, glm::vec3 pos2) const;
    float CalcEnergy(glm::vec3 pos1, glm::vec3 pos2) const;
  private:
    float epsilon_;
    float r_min_;
};
}  // namespace GLOO

#endif /* LennardJones_hpp */
