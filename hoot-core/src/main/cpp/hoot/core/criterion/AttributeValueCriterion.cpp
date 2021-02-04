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
 * @copyright Copyright (C) 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "AttributeValueCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/DateTimeUtils.h>

namespace hoot
{

int AttributeValueCriterion::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ElementCriterion, AttributeValueCriterion)

AttributeValueCriterion::AttributeValueCriterion() :
_comparisonType(0),
_isNumericComparison(false)
{
}

AttributeValueCriterion::AttributeValueCriterion(const ElementAttributeType& attributeType,
                                                 const QString& comparisonVal,
                                                 const TextComparisonType& comparisonType) :
_attributeType(attributeType),
_comparisonVal(comparisonVal),
_comparisonType(comparisonType.getEnum()),
_isNumericComparison(false)
{
  if (_attributeType.getEnum() == ElementAttributeType::Uid ||
      _attributeType.getEnum() == ElementAttributeType::Version)
  {
    throw IllegalArgumentException(
      "Invalid comparison type: textual, with attribute: " + _attributeType.toString());
  }
}

AttributeValueCriterion::AttributeValueCriterion(const ElementAttributeType& attributeType,
                                                 const double comparisonVal,
                                                 const NumericComparisonType& comparisonType) :
_attributeType(attributeType),
_comparisonVal(comparisonVal),
_comparisonType(comparisonType.getEnum()),
_isNumericComparison(true)
{
  if (_attributeType.getEnum() == ElementAttributeType::Timestamp ||
      _attributeType.getEnum() == ElementAttributeType::User)
  {
    throw IllegalArgumentException(
      "Invalid comparison type: numeric, with attribute: " + _attributeType.toString());
  }

  bool ok = false;
  _comparisonVal.toDouble(&ok);
  if (!ok)
  {
    throw IllegalArgumentException(
      "Unable to convert " + _comparisonVal.toString() + " to a numeric value.");
  }
}

void AttributeValueCriterion::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  _attributeType = ElementAttributeType::fromString(configOptions.getAttributeValueCriterionType());
  QString comparisonTypeStr = configOptions.getAttributeValueCriterionComparisonType();
  if (comparisonTypeStr.toLower().startsWith("text"))
  {
    _comparisonType =
       TextComparisonType(
         TextComparisonType::fromString(comparisonTypeStr.remove("text"))).getEnum();
    _isNumericComparison = false;
  }
  else
  {
    _comparisonType =
      NumericComparisonType(
        NumericComparisonType::fromString(comparisonTypeStr.remove("numeric"))).getEnum();
    _isNumericComparison = true;
  }
  _comparisonVal = configOptions.getAttributeValueCriterionComparisonValue();
}

bool AttributeValueCriterion::isSatisfied(const ConstElementPtr& e) const
{
  switch (_attributeType.getEnum())
  {
    case ElementAttributeType::Changeset:
      return _satisfiesComparison(QVariant((qlonglong)e->getChangeset()));
    case ElementAttributeType::Timestamp:
      return _satisfiesComparison(QVariant(DateTimeUtils::toTimeString(e->getTimestamp())));
    case ElementAttributeType::User:
      return _satisfiesComparison(QVariant(e->getUser()));
    case ElementAttributeType::Uid:
      return _satisfiesComparison(QVariant((qlonglong)e->getUid()));
    case ElementAttributeType::Version:
      return _satisfiesComparison(QVariant((qlonglong)e->getVersion()));
    case ElementAttributeType::Id:
      return _satisfiesComparison(QVariant((qlonglong)e->getId()));
    default:
      throw IllegalArgumentException("Invalid attribute type: " + _attributeType.toString());
  }

  return false;
}

bool AttributeValueCriterion::_satisfiesComparison(const QVariant& val) const
{
  if (_isNumericComparison)
  {
    bool ok = false;
    const double numericVal = val.toDouble(&ok);
    LOG_VART(numericVal);
    LOG_VART(_comparisonVal);

    if (!ok)
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Unable to convert " << val.toString() << " to a numeric value.");
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
      return false;
    }

    return
      NumericComparisonType(_comparisonType).satisfiesComparison(
        numericVal, _comparisonVal.toDouble());
  }
  else
  {
    const QString textVal = val.toString();
    switch (_comparisonType)
    {
      case TextComparisonType::EqualTo:
        return textVal == _comparisonVal.toString();
      case TextComparisonType::Contains:
        return textVal.contains(_comparisonVal.toString());
      case TextComparisonType::StartsWith:
        return textVal.startsWith(_comparisonVal.toString());
      case TextComparisonType::EndsWith:
        return textVal.endsWith(_comparisonVal.toString());
      default:
        throw IllegalArgumentException("Invalid comparison type: " + _comparisonType);
    }
  }
}

}
