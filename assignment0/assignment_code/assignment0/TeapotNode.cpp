//
//  TeapotNode.cpp
//  
//
//  Created by Renee Ge on 9/18/23.
//

#include "TeapotNode.hpp"

#include "glm/gtx/string_cast.hpp"

#include "gloo/shaders/PhongShader.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/CameraComponent.hpp"
#include "gloo/components/LightComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/MeshLoader.hpp"
#include "gloo/lights/PointLight.hpp"
#include "gloo/lights/AmbientLight.hpp"
#include "gloo/cameras/BasicCameraNode.hpp"

#include "gloo/Material.hpp"

#include "gloo/InputManager.hpp"
#include "MeshViewerApp.hpp"



namespace GLOO {
TeapotNode::TeapotNode() {
    // Constructor
    std::shared_ptr<PhongShader> shader = std::make_shared<PhongShader>();
    std::shared_ptr<VertexObject> mesh =
        MeshLoader::Import("assignment0/teapot.obj").vertex_obj;
    if (mesh == nullptr) {
      return;
    }
    auto replace_with_teapot = make_unique<SceneNode>();
    CreateComponent<ShadingComponent>(shader);
    CreateComponent<RenderingComponent>(mesh);
    GetTransform().SetPosition(glm::vec3(0.f, 0.f, 0.f));
    GetTransform().SetRotation(glm::vec3(1.0f, 0.0f, 0.0f), 0.3f);
    
    CreateComponent<MaterialComponent>(
    std::make_shared<Material>(Material::GetDefault()));
    
}

void TeapotNode::ToggleColor() {
    // change color of teapot
    Material& teapot_mat = GetComponentPtr<MaterialComponent>()->GetMaterial();
        
    if (color_toggle_counter % 3 == 0) {
        teapot_mat.SetAmbientColor(glm::vec3(1.0f, 0.1f, 0.1f));
        teapot_mat.SetDiffuseColor(glm::vec3(1.0f, 0.1f, 0.1f));
    }
    
    if (color_toggle_counter % 3 == 1) {
        teapot_mat.SetAmbientColor(glm::vec3(0.1f, 1.0f, 0.1f));
        teapot_mat.SetDiffuseColor(glm::vec3(0.1f, 1.0f, 0.1f));
        
    }
    
    if (color_toggle_counter % 3 == 2) {
        teapot_mat.SetAmbientColor(glm::vec3(0.1f, 0.1f, 1.0f));
        teapot_mat.SetDiffuseColor(glm::vec3(0.1f, 0.1f, 1.0f));
    }
    
    color_toggle_counter++;
}

void TeapotNode::Update(double delta_time) {
    // Update
    
    // Toggle color of teapot
    static bool prev_released = true;
    if (InputManager::GetInstance().IsKeyPressed('C')) {
        if (prev_released) {
            ToggleColor();
        }
        prev_released = false;
    } else if (InputManager::GetInstance().IsKeyReleased('C')) {
        prev_released = true;
    }
}
} // namespace GLOO
