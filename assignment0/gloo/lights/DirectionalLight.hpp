//
//  DirectionalLight.hpp
//  
//
//  Created by Renee Ge on 9/20/23.
//

#ifndef GLOO_DIR_LIGHT_H_
#define GLOO_DIR_LIGHT_H_

#include "LightBase.hpp"

namespace GLOO {
class DirectionalLight : public LightBase {
public:
    void SetDirection(const glm::vec3& direction) {
        direction_ = direction;
    }
    
    glm::vec3 GetDirection() const {
        return direction_;
    }
    
    LightType GetType() const override {
        return LightType::Directional;
    }
    
private:
    glm::vec3 direction_;
};
}

#endif /* DirectionalLight_hpp */
