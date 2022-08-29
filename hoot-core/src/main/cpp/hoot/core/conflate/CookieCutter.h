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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef COOKIECUTTER_H
#define COOKIECUTTER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * @brief The CookieCutter class cuts a shape out of a dough map.
 */
class CookieCutter
{
public:

  static QString className() { return "CookieCutter"; }

  CookieCutter(bool crop, double outputBuffer = 0.0, bool keepEntireFeaturesCrossingBounds = false,
               bool keepOnlyFeaturesInsideBounds = false, bool removeMissingElements = true);

  /**
   * @brief cut uses an input map geometry to cut the shape out of another map.
   * @param cutterShapeOutlineMap the input geometry use for cutting
   * @param doughMap the map to be cut from
   */
  void cut(OsmMapPtr& cutterShapeOutlineMap, OsmMapPtr& doughMap) const;

private:

  // If crop is true, then the cookie cutter portion is kept and the dough is dropped instead.
  bool _crop;
  // how far out to buffer the cropping from the cutter shape bounds
  double _outputBuffer;
  // see MapCropper
  bool _keepEntireFeaturesCrossingBounds;
  // see MapCropper
  bool _keepOnlyFeaturesInsideBounds;
  // removes references to missing elements after the cut occurs
  bool _removeMissingElements;
};

}

#endif // COOKIECUTTER_H
