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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef FINDINTERSECTIONSOP_H
#define FINDINTERSECTIONSOP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

class FindIntersectionsVisitor;

/**
 * Finds road intersections
 */
class FindIntersectionsOp : public OsmMapOperation
{
public:

  static QString className() { return "FindIntersectionsOp"; }

  static QString opsKey() { return "map.cleaner.transforms"; }

  FindIntersectionsOp() = default;
  virtual ~FindIntersectionsOp() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   *  Pure virtual function called by constructor to create the intersection visitor
   */
  virtual std::shared_ptr<FindIntersectionsVisitor> createVisitor() = 0;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Identifies generic intersections"; }
};

/**
 *  Op that finds all road intersections
 */
class FindHighwayIntersectionsOp : public FindIntersectionsOp
{
public:

  FindHighwayIntersectionsOp() = default;
  ~FindHighwayIntersectionsOp() override = default;

  static QString className() { return "FindHighwayIntersectionsOp"; }

  std::shared_ptr<FindIntersectionsVisitor> createVisitor() override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Identifies highway intersections"; }
};

/**
 *  Op that finds all railway intersections
 */
class FindRailwayIntersectionsOp : public FindIntersectionsOp
{
public:

  FindRailwayIntersectionsOp() = default;
  ~FindRailwayIntersectionsOp() override = default;

  static QString className() { return "FindRailwayIntersectionsOp"; }

  std::shared_ptr<FindIntersectionsVisitor> createVisitor() override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Identifies railway intersections"; }
};

}

#endif // FINDHIGHWAYINTERSECTIONSOP_H
