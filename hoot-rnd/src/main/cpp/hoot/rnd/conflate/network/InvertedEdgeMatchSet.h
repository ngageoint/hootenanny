#ifndef INVERTEDEDGEMATCHSET_H
#define INVERTEDEDGEMATCHSET_H

#include "EdgeMatchSet.h"

namespace hoot
{

/**
 * Presents the matches in a EdgeMatchSet so that the first string is the second and vice versa.
 */
class InvertedEdgeMatchSet : public EdgeMatchSet
{
public:
  InvertedEdgeMatchSet();

  virtual bool contains(const EdgeMatchPtr &em) const = 0;

  virtual QString toString() const { assert(false); }
};

}

#endif // INVERTEDEDGEMATCHSET_H
