#pragma once

namespace BurpMemory {
  namespace Immutable {

    template <class Type, class Params>
    class Interface {

      public:

        virtual const Type * init(const Params & params) = 0;
        virtual const Type * replace(const Type * previous, const Params & params) = 0;

    };

  }
}
