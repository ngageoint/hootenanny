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
#ifndef WAYTOPOLYGEOMODIFIERACTION_H
#define WAYTOPOLYGEOMODIFIERACTION_H

#include "GeometryModifierAction.h"

// Hoot
#include <hoot/core/geometry/CoordinateExt.h>

namespace hoot
{

/**
 * Creates a polygon from a way using a specified or default width setting.
 */
class WayToPolyGeoModifierAction : public GeometryModifierAction
{
public:

  static const QString WIDTH_TAG_PARAM;
  static const QString DEFAULT_WIDTH_PARAM;

  static QString className() { return "WayToPolyGeoModifierAction"; }

  WayToPolyGeoModifierAction() = default;
  ~WayToPolyGeoModifierAction() override = default;

  QString getCommandName() const override { return "way_to_poly"; }
  QList<QString> getParameterNames() const override
  { return QList<QString>( { WIDTH_TAG_PARAM, DEFAULT_WIDTH_PARAM } ); }

  void parseArguments(const QHash<QString, QString>& arguments) override;
  bool processElement(const ElementPtr& pElement, OsmMap* pMap) override;

private:

  const double DEFAULT_WIDTH = 5;
  double _width = DEFAULT_WIDTH;  
  QString _widthTag = QString();

  void addNodeToPoly( const CoordinateExt& pos, OsmMap* pMap, WayPtr pPoly ) const;
};

}

#endif // WAYTOPOLYGEOMODIFIERACTION_H


