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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef GEOMETRYMODIFIERACTION_H
#define GEOMETRYMODIFIERACTION_H

#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/OsmMap.h>

// Qt
#include <QString>
#include <QHash>

// boost
#include <boost/shared_ptr.hpp>

using namespace boost;

namespace hoot
{

static const std::string GEOMODRULES_FILTER_TAG = "filter";
static const std::string GEOMODRULES_ARGUMENT_TAG = "arguments";

class GeometryModifierAction
{
public:
  static std::string className() { return "hoot::GeometryModifierAction"; }

  virtual QString getCommandName() const = 0;
  virtual QList<QString> getParameterNames() const = 0;

  virtual bool process( const ElementPtr& pElement, OsmMap* pMap ) const = 0;
};

struct GeometryModifierActionDesc
{
public:
  QString command;
  QHash<QString,QString> filter;
  QHash<QString,QString> arguments;
  shared_ptr<GeometryModifierAction> pAction;
};

}

#endif // GEOMETRYMODIFIERACTION_H
