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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ConfigUtils.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/ops/DuplicateNodeRemover.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/elements/MapProjector.h>

namespace hoot
{

bool ConfigUtils::boundsOptionEnabled()
{
  return
    !conf().get(ConfigOptions::getBoundsKey()).toString().trimmed().isEmpty() ||
    !conf().get(ConfigOptions::getBoundsHootApiDatabaseKey()).toString().trimmed().isEmpty() ||
    !conf().get(ConfigOptions::getBoundsOsmApiDatabaseKey()).toString().trimmed().isEmpty();
}

std::shared_ptr<geos::geom::Geometry> ConfigUtils::getBounds()
{
  QString boundsStr = conf().get(ConfigOptions::getBoundsKey()).toString().trimmed();
  if (!boundsStr.isEmpty())
  {
    return GeometryUtils::boundsFromString(boundsStr);
  }
  boundsStr = conf().get(ConfigOptions::getBoundsHootApiDatabaseKey()).toString().trimmed();
  if (!boundsStr.isEmpty())
  {
    return GeometryUtils::boundsFromString(boundsStr);
  }
  boundsStr = conf().get(ConfigOptions::getBoundsOsmApiDatabaseKey()).toString().trimmed();
  if (!boundsStr.isEmpty())
  {
    return GeometryUtils::boundsFromString(boundsStr);
  }
  return std::shared_ptr<geos::geom::Geometry>();
}

QString ConfigUtils::getBoundsString()
{
  QString boundsStr = conf().get(ConfigOptions::getBoundsKey()).toString().trimmed();
  if (!boundsStr.isEmpty())
  {
    return boundsStr;
  }
  boundsStr = conf().get(ConfigOptions::getBoundsHootApiDatabaseKey()).toString().trimmed();
  if (!boundsStr.isEmpty())
  {
    return boundsStr;
  }
  boundsStr = conf().get(ConfigOptions::getBoundsOsmApiDatabaseKey()).toString().trimmed();
  if (!boundsStr.isEmpty())
  {
    return boundsStr;
  }
  return "";
}

GeometricRelationship ConfigUtils::getBoundsRelationship()
{
  if (ConfigOptions().getBoundsKeepOnlyFeaturesInsideBounds())
  {
    return GeometricRelationship::Contains;
  }
  return GeometricRelationship::Intersects;
}

std::shared_ptr<InBoundsCriterion> ConfigUtils::getBoundsFilter(const ConstOsmMapPtr& map)
{
  std::shared_ptr<InBoundsCriterion> boundsCrit;
  const QString boundsStr = ConfigOptions().getBounds().trimmed();
  if (!boundsStr.isEmpty())
  {
    const GeometricRelationship boundsRelationship = ConfigUtils::getBoundsRelationship();
    const bool mustContain = true ? (boundsRelationship == GeometricRelationship::Contains) : false;
    boundsCrit = std::make_shared<InBoundsCriterion>(mustContain);
    std::shared_ptr<geos::geom::Geometry> bounds = GeometryUtils::boundsFromString(boundsStr);
    if (!MapProjector::isGeographic(map))
    {
      // The bounds is always in WGS84, so if our map isn't currently in WGS84 we need to reproject
      // the bounds.
      LOG_DEBUG("Reprojecting bounds: " << boundsStr << "...");
      std::shared_ptr<OGRSpatialReference> srs84 = MapProjector::createWgs84Projection();
      MapProjector::project(bounds, srs84, map->getProjection());
    }
    boundsCrit->setBounds(bounds);
    boundsCrit->setOsmMap(map.get());
  }
  LOG_VART(boundsCrit.get());
  return boundsCrit;
}

void ConfigUtils::checkForTagValueTruncationOverride()
{
  // Disable tag truncation if the override option is in place.
  if (ConfigOptions().getConflateTagDisableValueTruncation())
  {
    QStringList conflatePreOps = ConfigOptions().getConflatePreOps();
    int numFound = conflatePreOps.removeAll(ApiTagTruncateVisitor::className());
    if (numFound > 0)
    {
      conf().set(ConfigOptions::getConflatePreOpsKey(), conflatePreOps);
    }
    QStringList conflatePostOps = ConfigOptions().getConflatePostOps();
    numFound = conflatePostOps.removeAll(ApiTagTruncateVisitor::className());
    if (numFound > 0)
    {
      conf().set(ConfigOptions::getConflatePostOpsKey(), conflatePostOps);
    }
  }
}

void ConfigUtils::checkForDuplicateElementCorrectionMismatch(const QStringList& ops)
{
  const QString dupeNodeRemoverClassName = DuplicateNodeRemover::className();
  if (ops.contains(dupeNodeRemoverClassName))
  {
    conf().set(ConfigOptions::getMapMergeIgnoreDuplicateIdsKey(), true);
  }
}

void ConfigUtils::removeListOpEntry(const QString& opName, const QString& entryToRemove)
{
  QStringList opValue = conf().getList(opName);
  opValue.removeAll(entryToRemove);
  conf().set(opName, opValue);
}

void ConfigUtils::insertListOpEntry(const QString& opName, const QString& opEntry, const int index)
{
  QStringList opValue = conf().getList(opName);
  opValue.insert(index, opEntry);
  conf().set(opName, opValue);
}

}
