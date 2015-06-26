#ifndef SPLITLONGLINEARWAYSVISITOR_H
#define SPLITLONGLINEARWAYSVISITOR_H

#include <string>
#include <cassert>

#include <boost/shared_ptr.hpp>

#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

class SplitLongLinearWaysVisitor : public ElementOsmMapVisitor, public Configurable
{
public:

  static std::string className() { return "hoot::SplitLongLinearWaysVisitor"; }

  SplitLongLinearWaysVisitor();

  virtual ~SplitLongLinearWaysVisitor() { }

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setOsmMap(const OsmMap*) { assert(false); }

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual void setConfiguration(const Settings& conf);

  unsigned int getMaxNumberOfNodes() const { return _maxNodesPerWay; }

private:
  unsigned int _maxNodesPerWay;

  // Actual max is 2000, but in order to allow editors to insert nodes without issues,
  //    leaving some breathing room
  static const unsigned int _defaultMaxNodesPerWay = 1900;
};

}

#endif // SPLITLONGLINEARWAYSVISITOR_H
