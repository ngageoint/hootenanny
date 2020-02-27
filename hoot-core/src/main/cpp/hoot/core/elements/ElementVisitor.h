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
#ifndef ELEMENTVISITOR_H
#define ELEMENTVISITOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/info/ApiEntityInfo.h>
#include <hoot/core/criterion/FilteredByCriteria.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

/**
 * Visits elements in a collection in a way that they can be modified.
 *
 * This allows for streaming I/O if not combined with an OsmMapConsumer.  Favor this over
 * OsmMapOperation when you do not need the entire input map in memory at once (visitor logic
 * does not require it and you are not running in the conflate pipeline where all map data must
 * be read into memory).
 *
 * @todo We could eventually remove the default empty string implementations of OperationStatusInfo
 * methods and require them to be implemented in children.
 */
class ElementVisitor : public ApiEntityInfo, public FilteredByCriteria, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::ElementVisitor"; }

  ElementVisitor() : _numAffected(0), _numProcessed(0) {}
  virtual ~ElementVisitor() {}

  /**
   * Performs the visitor's logic on a single element
   *
   * @param e the element to operate on
   */
  virtual void visit(const ElementPtr& e) = 0;

  /**
   * @see OperationStatusInfo
   */
  virtual long getNumFeaturesAffected() const { return _numAffected; }

  /**
   * @see OperationStatusInfo
   */
  virtual long getNumFeaturesProcessed() const { return _numProcessed; }

  /**
   * @see OperationStatusInfo
   */
  virtual QString getInitStatusMessage() const { return ""; }

  /**
   * @see OperationStatusInfo
   */
  virtual QString getCompletedStatusMessage() const { return ""; }

  /**
   * @see FilteredByCriteria
   *
   * An empty list returned here means that the visitor is associated no specific criteria and
   * can be run against any feature type. Any visitors that want to control which feature types
   * they are run against during conflation should populate this list. The list is treated in a
   * logical OR fashion.
   */
  virtual QStringList getCriteria() const { return QStringList(); }

  /**
   * Returns the visitor's class name
   *
   * @return class name string
   */
  virtual std::string getClassName() const = 0;

protected:

  // These will only be used by those implementing OperationStatusInfo.
  long _numAffected;    // how many elements the operation actually counted or did something to
  long _numProcessed;   // how many elements the operation processed total
};

typedef std::shared_ptr<ElementVisitor> ElementVisitorPtr;

}

#endif // ELEMENTVISITOR_H
