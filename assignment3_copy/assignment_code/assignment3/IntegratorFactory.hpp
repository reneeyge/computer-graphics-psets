#ifndef INTEGRATOR_FACTORY_H_
#define INTEGRATOR_FACTORY_H_

#include "IntegratorBase.hpp"

#include <stdexcept>

#include "gloo/utils.hpp"

#include "IntegratorType.hpp"

#include "ForwardEulerIntegrator.hpp"
#include "ForwardTrapezoidalIntegrator.hpp"
#include "ForwardRK4Integrator.hpp"

namespace GLOO {
class IntegratorFactory {
 public:
  template <class TSystem, class TState>
  static std::unique_ptr<IntegratorBase<TSystem, TState>> CreateIntegrator(
      IntegratorType type) {
      if (type == IntegratorType::Euler) {
                return make_unique<ForwardEulerIntegrator<TSystem, TState>>();
              }
              else if (type == IntegratorType::Trapezoidal) {
                return make_unique<ForwardTrapezoidalIntegrator<TSystem, TState>>();
              }
              else {
                return make_unique<ForwardRK4Integrator<TSystem, TState>>();
              }
  }
};
}  // namespace GLOO

#endif
