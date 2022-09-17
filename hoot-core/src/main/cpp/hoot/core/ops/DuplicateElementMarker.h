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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef DUPLICATE_ELEMENT_MARKER_H
#define DUPLICATE_ELEMENT_MARKER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * Marks exact duplicate elements
 */
class DuplicateElementMarker : public OsmMapOperation
{
public:

  static QString className() { return "DuplicateElementMarker"; }

  DuplicateElementMarker();
  ~DuplicateElementMarker() override = default;

  /**
   * @see OsmMapOperation
   */
  void apply(OsmMapPtr& map) override;

  QString getInitStatusMessage() const override { return "Marking duplicate elements..."; }
  QString getCompletedStatusMessage() const override
  {
    return QString("Marked %1 duplicate element pairs out of %2 elements total.")
            .arg(QString::number(_numAffected), QString::number(_numProcessed));
  }

  QString getDescription() const override { return "Adds a tag to elements who have duplicates"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  std::set<QString> getContainingWayTypes() const { return _containingWayTypes; }

  void setCoordinateComparisonSensitivity(int sensitivity)
  { _coordinateComparisonSensitivity = sensitivity; }

private:

  // node coord comparison sensitivty
  int _coordinateComparisonSensitivity;
  // collects the most specific type of any way owning a way node marked as a duplicate
  std::set<QString> _containingWayTypes;

  QString _getUuidVal(const QString& newUuid, const ConstElementPtr& element) const;
};

}

#endif // DUPLICATE_ELEMENT_MARKER_H
