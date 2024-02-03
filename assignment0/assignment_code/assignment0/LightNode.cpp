//
//  LightNode.cpp
//  
//
//  Created by Renee Ge on 9/19/23.
//

#include "LightNode.hpp"

#include "glm/gtx/string_cast.hpp"

#include "gloo/external.hpp"
#include "gloo/shaders/PhongShader.hpp"
#include "gloo/components/LightComponent.hpp"
#include "gloo/lights/PointLight.hpp"
#include "gloo/InputManager.hpp"

namespace GLOO {
LightNode::LightNode() {
    // Constructor

    auto point_light = std::make_shared<PointLight>();
    point_light->SetDiffuseColor(glm::vec3(0.8f, 0.8f, 0.8f));
    point_light->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
    point_light->SetAttenuation(glm::vec3(1.0f, 0.09f, 0.032f));
    
    CreateComponent<LightComponent>(point_light);
    GetTransform().SetPosition(glm::vec3(0.0f, 4.0f, 5.f));

}

void LightNode::MovePointLight(int dir) {
    // moves point light based on indicated direction
    glm::vec3 position_light = GetTransform().GetPosition();
    
    if (dir == 262) {
        //move RIGHT
        GetTransform().SetPosition(position_light + glm::vec3(0.1f, 0.0f, 0.0f));
    }
    else if (dir == 263) {
        // move LEFT
        GetTransform().SetPosition(position_light + glm::vec3(-0.1f, 0.0f, 0.0f));
    }
    else if (dir == 264) {
        // move DOWN
        GetTransform().SetPosition(position_light + glm::vec3(0.0f, -0.1f, 0.0f));
    }
    
    else if (dir == 265) {
        // move UP
        GetTransform().SetPosition(position_light + glm::vec3(0.0f, 0.1f, 0.0f));
    }
}

void LightNode::Update(double delta_time) {
    // Update
    
    // Change position of point light with arrow keys
    static bool prev_released = true;
    if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_RIGHT) || InputManager::GetInstance().IsKeyPressed(GLFW_KEY_LEFT) || InputManager::GetInstance().IsKeyPressed(GLFW_KEY_UP) || InputManager::GetInstance().IsKeyPressed(GLFW_KEY_DOWN)) {
        if (prev_released) {
            if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_RIGHT)) {
                MovePointLight(262);
            }
            else if (InputManager::GetInstance().IsKeyPressed(263)) {
                MovePointLight(263);
            }
            else if (InputManager::GetInstance().IsKeyPressed(264)) {
                MovePointLight(264);
            }
            else if (InputManager::GetInstance().IsKeyPressed(265)) {
                MovePointLight(265);
            }
        }
        prev_released = false;
    } else if (InputManager::GetInstance().IsKeyReleased(262 | 263 | 264 | 265)) {
        prev_released = true;
    }
}
}
