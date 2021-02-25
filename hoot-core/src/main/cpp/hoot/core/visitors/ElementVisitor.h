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
#ifndef ELEMENTVISITOR_H
#define ELEMENTVISITOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/info/ApiEntityInfo.h>
#include <hoot/core/criterion/FilteredByGeometryTypeCriteria.h>
#include <hoot/core/info/OperationStatus.h>
#include <hoot/core/conflate/ElementConflatableCheck.h>

namespace hoot
{

/**
 * Visits elements in a way that they can be modified.
 *
 * This allows for streaming I/O if not combined with an OsmMapConsumer.  Favor this over
 * OsmMapOperation when you do not need the entire input map in memory at once (visitor logic
 * does not require it and you are not running in the conflate pipeline where all map data must
 * be read into memory).
 *
 * Nearly all ElementVisitors that are added to the conflate pipeline (conflate.pre.ops or
 * conflate.post.ops) should override the default implementation of
 * FilteredByGeometryTypeCriteria::getCriteria. Optionally, visitors should make use of
 * ElementConflatableCheck::getCheckConflatable. The only visitors that would not need to implement
 * either of these would be cleaning ops that work against non-typed elements (e.g.
 * NoInformationElementRemover). Implement FilteredByGeometryTypeCriteria::getCriteria and return a
 * list of supported element criteria the visitor operates on (e.g. roads; return HighwayCriterion).
 * If the visitor operates generically on elements that may have multiple feature types (e.g. all
 * ways and FilteredByGeometryTypeCriteria::getCriteria returns LinearCriterion), add logic to the
 * visitor when ElementConflatableCheck::getCheckConflatable returns 'true' to only operate on
 * elements that are conflatable in the current conflation configuration (see
 * ConflateUtils::elementCanBeConflatedByActiveMatcher).
 *
 * We could eventually remove the default empty string implementations of OperationStatus methods
 * and require them to be implemented in children.
 */
class ElementVisitor : public ApiEntityInfo, public FilteredByGeometryTypeCriteria,
  public OperationStatus, public ElementConflatableCheck
{
public:

  static QString className() { return "hoot::ElementVisitor"; }

  ElementVisitor() : _checkConflatable(false) {}
  virtual ~ElementVisitor() = default;

  /**
   * Performs the visitor's logic on a single element
   *
   * @param e the element to operate on
   */
  virtual void visit(const ElementPtr& e) = 0;

  /**
   * @see OperationStatus
   */
  QString getInitStatusMessage() const override { return ""; }

  /**
   * @see OperationStatus
   */
  QString getCompletedStatusMessage() const override { return ""; }

  /**
   * @see FilteredByGeometryTypeCriteria
   *
   * An empty list returned here means that the visitor is associated with no specific element type
   * criteria and can be run against any feature type. Any visitors that want to control which
   * feature types they are run against during conflation should populate this list. The list is
   * treated in a logical OR fashion.
   */
  QStringList getCriteria() const override { return QStringList(); }

  QString toString() const override { return ""; }

  /**
   * @see ElementConflatableCheck
   */
  virtual bool getCheckConflatable() const { return _checkConflatable; }
  /**
   * @see ElementConflatableCheck
   */
  virtual void setCheckConflatable(const bool checkConflatable)
  { _checkConflatable = checkConflatable; }

protected:

  bool _checkConflatable;
};

typedef std::shared_ptr<ElementVisitor> ElementVisitorPtr;

}

#endif // ELEMENTVISITOR_H
