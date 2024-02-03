//
//  TeapotNode.hpp
//  
//
//  Created by Renee Ge on 9/18/23.
//

#ifndef TEAPOT_NODE_H_
#define TEAPOT_NODE_H_

#include "gloo/SceneNode.hpp"

namespace GLOO {
class TeapotNode : public SceneNode {
public:
    TeapotNode();
    void Update(double delta_time) override;
    void ToggleColor();
private:
    int color_toggle_counter = 0;
};
}

#endif /* TeapotNode_hpp */
