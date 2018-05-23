
#ifndef RAILWAYCRITERION_H
#define RAILWAYCRITERION_H

// hoot
#include "ElementCriterion.h"

namespace hoot
{

/**
 * A filter that will keep railways.
 */
class RailwayCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::RailwayCriterion"; }

  RailwayCriterion();

  bool isSatisfied(const boost::shared_ptr<const Element> &e) const;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new RailwayCriterion()); }

  virtual QString getDescription() const { return "Identifies railways"; }

};

}

#endif // RAILWAYCRITERION_H
