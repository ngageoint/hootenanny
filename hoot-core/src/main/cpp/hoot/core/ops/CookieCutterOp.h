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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COOKIECUTTEROP_H
#define COOKIECUTTEROP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class OsmMap;

/**
 * Splits the map into ref/source (unknown1/unknown2), cuts the ref's shape out of the source, then
 * combines the the entire ref map back with the source map with the hole cut in it.
 */
class CookieCutterOp : public OsmMapOperation, public Configurable
{
public:

  static string className() { return "hoot::CookieCutterOp"; }

  CookieCutterOp();

  virtual void apply(boost::shared_ptr<OsmMap>& map);

  virtual string getClassName() const { return className(); }

  virtual void setConfiguration(const Settings& conf);

  void setAlpha(double alpha) { _alpha = alpha; }
  void setAlphaShapeBuffer(double alphaShapeBuffer) { _alphaShapeBuffer = alphaShapeBuffer; }
  void setCrop(bool crop) { _crop = crop; }

private:

  double _alpha;
  double _alphaShapeBuffer;
  bool _crop;

};

}

#endif // COOKIECUTTEROP_H
