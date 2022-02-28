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
#ifndef WAYSVISITOR_H
#define WAYSVISITOR_H

// hoot
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * @brief The WaysVisitor class can be used to filter ways.
 */
class WaysVisitor : public ConstElementVisitor
{
public:

  static QString className() { return "WaysVisitor"; }

  WaysVisitor(std::vector<ConstWayPtr>& w);
  ~WaysVisitor() override = default;

  /**
   * @see ElementVisitor
   */
  void visit(const std::shared_ptr<Element const>& e) override;

  /**
   * Convenience method.
   */
  static std::vector<ConstWayPtr> extractWays(const ConstOsmMapPtr& map, const ConstElementPtr& e);

  QString getDescription() const override { return "Collects the ways visited"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  std::vector<ConstWayPtr>& _w;
};

}

#endif // WAYSVISITOR_H
