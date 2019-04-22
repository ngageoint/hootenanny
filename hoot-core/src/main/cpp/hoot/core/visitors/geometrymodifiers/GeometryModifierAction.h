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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
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

namespace hoot
{

/*
 * Base class for all geometry modifier actions.
 * Any derived action registered in the hoot catory with this base class will be
 * available to use by the GeometryModifierOp hoot operation.
 */
class GeometryModifierAction
{

public:
  // defined in GeometryModifierOp.cpp
  static const std::string FILTER_TAG;
  static const std::string ARGUMENT_TAG;

  // Class name for hoot factory
  static std::string className() { return "hoot::GeometryModifierAction"; }

  // Command name and parameter names
  virtual QString getCommandName() const = 0;
  virtual QList<QString> getParameterNames() const = 0;

  // Parses content of command specific arguments
  virtual void parseArguments(const QHash<QString, QString>& arguments) = 0;

  // Command function to process a filtered element
  virtual bool process(const ElementPtr& pElement, OsmMap* pMap) = 0;
};

/*
 * Data of a parsed entry of the GeometryModifierOp's rules file.
 * The contents define which geometry modifier command to run,
 * the filter for the targeted nodes and the arguments specific
 * to the command.
 * The filter is processed by the GeometryModifierVisitior, the
 * arguments by the specific command's process function.
 */
struct GeometryModifierActionDesc
{
  // Command name
  QString command;

  // Node tag key and value strings that must be present and matching
  // for the node to be included in the processing.
  QHash<QString, QString> filter;

  // Argument name and value specific to each command.
  QHash<QString, QString> arguments;

  // Instance of the command specific geometry modifier action class implementation.
  boost::shared_ptr<GeometryModifierAction> pAction;
};

}

#endif // GEOMETRYMODIFIERACTION_H
