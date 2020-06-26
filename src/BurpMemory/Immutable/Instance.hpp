#pragma once

#include <memory>
#include "Interface.hpp"

namespace BurpMemory {
  namespace Immutable {

    template <class Type, class Params>
    class Instance : public Interface<Type, Params> {

      public:

        const Type * init(const Params & params) override {
          return replace(nullptr, params);
        }

        const Type * replace(const Type * previous, const Params & params) override {
          _current++;
          _current %= 2;
          _uid++;
          const Type * next = new(&(_pair[_current])) Type(params, _uid);
          if (previous) {
            previous->~Type();
          }
          return next;
        }

      private:

        std::allocator<Type> allocator;
        Type * _pair = allocator.allocate(2);
        size_t _uid = 0;
        size_t _current = 1;

    };

  }
}
