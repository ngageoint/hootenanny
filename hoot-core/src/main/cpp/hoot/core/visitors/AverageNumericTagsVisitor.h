
#ifndef AVERAGE_NUMERIC_TAGS_VISITOR_H
#define AVERAGE_NUMERIC_TAGS_VISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/util/Configurable.h>

#include "SingleStatistic.h"

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Sums numeric tag values with a specified key
 *
 * TODO: have this support substrings
 */
class AverageNumericTagsVisitor : public ConstElementVisitor, public SingleStatistic,
  public Configurable
{
public:

  static std::string className() { return "hoot::AverageNumericTagsVisitor"; }

  static unsigned int logWarnCount;

  AverageNumericTagsVisitor();
  AverageNumericTagsVisitor(const QStringList keys);

  virtual ~AverageNumericTagsVisitor() {}

  /**
   * Given a tag key and for all features having the tag, averages the values of those tags.  If
   * the tag value cannot be converted to a number, a warning is logged and the tag is skipped.
   *
   * @param e element to check for tag on
   */
  virtual void visit(const ConstElementPtr& e);

  virtual double getStat() const;

  virtual QString getDescription() const
  { return "Averages numeric tag values with a specified key"; }

  virtual void setConfiguration(const Settings& conf);

private:

  QStringList _keys;
  double _sum;
  long _tagCount;
};

}

#endif // AVERAGE_NUMERIC_TAGS_VISITOR_H
