
#ifndef NORMALIZE_ADDRESSES_VISITOR_H
#define NORMALIZE_ADDRESSES_VISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/algorithms/AddressParser.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Normalizes element addresses
 */
class NormalizeAddressesVisitor : public ElementVisitor, public Configurable
{
public:

  static std::string className() { return "hoot::NormalizeAddressesVisitor"; }

  NormalizeAddressesVisitor();

  virtual void setConfiguration(const Settings& conf);

  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const { return "Normalizes element addresses"; }

private:

  AddressParser _addressParser;
};

}

#endif // NORMALIZE_ADDRESSES_VISITOR_H
