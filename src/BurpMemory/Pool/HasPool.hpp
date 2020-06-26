#pragma once

#include "../../HasError.hpp"
#include "Interface.hpp"

namespace BurpMemory {
  namespace Pool {

    template <class Type, class Params>
    class HasPool : public HasError {
      
      public:

        HasPool() :
          _pool(nullptr)
        {}

        HasPool(Pool::Interface<Type, Params> * pool) :
          _pool(pool)
        {}

        // We need the this address from the derived class as in the
        // case of multiple inheritance the base class this pointer
        // may be different and we need to return the correct address
        // to the pool
        const Error * free(void * address) {
          if (_pool) {
            const Error * e = _free();
            if (e) setError(*e);
            e = _pool->free(address);
            if (e) setError(*e);
          } else {
            setError(Error::Code::NO_POOL);
          }
          if (hasError) {
            return &error;
          }
          return nullptr;
        }

      protected:

        Pool::Interface<Type, Params> * _pool;

      private:

        virtual const Error * _free() = 0;

    };

  }
}
