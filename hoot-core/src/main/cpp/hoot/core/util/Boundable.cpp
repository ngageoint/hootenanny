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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#include "Boundable.h"

// Hoot
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

Boundable::Boundable()
  : _cropOnReadIfBounded(true)
{
}

std::shared_ptr<geos::geom::Geometry> Boundable::loadBounds(const ConfigOptions& options, bool forceEnvelopeBounds)
{
  //  First try loading the bounds from the `bounds` setting
  std::shared_ptr<geos::geom::Geometry> result = loadBoundsString(options, forceEnvelopeBounds);
  //  If `bounds` fails, try the `bounds.input.file` setting
  if (!result)
    result = loadBoundsFile(options, forceEnvelopeBounds);
  return result;
}

std::shared_ptr<geos::geom::Geometry> Boundable::loadBoundsString(const ConfigOptions& options, bool forceEnvelopeBounds)
{
  return loadBoundsByString(options.getBounds(), forceEnvelopeBounds);
}

std::shared_ptr<geos::geom::Geometry> Boundable::loadBoundsFile(const ConfigOptions& options, bool forceEnvelopeBounds)
{
  std::shared_ptr<geos::geom::Geometry> result;
  //  Get the bounds input filename
  QString boundsFile = options.getBoundsInputFile();
  if (!boundsFile.isEmpty())
  {
    //  Load the bounds from the file
    bool isEnvelope = false;
    result = GeometryUtils::readBoundsFromFile(boundsFile, isEnvelope);
    //  If required, force the bounds to be an envelope
    if (forceEnvelopeBounds && !isEnvelope)
      result = result->getEnvelope();
  }
  //  Return the result found or an empty geometry
  return result;
}

std::shared_ptr<geos::geom::Geometry> Boundable::loadCropBounds(const ConfigOptions& options, bool forceEnvelopeBounds)
{
  return loadBoundsByString(options.getCropBounds(), forceEnvelopeBounds);
}

std::shared_ptr<geos::geom::Geometry> Boundable::loadInBoundsCriterionBounds(const ConfigOptions& options, bool forceEnvelopeBounds)
{
  return loadBoundsByString(options.getInBoundsCriterionBounds(), forceEnvelopeBounds);
}

std::shared_ptr<geos::geom::Geometry> Boundable::loadBoundsByString(const QString& bounds, bool forceEnvelopeBounds)
{
  std::shared_ptr<geos::geom::Geometry> result;
  //  Ignore empty bounds
  if (!bounds.isEmpty())
  {
    //  Load the bounds from the string
    bool isEnvelope = false;
    result = GeometryUtils::boundsFromString(bounds, isEnvelope);
    //  If required, force the bounds to be an envelope
    if (forceEnvelopeBounds && !isEnvelope)
      result = result->getEnvelope();
  }
  //  Return the result found or an empty geometry
  return result;
}

}
