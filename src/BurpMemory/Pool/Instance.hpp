#pragma once

#include <cstddef>
#include <memory>
#include "Interface.hpp"

namespace BurpMemory {
  namespace Pool {

    template <class ReturnType, class InternalType, class Params, size_t size, bool safe>
    class Instance: public Interface<ReturnType, Params> {

      public:

        ReturnType * create(const Params & params) override {
          void * address = _alloc();
          if (address) {
            InternalType * ret = new(address) InternalType(this, params);
            return ret;
          }
          return nullptr;
        }

        const Error * free(void * address) override {
          if (safe) {
            if (_currentUsed == 0) {
              Interface<ReturnType, Params>::setError(Error::Code::MIN_ALLOC);
              return &(Interface<ReturnType, Params>::error);
            }

            // check that the address has been allocated.
            const size_t difference = (char *)address - (char *)_pool;
            if (!(
              address >= _pool &&
              address < _pool + (Interface<ReturnType, Params>::highUsed * sizeof(InternalType)) &&
              difference % sizeof(InternalType) == 0
            )) {
              Interface<ReturnType, Params>::setError(Error::Code::NOT_ALLOC);
              return &(Interface<ReturnType, Params>::error);
            }

            // check that the address has not already been freed
            for (int i = 0; i < _currentFree; i++) {
              if (address == _free[i]) {
                Interface<ReturnType, Params>::setError(Error::Code::FREED_ADDRESS);
                return &(Interface<ReturnType, Params>::error);
              }
            }
          }

          _currentUsed--;
          _free[_currentFree++] = address;
          return nullptr;
        }

        size_t getUsage() override {
          return _usage;
        }

      private:

        std::allocator<InternalType> allocator;
        InternalType * _pool = allocator.allocate(size);
        void * _free[size];
        size_t _currentFree = 0;
        size_t _currentUsed = 0;
        size_t _usage = 0;

        void * _alloc() {
          if (_currentUsed == size) {
            Interface<ReturnType, Params>::setError(Error::Code::MAX_ALLOC);
            return nullptr;
          }

          if (_currentFree) {
            // grab a free address
            _currentFree--;
            _currentUsed++;
            return _free[_currentFree];
          }

          // send the next address from the pool
          void * ret = &(_pool[_currentUsed++]);
          _usage = _currentUsed > _usage ? _currentUsed : _usage;
          return ret;
        }

    };

  }
}
