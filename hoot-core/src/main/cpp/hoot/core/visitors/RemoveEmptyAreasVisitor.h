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
#ifndef REMOVEEMPTYAREASVISITOR_H
#define REMOVEEMPTYAREASVISITOR_H

// Standard
#include <memory>

#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/info/OperationStatusInfo.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{
class ElementConverter;

/**
 * Removes all "area" elements that have an area of zero.
 */
class RemoveEmptyAreasVisitor : public ElementOsmMapVisitor, public OperationStatusInfo,
  public Configurable
{
public:

  static std::string className() { return "hoot::RemoveEmptyAreasVisitor"; }

  RemoveEmptyAreasVisitor();

  virtual void visit(const ConstElementPtr& e);

  virtual void visit(const std::shared_ptr<Element>& e);

  virtual QString getInitStatusMessage() const { return "Removing empty areas..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " empty areas"; }

  virtual QString getDescription() const { return "Removes empty areas"; }

  /**
   * @see FilteredByCriteria
   */
  virtual QStringList getCriteria() const;

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

private:

  std::shared_ptr<ElementConverter> _ec;
  bool _requireAreaForPolygonConversion;
};

}

#endif // REMOVEEMPTYAREASVISITOR_H
