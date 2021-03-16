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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "SuperfluousConflateOpRemover.h"

// hoot
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/PointCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

QSet<QString> SuperfluousConflateOpRemover::_geometryTypeClassNameCache;

void SuperfluousConflateOpRemover::removeSuperfluousOps()
{
  // get all crits involved in the current matcher configuration
  const QSet<QString> matcherCrits = _getMatchCreatorGeometryTypeCrits();

  QSet<QString> removedOps;

  // For each of the conflate pre/post and map cleaner transforms (if conflate pre/post specifies
  // MapCleaner), filter out any that aren't associated with the same ElementCriterion as the ones
  // associated with the matchers.

  const QStringList modifiedPreConflateOps =
    _filterOutUnneededOps(
      matcherCrits, ConfigOptions().getConflatePreOps(), removedOps);
  if (modifiedPreConflateOps.size() != ConfigOptions().getConflatePreOps().size())
  {
    conf().set(ConfigOptions::getConflatePreOpsKey(), modifiedPreConflateOps);
  }

  const QStringList modifiedPostConflateOps =
    _filterOutUnneededOps(
      matcherCrits, ConfigOptions().getConflatePostOps(), removedOps);
  if (modifiedPostConflateOps.size() != ConfigOptions().getConflatePostOps().size())
  {
    conf().set(ConfigOptions::getConflatePostOpsKey(), modifiedPostConflateOps);
  }

  const QString mapCleanerName = MapCleaner::className();
  if (modifiedPreConflateOps.contains(mapCleanerName) ||
      modifiedPostConflateOps.contains(mapCleanerName))
  {
    const QStringList modifiedCleaningOps =
      _filterOutUnneededOps(
        matcherCrits, ConfigOptions().getMapCleanerTransforms(), removedOps);
    if (modifiedCleaningOps.size() != ConfigOptions().getMapCleanerTransforms().size())
    {
      conf().set(ConfigOptions::getMapCleanerTransformsKey(), modifiedCleaningOps);
    }
  }

  if (!removedOps.empty())
  {
    QStringList removedOpsList = removedOps.values();
    qSort(removedOpsList);
    LOG_INFO(
      "Removed the following conflate pre/post operations with no relevance to the selected " <<
      "matchers: " << removedOpsList.join(", "));
  }
}

QStringList SuperfluousConflateOpRemover::_filterOutUnneededOps(
  const QSet<QString>& geometryTypeCrits, const QStringList& ops, QSet<QString>& removedOps)
{
  LOG_TRACE("ops before: " << ops);

  QStringList modifiedOps;

  for (int i = 0; i < ops.size(); i++)
  {
    const QString opName = ops.at(i);
    LOG_VART(opName);

    // MapCleaner's ops are configured with map.cleaner.transforms, so don't exclude it here.
    if (opName == MapCleaner::className())
    {
      modifiedOps.append(opName);
      continue;
    }

    // All the ops should be map ops or element vis and, thus, support
    // FilteredByGeometryTypeCriteria, but we'll check anyway to be safe.
    std::shared_ptr<FilteredByGeometryTypeCriteria> op;
    if (Factory::getInstance().hasBase<OsmMapOperation>(opName))
    {
      op =
        std::dynamic_pointer_cast<FilteredByGeometryTypeCriteria>(
          std::shared_ptr<OsmMapOperation>(
            Factory::getInstance().constructObject<OsmMapOperation>(opName)));
    }
    else if (Factory::getInstance().hasBase<ElementVisitor>(opName))
    {
      op =
        std::dynamic_pointer_cast<FilteredByGeometryTypeCriteria>(
          std::shared_ptr<ElementVisitor>(
            Factory::getInstance().constructObject<ElementVisitor>(opName)));
    }

    if (op)
    {
      // get all the class names of the crits that this op is associated with
      const QStringList opCrits = op->getCriteria();
      LOG_VART(opCrits);

      // If the op is not associated with any crit, we assume it should never be disabled based on
      // the feature type being conflated.
      if (opCrits.isEmpty())
      {
        modifiedOps.append(opName);
        continue;
      }

      // If any of the op's crits match with those in the matchers' list, we'll use it. Otherwise,
      // we disable it.
      bool opAdded = false;
      for (int j = 0; j < opCrits.size(); j++)
      {
        const QString opCrit = opCrits.at(j);

        if (!_isGeometryTypeCrit(opCrit))
        {
          throw HootException(
            "FilteredByGeometryTypeCriteria::getCriteria implementation in " + opName +
            " returned a non-GeometryTypeCriterion class: " + opCrit);
        }

        if (!_geometryTypeClassNameCache.contains(opCrit))
        {
          _geometryTypeClassNameCache.insert(opCrit);
        }

        if (geometryTypeCrits.contains(opCrit))
        {
          modifiedOps.append(opName);
          opAdded = true;
          break;
        }
      }
      if (!opAdded)
      {
        removedOps.insert(opName);
      }
    }
    else
    {
      removedOps.insert(opName);
    }
  }

  LOG_TRACE("ops after: " << modifiedOps);
  LOG_VART(removedOps);
  return modifiedOps;
}

QSet<QString> SuperfluousConflateOpRemover::_getMatchCreatorGeometryTypeCrits()
{
  QSet<QString> matcherCrits;

  // Get all of the matchers from our current config.
  std::vector<std::shared_ptr<MatchCreator>> matchCreators =
      MatchFactory::getInstance().getCreators();
  for (std::vector<std::shared_ptr<MatchCreator>>::const_iterator it = matchCreators.begin();
       it != matchCreators.end(); ++it)
  {
    std::shared_ptr<MatchCreator> matchCreator = *it;
    std::shared_ptr<FilteredByGeometryTypeCriteria> critFilter =
      std::dynamic_pointer_cast<FilteredByGeometryTypeCriteria>(matchCreator);
    const QStringList crits = critFilter->getCriteria();

    // Technically not sure we'd have to error out here, but it will be good to know if any
    // matchers weren't configured with crits to keep conflate bugs from sneaking in over time.
    if (crits.empty())
    {
      throw HootException(
        "Match creator: " + matchCreator->getName() +
        " does not specify any associated feature type criteria.");
    }

    for (int i = 0; i < crits.size(); i++)
    {
      const QString critStr = crits.at(i);
      LOG_VART(critStr);

      // doublecheck this is a valid crit
      if (!_isGeometryTypeCrit(critStr))
      {
        throw HootException(
          "FilteredByGeometryTypeCriteria::getCriteria implementation in " +
          matchCreator->getName() + " returned a non-GeometryTypeCriterion class: " + critStr);
      }

      if (!_geometryTypeClassNameCache.contains(critStr))
      {
        _geometryTypeClassNameCache.insert(critStr);
      }

      // add the crit
      matcherCrits.insert(critStr);

      // also add any generic geometry crits the crit inherits from

      const QStringList pointCrits =
        GeometryTypeCriterion::getCriterionClassNamesByGeometryType(
          GeometryTypeCriterion::GeometryType::Point);
      LOG_VART(pointCrits);
      if (pointCrits.contains(critStr))
      {
        matcherCrits.insert(PointCriterion::className());
      }

      const QStringList lineCrits =
        GeometryTypeCriterion::getCriterionClassNamesByGeometryType(
          GeometryTypeCriterion::GeometryType::Line);
      LOG_VART(lineCrits);
      if (lineCrits.contains(critStr))
      {
        matcherCrits.insert(LinearCriterion::className());
      }

      const QStringList polyCrits =
        GeometryTypeCriterion::getCriterionClassNamesByGeometryType(
          GeometryTypeCriterion::GeometryType::Polygon);
      LOG_VART(polyCrits);
      if (polyCrits.contains(critStr))
      {
        matcherCrits.insert(PolygonCriterion::className());
      }
    }
  }

  LOG_VART(matcherCrits);
  return matcherCrits;
}

bool SuperfluousConflateOpRemover::_isGeometryTypeCrit(const QString& className)
{
  if (_geometryTypeClassNameCache.contains(className))
  {
    return true;
  }

  // can't use hasBase with GeometryTypeCriterion here, since GeometryTypeCriterion are registered
  // as ElementCriterion
  std::shared_ptr<GeometryTypeCriterion> crit;
  if (Factory::getInstance().hasBase<ElementCriterion>(className))
  {
    crit =
      std::dynamic_pointer_cast<GeometryTypeCriterion>(
        std::shared_ptr<ElementCriterion>(
          Factory::getInstance().constructObject<ElementCriterion>(className)));
    return crit.get();
  }

  return false;
}

}
