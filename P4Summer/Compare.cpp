#include "Compare.hpp"

bool Compare::operator<(Compare const & o) const {
  return this->dist > o.dist;
}
