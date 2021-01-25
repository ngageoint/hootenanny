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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef TAGVALUENUMERICRANGECRITERION_H
#define TAGVALUENUMERICRANGECRITERION_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

class Element;

/**
 * Finds numeric tag values falling within a range
 */
class TagValueNumericRangeCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "hoot::TagValueNumericRangeCriterion"; }

  TagValueNumericRangeCriterion();
  TagValueNumericRangeCriterion(const QStringList tagKeys, const long rangeMin,
                                const long rangeMax);
  virtual ~TagValueNumericRangeCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new TagValueNumericRangeCriterion()); }

  void setConfiguration(const Settings& conf);

  virtual QString getDescription() const
  { return "Identifies elements having numeric tag values falling within a range"; }

  virtual QString getName() const override { return className(); }

private:

  QStringList _tagKeys;
  long _rangeMin;
  long _rangeMax;
};

}

#endif // TAGVALUENUMERICRANGECRITERION_H
