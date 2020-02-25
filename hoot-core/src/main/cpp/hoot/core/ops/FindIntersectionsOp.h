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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FINDINTERSECTIONSOP_H
#define FINDINTERSECTIONSOP_H

// Hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/ops/OsmMapOperation.h>

// Qt
#include <QString>

namespace hoot
{

class FindIntersectionsVisitor;

/**
 * Finds road intersections
 */
class FindIntersectionsOp : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::FindIntersectionsOp"; }

  static QString opsKey() { return "map.cleaner.transforms"; }

  FindIntersectionsOp();

  virtual void apply(std::shared_ptr<OsmMap>& map) override;

  virtual QString getDescription() const override { return "Identifies generic intersections"; }

  /**
   *  Pure virtual function called by constructor to create the intersection visitor
   */
  virtual std::shared_ptr<FindIntersectionsVisitor> createVisitor() = 0;

  virtual std::string getClassName() const { return className(); }
};

/**
 *  Op that finds all road intersections
 */
class FindHighwayIntersectionsOp : public FindIntersectionsOp
{
public:

  static std::string className() { return "hoot::FindHighwayIntersectionsOp"; }

  virtual QString getDescription() const override { return "Identifies highway intersections"; }

  virtual std::shared_ptr<FindIntersectionsVisitor> createVisitor();

  virtual std::string getClassName() const { return className(); }
};

/**
 *  Op that finds all railway intersections
 */
class FindRailwayIntersectionsOp : public FindIntersectionsOp
{
public:

  static std::string className() { return "hoot::FindRailwayIntersectionsOp"; }

  virtual QString getDescription() const override { return "Identifies railway intersections"; }

  virtual std::shared_ptr<FindIntersectionsVisitor> createVisitor();

  virtual std::string getClassName() const { return className(); }
};

}

#endif // FINDHIGHWAYINTERSECTIONSOP_H
