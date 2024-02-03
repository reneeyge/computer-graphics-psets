//
//  ShadowShader.hpp
//  
//
//  Created by Renee Ge on 11/27/23.
//

#ifndef ShadowShader_hpp
#define ShadowShader_hpp

#include "ShaderProgram.hpp"


#include <stdio.h>

namespace GLOO {
class ShadowShader : public ShaderProgram {
 public:
  ShadowShader();
  void SetTargetNode(const SceneNode& node,
                     const glm::mat4& model_matrix) const override;

  void SetMatrix(const glm::mat4& world_to_light_ndc_matrix) const;

 private:
  void AssociateVertexArray(VertexArray& vertex_array) const;
};
}

#endif /* ShadowShader_hpp */
