#include <exception>
#include <memory>
#include <ostream>
#include <vector>
#include <random>

namespace matrix {

template<typename T>
class M {
 public:
  M() = delete;

  M(size_t m, size_t n) : 
      m_(m),
      n_(n),
    data_(n_ * m_){
  }

  M(std::initializer_list<std::initializer_list<T>> list) :
      M(list.size(), list.begin()->size()) {
    size_t i = 0;
    for (auto i_it = list.begin(); i_it != list.end(); i_it++) {
      if (i_it->size() != n_) {
        throw std::bad_array_new_length();
      }
      size_t j = 0;
      for (auto j_it = i_it->begin(); j_it != i_it->end(); j_it++) {
        elem(i, j) = *j_it;
        j++;
      }
      i++;
    }
  }

  inline const T& elem(size_t i, size_t j) const {
    return data_[i * n_ + j];
  }

  inline T& elem(size_t i, size_t j) {
    return data_[i * n_ + j];
  }

  const size_t m_;
  const size_t n_;

  std::vector<T> data_;
};

// ----------------------------------------------------------------------------
// Operators
// ----------------------------------------------------------------------------
template<typename T>
std::ostream& operator << (std::ostream& os, const M<T>& mat) {
  for (size_t i = 0; i < mat.m_; i++) {
    const char *sep = "";
    for (size_t j = 0; j < mat.n_; j++) {
      os << sep << mat.elem(i, j);
      sep = " "; 
    }
    os << std::endl;
  }
  return os;
}

template<typename T>
M<T> operator *(const M<T> &a, const M<T> &b) {
  if (a.n_ != b.m_) {
    throw std::domain_error("Inner dimensions do not match.");
  }

  M<T> c(a.m_, b.n_);
  mul_00(a, b, c);
  return c;
}

// ----------------------------------------------------------------------------
// Helper functions
// ----------------------------------------------------------------------------
template<typename T>
M<T> make_ident(size_t n) {
  M<T> mat(n, n);
  for (size_t i = 0; i < n; i++) {
    mat.elem(i, i) = 1;
  }
  return mat;
}

template<typename T>
M<T> make_rand(size_t m, size_t n) {
  std::mt19937 mt(0);
  std::uniform_real_distribution<T> dist(-1, 1);
  M<T> x(m, n);
  for (auto &elem : x.data_) {
    elem = dist(mt);
  }
  return x;
}

template<typename T>
void mul_00(const M<T> &a, const M<T> &b, M<T> &c) {
  for (size_t i = 0; i < a.m_; i++) {
    for (size_t j = 0; j < b.n_; j++) {
      T c_ij = 0;
      for (size_t k = 0; k < a.n_; k++) {
        c_ij += a.elem(i, k) * b.elem(k, j);
      }
      c.elem(i, j) = c_ij;
    }
  }
}

template<typename T>
void mul_01(const M<T> &a, const M<T> &b, M<T> &c) {
  std::vector<T> column_b(b.m_);

  for (size_t j = 0; j < b.n_; j++) {
    // Copy column of b into a temporary array.
    for (size_t i = 0; i < b.m_; i++) {
      column_b[i] = b.elem(i, j);
    }

    for (size_t i = 0; i < a.m_; i++) {
      const T *row_a = &a.data_[i * a.n_];

      T c_ij = 0;
      for (size_t k = 0; k < a.n_; k++) {
        c_ij += row_a[k] * column_b[k];
      }
      c.elem(i, j) = c_ij;
    }
  }
}

} // end namespace matrix
