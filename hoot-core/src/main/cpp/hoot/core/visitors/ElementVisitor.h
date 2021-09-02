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

namespace hoot
{

/**
 * Visits elements in a manner in which they can be modified.
 *
 * This allows for streaming I/O during data conversions if not combined with an OsmMapConsumer.
 * Favor this over OsmMapOperation when you do not need the entire input map in memory at once
 * (visitor logic does not require it and you are not running in the conflate pipeline where all map
 * data must be read into memory).
 *
 * Most ElementVisitors added to the conflate pipeline (conflate.pre.ops and conflate.post.ops)
 * should either:
 *
 * 1) override the default implementation of FilteredByGeometryTypeCriteria::getCriteria or
 * 2) implement the ConflateInfoCacheConsumer interface (doing both is ok).
 *
 * To implement FilteredByGeometryTypeCriteria::getCriteria, return a list of supported element
 * criteria the visitor operates on (e.g. for roads, return HighwayCriterion). If the visitor
 * operates on elements whose types maps to a conflate matcher, then the
 * SuperfluousConflateOpRemover will ensure the visitor is not run in the conflate pipeline if that
 * matcher isn't part of the configuration.
 *
 * Alternatively, if the visitor only operates generically on elements that may have multiple
 * feature types (e.g. operates on all ways and FilteredByGeometryTypeCriteria::getCriteria returns
 * LinearCriterion), ConflateInfoCacheConsumer should be implemented and the info in the cache be
 * used to ensure that only elements that are conflatable in the current conflation configuration
 * are modified (see ConflateInfoCache::elementCanBeConflatedByActiveMatcher and
 * DuplicateNameRemover as an example implementation). An example of a visitor that doesn't need to
 * implement either interface is RemoveMissingElementsVisitor, due to the fact that we always want
 * to remove references to all missing elements regardless of whether they are conflatable in the
 * current configuration or not.
 *
 * We could eventually remove the default empty string implementations of OperationStatus methods
 * and require them to be implemented in children. If we ever have multiple inheritance issues via
 * inheritance from the OperationStatus, we can change it to be a proper interface.
 */
class ElementVisitor : public ApiEntityInfo, public FilteredByGeometryTypeCriteria,
  public OperationStatus
{
public:

  static QString className() { return "ElementVisitor"; }

  ElementVisitor() = default;
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
};

using ElementVisitorPtr = std::shared_ptr<ElementVisitor>;

}

#endif // ELEMENTVISITOR_H
