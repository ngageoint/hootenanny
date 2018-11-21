
#ifndef PHONE_NUMBER_COUNT_VISITOR_H
#define PHONE_NUMBER_COUNT_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/visitors/SingleStatistic.h>
#include <hoot/core/algorithms/PhoneNumberParser.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Counts the number of valid element phone numbers
 */
class PhoneNumberCountVisitor : public ConstElementVisitor, public SingleStatistic,
  public Configurable
{
public:

  static std::string className() { return "hoot::PhoneNumberCountVisitor"; }

  PhoneNumberCountVisitor();

  virtual void setConfiguration(const Settings& conf);

  double getStat() const { return _count; }

  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const
  { return "Counts the number of valid element phone numbers"; }

private:

  int _count;
  PhoneNumberParser _phoneNumberParser;
};

}

#endif // PHONE_NUMBER_COUNT_VISITOR_H
