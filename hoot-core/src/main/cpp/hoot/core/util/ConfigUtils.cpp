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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ConfigUtils.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/ops/DuplicateNodeRemover.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/elements/MapProjector.h>


namespace hoot
{

bool ConfigUtils::boundsOptionEnabled()
{
  return
    !conf().get(ConfigOptions::getConvertBoundsKey()).toString().trimmed().isEmpty() ||
    !conf().get(ConfigOptions::getConvertBoundsHootApiDatabaseKey()).toString().trimmed().isEmpty() ||
    !conf().get(ConfigOptions::getConvertBoundsOsmApiDatabaseKey()).toString().trimmed().isEmpty();
}

std::shared_ptr<geos::geom::Geometry> ConfigUtils::getConvertBounds()
{
  QString boundsStr = conf().get(ConfigOptions::getConvertBoundsKey()).toString().trimmed();
  if (!boundsStr.isEmpty())
  {
    return GeometryUtils::boundsFromString(boundsStr);
  }
  boundsStr = conf().get(ConfigOptions::getConvertBoundsHootApiDatabaseKey()).toString().trimmed();
  if (!boundsStr.isEmpty())
  {
    return GeometryUtils::boundsFromString(boundsStr);
  }
  boundsStr = conf().get(ConfigOptions::getConvertBoundsOsmApiDatabaseKey()).toString().trimmed();
  if (!boundsStr.isEmpty())
  {
    return GeometryUtils::boundsFromString(boundsStr);
  }
  return std::shared_ptr<geos::geom::Geometry>();
}

GeometricRelationship ConfigUtils::getConvertBoundsRelationship()
{
  if (ConfigOptions().getConvertBoundsKeepOnlyFeaturesInsideBounds())
  {
    return GeometricRelationship::Contains;
  }
  return GeometricRelationship::Intersects;
}

std::shared_ptr<InBoundsCriterion> ConfigUtils::getConflateBoundsCrit(const ConstOsmMapPtr& map)
{
  std::shared_ptr<InBoundsCriterion> boundsCrit;
  const QString boundsStr = ConfigOptions().getConvertBounds().trimmed();
  LOG_VARD(boundsStr);
  if (!boundsStr.isEmpty())
  {
    const GeometricRelationship boundsRelationship = ConfigUtils::getConvertBoundsRelationship();
    const bool mustContain = true ? (boundsRelationship == GeometricRelationship::Contains) : false;
    boundsCrit.reset(new InBoundsCriterion(mustContain));
    std::shared_ptr<geos::geom::Geometry> bounds = GeometryUtils::boundsFromString(boundsStr);
    if (!MapProjector::isGeographic(map))
    {
      // The bounds is always in WGS84, so if our map isn't currently in WGS84 we need to reproject
      // the bounds.
      std::shared_ptr<OGRSpatialReference> srs84(new OGRSpatialReference());
      srs84->SetWellKnownGeogCS("WGS84");
      MapProjector::project(bounds, srs84, map->getProjection());
    }
    boundsCrit->setBounds(bounds);
    boundsCrit->setOsmMap(map.get());
  }
  return boundsCrit;
}

void ConfigUtils::checkForTagValueTruncationOverride()
{
  // Disable tag truncation if the override option is in place.
  if (ConfigOptions().getConflateTagDisableValueTruncation())
  {
    QStringList conflatePreOps = ConfigOptions().getConflatePreOps();
    int numFound =
      conflatePreOps.removeAll(QString::fromStdString(ApiTagTruncateVisitor::className()));
    if (numFound > 0)
    {
      conf().set("conflate.pre.ops", conflatePreOps);
    }
    QStringList conflatePostOps = ConfigOptions().getConflatePostOps();
    numFound =
      conflatePostOps.removeAll(QString::fromStdString(ApiTagTruncateVisitor::className()));
    if (numFound > 0)
    {
      conf().set("conflate.post.ops", conflatePostOps);
    }
  }
}

void ConfigUtils::checkForDuplicateElementCorrectionMismatch(const QStringList& ops)
{
  const QString dupeNodeRemoverClassName =
    QString::fromStdString(DuplicateNodeRemover::className());
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

}
