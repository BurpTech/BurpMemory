#pragma once

#include <cstddef>
#include <memory>
#include "Interface.hpp"

namespace BurpMemory {
  namespace Stack {

    template <
      class ReturnType,
      class InternalType,
      class Params,
      size_t size,
      bool safe
    >
    class Instance: public Interface<ReturnType, Params> {

      public:

        const Error * push(const Params & params) override {
          if (size == _current) {
            Interface<ReturnType, Params>::setError(Error::Code::MAX_ALLOC);
            return &(Interface<ReturnType, Params>::error);
          }
          void * address = &(_stack[_current++]);
          _usage = _current > _usage ? _current : _usage;
          new(address) InternalType(params);
          return nullptr;
        }

        const Error * pop() override {
          if (safe) {
            if (_current == 0) {
              Interface<ReturnType, Params>::setError(Error::Code::MIN_ALLOC);
              return &(Interface<ReturnType, Params>::error);
            }
          }
          _current--;
          return nullptr;
        }

        const Error * forEach(typename Interface<ReturnType, Params>::f_each each) override {
          for (int i = 0; i < _current; i++) {
            const Error * e = each(_stack[i]);
            if (e) return e;
          }
          return nullptr;
        }

        const size_t currentIndex() override {
          return _current - 1;
        }

        ReturnType * current() override {
          if (safe) {
            if (_current == 0) {
              Interface<ReturnType, Params>::setError(Error::Code::EMPTY_POOL);
              return nullptr;
            }
          }
          return &(_stack[_current - 1]);
        }

        ReturnType * get(const size_t index) override {
          if (safe) {
            if (index >= _current) {
              Interface<ReturnType, Params>::setError(Error::Code::OUT_OF_BOUNDS);
              return nullptr;
            }
          }
          return &(_stack[index]);
        }

        size_t getUsage() override {
          return _usage;
        }

      private:

        std::allocator<InternalType> allocator;
        InternalType * _stack = allocator.allocate(size);
        size_t _current = 0;
        size_t _usage = 0;

    };

  }
}
