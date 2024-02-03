//
//  DirectionalLightNode.cpp
//  
//
//  Created by Renee Ge on 9/20/23.
//

#include "DirectionalLightNode.hpp"

#include "glm/gtx/string_cast.hpp"

#include "gloo/external.hpp"
#include "gloo/shaders/PhongShader.hpp"
#include "gloo/components/LightComponent.hpp"
#include "gloo/lights/PointLight.hpp"
#include "gloo/InputManager.hpp"
#include "gloo/lights/DirectionalLight.hpp"

namespace GLOO {
DirectionalLightNode::DirectionalLightNode() {
    // Constructor
    
    auto directional_light = std::make_shared<DirectionalLight>();
    directional_light->SetDiffuseColor(glm::vec3(0.8f, 0.8f, 0.8f));
    directional_light->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
    directional_light->SetDirection(glm::vec3(0.f, 4.0f, 5.0f));
    
    CreateComponent<LightComponent>(directional_light);
}

void DirectionalLightNode::Update(double delta_time) {
    // Update
}
}
