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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef RANDOM_MAP_CROPPER_H
#define RANDOM_MAP_CROPPER_H

// Hoot
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * Uses MapCropper to randomly crop a section of data with a specified maximum number of nodes.
 */
class RandomMapCropper : public OsmMapOperation, public Configurable
{
public:

  static QString className() { return "RandomMapCropper"; }

  RandomMapCropper();
  ~RandomMapCropper() override = default;

  /**
   * @see OsmMapOperation
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override
  { return _cropper.getInitStatusMessage(); }
  QString getCompletedStatusMessage() const override
  { return _cropper.getCompletedStatusMessage(); }

  QString getDescription() const override
  { return "Crops a random tile out of a map of a specified node size (experimental)"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setMaxNodeCount(int count) { _maxNodeCount = count; }
  void setRandomSeed(int seed) { _randomSeed = seed; }
  void setTileFootprintOutputPath(QString path);
  void setPixelSize(double size) { _pixelSize = size; }

private:

  int _maxNodeCount;
  int _randomSeed;
  double _pixelSize;
  MapCropper _cropper;
  QString _tileFootprintOutputPath;
};

}

#endif // RANDOM_MAP_CROPPER_H
