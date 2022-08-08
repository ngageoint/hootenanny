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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef FINDINTERSECTIONSVISITOR_H
#define FINDINTERSECTIONSVISITOR_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{

/**
 * Finds all intersections (nodes), adds some parameters to them and records their node ids
 */
class FindIntersectionsVisitor : public ConstElementVisitor, public OsmMapConsumer
{
public:

  static QString className() { return "FindIntersectionsVisitor"; }

  FindIntersectionsVisitor() = default;
  ~FindIntersectionsVisitor() override = default;

  /**
   * @see ElementVisitor
   */
  void visit(const ConstElementPtr& e) override;

  void setOsmMap(OsmMap* map) override { _map = map; }
  virtual void setOsmMap(const OsmMap* /*map*/) { assert(false); }

  std::vector<long>& getIntersections() { return _ids; }

  QString getInitStatusMessage() const override { return "Finding intersections..."; }
  QString getCompletedStatusMessage() const override
  { return "Found " + QString::number(_numAffected) + " intersections"; }

  virtual ElementCriterionPtr createCriterion(ConstOsmMapPtr map) = 0;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Identifies generic intersections"; }

private:

  OsmMap* _map;
  std::vector<long> _ids;
  ElementCriterionPtr _criterion;
};

/**
 *  Finds all road intersection nodes, adds some parameters to them and records their node ids
 */
class FindHighwayIntersectionsVisitor : public FindIntersectionsVisitor
{
public:

  static QString className() { return "FindHighwayIntersectionsVisitor"; }

  FindHighwayIntersectionsVisitor() = default;
  ~FindHighwayIntersectionsVisitor() override = default;

  QString getDescription() const override { return "Identifies road intersections"; }

  QString getInitStatusMessage() const override
  { return "Finding road intersections..."; }
  QString getCompletedStatusMessage() const override
  { return "Found " + QString::number(_numAffected) + " road intersections"; }

  ElementCriterionPtr createCriterion(ConstOsmMapPtr map) override;
};

/**
 *  Finds all railway intersection nodes, adds some parameters to them and records their node ids
 */
class FindRailwayIntersectionsVisitor : public FindIntersectionsVisitor
{
public:

  static QString className() { return "FindRailwayIntersectionsVisitor"; }

  FindRailwayIntersectionsVisitor() = default;
  ~FindRailwayIntersectionsVisitor() override = default;

  QString getDescription() const override { return "Identifies railway intersections"; }

  QString getInitStatusMessage() const override
  { return "Finding railway intersections..."; }
  QString getCompletedStatusMessage() const override
  { return "Found " + QString::number(_numAffected) + " railway intersections"; }

  ElementCriterionPtr createCriterion(ConstOsmMapPtr map) override;
};

}

#endif // FINDINTERSECTIONSVISITOR_H
