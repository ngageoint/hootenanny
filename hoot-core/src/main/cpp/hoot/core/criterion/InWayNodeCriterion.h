
#ifndef INWAYNODECRITERION_H
#define INWAYNODECRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>

// Standard
#include <set>
#include <vector>

namespace hoot
{

class OsmMap;

/**
 * Identifies nodes contained in a specified set of ways
 */
class InWayNodeCriterion : public ElementCriterion
{
public:

  static std::string className() { return "hoot::InWayNodeCriterion"; }

  InWayNodeCriterion() {}
  InWayNodeCriterion(const OsmMap& map, const std::vector<long>& wayIds);

  virtual bool isSatisfied(const boost::shared_ptr<const Element> &e) const;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new InWayNodeCriterion()); }

  virtual QString getDescription() const
  { return "Identifies nodes contained in a specified set of ways"; }

private:

  std::set<long> _nids;
};

}

#endif // INWAYNODECRITERION_H
