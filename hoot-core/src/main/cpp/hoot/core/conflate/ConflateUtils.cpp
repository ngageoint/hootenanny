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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "ConflateUtils.h"

// Hoot
#include <hoot/core/conflate/ConflateExecutor.h>
#include <hoot/core/conflate/network/NetworkMatchCreator.h>
#include <hoot/core/criterion/NonConflatableCriterion.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

int ConflateUtils::writeNonConflatable(
  const ConstOsmMapPtr& map, const QString& output, const bool ignoreGenericConflators)
{
  LOG_STATUS("Writing non-conflatable data to: ..." << FileUtils::toLogFormat(output, 25) << " ...");

  OsmMapPtr nonConflatableMap = std::make_shared<OsmMap>(map);
  LOG_VART(nonConflatableMap->size());
  std::shared_ptr<RemoveElementsVisitor> elementRemover =
    std::make_shared<RemoveElementsVisitor>(true);
  elementRemover->setRecursive(true);
  std::shared_ptr<NonConflatableCriterion> nonConflatableCrit =
    std::make_shared<NonConflatableCriterion>(nonConflatableMap);
  nonConflatableCrit->setIgnoreGenericConflators(ignoreGenericConflators);
  elementRemover->addCriterion(nonConflatableCrit);
  nonConflatableMap->visitRw(*elementRemover);

  LOG_VART(nonConflatableMap->size());
  if (nonConflatableMap->size() > 0)
  {
    OsmMapWriterFactory::write(nonConflatableMap, output);
  }

  return nonConflatableMap->size();
}

void ConflateUtils::writeDiff(
  const QString& mapUrl1, const QString& mapUrl2, const geos::geom::Envelope& bounds,
  const QString& output)
{
  conf().set(ConfigOptions::getBoundsKey(), GeometryUtils::toConfigString(bounds));

  ConflateExecutor conflator;
  conflator.setIsDiffConflate(true);
  conflator.conflate(mapUrl1, mapUrl2, output);

  conf().set(ConfigOptions::getBoundsKey(), "");
}

bool ConflateUtils::isNetworkConflate()
{
  // This could be brittle to future changes to how the Network alg is applied.
  return
    conf().getList(ConfigOptions::getMatchCreatorsKey()).contains(NetworkMatchCreator::className());
}

}
