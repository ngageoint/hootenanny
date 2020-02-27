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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef MAPCLEANER_H
#define MAPCLEANER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Progress.h>

// Qt
#include <QString>

namespace hoot
{

/**
 * A composite class for cleaning maps and prepping them for conflation. This performs operations
 * like splitting intersections, fixing some common errors, etc.
 */
class MapCleaner : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::MapCleaner"; }

  static QString opsKey() { return "map.cleaner.transforms"; }

  MapCleaner();
  MapCleaner(const Progress& progress);

  virtual ~MapCleaner() {}

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual QString getDescription() const override { return "Cleans map data"; }

  virtual std::string getClassName() const { return className(); }

private:

  Progress _progress;
};

}

#endif // MAPCLEANER_H
