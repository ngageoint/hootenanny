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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FINDHIGHWAYINTERSECTIONSVISITOR_H
#define FINDHIGHWAYINTERSECTIONSVISITOR_H

// hoot
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

/**
 * Finds all intersections (nodes), adds some parameters to them and records their node ids
 */
class FindHighwayIntersectionsVisitor : public ConstElementVisitor, public OsmMapConsumer,
  public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::FindHighwayIntersectionsVisitor"; }

  FindHighwayIntersectionsVisitor() {}

  virtual void visit(const ConstElementPtr& e);

  virtual void setOsmMap(OsmMap* map) { _map = map; }

  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  std::vector<long>& getIntersections() { return _ids; }

  virtual QString getDescription() const { return "Identifies road intersections"; }

  virtual QString getInitStatusMessage() const
  { return "Finding road intersections..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Found " + QString::number(_numAffected) + " road intersections"; }

private:

  OsmMap* _map;
  std::vector<long> _ids;
};

}

#endif // FINDHIGHWAYINTERSECTIONSVISITOR_H
