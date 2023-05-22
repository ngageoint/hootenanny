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

#ifndef CLOSED_HIGHWAY_SPLITTER_H
#define CLOSED_HIGHWAY_SPLITTER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QMultiHash>

namespace hoot
{

class OsmMap;

/**
 * @brief The ClosedHighwaySplitter class given an OsmMap, splits closed highways in 'half'
 * (or approximately half without adding a new node
 *
 * This can help network conflation operations that struggle to conflate closed highways.
 */
class ClosedHighwaySplitter : public OsmMapOperation
{
public:

  static QString className() { return "ClosedHighwaySplitter"; }

  ClosedHighwaySplitter() = default;
  ClosedHighwaySplitter(const std::shared_ptr<OsmMap>& map) : _map(map) { }
  ~ClosedHighwaySplitter() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  static void splitClosedHighways(const std::shared_ptr<OsmMap>& map);
  void splitClosedHighways();

  QString getInitStatusMessage() const override { return "Splitting closed highways..."; }
  QString getCompletedStatusMessage() const override
  { return "Split " + QString::number(_numAffected) + " closed highways"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Splits closed highways"; }

private:
  /** Pointer to the OsmMap */
  std::shared_ptr<OsmMap> _map;
};

}

#endif // CLOSED_HIGHWAY_SPLITTER_H
