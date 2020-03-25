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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RoadCrossingPolyRule.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/SpatialIndexer.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/util/Factory.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Qt
#include <QFileInfo>

// Boost
#include <boost/bind.hpp>

// Standard
#include <functional>

namespace hoot
{

RoadCrossingPolyRule::RoadCrossingPolyRule(ConstOsmMapPtr map) :
_map(map)
{
}

QList<RoadCrossingPolyRule> RoadCrossingPolyRule::readRules(const QString& rulesFile,
                                                            ConstOsmMapPtr map)
{ 
  QFileInfo rulesFileInfo(rulesFile);
  if (!rulesFileInfo.exists())
  {
    throw IllegalArgumentException("Road crossing polygon rules file does not exist.");
  }

  LOG_DEBUG("Reading rules...");

  boost::property_tree::ptree propTree;
  try
  {
    boost::property_tree::read_json(rulesFile.toStdString(), propTree);
  }
  catch (boost::property_tree::json_parser::json_parser_error& e)
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

  // TODO: parse these as arrays instead

  QList<RoadCrossingPolyRule> rules;
  for (boost::property_tree::ptree::value_type& ruleProp : propTree.get_child("rules"))
  {
    boost::optional<std::string> polyTypeFilterProp =
      ruleProp.second.get_optional<std::string>("polyTypeFilter");
    QString polyTypeFilterStr;
    if (polyTypeFilterProp)
    {
      polyTypeFilterStr = QString::fromStdString(polyTypeFilterProp.get()).trimmed();
    }
    LOG_VART(polyTypeFilterStr);

    boost::optional<std::string> polyTagFilterProp =
      ruleProp.second.get_optional<std::string>("polyTagFilter");
    QString polyTagFilterStr;
    if (polyTagFilterProp)
    {
      polyTagFilterStr = QString::fromStdString(polyTagFilterProp.get()).trimmed();
    }
    LOG_VART(polyTagFilterStr);

    if (polyTypeFilterStr.isEmpty() && polyTagFilterStr.isEmpty())
    {
      throw IllegalArgumentException(
        "A road crossing rule must specify either a polygon type filter (polyTypeFilter) or a "
        "polygon tag filter (polyTagFilter).");
    }

    boost::optional<std::string> allowedRoadTagFilterProp =
      ruleProp.second.get_optional<std::string>("allowedRoadTagFilter");
    QString allowedRoadTagFilterStr;
    if (allowedRoadTagFilterProp)
    {
      allowedRoadTagFilterStr = QString::fromStdString(allowedRoadTagFilterProp.get()).trimmed();
    }
    LOG_VART(allowedRoadTagFilterStr);

    RoadCrossingPolyRule rule(map);
    rule.setPolyFilterString(
      "poly type filter: " + polyTypeFilterStr + "; poly tag filter: " + polyTagFilterStr);
    rule.setPolyFilter(
      RoadCrossingPolyRule::polyRuleStringsToCrit(polyTypeFilterStr, polyTagFilterStr));
    LOG_VART(rule.getPolyFilter());
    rule.setAllowedRoadTagFilterString("allowed road tag filter: " + allowedRoadTagFilterStr);
    rule.setAllowedRoadTagFilter(
      RoadCrossingPolyRule::kvpRuleStringToTagCrit(allowedRoadTagFilterStr));
    if (rule.getAllowedRoadTagFilter())
    {
      LOG_VART(rule.getAllowedRoadTagFilter());
    }
    rule.createIndex();
    rules.append(rule);
  }
  LOG_VARD(rules.size());
  return rules;
}

ElementCriterionPtr RoadCrossingPolyRule::polyRuleStringsToCrit(
  const QString& polyTypeFilterStr, const QString& polyTagFilterStr)
{
  // TODO: pass the inputs in as lists instead

  LOG_VART(polyTypeFilterStr);
  LOG_VART(polyTagFilterStr);

  if (polyTypeFilterStr.isEmpty() && polyTagFilterStr.isEmpty())
  {
    throw IllegalArgumentException(
      "A road crossing rule must specify either a polygon type filter (polyTypeFilter) or a "
      "polygon tag filter (polyTagFilter).");
  }

  ElementCriterionPtr polyTypeFilter;
  if (!polyTypeFilterStr.isEmpty())
  {
    if (!polyTypeFilterStr.toLower().startsWith("hoot::"))
    {
      throw IllegalArgumentException(
        "A road crossing rule polygon type filter must be a valid Hooteanny ElementCriterion "
        "class name.");
    }
    else
    {
      polyTypeFilter.reset(
        Factory::getInstance().constructObject<ElementCriterion>(polyTypeFilterStr.trimmed()));
    }
  }

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
    {
      polyTagFilter = kvpRuleStringToTagCrit(polyTagFilterStr);
    }
  }

  if (polyTypeFilter && polyTagFilter)
  {
    return std::shared_ptr<ChainCriterion>(new ChainCriterion(polyTypeFilter, polyTagFilter));
  }
  else if (polyTypeFilter)
  {
    return polyTypeFilter;
  }
  else
  {
    return polyTagFilter;
  }
}

ElementCriterionPtr RoadCrossingPolyRule::kvpRuleStringToTagCrit(const QString& kvpStr)
{
  // TODO: pass the inputs in as lists instead

  LOG_VART(kvpStr);
  const QString kvpFormatErrMsg =
    "A road crossing rule tag filter must be of the form <key1>=<value1>;<key2>=<value2>...";
  if (kvpStr.trimmed().isEmpty())
  {
    return ElementCriterionPtr();
  }
  else if (!kvpStr.contains("="))
  {
    throw IllegalArgumentException(kvpFormatErrMsg);
  }

  std::shared_ptr<OrCriterion> crit(new OrCriterion());

  const QStringList kvpStrParts = kvpStr.split(";");
  LOG_VART(kvpStrParts.size());
  for (int i = 0; i < kvpStrParts.size(); i++)
  {
    LOG_VART(kvpStrParts.at(i));
    const QStringList filterStrParts = kvpStrParts.at(i).split("=");
    LOG_VART(filterStrParts);
    if (filterStrParts.size() != 2)
    {
      throw IllegalArgumentException(kvpFormatErrMsg);
    }
    if (filterStrParts[1] == "*")
    {
      crit->addCriterion(std::shared_ptr<ElementCriterion>(new TagKeyCriterion(filterStrParts[0])));
    }
    else
    {
      crit->addCriterion(
        std::shared_ptr<ElementCriterion>(new TagCriterion(filterStrParts[0], filterStrParts[1])));
    }
  }

  return crit;
}

void RoadCrossingPolyRule::createIndex()
{
  LOG_STATUS("Creating roads crossing polys index...");

  // create an index for all roads and all polys that satisfy our crit within the default
  // search radius

  // No tuning was done, I just copied these settings from OsmMapIndex.
  // 10 children - 368 - see #3054
  std::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
  _index.reset(new Tgs::HilbertRTree(mps, 2));

  // Only index elements satisfy isMatchCandidate(e)
  std::function<bool (ConstElementPtr e)> f =
    std::bind(&RoadCrossingPolyRule::_isMatchCandidate, this, std::placeholders::_1);
  std::shared_ptr<ArbitraryCriterion> pCrit(new ArbitraryCriterion(f));

  SpatialIndexer v(
    _index, _indexToEid, pCrit,
    std::bind(&RoadCrossingPolyRule::_getSearchRadius, this, std::placeholders::_1), _map);
  _map->visitRo(v);
  v.finalizeIndex();

  LOG_STATUS(
    "Roads crossing polys feature index created with " <<
    StringUtils::formatLargeNumber(v.getSize()) << " elements.");
}

Meters RoadCrossingPolyRule::_getSearchRadius(const ConstElementPtr& e) const
{
  return e->getCircularError();
}

bool RoadCrossingPolyRule::_isMatchCandidate(ConstElementPtr element)
{
  LOG_VART(element->getElementId());

  // special tag is currently only used by roundabout processing to mark temporary features
  if (element->getTags().contains(MetadataTags::HootSpecial()))
  {
    return false;
  }

  // index polys passing the filter and all roads not allowed to cross over those polys
//  return
//    ((!_allowedRoadTagFilter || !_allowedRoadTagFilter->isSatisfied(element)) &&
//     HighwayCriterion(_map).isSatisfied(element)) ||
//    _polyFilter->isSatisfied(element);
  const bool isAllowedRoad = _allowedRoadTagFilter && _allowedRoadTagFilter->isSatisfied(element);
  LOG_VART(isAllowedRoad);
  if (isAllowedRoad)
  {
    return false;
  }
  const bool isHighway = HighwayCriterion(_map).isSatisfied(element);
  LOG_VART(isHighway);
  const bool polyPassedFilter = _polyFilter->isSatisfied(element);
  LOG_VART(polyPassedFilter);
  return isHighway || polyPassedFilter;
}

}
