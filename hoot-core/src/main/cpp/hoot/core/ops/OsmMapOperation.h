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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OsmMapOperation_H
#define OsmMapOperation_H

// Hoot
#include <hoot/core/info/ApiEntityInfo.h>
#include <hoot/core/criterion/FilteredByGeometryTypeCriteria.h>
#include <hoot/core/info/OperationStatus.h>

// Standard
#include <string>

// Boost
#include <boost/any.hpp>

// Qt
#include <QString>

namespace hoot
{

class OsmMap;

/**
 * Modifies an OsmMap in some way.
 *
 * Due to needing an entire map, this does not support streaming I/O.  If you do not need the
 * entire input map in memory at one time (operation logic does not require it and you are not
 * running in the conflate pipeline), consider using ElementVisitor instead.
 *
 * @todo We should eventually remove the default empty string implementations of OperationStatus
 * methods and require them to be implemented in children.
 */
class OsmMapOperation : public ApiEntityInfo, public FilteredByGeometryTypeCriteria,
  public OperationStatus
{
public:

  static QString className() { return "hoot::OsmMapOperation"; }

  OsmMapOperation() : _numAffected(0), _numProcessed(0) {}
  virtual ~OsmMapOperation() = default;

  /**
   * The operation may make changes to any aspect of the map including creating a new map and
   * assigning the new map to the shared pointer. The only guarantee is that the "map" parameter
   * will point to a valid OsmMap upon completion.
   */
  virtual void apply(std::shared_ptr<OsmMap>& map) = 0;

  /**
   * Allows for returning the result of some calculation done as a result of the map operation.
   *
   * @return boost any pointer to the result data
   */
  virtual boost::any getResult() { boost::any ptr; return ptr; }

  /**
   * @see OperationStatus
   */
  virtual long getNumFeaturesAffected() const { return _numAffected; }

  /**
   * @see OperationStatus
   */
  virtual long getNumFeaturesProcessed() const { return _numProcessed; }

  /**
   * @see OperationStatus
   */
  virtual QString getInitStatusMessage() const { return ""; }

  /**
   * @see OperationStatus
   */
  virtual QString getCompletedStatusMessage() const { return ""; }

  /**
   * @see FilteredByGeometryTypeCriteria
   *
   * An empty list returned here means that the operation is associated no specific criteria and
   * can be run against any feature type. Any operations that want to control which feature types
   * they are run against during conflation should populate this list. The list is treated in a
   * logical OR fashion.
   */
  virtual QStringList getCriteria() const { return QStringList(); }

  virtual QString toString() const override { return ""; }

protected:

  // These will only be used by those implementing OperationStatus.
  long _numAffected;    // how many elements the operation actually counted or did something to
  long _numProcessed;   // how many elements the operation processed total
};

}

#endif // OsmMapOperation_H
