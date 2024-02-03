#include "SplineViewerApp.hpp"

#include <fstream>

#include "gloo/cameras/ArcBallCameraNode.hpp"
#include "gloo/lights/AmbientLight.hpp"
#include "gloo/lights/PointLight.hpp"
#include "gloo/components/LightComponent.hpp"

#include "CurveNode.hpp"
#include "PatchNode.hpp"

namespace GLOO {

SplineViewerApp::SplineViewerApp(const std::string& app_name,
                                 glm::ivec2 window_size,
                                 const std::string& filename)
    : Application(app_name, window_size), filename_(filename) {

}

void SplineViewerApp::SetupScene() {
  SceneNode& root = scene_->GetRootNode();

  LoadFile(filename_, root);

  auto camera_node = make_unique<ArcBallCameraNode>();
  scene_->ActivateCamera(camera_node->GetComponentPtr<CameraComponent>());
  root.AddChild(std::move(camera_node));

  auto ambient_light = std::make_shared<AmbientLight>();
  ambient_light->SetAmbientColor(glm::vec3(0.7f));
  root.CreateComponent<LightComponent>(ambient_light);

  auto point_light = std::make_shared<PointLight>();
  point_light->SetDiffuseColor(glm::vec3(0.9f, 0.9f, 0.9f));
  point_light->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
  point_light->SetAttenuation(glm::vec3(1.0f, 0.09f, 0.032f));
  auto point_light_node = make_unique<SceneNode>();
  point_light_node->CreateComponent<LightComponent>(point_light);
  point_light_node->GetTransform().SetPosition(glm::vec3(0.0f, 4.0f, 5.f));
  root.AddChild(std::move(point_light_node));
}

void SplineViewerApp::LoadFile(const std::string& filename, SceneNode& root) {
  std::fstream fs(GetAssetDir() + filename);
  if (!fs) {
    std::cerr << "ERROR: Unable to open file " + filename + "!" << std::endl;
    return;
  }

  std::string spline_type;
  std::getline(fs, spline_type);

  std::vector<glm::vec3> control_points;
  std::string line;
  for (size_t i = 0; std::getline(fs, line); i++) {
    std::stringstream ss(line);
    float x, y, z;
    ss >> x >> y >> z;
    control_points.push_back(glm::vec3(x, y, z));
  }

    four_points = true;
    if (control_points.size() != 4) {
        four_points = false;
    }
    
    if (spline_type == "Bezier curve") {
        for (int i = 0; i < control_points.size() - 3; i+=3) {
            auto control_points_mat = glm::mat4x3 (control_points[i], control_points[i + 1], control_points[i + 2], control_points[i + 3]);
            std::unique_ptr<CurveNode> curve = make_unique<CurveNode>(control_points_mat, SplineBasis::Bezier, four_points);
            root.AddChild(std::move(curve));
        }
    }
    if (spline_type == "B-Spline curve") {
        for (int i = 0; i < control_points.size() - 3; i++) {
            auto control_points_mat = glm::mat4x3 (control_points[i], control_points[i + 1], control_points[i + 2], control_points[i + 3]);
            std::unique_ptr<CurveNode> curve = make_unique<CurveNode>(control_points_mat, SplineBasis::BSpline, four_points);
            root.AddChild(std::move(curve));
        }
    }
    if (spline_type == "Bezier patch") {
        for (int i = 0; i < control_points.size() - 15; i+=16) {
            std::vector<glm::vec3> patch_coords;
            for (int j = 0; j < 16; j++) {
                patch_coords.push_back(control_points[i + j]);
            }
            std::unique_ptr<PatchNode> patch = make_unique<PatchNode>(patch_coords, SplineBasis::Bezier);
            root.AddChild(std::move(patch));
        }
    }
    if (spline_type == "B-Spline patch") {
        for (int i = 0; i < control_points.size() - 15; i+=16) {
            std::vector<glm::vec3> patch_coords;
            for (int j = 0; j < 16; j++) {
                patch_coords.push_back(control_points[i + j]);
            }
            std::unique_ptr<PatchNode> patch = make_unique<PatchNode>(patch_coords, SplineBasis::BSpline);
            root.AddChild(std::move(patch));
        }
    }
}
}  // namespace GLOO
