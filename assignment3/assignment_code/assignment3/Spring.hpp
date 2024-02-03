//
//  Spring.hpp
//  
//
//  Created by Renee Ge on 10/31/23.
//

#ifndef Spring_hpp
#define Spring_hpp

#include <stdio.h>

namespace GLOO {
struct Spring {
    std::vector<int> sphere_indices;
    float spring_const;
    float rest_length;
};
} // namespace GLOO

#endif /* Spring_hpp */
