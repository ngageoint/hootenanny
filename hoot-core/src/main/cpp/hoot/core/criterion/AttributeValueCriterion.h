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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ATTRIBUTE_VALUE_CRITERION_H
#define ATTRIBUTE_VALUE_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ElementAttributeType.h>

namespace hoot
{

/**
 * TODO
 */
class AttributeValueCriterion : public ElementCriterion
{
public:

  static int logWarnCount;

  enum TextualRelationship
  {
    EquivalentTo = 0,
    Contains,
    StartsWith,
    EndsWith
  };

  enum NumericRelationship
  {
    Equals = 0,
    LessThan,
    LessThanOrEqualTo,
    GreaterThan,
    GreaterThanOrEqualTo
  };

  static std::string className() { return "hoot::AttributeValueCriterion"; }

  AttributeValueCriterion();
  AttributeValueCriterion(const ElementAttributeType& attributeType,
                          const QString& comparisonVal,
                          const TextualRelationship& comparisonType);
  AttributeValueCriterion(const ElementAttributeType& attributeType,
                          const double comparisonVal,
                          const NumericRelationship& comparisonType);

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new AttributeValueCriterion()); }

  virtual QString getDescription() const { return "TODO"; }

private:

  ElementAttributeType _attributeType;
  QVariant _comparisonVal;
  int _comparisonType;
  bool _isNumericComparison;

  bool _satisfiesComparison(const QVariant& val) const;
};

}

#endif // ATTRIBUTE_VALUE_CRITERION_H
