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

#include "ConflateUtils.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/NonConflatableCriterion.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

int ConflateUtils::writeNonConflatable(const ConstOsmMapPtr& map, const QString& outputFile)
{
  LOG_STATUS("Writing non-conflatable data to: ..." << outputFile.right(25) << " ...");
  OsmMapPtr nonConflatableMap(new OsmMap(map));
  std::shared_ptr<RemoveElementsVisitor> elementRemover(new RemoveElementsVisitor(true));
  elementRemover->setRecursive(true);
  std::shared_ptr<ElementCriterion> nonConflatableCrit(
    new NonConflatableCriterion(nonConflatableMap));
  elementRemover->addCriterion(nonConflatableCrit);;
  nonConflatableMap->visitRw(*elementRemover);
  if (nonConflatableMap->size() > 0)
  {
    OsmMapWriterFactory::write(nonConflatableMap, outputFile);
  }
  return nonConflatableMap->size();
}

}
