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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef OPERATION_STATUS_H
#define OPERATION_STATUS_H



namespace hoot
{

/**
 * Interface used to describe information about an operation performed on data.
 *
 * Implementing this interface may not make sense on some operations. If desired, implementers
 * can force a subset of any of the status messages to not be displayed by returning an empty
 * string. Note that this is not a true interface in that parts are implemented. If we ever have
 * multiple inheritance issues with children, it can be converted to a true interface.
 */
class OperationStatus
{
public:

  OperationStatus() : _numAffected(0), _numProcessed(0) {}
  virtual ~OperationStatus() = default;

  /**
    Returns a user friendly message indicating the visitor has begun its work

    e.g. "Removing duplicate ways..."
    */
  virtual QString getInitStatusMessage() const = 0;

  /**
    Returns a user friendly message indicating a summary of the work performed by the visitor

    e.g. "Removed 20 duplicate ways"
    */
  virtual QString getCompletedStatusMessage() const = 0;

  /**
   * Returns the number of elements affected by the visitor
   *
   * @return a number of elements
   */
  virtual long getNumFeaturesAffected() const { return _numAffected; }

  /**
   * Returns the number of elements processed by the visitor
   *
   * @return a number of elements
   */
  virtual long getNumFeaturesProcessed() const { return _numProcessed; }

protected:

  /** Number of elements the operation actually counted or modified */
  long _numAffected;
  /** Number of elements the operation processed in total */
  long _numProcessed;
};

}

#endif // OPERATION_STATUS_H
