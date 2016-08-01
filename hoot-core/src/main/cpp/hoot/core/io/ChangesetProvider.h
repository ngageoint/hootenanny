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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHANGSETPROVIDER_H
#define CHANGSETPROVIDER_H

// GDAL
#include <ogr_spatialref.h>

// hoot
#include <hoot/core/elements/Element.h>

namespace hoot
{

/**
 * Represents an individual OSM change in a changeset
 */
class Change
{

public:

  /**
   * The allowable changeset types
   */
  enum ChangeType
  {
    Create = 0,
    Modify = 1,
    Delete = 2,
    Unknown = 3
  };

   Change() {}

  static QString changeTypeToString(const ChangeType changeType)
  {
    switch (changeType)
    {
      case Create:
        return "Create";
      case Modify:
        return "Modify";
      case Delete:
        return "Delete";
      case Unknown:
        return "Unknown";
    }
    throw HootException("Invalid change type.");
  }

  QString toString() const
  {
    return
      "Change type: " + changeTypeToString(type) + ", ID: " + e->getElementId().toString() +
      ", Note: " + note;
  }

  ConstElementPtr e;
  ChangeType type;
  QString note;

};


/**
 * Interface for classes implementing OSM changeset capabilities
 */
class ChangeSetProvider
{

public:

  /**
   * @brief getProjection
   * @return
   */
  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const = 0;

  /**
   * @brief ~ElementInputStream
   *
   * If the stream is open when the destructor is called, closeStream must be called in the destructor
   */
  virtual ~ChangeSetProvider() {}

  /**
   * @brief closeStream
   *
   * Releases all resources associated with the stream, if any
   */
  virtual void close() = 0;

  /**
   * Determines if the changeset has any unparsed changes
   *
   * @return true if the changeset has more changes; false otherwise
   */
  virtual bool hasMoreChanges() = 0;

  /**
   * Returns the next change in the changeset
   *
   * @return a changeset change
   */
  virtual Change readNextChange() = 0;

};

typedef shared_ptr<ChangeSetProvider> ChangeSetProviderPtr;

}

#endif // CHANGSETPROVIDER_H
