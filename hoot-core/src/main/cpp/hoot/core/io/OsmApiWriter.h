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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_API_WRITER_H
#define OSM_API_WRITER_H

//  hoot
#include <hoot/core/util/Configurable.h>

// Boost
#include <boost/shared_ptr.hpp>

//  Qt
#include <QList>
#include <QNetworkAccessManager>
#include <QString>
#include <QUrl>

//  Forward declaration
class QNetworkReply;

namespace hoot
{

//  Forward declarations
class OsmApiWriterTest;

enum OsmApiStatus
{
  ONLINE,
  READONLY,
  OFFLINE
};

class OsmApiCapabilites
{
public:
  OsmApiCapabilites()
    : _version("0.6"), _tracepoints(5000), _waynodes(2000), _changesets(10000), _timeout(300),
      _database(OsmApiStatus::OFFLINE), _api(OsmApiStatus::OFFLINE), _gpx(OsmApiStatus::OFFLINE)
  {}
  QString getVersion() { return _version; }
  void setVersion(const QString& version) { _version = version; }
  long getTracepoints() { return _tracepoints; }
  void setTracepoints(long tracepoints) { _tracepoints = tracepoints; }
  long getWayNodes() { return _waynodes; }
  void setWayNodes(long waynodes) { _waynodes = waynodes; }
  long getChangesets() { return _changesets; }
  void setChangesets(long changesets) { _changesets = changesets; }
  long getTimeout() { return _timeout; }
  void setTimeout(long timeout) { _timeout = timeout; }
  OsmApiStatus getDatabaseStatus() { return _database; }
  void setDatabaseStatus(OsmApiStatus status) { _database = status; }
  OsmApiStatus getApiStatus() { return _api; }
  void setApiStatus(OsmApiStatus status) { _api = status; }
  OsmApiStatus getGpxStatus() { return _gpx; }
  void setGpxStatus(OsmApiStatus status) { _gpx = status; }

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

class OsmApiWriter : public Configurable
{
public:

  OsmApiWriter(const QUrl& url, const QList<QString>& changesets);

  virtual void setConfiguration(const Settings& conf);

  /**
   * Check if the url is supported.
   */
  bool isSupported(const QUrl& url);

  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Capabilities:_GET_.2Fapi.2Fcapabilities
  bool queryCapabilities();
  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Retrieving_permissions:_GET_.2Fapi.2F0.6.2Fpermissions
  bool validatePermissions();

  bool apply(const QString& description);

private:
  //  for white box testing
  friend class OsmApiWriterTest;
  OsmApiWriter() {}

  OsmApiCapabilites _parseCapabilities(const QString& capabilites);
  OsmApiStatus _parseStatus(const QString& status);

  bool _parsePermissions(const QString& permissions);

  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Create:_PUT_.2Fapi.2F0.6.2Fchangeset.2Fcreate
  long _createChangeset(const QString& description);
  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Close:_PUT_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fclose
  void _closeChangeset(long id);
  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Diff_upload:_POST_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fupload
  bool _uploadChangeset(long id, const QString& changeset);

  bool _networkRequest(QUrl url,
    QNetworkAccessManager::Operation http_op = QNetworkAccessManager::Operation::GetOperation,
    const QByteArray& data = QByteArray());

  int _getHttpResponseCode(QNetworkReply* reply);

  QUrl _url;
  QList<QString> _changesets;
  int _maxWriters;
  long _maxChangesetSize;
  OsmApiCapabilites _capabilities;
  QByteArray _content;
  int _status;

  const QString API_PATH_CAPABILITIES = "/api/capabilities/";
  const QString API_PATH_PERMISSIONS = "/api/0.6/permissions/";
  const QString API_PATH_CREATE_CHANGESET = "/api/0.6/changeset/create/";
  const QString API_PATH_CLOSE_CHANGESET = "/api/0.6/changeset/%1/close/";
  const QString API_PATH_UPLOAD_CHANGESET = "/api/0.6/changeset/%1/upload/";
};

}

#endif  //  OSM_API_WRITER_H

