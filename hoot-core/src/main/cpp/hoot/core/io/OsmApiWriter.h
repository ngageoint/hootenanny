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
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/io/OsmApiCapabilites.h>
#include <hoot/core/io/OsmApiChangeset.h>
#include <hoot/core/io/OsmApiChangesetElement.h>
#include <hoot/core/ops/stats/SingleStat.h>
#include <hoot/core/util/Configurable.h>

//  Standard
#include <mutex>
#include <queue>
#include <thread>

//  Qt
#include <QList>
#include <QString>

namespace hoot
{

//  Forward declarations
class OsmApiWriterTest;

class OsmApiWriter : public Configurable
{
  /** OSM API URL paths */
  const QString API_PATH_CAPABILITIES = "/api/capabilities/";
  const QString API_PATH_PERMISSIONS = "/api/0.6/permissions/";
  const QString API_PATH_CREATE_CHANGESET = "/api/0.6/changeset/create/";
  const QString API_PATH_CLOSE_CHANGESET = "/api/0.6/changeset/%1/close/";
  const QString API_PATH_UPLOAD_CHANGESET = "/api/0.6/changeset/%1/upload/";
  /**
   *  Max number of jobs waiting in the work queue = multiplier * number of threads,
   *  this keeps the producer thread from creating too many sub-changesets too early
   *  that only consist of nodes after ways are blocked.
   */
  const int QUEUE_SIZE_MULTIPLIER = 2;
public:
  /** Constructors with one or multiple files consisting of one large changeset */
  OsmApiWriter(const QUrl& url, const QString& changeset);
  OsmApiWriter(const QUrl& url, const QList<QString>& changesets);
  /**
   * @brief setConfiguration Update the configuration settings with new configuration
   * @param conf - Updated configurations
   */
  virtual void setConfiguration(const Settings& conf);
  /**
   * @brief isSupported
   * @param url - Must be a valid, full HTTP[S] URL pointing to an OSM website
   * @return validity of url
   */
  bool isSupported(const QUrl& url);
  /**
   * @brief apply Actually load, divide, and write the changeset to the selected OSM API
   * @return success
   */
  bool apply();
  /**
   * @brief containsFailed
   * @return true if there are failed changes in the changeset
   */
  bool containsFailed() { return _changeset.hasFailedElements(); }
  /**
   * @brief getFailedChangeset Creates a changeset will all elements that failed to push to API
   * @return Full .OSC changeset string
   */
  QString getFailedChangeset() { return _changeset.getFailedChangesetString(); }
  /**
   * @brief queryCapabilities GET the capabilities of the OSM API
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Capabilities:_GET_.2Fapi.2Fcapabilities
   * @param request - Network request object initialized with OSM API URL
   * @return successful query and parse of data
   */
  bool queryCapabilities(HootNetworkRequestPtr request);
  /**
   * @brief validatePermissions Check the permissions of the current user against the current OSM API
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Retrieving_permissions:_GET_.2Fapi.2F0.6.2Fpermissions
   * @param request - Network request object initialized with OSM API URL
   * @return true if the current user has write permission for the OSM API
   */
  bool validatePermissions(HootNetworkRequestPtr request);
  /**
   * @brief getStats Get the stats object
   * @return
   */
  QList<SingleStat> getStats() const { return _stats; }
  /**
   * @brief showProgress Set the show progress flag for progress during upload
   * @param show True shows an adaptive, granular progress
   */
  void showProgress(bool show) { _showProgress = show; }

private:
  /**
   * @brief _createChangeset Request a changeset ID from the API
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Create:_PUT_.2Fapi.2F0.6.2Fchangeset.2Fcreate
   * @param request - Network request object initialized with OSM API URL
   * @param description - Text description of the changeset to create
   * @return ID of the changeset that was created on the server
   */
  long _createChangeset(HootNetworkRequestPtr request, const QString& description);
  /**
   * @brief _closeChangeset End the changeset
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Close:_PUT_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fclose
   * @param request - Network request object initialized with OSM API URL
   * @param id - ID of the changeset to close
   */
  void _closeChangeset(HootNetworkRequestPtr request, long id);
  /**
   * @brief _uploadChangeset Upload a changeset to the OSM API
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Diff_upload:_POST_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fupload
   * @param request - Network request object initialized with OSM API URL
   * @param id - ID of the changeset to upload the data to
   * @param changeset - Atomic changeset to upload
   * @return true if the changeset was uploaded correctly
   */
  bool _uploadChangeset(HootNetworkRequestPtr request, long id, const QString& changeset);
  /**
   * @brief _parseCapabilities Parse the OSM API capabilities
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Capabilities:_GET_.2Fapi.2Fcapabilities
   * @param capabilites - XML response from the capabilites request
   * @return parsed capabilities object
   */
  OsmApiCapabilites _parseCapabilities(const QString& capabilites);
  /**
   * @brief _parsePermissions Parse the OSM API perissions
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Retrieving_permissions:_GET_.2Fapi.2F0.6.2Fpermissions
   * @param permissions - XML response from the permissions request
   * @return true if the current user has write permissions to the OSM API
   */
  bool _parsePermissions(const QString& permissions);
  /**
   * @brief _parseStatus Convert string status to enumeration status
   * @param status String version of the API status
   * @return Enum version of the API status
   */
  OsmApiStatus _parseStatus(const QString& status);
  /**
   * @brief _changesetThreadFunc Thread function that does the actual work of creating a changeset ID
   *  via the API, pushing the changeset data, closing the changeset, and splitting a failing changeset
   *  if necessary
   */
  void _changesetThreadFunc();
  /** Changeset processing thread pool */
  std::vector<std::thread> _threadPool;
  /** Queue for producer/consumer work model */
  std::queue<ChangesetInfoPtr> _workQueue;
  /** Mutex protecting work queue */
  std::mutex _workQueueMutex;
  /** All data is loaded into one large changeset and divided from there */
  XmlChangeset _changeset;
  /** Mutex protecting large changeset */
  std::mutex _changesetMutex;
  /** Base URL for the target OSM API, including authentication information */
  QUrl _url;
  /** List of pathnames for changeset divided across files */
  QList<QString> _changesets;
  /** Changeset description for all changesets loaded, loaded with 'changeset.description' option */
  QString _description;
  /** Maximum number of writer threads processing changesets, loaded with 'changeset.apidb.max.writers' option */
  int _maxWriters;
  /** Soft maximum number of elements per changeset, size could be larger than the soft max in order to include
   *  the entirety of a way or relation, loaded with 'changeset.apidb.max.size' option
   */
  long _maxChangesetSize;
  /** Queried capabilities from the target OSM API */
  OsmApiCapabilites _capabilities;
  /** Actual statistics */
  QList<SingleStat> _stats;
  /** Progress flag */
  bool _showProgress;
  /** Default constructor for testing purposes only */
  OsmApiWriter() {}
  //  for white box testing
  friend class OsmApiWriterTest;
};

}

#endif  //  OSM_API_WRITER_H

