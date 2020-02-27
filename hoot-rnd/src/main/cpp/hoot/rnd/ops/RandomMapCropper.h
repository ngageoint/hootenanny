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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef RANDOM_MAP_CROPPER_H
#define RANDOM_MAP_CROPPER_H

// Hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/MapCropper.h>

namespace hoot
{

/**
 * Uses MapCropper to randomly crop a section of data with a specified maximum number of nodes.
 */
class RandomMapCropper : public OsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::RandomMapCropper"; }

  RandomMapCropper();

  virtual void apply(std::shared_ptr<OsmMap>& map);

  virtual void setConfiguration(const Settings& conf) override;

  virtual QString getDescription() const override
  { return "Crops a random tile out of a map of a specified node size (experimental)"; }

  virtual QString getInitStatusMessage() const override
  { return _cropper.getInitStatusMessage(); }

  virtual QString getCompletedStatusMessage() const override
  {
    return _cropper.getCompletedStatusMessage();
  }

  void setMaxNodeCount(int count) { _maxNodeCount = count; }
  void setRandomSeed(int seed) { _randomSeed = seed; }
  void setTileFootprintOutputPath(QString path);
  void setPixelSize(double size) { _pixelSize = size; }

  virtual std::string getClassName() const { return className(); }

private:

  int _maxNodeCount;
  int _randomSeed;
  double _pixelSize;
  MapCropper _cropper;
  QString _tileFootprintOutputPath;
};

}

#endif // RANDOM_MAP_CROPPER_H
