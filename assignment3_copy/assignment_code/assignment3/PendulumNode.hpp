//
//  PendulumNode.hpp
//  
//
//  Created by Renee Ge on 11/1/23.
//

#ifndef PendulumNode_hpp
#define PendulumNode_hpp

#include "gloo/SceneNode.hpp"
#include "gloo/VertexObject.hpp"
#include "gloo/shaders/ShaderProgram.hpp"

#include "ParticleState.hpp"
#include "IntegratorBase.hpp"
#include "IntegratorType.hpp"
#include "PendulumSystem.hpp"

#include <stdio.h>

namespace GLOO {
class PendulumNode : public SceneNode {
 public:
  PendulumNode(ParticleState state, IntegratorType type, float step, bool cloth);
  void Update(double delta_time) override;

 private:
  void InitPendulum();
  void InitCloth();
  int IndexOf(int i, int j);

  std::vector<SceneNode*> spheres_;
  std::shared_ptr<VertexObject> sphere_mesh_;
  std::shared_ptr<VertexObject> segments_;
  std::shared_ptr<ShaderProgram> shader_;
  

  ParticleState reset_;
  ParticleState state_;
  float step_;
  float time_;
  std::unique_ptr<IntegratorBase<PendulumSystem, ParticleState>> integrator_;
  PendulumSystem system_;
};
} // namespace GLOO

#endif /* PendulumNode_hpp */
