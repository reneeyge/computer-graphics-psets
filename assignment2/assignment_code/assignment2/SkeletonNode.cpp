#include "SkeletonNode.hpp"

#include "gloo/utils.hpp"
#include "gloo/InputManager.hpp"
#include "gloo/MeshLoader.hpp"

#include <fstream> // reading files
#include <string>

#include "gloo/utils.hpp"
#include "gloo/shaders/PhongShader.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/debug/PrimitiveFactory.hpp"

namespace GLOO {
SkeletonNode::SkeletonNode(const std::string& filename)
    : SceneNode(), draw_mode_(DrawMode::Skeleton) {
  
  shader_ = std::make_shared<PhongShader>();
  sphere_mesh_ = PrimitiveFactory::CreateSphere(0.02f, 12, 12);
  cylinder_mesh_ = PrimitiveFactory::CreateCylinder(0.01f, 1.f, 25);
  
  LoadAllFiles(filename);

    // find & save initial B & T matrices
  for (int i = 0; i < joints_list.size(); i++) {
    B_.push_back(glm::inverse(joints_list[i]->GetTransform().GetLocalToWorldMatrix()));
    T_.push_back(joints_list[i]->GetTransform().GetLocalToWorldMatrix());
  }

  DecorateTree();

  // Force initial update.
  OnJointChanged();
}

void SkeletonNode::ToggleDrawMode() {
  draw_mode_ =
      draw_mode_ == DrawMode::Skeleton ? DrawMode::SSD : DrawMode::Skeleton;
    
  if (draw_mode_ == DrawMode::SSD) {
    SSD->SetActive(true);
      
      // take away skeleton
      for (int i = 0; i < joints_list.size(); i++) {
      joints_list[i]->SetActive(false);
      sphere_node_ptrs_[i]->SetActive(false);
    }
    for (int i = 0; i < bones_list.size(); i++) {
      bones_list[i]->SetActive(false);
    }
  }
  else {
    SSD->SetActive(false);
      
      // add skeleton
    for (int i = 0; i < joints_list.size(); i++) {
      joints_list[i]->SetActive(true);
      sphere_node_ptrs_[i]->SetActive(true);
    }
    for (int i = 0; i < bones_list.size(); i++) {
      bones_list[i]->SetActive(true);
    }
  }
  return;
}

void SkeletonNode::DecorateTree() {
  auto material_ = std::make_shared<Material>(Material::GetDefault());

  for (int i = 0; i < joints_list.size(); i++) {
      
      // make spheres
    auto joint = joints_list[i];
    auto sphere_node = make_unique<SceneNode>();
    sphere_node->CreateComponent<ShadingComponent>(shader_);
    sphere_node->CreateComponent<RenderingComponent>(sphere_mesh_);
    sphere_node->CreateComponent<MaterialComponent>(material_);

      // make cylinders
    if (joint->GetParentPtr() != joints_list[0] && i > 0) { // joint's parent is not root node, and joint is not root node
      auto cyl_node = make_unique<SceneNode>();
      cyl_node->CreateComponent<ShadingComponent>(shader_);
      cyl_node->CreateComponent<RenderingComponent>(cylinder_mesh_);
      cyl_node->CreateComponent<MaterialComponent>(material_);
        
        // add cylinders under parent joints
      bones_list.push_back(cyl_node.get());
      joint->GetParentPtr()->AddChild(std::move(cyl_node));
    }
      
      // add spheres to joints
    sphere_node_ptrs_.push_back(sphere_node.get());
    joint->AddChild(std::move(sphere_node));
  }

    // position cylinders correctly
  for (int i = 0; i < bones_list.size(); i++) {
    auto bone = bones_list[i];
    auto parent_joint = bone->GetParentPtr();

    if (parent_joint == joints_list[0]) { // if parent is root
      continue;
    }
      
      // scale bone length
    glm::vec3 parent_to_child = parent_joint->GetChild(0).GetTransform().GetWorldPosition() - parent_joint->GetTransform().GetWorldPosition();
    auto length = glm::length(parent_to_child);
    bone->GetTransform().SetScale(glm::vec3(1.f, length, 1.f));
    
      // find axis -- normal from plane of rotation
    glm::vec3 up = glm::normalize(parent_joint->GetTransform().GetWorldUp());
    glm::vec3 axis = glm::normalize(glm::cross(up, parent_to_child));
      
      // find angle of rotation
    float theta = glm::acos(glm::dot(glm::normalize(parent_to_child), up));
    bone->GetTransform().SetRotation(axis, theta);
  }

    // save positions
  for (int i = 0; i < skin_mesh_->GetPositions().size(); i++) {
    positions_.push_back(skin_mesh_->GetPositions().at(i));
  }

    // get normals for each vertex
  for (int i = 0; i < skin_mesh_->GetIndices().size() - 2; i+=3) {
    int vert_1 = skin_mesh_->GetIndices().at(i);
    int vert_2 = skin_mesh_->GetIndices().at(i + 1);
    int vert_3 = skin_mesh_->GetIndices().at(i + 2);

    glm::vec3 edge_1 = skin_mesh_->GetPositions().at(vert_2) - skin_mesh_->GetPositions().at(vert_1);
    glm::vec3 edge_2 = skin_mesh_->GetPositions().at(vert_3) - skin_mesh_->GetPositions().at(vert_1);
    glm::vec3 face_normal = glm::cross(edge_1, edge_2);

    vertex_norms[vert_1].push_back(face_normal);
    vertex_norms[vert_2].push_back(face_normal);
      vertex_norms[vert_3].push_back(face_normal);
  }
    
    // average normals for each vertex
  auto init_normals = make_unique<NormalArray>();
  for (int i = 0; i < skin_mesh_->GetIndices().size(); i++) {
    int vertex = skin_mesh_->GetIndices().at(i);
    std::vector<glm::vec3> adj_norms = vertex_norms[vertex];
    glm::vec3 sum_norm;
    
      // add all adj norms together & then normalize to find average
      for (int j = 0; j < adj_norms.size(); j++) {
          sum_norm += adj_norms[j];
    }
      
      // update normals
    init_normals->push_back(glm::normalize(sum_norm));
    normals_.push_back(glm::normalize(sum_norm));
  }
    
    // update normals to mesh
    skin_mesh_->UpdateNormals(std::move(init_normals));
    
    // make SSD node
  auto SSD_ptr = make_unique<SceneNode>();
  SSD = SSD_ptr.get();
  SSD->SetActive(false); // initially does not show up
    
    // create SSD components
  SSD->CreateComponent<ShadingComponent>(shader_);
  SSD->CreateComponent<RenderingComponent>(skin_mesh_);
  auto material = CreateComponent<MaterialComponent>(std::make_shared<Material>());
  material.GetMaterial().SetAmbientColor(glm::vec3(1.f, 1.f, 1.f));
  SSD->CreateComponent<MaterialComponent>(material);

    // add SSD as child
  AddChild(std::move(SSD_ptr));
}

void SkeletonNode::Update(double delta_time) {
  // Prevent multiple toggle.
  static bool prev_released = true;
  if (InputManager::GetInstance().IsKeyPressed('S')) {
    if (prev_released) {
      ToggleDrawMode();
    }
    prev_released = false;
  } else if (InputManager::GetInstance().IsKeyReleased('S')) {
    prev_released = true;
  }
}

void SkeletonNode::OnJointChanged() {
    // update joint positions
  for (int i = 0; i < linked_angles_.size(); i++) {
    auto curr_angle = linked_angles_[i];
    auto joint = joints_list[i];
    glm::quat Q = glm::quat(glm::vec3(curr_angle->rx, curr_angle->ry, curr_angle->rz));
    joint->GetTransform().SetRotation(Q);
  }

    // update T matrices given new local frames
  for (int i = 0; i < joints_list.size(); i++) {
    T_[i] = (joints_list[i]->GetTransform().GetLocalToWorldMatrix());
  }

  auto new_positions = make_unique<PositionArray>();
  auto new_normals = make_unique<NormalArray>();

  for (int i = 0; i < skin_mesh_->GetPositions().size(); i++) {
    glm::vec3 p = positions_[i];
    glm::vec4 p_new = glm::vec4(0);
      
    glm::vec3 normal = normals_[i];
    glm::mat4 M = glm::mat4(0);

      // find new point p, M
    for (int j = 0; j < joints_list.size(); j++) {
      p_new += skin_weights_[i][j] * T_[j] * B_[j] * glm::vec4(p[0], p[1], p[2], 1);
      M += skin_weights_[i][j] * T_[j] * B_[j];
    }
      // update positions
    glm::vec3 P = glm::vec3(p_new[0], p_new[1], p_new[2]);
    new_positions->push_back(P);

      // use M to update normals
    M = glm::inverse(glm::transpose(M));
    glm::vec4 n_new = M * glm::vec4(normal[0], normal[1], normal[2], 0);;
    glm::vec3 N = glm::vec3(n_new[0], n_new[1], n_new[2]);
    new_normals->push_back(N);
  }
    
    // update positions and normals to mesh
  skin_mesh_->UpdatePositions(std::move(new_positions));
    skin_mesh_->UpdateNormals(std::move(new_normals));
}

void SkeletonNode::LinkRotationControl(const std::vector<EulerAngle*>& angles) {
  linked_angles_ = angles;
}

void SkeletonNode::LoadSkeletonFile(const std::string& path) {
  std::string line;
  std::fstream file(path);
  while (std::getline(file, line)) {
    std::istringstream s(line);
    float x, y, z;
    int parent_index;
    s >> x >> y >> z >> parent_index;

    auto joint = make_unique<SceneNode>();
    joint->GetTransform().SetPosition(glm::vec3(x, y, z));
    joints_list.push_back(joint.get());
    
    if (parent_index == -1) { // add root node directly under
      AddChild(std::move(joint));
    }
    else { // add child to parent
      auto parent_joint = joints_list[parent_index];
      parent_joint->AddChild(std::move(joint));
    }
  }
}

void SkeletonNode::LoadMeshFile(const std::string& filename) {
  std::shared_ptr<VertexObject> vtx_obj =
      MeshLoader::Import(filename).vertex_obj;
    skin_mesh_ = vtx_obj;
}

void SkeletonNode::LoadAttachmentWeights(const std::string& path) {
  std::string line;
  std::fstream file(path);
  while (std::getline(file, line)) {
    std::stringstream s(line);
      
    std::vector<float> weight_vector;
    weight_vector.push_back(0);
    float w;
    while (s >> w) {
      weight_vector.push_back(w);
    }
    skin_weights_.push_back(weight_vector);
  }
}

void SkeletonNode::LoadAllFiles(const std::string& prefix) {
  std::string prefix_full = GetAssetDir() + prefix;
  LoadSkeletonFile(prefix_full + ".skel");
  LoadMeshFile(prefix + ".obj");
  LoadAttachmentWeights(prefix_full + ".attach");
}
}  // namespace GLOO
