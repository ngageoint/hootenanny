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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVEINVALIDMULTILINESTRINGMEMBERSVISITOR_H
#define REMOVEINVALIDMULTILINESTRINGMEMBERSVISITOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * Removes invalid multiline string relation members
 *
 * These are sometimes produced as an artifact from cropping or conflation.
 */
class RemoveInvalidMultilineStringMembersVisitor : public ElementOsmMapVisitor
{
public:

  static QString className() { return "RemoveInvalidMultilineStringMembersVisitor"; }

  RemoveInvalidMultilineStringMembersVisitor();
  ~RemoveInvalidMultilineStringMembersVisitor() override = default;

  /**
   * @see ElementVisitor
   */
  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override
  { return "Removing invalid multiline string relation members..."; }
  QString getCompletedStatusMessage() const override
  {
    return
      "Removed " + StringUtils::formatLargeNumber(_numAffected) + " invalid relations / " +
      StringUtils::formatLargeNumber(_map->getRelationCount()) + " total relations.";
  }

  QString getDescription() const override
  { return "Removes invalid multiline string relation members"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  int _taskStatusUpdateInterval;
};

}

#endif  //  REMOVEINVALIDMULTILINESTRINGMEMBERSVISITOR_H
