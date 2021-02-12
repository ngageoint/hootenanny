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
 * @copyright Copyright (C) 2018 Maxar (http://www.maxar.com/)
 */

#ifndef OSM_API_CAPABILITIES_H
#define OSM_API_CAPABILITIES_H
#include <QString>

namespace hoot
{
//  OSM API status
enum OsmApiStatus
{
  ONLINE,
  READONLY,
  OFFLINE
};

/**
 *  Class to match the capabilities response from the OSM API '/api/capabilities/' request
 *  See:    https://wiki.openstreetmap.org/wiki/API_v0.6#Capabilities:_GET_.2Fapi.2Fcapabilities
 */
class OsmApiCapabilites
{
public:
  /**
   *
   */
  OsmApiCapabilites()
    : _version("0.6"),
      _tracepoints(5000),
      _waynodes(2000),
      _changesets(10000),
      _timeout(300),
      _database(OsmApiStatus::OFFLINE),
      _api(OsmApiStatus::OFFLINE),
      _gpx(OsmApiStatus::OFFLINE)
  {}

  /**
   *  Setter/getter for OSM API version, should be "0.6"
   */
  QString getVersion()                    { return _version; }
  void setVersion(const QString& version) { _version = version; }

  /**
   *  Setter/getter for max number of GPS trace points allowed to ingest
   */
  long getTracepoints()                 { return _tracepoints; }
  void setTracepoints(long tracepoints) { _tracepoints = tracepoints; }

  /**
   *  Setter/getter for max number of nodes per way allowed
   */
  long getWayNodes()              { return _waynodes; }
  void setWayNodes(long waynodes) { _waynodes = waynodes; }

  /**
   *  Setter/getter for max number of elements per changeset
   */
  long getChangesets()                { return _changesets; }
  void setChangesets(long changesets) { _changesets = changesets; }

  /**
   *  Setter/getter for API timeout in seconds
   */
  long getTimeout()             { return _timeout; }
  void setTimeout(long timeout) { _timeout = timeout; }

  /**
   *  Setter/getter for database status
   */
  OsmApiStatus getDatabaseStatus()            { return _database; }
  void setDatabaseStatus(OsmApiStatus status) { _database = status; }

  /**
   *  Setter/getter for API status
   */
  OsmApiStatus getApiStatus()             { return _api; }
  void setApiStatus(OsmApiStatus status)  { _api = status; }

  /**
   *  Setter/getter for GPX status
   */
  OsmApiStatus getGpxStatus()             { return _gpx; }
  void setGpxStatus(OsmApiStatus status)  { _gpx = status; }

private:
  QString _version;
  long _tracepoints;
  long _waynodes;
  long _changesets;
  long _timeout;
  OsmApiStatus _database;
  OsmApiStatus _api;
  OsmApiStatus _gpx;
};

}

#endif  //  OSM_API_CAPABILITIES_H
