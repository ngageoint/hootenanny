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
#ifndef MULTILINESTRINGVISITOR_H
#define MULTILINESTRINGVISITOR_H

// geos
#include <geos/geom/MultiLineString.h>

// hoot
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{
/**
 * Creates a multi-linestring out of all ways that are visited.
 *
 * There are no checks to be certain that the ways are actually linear ways. Any way with < 2 nodes
 * will be skipped.
 */
class MultiLineStringVisitor : public ConstElementVisitor
{
public:

  static QString className() { return "MultiLineStringVisitor"; }

  MultiLineStringVisitor() = default;
  ~MultiLineStringVisitor() override = default;

  /**
   * Retrieves the multiline string created by this visitor. If the method is called multiple times
   * the first call will create a multi line string for the elements visited and clear any history.
   * If it is called again (without visiting) it will return a valid but empty geometry.
   */
  std::shared_ptr<geos::geom::Geometry> createGeometry();

  void visit(const ConstElementPtr& e) override;

  QString getInitStatusMessage() const override
  { return "Creating multi-linestrings..."; }
  QString getCompletedStatusMessage() const override
  { return "Created " + QString::number(_numAffected) + " multi-line strings"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Creates a multi-linestring out of ways"; }

  void setElementProvider(const ConstElementProviderPtr& provider) { _provider = provider; }

private:

  ConstElementProviderPtr _provider;
  std::vector<ConstWayPtr> _ways;

  void _visit(const ConstWayPtr& w);
};

}

#endif // MULTILINESTRINGVISITOR_H
