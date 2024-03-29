//
//  EAM.hpp
//  
//
//  Created by Renee Ge on 12/8/23.
//

#ifndef EAM_hpp
#define EAM_hpp

#include <stdio.h>

#include <glm/glm.hpp>

namespace GLOO {
class EAM {
  public:
    EAM();
    EAM(float A, float B, float n);
    float CalcPotential(glm::vec3 pos1, glm::vec3 pos2) const;
    float CalcEmbedding(glm::vec3 pos1, glm::vec3 pos2) const;
    float A_;
    float B_;
    float n_;
  private:
    
};
}  // namespace GLOO

#endif /* EAM_hpp */
