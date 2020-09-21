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

#ifndef DUPLICATE_ELEMENT_MARKER_H
#define DUPLICATE_ELEMENT_MARKER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * TODO
 */
class DuplicateElementMarker : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::DuplicateElementMarker"; }

  DuplicateElementMarker();
  virtual ~DuplicateElementMarker() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(OsmMapPtr& map);

  /**
   * TODO
   *
   * @param map
   * @param coordinateComparisonSensitivity
   * @return
   */
  static int markDuplicates(
    OsmMapPtr& map,
    const int coordinateComparisonSensitivity =
      ConfigOptions().getNodeComparisonCoordinateSensitivity());

  virtual QString getInitStatusMessage() const { return "Marking duplicate elements..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Marked " + QString::number(_numAffected) + " duplicate element pairs out of " +
      QString::number(_numProcessed) + " elements total.";
  }

  virtual QString getDescription() const { return "Adds a tag to elements who have duplicates"; }

  virtual std::string getClassName() const { return className(); }

  void setCoordinateComparisonSensitivity(int sensitivity)
  { _coordinateComparisonSensitivity = sensitivity; }

private:

  // TODO
  int _coordinateComparisonSensitivity;

  QString _getUuidVal(const QString& newUuid, const ConstElementPtr& element);
};

}

#endif // DUPLICATE_ELEMENT_MARKER_H
