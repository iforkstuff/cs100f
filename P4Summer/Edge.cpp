#include "Edge.hpp"

/**
 * @author Kevin
 * Implementation class for the Edge interface.
 */

/** Overload the operator < for the application of priority queue.
 */
bool Edge::operator<(const Edge &other) const {
  return this->cost > other.cost;
}
