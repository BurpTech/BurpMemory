#pragma once

#include <cstddef>
#include "../../HasError.hpp"

namespace BurpMemory {
  namespace Pool {

    template <class Type, class Params>
    class Interface : public HasError {

      public:

        virtual Type * create(const Params & params) = 0;
        virtual const Error * free(void * address) = 0;
        virtual size_t getUsage() = 0;

    };

  }
}
