
#ifndef ADDRESS_COUNT_VISITOR_H
#define ADDRESS_COUNT_VISITOR_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/SingleStatistic.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/AddressParser.h>

namespace hoot
{

/**
 * Counts the number of valid element addresses
 */
class AddressCountVisitor : public ElementOsmMapVisitor, public SingleStatistic, public Configurable
{
public:

  static std::string className() { return "hoot::AddressCountVisitor"; }

  AddressCountVisitor();

  virtual void setConfiguration(const Settings& conf);

  double getStat() const { return _count; }

  virtual void visit(const boost::shared_ptr<Element>& e);

  virtual QString getDescription() const { return "Counts the number of valid element addresses"; }

private:

  int _count;
  AddressParser _addressParser;
};

}

#endif // ADDRESS_COUNT_VISITOR_H
