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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SUMNUMERICTAGSVISITOR_H
#define SUMNUMERICTAGSVISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>

#include "SingleStatistic.h"

namespace hoot
{

/**
 * Sums numeric tag values with a specified key
 */
class SumNumericTagsVisitor : public ConstElementVisitor, public SingleStatistic
{
public:

  static std::string className() { return "hoot::SumNumericTagsVisitor"; }

  static unsigned int logWarnCount;

  SumNumericTagsVisitor();
  SumNumericTagsVisitor(const QString key);

  virtual ~SumNumericTagsVisitor() {}

  /**
   * Given a tag key and for all features having the tag, sums the values of those tags.  If
   * the tag value cannot be converted to a number, a warning is logged and the tag is skipped.
   *
   * @param e element to check for tag on
   */
  virtual void visit(const ConstElementPtr& e);

  virtual double getStat() const { return _sum; }

  virtual QString getDescription() const { return "Sums numeric tag values with a specified key"; }

private:

  QString _key;
  long _sum;
};

}

#endif // SUMNUMERICTAGSVISITOR_H
