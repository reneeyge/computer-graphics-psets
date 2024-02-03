//
//  SimpleExampleNode.hpp
//
//
//  Created by Renee Ge on 10/30/23.
//

#ifndef SimpleExampleNode_hpp
#define SimpleExampleNode_hpp

#include <stdio.h>

#include "gloo/SceneNode.hpp"
#include "gloo/VertexObject.hpp"
#include "gloo/shaders/ShaderProgram.hpp"

#include "ParticleState.hpp"
#include "IntegratorBase.hpp"
#include "IntegratorType.hpp"
#include "SimpleBase.hpp"

namespace GLOO {
class SimpleExampleNode : public SceneNode {
public:
    SimpleExampleNode(ParticleState state, IntegratorType type, float step);
    void Update(double delta_time) override;
private:
    void InitSphere();
    SceneNode* sphere_node_;

    std::shared_ptr<VertexObject> mesh_;
    std::shared_ptr<ShaderProgram> shader_;

    ParticleState state_;
    float step_;
    float time_;
    std::unique_ptr<IntegratorBase<SimpleBase, ParticleState>> integrator_;
    SimpleBase system_;
};
} // namespace GLOO

#endif /* SimpleExampleNode_hpp */
