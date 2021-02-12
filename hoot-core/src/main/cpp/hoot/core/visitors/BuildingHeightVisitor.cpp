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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "BuildingHeightVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

int BuildingHeightVisitor::logWarnCount = 0;

HOOT_FACTORY_REGISTER(ElementVisitor, BuildingHeightVisitor)

BuildingHeightVisitor::BuildingHeightVisitor() :
_totalHeight(0.0),
_minHeight(0.0),
_maxHeight(0.0)
{
  _createFeetRegEx();
}

void BuildingHeightVisitor::_createFeetRegEx()
{
  // At least one of feet or inches must be present, so OR together two feet/inch pattern combos
  // where feet is optional in one combo and inches in the other.

  const QString baseFeetPatternStr = "((?<feet>-*\\d+\\.*\\d*)')";
  QString baseFeetPatterStr1 = baseFeetPatternStr;
  baseFeetPatterStr1.replace("feet", "feet1");
  QString baseFeetPatterStr2 = baseFeetPatternStr;
  baseFeetPatterStr2.replace("feet", "feet2");

  const QString baseInchesPatternStr = "((?<inches>-*\\d+\\.*\\d*)\")";
  QString baseInchesPatterStr1 = baseInchesPatternStr;
  baseInchesPatterStr1.replace("inches", "inches1");
  QString baseInchesPatterStr2 = baseInchesPatternStr;
  baseInchesPatterStr2.replace("inches", "inches2");

  const QString patternStr =
    baseFeetPatterStr1 + "\\s*" + baseInchesPatterStr1 + "*|" +
    baseFeetPatterStr2 + "*\\s*" + baseInchesPatterStr2;
  LOG_VART(patternStr);
  _feetRegEx.setPattern(patternStr);
}

void BuildingHeightVisitor::visit(const ConstElementPtr& e)
{
  if (_crit.isSatisfied(e))
  {
    const Meters height = _getHeight(e);
    LOG_VART(height);
    // height = 0 denotes no height tag present. We're only going to return stats for buildings
    // with the tag.
    if (height > 0.0)
    {
      _totalHeight += height;
      if (_minHeight == 0 || height < _minHeight)
      {
        _minHeight = height;
      }
      if (height > _maxHeight)
      {
        _maxHeight = height;
      }
      _numAffected++;
    }
  }
}

Meters BuildingHeightVisitor::_getHeight(const ConstElementPtr& e)
{
  QString heightStr = e->getTags().get("height");
  LOG_VART(heightStr);
  _cleanHeightStr(heightStr);
  LOG_VART(heightStr);
  if (!heightStr.isEmpty())
  {
    Meters height = 0.0;
    const QRegularExpressionMatch feetMatch = _feetRegEx.match(heightStr);
    if (feetMatch.hasMatch())
    {
      // convert feet to meters
      height = _parseFeetVal(heightStr, feetMatch);
    }
    else
    {
      // already in meters
      height = _parseMetersVal(heightStr);
    }
    LOG_VART(height);
    if (height > 0.0)
    {
      LOG_TRACE("Accepted height: " << height << " from input string: " << heightStr);
      LOG_VART(e->getTags().get("name"));
      return height;
    }
    else
    {
      LOG_TRACE("Did not accept height from input string: " << heightStr);
      LOG_VART(e->getTags().get("name"));
    }
  }
  return 0.0;
}

void BuildingHeightVisitor::_cleanHeightStr(QString& heightStr)
{
  heightStr =
    heightStr.toLower()
    .replace(",", ".")    // common decimal format substitution error
    .replace("m", "")     // including 'm' is standard (doesn't always happen)
    .simplified();
}

Meters BuildingHeightVisitor::_parseMetersVal(const QString& heightStr)
{
  bool ok = false;
  const Meters height = heightStr.toDouble(&ok);
  if (ok)
  {
    return height;
  }
  return 0.0;
}

Meters BuildingHeightVisitor::_parseFeetVal(const QString& heightStr,
                                            const QRegularExpressionMatch& regexMatch) const
{
  // don't allow swapping of feet/inches and warn about it; could also handle this in the regex
  const int indexOfFeetSign = heightStr.indexOf("'");
  const int indexOfInchesSign = heightStr.indexOf("\"");
  if (indexOfInchesSign != -1 && indexOfFeetSign != -1 && indexOfInchesSign < indexOfFeetSign)
  {
    _logInvalidFeetHeight(heightStr);
    return 0.0;
  }

  LOG_VART(regexMatch.capturedTexts());
  assert(regexMatch.capturedTexts().size() > 0);

  // figure out which named capture group the feet and/or inches values fell into
  LOG_VART(_feetRegEx.namedCaptureGroups());
  const QString feetCaptureGroupName =
    (_feetRegEx.namedCaptureGroups().contains("feet1") &&
     !regexMatch.captured("feet1").isEmpty()) ? "feet1" : "feet2";
  LOG_VART(feetCaptureGroupName);
  const QString inchesCaptureGroupName =
    (_feetRegEx.namedCaptureGroups().contains("inches1") &&
     !regexMatch.captured("inches1").isEmpty()) ? "inches1" : "inches2";
  LOG_VART(inchesCaptureGroupName);

  // parse the value from the captured string
  bool noParseErrors = true;
  bool successfulParse = true;
  // These shouldn't really ever be in decimal format, but let's support it anyway.
  const double feet = _parseFeetToken(feetCaptureGroupName, regexMatch, successfulParse);
  noParseErrors &= successfulParse;
  const double inches = _parseFeetToken(inchesCaptureGroupName, regexMatch, successfulParse);
  noParseErrors &= successfulParse;
  LOG_VARD(noParseErrors);
  LOG_VART(feet);
  LOG_VART(inches);

  // warn about invalid values
  if (!noParseErrors || feet < 0.0 || inches < 0.0)
  {
    _logInvalidFeetHeight(heightStr);
    return 0.0;
  }

  // return height in meters
  return (feet + (inches / 12)) / 3.2808;
}

Meters BuildingHeightVisitor::_parseFeetToken(const QString& type,
                                              const QRegularExpressionMatch& regexMatch,
                                              bool& successfulParse) const
{
  double val = 0.0;
  LOG_VART(regexMatch.captured(type));
  if (!regexMatch.captured(type).isEmpty())
  {
    val = regexMatch.captured(type).toDouble(&successfulParse);
    if (!successfulParse)
    {
      val = 0.0;
    }
  }
  return val;
}

void BuildingHeightVisitor::_logInvalidFeetHeight(const QString& heightStr) const
{
  if (logWarnCount < Log::getWarnMessageLimit())
  {
    LOG_WARN("Invalid building height in feet: " << heightStr << ".  Skipping height.");
  }
  else if (logWarnCount == Log::getWarnMessageLimit())
  {
    LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
  }
  logWarnCount++;
}

}
