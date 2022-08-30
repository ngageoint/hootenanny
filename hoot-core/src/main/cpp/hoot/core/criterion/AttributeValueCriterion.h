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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ATTRIBUTE_VALUE_CRITERION_H
#define ATTRIBUTE_VALUE_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ElementAttributeType.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/NumericComparisonType.h>
#include <hoot/core/util/TextComparisonType.h>

namespace hoot
{

class AttributeValueCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "AttributeValueCriterion"; }

  AttributeValueCriterion();
  AttributeValueCriterion(const ElementAttributeType& attributeType, const QString& comparisonVal,
                          const TextComparisonType& comparisonType);
  AttributeValueCriterion(const ElementAttributeType& attributeType, const double comparisonVal,
                          const NumericComparisonType& comparisonType);
  ~AttributeValueCriterion() override = default;

  /**
   * @see ElementCriterion
   */
  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<AttributeValueCriterion>(); }

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  /**
   * @see ApiEntityInfo
   */
  QString getDescription() const override { return "Identifies element attributes by value"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }

private:

  static int _logWarnCount;

  // attribute to examine
  ElementAttributeType _attributeType;
  // attribute value being compared
  QVariant _comparisonVal;
  // comparison relationship used between read attribute values and _comparisonVal
  int _comparisonType;
  // comparison types are numeric or text
  bool _isNumericComparison;

  bool _satisfiesComparison(const QVariant& val) const;
};

}

#endif // ATTRIBUTE_VALUE_CRITERION_H
