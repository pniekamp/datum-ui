//
// datum-ui - expression
//

//
// Copyright (c) 2018 Peter Niekamp
//

#pragma once

#include <leap.h>
#include <vector>
#include <cstdint>
#include <cassert>

namespace Ui
{
  //|---------------------- Expression ----------------------------------------
  //|--------------------------------------------------------------------------

  struct Expression
  {
    enum class OpCode : uint8_t
    {
      plus, minus, mod, div, mul, add, sub,
      min, max, abs, floor, ceil, round, trunc, clamp, sin, cos, tan, asin, acos, atan, pow, sqrt, log, exp, log2, exp2, atan2, bnot, leq, geq, le, ge, eq, neq, band, bor, cond,
      literal, intvariable, boolvariable, floatvariable,
    };

    size_t size;
    uint8_t const *data;

    protected:
      Expression() = default;
  };

  template<size_t N>
  struct stack_expression : Expression
  {
    stack_expression()
    {
      size = bytecode.size();
      data = bytecode.data();
    }

    stack_expression(std::initializer_list<uint8_t> v)
      : stack_expression()
    {
      bytecode = ((v.size() <= N) ? leap::gather(v.begin(), leap::make_index_sequence<0, N>()) : throw 0);
    }

    stack_expression(stack_expression const &other)
      : stack_expression()
    {
      bytecode = other.bytecode;
    }

    stack_expression &operator=(stack_expression const &other)
    {
      bytecode = other.bytecode;

      return *this;
    }

    void patch(size_t offset, OpCode op) { replace(offset, &op, 1); }
    void patch(size_t offset, float literal) { patch(offset, OpCode::literal); replace(offset + 1, &literal, sizeof(literal)); }
    void patch(size_t offset, int const *variable) { patch(offset, OpCode::intvariable); replace(offset + 1, &variable, sizeof(variable)); }
    void patch(size_t offset, bool const *variable) { patch(offset, OpCode::boolvariable); replace(offset + 1, &variable, sizeof(variable)); }
    void patch(size_t offset, float const *variable) { patch(offset, OpCode::floatvariable); replace(offset + 1, &variable, sizeof(variable)); }

    private:

      void replace(size_t offset, void const *ptr, size_t n)
      {
        assert(offset >= 0 && offset + n <= N);

        memcpy(bytecode.data() + offset, ptr, n);
      }

      std::array<uint8_t, N> bytecode;
  };

  template<class Alloc>
  struct basic_expression : Expression
  {
    public:

      typedef Alloc allocator_type;

      basic_expression(Alloc const &alloc = Alloc())
        : bytecode(alloc)
      {
        size = 0;
      }

      basic_expression(Expression const &other, Alloc const &alloc = Alloc())
        : bytecode(other.data, other.data + other.size, alloc)
      {
        size = bytecode.size();
        data = bytecode.data();
      }

      basic_expression(uint8_t const *ptr, size_t n, Alloc const &alloc = Alloc())
        : bytecode(ptr, n, alloc)
      {
        size = bytecode.size();
        data = bytecode.data();
      }

      basic_expression(basic_expression const &other)
        : bytecode(other.bytecode)
      {
        size = bytecode.size();
        data = bytecode.data();
      }

      basic_expression(basic_expression const &other, Alloc const &alloc)
        : bytecode(other.bytecode, alloc)
      {
        size = bytecode.size();
        data = bytecode.data();
      }

      basic_expression &operator=(basic_expression const &other)
      {
        bytecode = other.bytecode;

        size = bytecode.size();
        data = bytecode.data();

        return *this;
      }

      void clear() { bytecode.clear(); size = 0; }

      void push(OpCode op) { append(&op, 1); }
      void push(float literal) { push(OpCode::literal); append(&literal, sizeof(literal)); }
      void push(int const *variable) { push(OpCode::intvariable); append(&variable, sizeof(variable)); }
      void push(bool const *variable) { push(OpCode::boolvariable); append(&variable, sizeof(variable)); }
      void push(float const *variable) { push(OpCode::floatvariable); append(&variable, sizeof(variable)); }
      void push(uint8_t const *data, size_t size) { append(data, size); }

      void patch(size_t offset, OpCode op) { replace(offset, &op, 1); }
      void patch(size_t offset, float literal) { patch(offset, OpCode::literal); replace(offset + 1, &literal, sizeof(literal)); }
      void patch(size_t offset, int const *variable) { patch(offset, OpCode::intvariable); replace(offset + 1, &variable, sizeof(variable)); }
      void patch(size_t offset, bool const *variable) { patch(offset, OpCode::boolvariable); replace(offset + 1, &variable, sizeof(variable)); }
      void patch(size_t offset, float const *variable) { patch(offset, OpCode::floatvariable); replace(offset + 1, &variable, sizeof(variable)); }

    private:

      void append(void const *ptr, size_t n)
      {
        bytecode.insert(bytecode.end(), (uint8_t const *)ptr, (uint8_t const *)ptr + n);

        size = bytecode.size();
        data = bytecode.data();
      }

      void replace(size_t offset, void const *ptr, size_t n)
      {
        memcpy(bytecode.data() + offset, ptr, n);
      }

      std::vector<uint8_t, Alloc> bytecode;
  };


  //|////////////////////// evaluate ////////////////////////////////////////
  inline float evaluate(uint8_t const *bytecode, size_t length)
  {
    size_t valuecount = 0;
    std::array<float, 64> valuestack;

    int *int_ptr;
    bool *bool_ptr;
    float *float_ptr;

    for(size_t i = 0; i < length; ++i)
    {
      auto opcode = static_cast<Expression::OpCode>(bytecode[i]);

      if (valuecount >= 1)
      {
        switch (opcode)
        {
          case Expression::OpCode::minus:
            valuestack[valuecount-1] = -valuestack[valuecount-1];
            break;

          case Expression::OpCode::abs:
            valuestack[valuecount-1] = std::abs(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::floor:
            valuestack[valuecount-1] = std::floor(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::ceil:
            valuestack[valuecount-1] = std::ceil(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::round:
            valuestack[valuecount-1] = std::round(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::trunc:
            valuestack[valuecount-1] = std::trunc(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::sin:
            valuestack[valuecount-1] = std::sin(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::cos:
            valuestack[valuecount-1] = std::cos(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::tan:
            valuestack[valuecount-1] = std::tan(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::asin:
            valuestack[valuecount-1] = std::asin(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::acos:
            valuestack[valuecount-1] = std::acos(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::atan:
            valuestack[valuecount-1] = std::atan(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::sqrt:
            valuestack[valuecount-1] = std::sqrt(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::log:
            valuestack[valuecount-1] = std::log(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::exp:
            valuestack[valuecount-1] = std::exp(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::log2:
            valuestack[valuecount-1] = std::log2(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::exp2:
            valuestack[valuecount-1] = std::exp2(valuestack[valuecount-1]);
            break;

          case Expression::OpCode::bnot:
            valuestack[valuecount-1] = leap::fcmp(valuestack[valuecount-1], 0.0f);
            break;

          default:
            break;
        }
      }

      if (valuecount >= 2)
      {
        switch (opcode)
        {
          case Expression::OpCode::mod:
            valuestack[valuecount-2] = std::fmod(valuestack[valuecount-2], valuestack[valuecount-1]);
            valuecount -= 1;
            break;

          case Expression::OpCode::div:
            valuestack[valuecount-2] = valuestack[valuecount-2] / valuestack[valuecount-1];
            valuecount -= 1;
            break;

          case Expression::OpCode::mul:
            valuestack[valuecount-2] = valuestack[valuecount-2] * valuestack[valuecount-1];
            valuecount -= 1;
            break;

          case Expression::OpCode::add:
            valuestack[valuecount-2] = valuestack[valuecount-2] + valuestack[valuecount-1];
            valuecount -= 1;
            break;

          case Expression::OpCode::sub:
            valuestack[valuecount-2] = valuestack[valuecount-2] - valuestack[valuecount-1];
            valuecount -= 1;
            break;

          case Expression::OpCode::min:
            valuestack[valuecount-2] = std::min(valuestack[valuecount-2], valuestack[valuecount-1]);
            valuecount -= 1;
            break;

          case Expression::OpCode::max:
            valuestack[valuecount-2] = std::max(valuestack[valuecount-2], valuestack[valuecount-1]);
            valuecount -= 1;
            break;

          case Expression::OpCode::pow:
            valuestack[valuecount-2] = std::pow(valuestack[valuecount-2], valuestack[valuecount-1]);
            valuecount -= 1;
            break;

          case Expression::OpCode::atan2:
            valuestack[valuecount-2] = std::atan2(valuestack[valuecount-2], valuestack[valuecount-1]);
            valuecount -= 1;
            break;

          case Expression::OpCode::leq:
            valuestack[valuecount-2] = (valuestack[valuecount-2] <= valuestack[valuecount-1]);
            valuecount -= 1;
            break;

          case Expression::OpCode::geq:
            valuestack[valuecount-2] = (valuestack[valuecount-2] >= valuestack[valuecount-1]);
            valuecount -= 1;
            break;

          case Expression::OpCode::le:
            valuestack[valuecount-2] = (valuestack[valuecount-2] < valuestack[valuecount-1]);
            valuecount -= 1;
            break;

          case Expression::OpCode::ge:
            valuestack[valuecount-2] = (valuestack[valuecount-2] > valuestack[valuecount-1]);
            valuecount -= 1;
            break;

          case Expression::OpCode::eq:
            valuestack[valuecount-2] = float(leap::fcmp(valuestack[valuecount-2], valuestack[valuecount-1]));
            valuecount -= 1;
            break;

          case Expression::OpCode::neq:
            valuestack[valuecount-2] = float(!leap::fcmp(valuestack[valuecount-2], valuestack[valuecount-1]));
            valuecount -= 1;
            break;

          case Expression::OpCode::band:
            valuestack[valuecount-2] = float(!leap::fcmp(valuestack[valuecount-2], 0.0f) && !leap::fcmp(valuestack[valuecount-1], 0.0f));
            valuecount -= 1;
            break;

          case Expression::OpCode::bor:
            valuestack[valuecount-2] = float(!leap::fcmp(valuestack[valuecount-2], 0.0f) || !leap::fcmp(valuestack[valuecount-1], 0.0f));
            valuecount -= 1;
            break;

          default:
            break;
        }
      }

      if (valuecount >= 3)
      {
        switch (opcode)
        {
          case Expression::OpCode::clamp:
            valuestack[valuecount-3] = leap::clamp(valuestack[valuecount-3], valuestack[valuecount-2], valuestack[valuecount-1]);
            valuecount -= 2;
            break;

          case Expression::OpCode::cond:
            valuestack[valuecount-3] = leap::fcmp(valuestack[valuecount-3], 0.0f) ? valuestack[valuecount-1] : valuestack[valuecount-2];
            valuecount -= 2;
            break;

          default:
            break;
        }
      }

      switch (opcode)
      {
        case Expression::OpCode::literal:
          memcpy(&valuestack[valuecount++], &bytecode[i+1], sizeof(float));
          i += sizeof(float);
          break;

        case Expression::OpCode::intvariable:
          memcpy(&int_ptr, &bytecode[i+1], sizeof(int*));
          valuestack[valuecount++] = *int_ptr;
          i += sizeof(int*);
          break;

        case Expression::OpCode::boolvariable:
          memcpy(&bool_ptr, &bytecode[i+1], sizeof(bool*));
          valuestack[valuecount++] = *bool_ptr;
          i += sizeof(bool*);
          break;

        case Expression::OpCode::floatvariable:
          memcpy(&float_ptr, &bytecode[i+1], sizeof(float*));
          valuestack[valuecount++] = *float_ptr;
          i += sizeof(float*);
          break;

        default:
          break;
      }
    }

    return valuestack[0];
  }

  inline float evaluate(Expression const &expression)
  {
    return evaluate(expression.data, expression.size);
  }

  inline float evaluate(Expression const *expression)
  {
    return evaluate(expression->data, expression->size);
  }
}
