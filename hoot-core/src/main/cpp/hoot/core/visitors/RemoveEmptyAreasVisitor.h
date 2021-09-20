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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVEEMPTYAREASVISITOR_H
#define REMOVEEMPTYAREASVISITOR_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{
class ElementToGeometryConverter;

/**
 * Removes all "area" elements that have an area of zero.
 */
class RemoveEmptyAreasVisitor : public ElementOsmMapVisitor, public Configurable
{
public:

  static QString className() { return "RemoveEmptyAreasVisitor"; }

  RemoveEmptyAreasVisitor();
  ~RemoveEmptyAreasVisitor() override = default;

  void visit(const std::shared_ptr<Element>& e) override;

  QString getInitStatusMessage() const override { return "Removing empty areas..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " empty areas"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes empty areas"; }

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

private:

  std::shared_ptr<ElementToGeometryConverter> _ec;
  bool _requireAreaForPolygonConversion;
};

}

#endif // REMOVEEMPTYAREASVISITOR_H
