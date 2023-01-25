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
 * @copyright Copyright (C) 2020, 2021, 2022, 2023 Maxar (http://www.maxar.com/)
 */

#include "RoadCrossingPolyRule.h"

// Hoot
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/SpatialIndexer.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Boost
#include <boost/bind.hpp>

// Standard
#include <functional>

namespace hoot
{

RoadCrossingPolyRule::RoadCrossingPolyRule(ConstOsmMapPtr map)
  : _map(map)
{
}

RoadCrossingPolyRule::RoadCrossingPolyRule(RoadCrossingPolyRule && rule) noexcept
  : _map(rule._map),
    _name(rule._name),
    _polyFilterStr(rule._polyFilterStr),
    _polyFilter(rule._polyFilter),
    _allowedRoadTagFilterStr(rule._allowedRoadTagFilterStr),
    _allowedRoadTagFilter(rule._allowedRoadTagFilter),
    _index(rule._index),
    _indexToEid(rule._indexToEid)
{
  rule._map.reset();
  rule._name.clear();
  rule._polyFilterStr.clear();
  rule._polyFilter.reset();
  rule._allowedRoadTagFilterStr.clear();
  rule._allowedRoadTagFilter.reset();
  rule._index.reset();
  rule._indexToEid.clear();
}

bool RoadCrossingPolyRule::operator<(const RoadCrossingPolyRule& other) const
{
  return getName() < other.getName();
}

QList<RoadCrossingPolyRule> RoadCrossingPolyRule::readRules(const QString& rulesFile, ConstOsmMapPtr map)
{ 
  QFileInfo rulesFileInfo(rulesFile);
  if (!rulesFileInfo.exists())
    throw IllegalArgumentException("Road crossing polygon rules file does not exist.");

  LOG_INFO("Creating road crossing polygon rules...");

  boost::property_tree::ptree propTree;
  try
  {
    boost::property_tree::read_json(rulesFile.toStdString(), propTree);
  }
  catch (const boost::property_tree::json_parser::json_parser_error& e)
  {
    throw HootException(
      QString("Error parsing JSON: %1 (line %2)")
        .arg(QString::fromStdString(e.message()))
        .arg(QString::number(e.line())));
  }
  catch (const std::exception& e)
  {
    const QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }

  QList<RoadCrossingPolyRule> rules;
  for (const auto& ruleProp : propTree.get_child("rules"))
  {
    const QString ruleName = QString::fromStdString(ruleProp.second.get<std::string>("name", "")).trimmed();
    LOG_VART(ruleName);
    if (ruleName.isEmpty())
      throw IllegalArgumentException("A road crossing rule must have a name.");

    // Each rule must have at least one criteria or tag filter and may have both. They define the
    // polygons we search for around roads.

    boost::optional<std::string> polyCriteriaFilterProp = ruleProp.second.get_optional<std::string>("polyCriteriaFilter");
    QString polyCriteriaFilterStr;
    if (polyCriteriaFilterProp)
      polyCriteriaFilterStr = QString::fromStdString(polyCriteriaFilterProp.get()).trimmed();
    LOG_VART(polyCriteriaFilterStr);

    boost::optional<std::string> polyTagFilterProp = ruleProp.second.get_optional<std::string>("polyTagFilter");
    QString polyTagFilterStr;
    if (polyTagFilterProp)
      polyTagFilterStr = QString::fromStdString(polyTagFilterProp.get()).trimmed();
    LOG_VART(polyTagFilterStr);

    if (polyCriteriaFilterStr.isEmpty() && polyTagFilterStr.isEmpty())
    {
      throw IllegalArgumentException(
        "A road crossing rule must specify either a polygon criteria filter (polyCriteriaFilter) "
        "or a polygon tag filter (polyTagFilter).");
    }

    // Allowed road tag filters are optional, and they define which roads we skip indexing per rule
    // (exempt from being flagged for review).

    boost::optional<std::string> allowedRoadTagFilterProp = ruleProp.second.get_optional<std::string>("allowedRoadTagFilter");
    QString allowedRoadTagFilterStr;
    if (allowedRoadTagFilterProp)
      allowedRoadTagFilterStr = QString::fromStdString(allowedRoadTagFilterProp.get()).trimmed();
    LOG_VART(allowedRoadTagFilterStr);

    RoadCrossingPolyRule rule(map);
    rule.setName(ruleName);
    // store the raw poly filter string for review display purposes
    rule.setPolyFilterString("poly criteria filter: " + polyCriteriaFilterStr + "; poly tag filter: " + polyTagFilterStr);
    rule.setPolyFilter(RoadCrossingPolyRule::polyRuleFilterStringsToFilter(polyCriteriaFilterStr, polyTagFilterStr));
    LOG_VART(rule.getPolyFilter());
    // store the raw tag filter string for review display purposes
    rule.setAllowedRoadTagFilterString("allowed road tag filter: " + allowedRoadTagFilterStr);
    rule.setAllowedRoadTagFilter(RoadCrossingPolyRule::tagRuleStringToFilter(allowedRoadTagFilterStr, QStringList("highway")));
    if (rule.getAllowedRoadTagFilter())
    {
      LOG_VART(rule.getAllowedRoadTagFilter());
    }
    rule.createIndex();
    rules.append(rule);
  }
  qSort(rules);

  LOG_VARD(rules.size());
  return rules;
}

ElementCriterionPtr RoadCrossingPolyRule::polyRuleFilterStringsToFilter(const QString& polyCriteriaFilterStr, const QString& polyTagFilterStr)
{
  LOG_VART(polyCriteriaFilterStr);
  LOG_VART(polyTagFilterStr);

  if (polyCriteriaFilterStr.isEmpty() && polyTagFilterStr.isEmpty())
  {
    throw IllegalArgumentException(
      "A road crossing rule must specify either a polygon criteria filter (polyCriteriaFilter) "
      "or a polygon tag filter (polyTagFilter).");
  }

  // logically OR each type criteria filter together

  std::shared_ptr<OrCriterion> polyCriteriaFilter;
  if (!polyCriteriaFilterStr.isEmpty())
  {
    polyCriteriaFilter = std::make_shared<OrCriterion>();

    const QStringList critStrParts = polyCriteriaFilterStr.split(";");
    LOG_VART(critStrParts.size());
    for (const auto& critPart : qAsConst(critStrParts))
    {
      LOG_VART(critPart);
      polyCriteriaFilter->addCriterion(Factory::getInstance().constructObject<ElementCriterion>(critPart.trimmed()));
    }
  }

  // logically OR each tag filter together

  ElementCriterionPtr polyTagFilter;
  if (!polyTagFilterStr.isEmpty())
  {
    if (!polyTagFilterStr.contains("="))
    {
      throw IllegalArgumentException(
        "A road crossing rule polygon tag filter must be of the form "
        "<key1>=<value1>;<key2>=<value2>...");
    }
    else
      polyTagFilter = tagRuleStringToFilter(polyTagFilterStr);
  }

  if (polyCriteriaFilter && polyTagFilter)  // Logically AND the type and tag filters together to get the final poly filter.
    return std::make_shared<ChainCriterion>(polyCriteriaFilter, polyTagFilter);
  else if (polyCriteriaFilter)
    return polyCriteriaFilter;
  else
    return polyTagFilter;
}

ElementCriterionPtr RoadCrossingPolyRule::tagRuleStringToFilter(const QString& kvpStr, const QStringList& allowedKeys)
{
  LOG_VART(kvpStr);
  const QString kvpFormatErrMsg = "A road crossing rule tag filter must be of the form <key1>=<value1>;<key2>=<value2>...";
  if (kvpStr.trimmed().isEmpty()) // empty crit is the same as no filter
    return ElementCriterionPtr();
  else if (!kvpStr.contains("="))
    throw IllegalArgumentException(kvpFormatErrMsg);

  std::shared_ptr<OrCriterion> crit = std::make_shared<OrCriterion>();

  const QStringList kvpStrParts = kvpStr.split(";");
  LOG_VART(kvpStrParts.size());
  for (const auto& part : qAsConst(kvpStrParts))
  {
    LOG_VART(part);
    const QStringList filterStrParts = part.split("=");
    LOG_VART(filterStrParts);
    if (filterStrParts.size() != 2)
      throw IllegalArgumentException(kvpFormatErrMsg);

    const QString key = filterStrParts[0].trimmed();
    const QString val = filterStrParts[1].trimmed();

    if (!key.isEmpty() && !allowedKeys.isEmpty() && !allowedKeys.contains(key))
    {
      throw IllegalArgumentException(
        "Specified tag rule: " + key + "=" + val + " must have one of the following keys: " +
        allowedKeys.join(";"));
    }

    if (val == "*") // This allows for wildcard values (not keys).
      crit->addCriterion(std::make_shared<TagKeyCriterion>(key));
    else
      crit->addCriterion(std::make_shared<TagCriterion>(key, val));
  }
  return crit;
}

void RoadCrossingPolyRule::createIndex()
{
  LOG_INFO("\tCreating roads crossing polys index for rule: " << _name << "...");

  // create an index for all roads and all polys that satisfy our crit within the default
  // search radius

  // No tuning was done, just copied these settings from OsmMapIndex. 10 children - 368 - see #3054
  _index = std::make_shared<Tgs::HilbertRTree>(std::make_shared<Tgs::MemoryPageStore>(728), 2);

  // Only index elements that satisfy isMatchCandidate.
  std::function<bool (ConstElementPtr e)> f = std::bind(&RoadCrossingPolyRule::_isMatchCandidate, this, std::placeholders::_1);
  std::shared_ptr<ArbitraryCriterion> pCrit = std::make_shared<ArbitraryCriterion>(f);

  SpatialIndexer v(
    _index, _indexToEid, pCrit,
    std::bind(&RoadCrossingPolyRule::_getSearchRadius, this, std::placeholders::_1), _map);
  _map->visitRo(v);
  v.finalizeIndex();

  LOG_DEBUG(
    "\tRoads crossing polys feature index for rule: " << _name << " created with " <<
    StringUtils::formatLargeNumber(v.getSize()) << " elements.");
}

Meters RoadCrossingPolyRule::_getSearchRadius(const ConstElementPtr& e) const
{
  return e->getCircularError();
}

bool RoadCrossingPolyRule::_isMatchCandidate(ConstElementPtr element) const
{
  LOG_VART(element->getElementId());

  // special tag is currently only used by roundabout processing to mark temporary features
  if (element->getTags().contains(MetadataTags::HootSpecial()))
    return false;

  // index polys passing the filter and all roads not allowed to cross over those polys
  return
    ((!_allowedRoadTagFilter || !_allowedRoadTagFilter->isSatisfied(element)) &&
     HighwayCriterion(_map).isSatisfied(element)) ||
    _polyFilter->isSatisfied(element);
}

}
