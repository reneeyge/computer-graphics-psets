#include "CurveNode.hpp"

#include "gloo/debug/PrimitiveFactory.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/shaders/PhongShader.hpp"
#include "gloo/shaders/SimpleShader.hpp"
#include "gloo/InputManager.hpp"

namespace GLOO {
CurveNode::CurveNode(const glm::mat4x3 control_points, SplineBasis basis_type, bool four_points) {
  // TODO: this node should represent a single spline curve.
  // Think carefully about what data defines a curve and how you can
  // render it.

  sphere_mesh_ = PrimitiveFactory::CreateSphere(0.015f, 25, 25);
  curve_polyline_ = std::make_shared<VertexObject>();
  tangent_line_ = std::make_shared<VertexObject>();
  shader_ = std::make_shared<PhongShader>();
  polyline_shader_ = std::make_shared<SimpleShader>();

    control_points_ = control_points;
    spline_basis_ = basis_type;
    four_points_ = four_points;

    PlotCurve();
    InitCurve();
    PlotControlPoints();
    PlotTangentLine();
}

void CurveNode::Update(double delta_time) {

  if (four_points_) {
  
  static bool prev_released = true;

  if (InputManager::GetInstance().IsKeyPressed('T')) {
    if (prev_released) {
      // TODO: implement toggling spline bases.
        ToggleSplineBasis();
    }
    prev_released = false;
  } else if (InputManager::GetInstance().IsKeyPressed('B')) {
    if (prev_released) {
      // TODO: implement converting conrol point geometry from Bezier to
      // B-Spline basis.
        ConvertGeometry(SplineBasis::BSpline);
        PlotControlPoints();
        PlotCurve();
        PlotTangentLine();
    }
    prev_released = false;
  } else if (InputManager::GetInstance().IsKeyPressed('Z')) {
    if (prev_released) {
      // TODO: implement converting conrol point geometry from B-Spline to
      // Bezier basis.
        ConvertGeometry(SplineBasis::Bezier);
        PlotControlPoints();
        PlotCurve();
        PlotTangentLine();
    }
    prev_released = false;
  } else {
    prev_released = true;
  }
}
}

void CurveNode::ToggleSplineBasis() {
  // TODO: implement toggling between Bezier and B-Spline bases.
    if (spline_basis_ == SplineBasis::Bezier) {
        spline_basis_ = SplineBasis::BSpline;
    }
    else {
        spline_basis_ = SplineBasis::Bezier;
    }
    PlotControlPoints();
    PlotCurve();
    PlotTangentLine();
}

void CurveNode::ConvertGeometry(SplineBasis basis) {
  // TODO: implement converting the control points between bases.
    
    glm::mat4 B;
    glm:: mat4 B_inv;
    if (basis == SplineBasis::Bezier) {
        B = BBspline;
        B_inv = inverse(BBezier);
    }
    else {
        B = BBezier;
        B_inv = inverse(BBspline);
    }
    control_points_ = control_points_ * B * B_inv;
}

CurvePoint CurveNode::EvalCurve(float t) {
  // TODO: implement evaluating the spline curve at parameter value t.
    
    auto curve_point_ = CurvePoint();
    auto monomial = glm::vec4 (1.f, t, pow(t, 2.f), pow(t, 3.f));
    auto velocity = glm::vec4 (0.f, 1.f, 2.f*t, 3*pow(t, 2.f));
    
    if (spline_basis_ == SplineBasis::Bezier) {
        curve_point_.P = control_points_ * (BBezier * monomial);
        curve_point_.T = control_points_ * (BBezier * velocity);
    }
    else {
        curve_point_.P = control_points_ * (BBspline * monomial);
        curve_point_.T = control_points_ * (BBspline * velocity);
    }
    
    return curve_point_;
}

void CurveNode::InitCurve() {
  // TODO: create all of the  nodes and components necessary for rendering the
  // curve, its control points, and its tangent line. You will want to use the
  // VertexObjects and shaders that are initialized in the class constructor.
  
    for (int i = 0; i < 4; i++) {
        auto control_point_node = make_unique<SceneNode>();
        control_point_node->CreateComponent<ShadingComponent>(shader_);
        control_point_node->CreateComponent<RenderingComponent>(sphere_mesh_);

        if (spline_basis_ == SplineBasis::Bezier) {
            glm::vec3 color(1.f, 0.f, 0.f);
            auto material = std::make_shared<Material>(color, color, color, 0);
            control_point_node->CreateComponent<MaterialComponent>(material);
        }
        else {
            glm::vec3 color(0.f, 1.f, 0.f);
            auto material = std::make_shared<Material>(color, color, color, 0);
            control_point_node->CreateComponent<MaterialComponent>(material);
        }
        position_control_.push_back(control_point_node.get());
        AddChild(std::move(control_point_node));
    }

    auto line_node = make_unique<SceneNode>();
    line_node->CreateComponent<ShadingComponent>(polyline_shader_);

    auto& rc = line_node->CreateComponent<RenderingComponent>(curve_polyline_);
    rc.SetDrawMode(DrawMode::Lines);

    glm::vec3 color(1.f, 1.f, 1.f);
    auto material = std::make_shared<Material>(color, color, color, 0);
    line_node->CreateComponent<MaterialComponent>(material);
    AddChild(std::move(line_node));

}

void CurveNode::PlotCurve() {
  // TODO: plot the curve by updating the positions of its VertexObject.
    auto positions = make_unique<PositionArray>();
    auto indices = make_unique<IndexArray>();
    
    for (int i = 0; i < N_SUBDIV_; i++) {
        CurvePoint point = EvalCurve(i/N_SUBDIV_);
        positions->push_back(point.P);
        indices->push_back(i);
        indices->push_back(i + 1);
    }

    CurvePoint last = EvalCurve(1.0);
    positions->push_back(last.P);

    curve_polyline_->UpdatePositions(std::move(positions));
    curve_polyline_->UpdateIndices(std::move(indices));
}

void CurveNode::PlotControlPoints() {
  // TODO: plot the curve control points.
    for (int i = 0; i < 4; i ++) {
        if (spline_basis_ == SplineBasis::Bezier) {
            position_control_[i]->GetComponentPtr<MaterialComponent>()->GetMaterial().SetAmbientColor(glm::vec3(1.f, 0.f, 0.f));
        }
        else {
            position_control_[i]->GetComponentPtr<MaterialComponent>()->GetMaterial().SetAmbientColor(glm::vec3(0.f, 1.f, 0.f));
        }
        position_control_[i]->GetTransform().SetPosition(control_points_[i]);
    }
}

void CurveNode::PlotTangentLine() {
  // TODO: implement plotting a line tangent to the curve.

    CurvePoint intersect = EvalCurve(0.5f);
    glm::vec3 begin = intersect.P - 0.2f * intersect.T;
    glm::vec3 end = intersect.P + 0.2f * intersect.T;

    auto positions = make_unique<PositionArray>();
    positions->push_back(begin);
    positions->push_back(end);

    auto indices = make_unique<IndexArray>();
    indices->push_back(0);
    indices->push_back(1);

    tangent_line_->UpdatePositions(std::move(positions));
    tangent_line_->UpdateIndices(std::move(indices));

    auto shader = std::make_shared<SimpleShader>();

    auto line_node = make_unique<SceneNode>();
    line_node->CreateComponent<ShadingComponent>(shader);

    auto& rc = line_node->CreateComponent<RenderingComponent>(tangent_line_);
    rc.SetDrawMode(DrawMode::Lines);

    glm::vec3 color(1.f, 1.f, 1.f);
    auto material = std::make_shared<Material>(color, color, color, 0);
    line_node->CreateComponent<MaterialComponent>(material);

    AddChild(std::move(line_node));
}
}  // namespace GLOO
