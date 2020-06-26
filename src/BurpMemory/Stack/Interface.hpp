#pragma once

#include <cstddef>
#include <functional>
#include "../../HasError.hpp"

namespace BurpMemory {
  namespace Stack {

    template <class Type, class Params>
    class Interface: public HasError {

      public:

        using f_each = std::function<const Error * (const Type & item)>;

        virtual const Error * push(const Params & params) = 0;
        virtual const Error * pop() = 0;
        virtual const Error * forEach(f_each each) = 0;
        virtual Type * current() = 0;
        virtual const size_t currentIndex() = 0;
        virtual Type * get(const size_t index) = 0;
        virtual size_t getUsage() = 0;

    };

  }
}
