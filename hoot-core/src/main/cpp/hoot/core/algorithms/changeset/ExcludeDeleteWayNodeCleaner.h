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

#ifndef EXCLUDE_DELETE_WAY_NODE_CLEANER_H
#define EXCLUDE_DELETE_WAY_NODE_CLEANER_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

namespace hoot
{

/**
 * Removes hoot:change:exclude:delete from way nodes which should not have it. This is functionality
 * specifically for use as a cleanup tool by ChangesetReplacementCreator
 */
class ExcludeDeleteWayNodeCleaner : public ElementOsmMapVisitor
{
public:

  static std::string className() { return "hoot::ExcludeDeleteWayNodeCleaner"; }

  ExcludeDeleteWayNodeCleaner();
  virtual ~ExcludeDeleteWayNodeCleaner() = default;

  virtual void visit(const ElementPtr& e) override;

  virtual void setOsmMap(OsmMap* map) override;

  void setComparisonMap(const ConstOsmMapPtr& map) { _comparisonMap = map; }

  // return empty here, as it doesn't really need to show up for general use when using the info
  // command
  virtual QString getDescription() const
  { return ""; }

  virtual std::string getClassName() const { return className(); }

  virtual QString getInitStatusMessage() const override
  { return "Checking for invalid exclude delete tags..."; }

  virtual QString getCompletedStatusMessage() const override
  {
    return
      "Removed " + StringUtils::formatLargeNumber(_numAffected) +
      " invalid exclude delete tags / " + StringUtils::formatLargeNumber(_numProcessed) +
      " way nodes with those tags.";
  }

private:

  WayNodeCriterion _wayNodeCrit;
  ConstOsmMapPtr _comparisonMap;
};

}

#endif // EXCLUDE_DELETE_WAY_NODE_CLEANER_H
