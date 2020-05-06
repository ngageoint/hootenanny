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
#ifndef MULTILINESTRINGVISITOR_H
#define MULTILINESTRINGVISITOR_H

// geos
#include <geos/geom/MultiLineString.h>

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/ElementProvider.h>

// standard
#include <string>

namespace hoot
{

class Way;

/**
 * Creates a multi-linestring out of all ways that are visited. There are no checks to be certain
 * that the ways are actually linear ways. Any way with < 2 nodes will be skipped.
 */
class MultiLineStringVisitor : public ConstElementVisitor
{
public:

  static std::string className() { return "hoot::MultiLineStringVisitor"; }

  MultiLineStringVisitor();

  void setElementProvider(const ConstElementProviderPtr& provider ) { _provider = provider; }

  /**
   * Retrieves the multi line string created by this visitor. The caller retains ownership. If the
   * method is called multiple times the first call will create a multi line string for the elements
   * visited and clear any history. If it is called again (without visiting) it will return a valid
   * but empty geometry.
   */
  geos::geom::MultiLineString* createMultiLineString();

  virtual void visit(const ConstElementPtr& e);

  virtual void visit(const ConstWayPtr& w);

  virtual QString getDescription() const { return "Creates a multi-linestring out of ways"; }

  virtual QString getInitStatusMessage() const
  { return "Creating multi-linestrings..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Created " + QString::number(_numAffected) + " multi-line strings"; }

  virtual std::string getClassName() const { return className(); }

protected:

  ConstElementProviderPtr _provider;
  std::vector<geos::geom::Geometry*>* _ls;
};

}

#endif // MULTILINESTRINGVISITOR_H
