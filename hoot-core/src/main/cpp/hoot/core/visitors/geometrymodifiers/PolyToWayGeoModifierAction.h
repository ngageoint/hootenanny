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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef POLYTOWAYGEOMODIFIERACTION_H
#define POLYTOWAYGEOMODIFIERACTION_H

#include "GeometryModifierAction.h"

// Hoot
//#include <hoot/core/geometry/CoordinateExt.h>

namespace hoot
{

/**
 * Converts Polygons to Ways.
 */
class PolyToWayGeoModifierAction : public GeometryModifierAction
{
public:
  static const QString NOOP_PARAM;

  static QString className() { return "PolyToWayGeoModifierAction"; }

  PolyToWayGeoModifierAction() = default;
  ~PolyToWayGeoModifierAction() override = default;

  QString getCommandName() const override { return "poly_to_way"; }
  QList<QString> getParameterNames() const override
  { return QList<QString> {}; }

  void parseArguments(const QHash<QString, QString>& arguments) override;
  bool processElement(const ElementPtr& pElement, OsmMap* pMap) override;
};

}

#endif // POLYTOWAYGEOMODIFIERACTION_H
