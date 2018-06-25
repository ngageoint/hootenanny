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
#include <hoot/core/io/OsmChangesetElement.h>
#include <hoot/core/util/Configurable.h>

//  Standard
#include <mutex>
#include <queue>
#include <thread>

//  Boost
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

class OsmApiNetworkRequest
{
public:
  OsmApiNetworkRequest();

  bool networkRequest(QUrl url,
    QNetworkAccessManager::Operation http_op = QNetworkAccessManager::Operation::GetOperation,
    const QByteArray& data = QByteArray());

  const QByteArray& getResponseContent() { return _content; }
  int getHttpStatus() { return _status; }

private:

  int _getHttpResponseCode(QNetworkReply* reply);

  QByteArray _content;
  int _status;
};

typedef boost::shared_ptr<OsmApiNetworkRequest> OsmApiNetworkRequestPtr;

class OsmApiWriter : public Configurable
{
public:

  OsmApiWriter(const QUrl& url, const QList<QString>& changesets);

  virtual void setConfiguration(const Settings& conf);

  /**
   * Check if the url is supported.
   */
  bool isSupported(const QUrl& url);

  bool apply();

  bool containsFailed() { return _changeset.hasFailedElements(); }

  QString getFailedChangeset() { return _changeset.getFailedChangesetString(); }

  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Capabilities:_GET_.2Fapi.2Fcapabilities
  bool queryCapabilities(OsmApiNetworkRequestPtr request);
  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Retrieving_permissions:_GET_.2Fapi.2F0.6.2Fpermissions
  bool validatePermissions(OsmApiNetworkRequestPtr request);

  //  TEMPORARY:
  void doParallel(bool parallel) { _parallel = parallel; }

private:
  //  for white box testing
  friend class OsmApiWriterTest;
  OsmApiWriter() {}
  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Create:_PUT_.2Fapi.2F0.6.2Fchangeset.2Fcreate
  long _createChangeset(OsmApiNetworkRequestPtr request, const QString& description);
  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Close:_PUT_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fclose
  void _closeChangeset(OsmApiNetworkRequestPtr request, long id);
  //  https://wiki.openstreetmap.org/wiki/API_v0.6#Diff_upload:_POST_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fupload
  bool _uploadChangeset(OsmApiNetworkRequestPtr request, long id, const QString& changeset);

  OsmApiCapabilites _parseCapabilities(const QString& capabilites);
  OsmApiStatus _parseStatus(const QString& status);

  bool _parsePermissions(const QString& permissions);

  void _changesetThreadFunc();

  std::vector<std::thread> _threadPool;

  std::queue<ChangesetInfoPtr> _workQueue;
  std::mutex _workQueueMutex;

  XmlChangeset _changeset;
  std::mutex _changesetMutex;

  QUrl _url;
  QList<QString> _changesets;
  QString _description;
  int _maxWriters;
  long _maxChangesetSize;
  OsmApiCapabilites _capabilities;

  //  TEMPORARY:
  bool _parallel;

  const QString API_PATH_CAPABILITIES = "/api/capabilities/";
  const QString API_PATH_PERMISSIONS = "/api/0.6/permissions/";
  const QString API_PATH_CREATE_CHANGESET = "/api/0.6/changeset/create/";
  const QString API_PATH_CLOSE_CHANGESET = "/api/0.6/changeset/%1/close/";
  const QString API_PATH_UPLOAD_CHANGESET = "/api/0.6/changeset/%1/upload/";
};

}

#endif  //  OSM_API_WRITER_H

