//
//  LightNode.hpp
//  
//
//  Created by Renee Ge on 9/19/23.
//

#ifndef LIGHT_NODE_H_
#define LIGHT_NODE_H_

#include "gloo/SceneNode.hpp"
//include gloo/external??

namespace GLOO {
class LightNode : public SceneNode {
public:
    LightNode();
    void Update(double delta_time) override;
    void MovePointLight(int dir);
};
} // namespace GLOO

#endif /* LightNode_hpp */
