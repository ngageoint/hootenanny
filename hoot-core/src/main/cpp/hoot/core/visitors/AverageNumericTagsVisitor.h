/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

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
