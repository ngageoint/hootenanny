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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef COOKIECUTTER_H
#define COOKIECUTTER_H

// Hoot
#include <hoot/core/OsmMap.h>

namespace hoot
{

/**
 * Cuts a shape out of a dough map
 */
class CookieCutter
{
public:

  static std::string className() { return "hoot::CookieCutter"; }

  static unsigned int logWarnCount;

  CookieCutter(bool crop, double outputBuffer = 0.0);

  /**
   * Uses an input map geometry to cut the shape out of another map
   *
   * @param cutterShapeMap the input geometry use for cutting
   * @param doughMap the map to be cut from
   */
  void cut(OsmMapPtr cutterShapeMap, OsmMapPtr doughMap);

private:

  bool _crop;
  double _outputBuffer;
};

}

#endif // COOKIECUTTER_H
