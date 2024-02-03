//
//  DirectionalLightNode.hpp
//  
//
//  Created by Renee Ge on 9/20/23.
//

#ifndef DIRECTIONAL_LIGHT_NODE_H_
#define DIRECTIONAL_LIGHT_NODE_H_

#include "gloo/SceneNode.hpp"

namespace GLOO {
class DirectionalLightNode : public SceneNode {
public:
    DirectionalLightNode();
    void Update(double delta_time) override;
};
}

#endif /* DirectionalLightNode_hpp */
