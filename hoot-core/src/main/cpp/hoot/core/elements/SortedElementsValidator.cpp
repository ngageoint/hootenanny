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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "SortedElementsValidator.h"

// Hoot
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/visitors/IsSortedVisitor.h>
#include <hoot/core/util/FileUtils.h>

namespace hoot
{

bool SortedElementsValidator::validate(const QString& input)
{
  QFileInfo fileInfo(input);
  if (!fileInfo.exists())
  {
    throw IllegalArgumentException("Specified input: " + input + " does not exist.");
  }

  LOG_STATUS("Determining if ..." << FileUtils::toLogFormat(input, 25) << " is sorted...");

  bool result = true;

  if (OsmPbfReader().isSupported(input))
  {
    result = OsmPbfReader().isSorted(input);
  }
  else
  {
    std::shared_ptr<PartialOsmMapReader> reader =
      std::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::createReader(input));
    reader->setUseDataSourceIds(true);
    reader->open(input);
    reader->initializePartial();

    IsSortedVisitor vis;
    while (reader->hasMoreElements())
    {
      ElementPtr element = reader->readNextElement();
      if (element)
      {
        vis.visit(element);
        if (!vis.getIsSorted())
        {
          result = false;
          LOG_VART(result);
          break;
        }
      }
    }

    reader->finalizePartial();
    reader->close();
  }

  return result;
}

}
