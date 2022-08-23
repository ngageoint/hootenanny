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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef FILTEREDVISITOR_H
#define FILTEREDVISITOR_H

// hoot
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/visitors/ElementVisitorConsumer.h>

namespace hoot
{

class FilteredVisitor : public ConstElementVisitor, public ConstOsmMapConsumer,
  public ElementCriterionConsumer, public ElementVisitorConsumer
{

public:

  static QString className() { return "FilteredVisitor"; }

  FilteredVisitor();
  /**
   * Calls the visit method on visitor whenever ElementCriterion::isSatisfied == true.
   */
  FilteredVisitor(const ElementCriterion& criterion, ElementVisitor& visitor);
  /**
   * Similar to the first, but takes smart pointer params.
   */
  FilteredVisitor(ElementCriterionPtr criterion, ElementVisitorPtr visitor);
  ~FilteredVisitor() override = default;

  static double getStat(ElementCriterionPtr criterion, ElementVisitorPtr visitor, const ConstOsmMapPtr& map);
  static double getStat(ElementCriterionPtr criterion, ElementVisitorPtr visitor, const ConstOsmMapPtr& map,
                        const ElementPtr& element);
  static double getStat(const ElementCriterion& criterion, ElementVisitor& visitor, const ConstOsmMapPtr& map,
                        const ElementPtr& element);

  void addCriterion(const ElementCriterionPtr& e) override;

  void addVisitor(const ElementVisitorPtr& v) override;

  void setOsmMap(OsmMap* map) override;
  void setOsmMap(const OsmMap* map) override;

  /**
   * @see ElementVisitor
   */
  void visit(const ConstElementPtr& e) override;

  QString getDescription() const override { return ""; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  ElementVisitor& getChildVisitor() const { return *_visitor; }

private:

  const ElementCriterion* _criterion;
  ElementVisitor* _visitor;
  const OsmMap* _map;
};

}

#endif // FILTEREDVISITOR_H
