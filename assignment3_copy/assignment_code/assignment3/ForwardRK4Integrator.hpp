//
//  ForwardRK4Integrator.hpp
//  
//
//  Created by Renee Ge on 10/30/23.
//

#ifndef ForwardRK4Integrator_hpp
#define ForwardRK4Integrator_hpp

#include "IntegratorBase.hpp"

#include <stdio.h>

namespace GLOO {
template <class TSystem, class TState>
class ForwardRK4Integrator : public IntegratorBase<TSystem, TState> {
    TState Integrate(const TSystem& system, const TState& state, float start_time, float dt) const override {
        
        TState f_0 = system.ComputeTimeDerivative(state, start_time);
        TState f_1 = system.ComputeTimeDerivative(state + (dt / 2.0) * f_0, start_time + (dt / 2.0));
        TState f_2 = system.ComputeTimeDerivative(state + (dt / 2.0) * f_1, start_time + (dt / 2.0));
        TState f_3 = system.ComputeTimeDerivative(state + dt * f_2, start_time + dt);
        
        TState f = state + (dt / 6.0) * (f_0 + 2 * f_1 + 2 * f_2 + f_3);
        return f;
    }
};
} // namespace GLOO

#endif /* ForwardRK4Integrator_hpp */
