
#ifndef NORMALIZE_ADDRESSES_VISITOR_H
#define NORMALIZE_ADDRESSES_VISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/algorithms/AddressNormalizer.h>

namespace hoot
{

/**
 * Normalizes element addresses
 */
class NormalizeAddressesVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::NormalizeAddressesVisitor"; }

  NormalizeAddressesVisitor();

  virtual void visit(const ElementPtr& e);

  virtual QString getDescription() const { return "Normalizes element addresses"; }

private:

  AddressNormalizer _addressNormalizer;
};

}

#endif // NORMALIZE_ADDRESSES_VISITOR_H
