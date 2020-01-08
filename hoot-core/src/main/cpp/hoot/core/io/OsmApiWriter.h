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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_API_WRITER_H
#define OSM_API_WRITER_H

//  hoot
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/io/OsmApiCapabilites.h>
#include <hoot/core/io/OsmApiChangeset.h>
#include <hoot/core/io/OsmApiChangesetElement.h>
#include <hoot/core/info/SingleStat.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ProgressReporter.h>

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

class OsmApiWriter : public Configurable, public ProgressReporter
{
public:
  /** OSM API URL paths */
  const static char* API_PATH_CAPABILITIES;
  const static char* API_PATH_PERMISSIONS;
  const static char* API_PATH_CREATE_CHANGESET;
  const static char* API_PATH_CLOSE_CHANGESET;
  const static char* API_PATH_UPLOAD_CHANGESET;
  const static char* API_PATH_GET_ELEMENT;
  /**
   *  Max number of jobs waiting in the work queue = multiplier * number of threads,
   *  this keeps the producer thread from creating too many sub-changesets too early
   *  that only consist of nodes after ways are blocked.
   */
  const int QUEUE_SIZE_MULTIPLIER = 2;
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

  /**
   * @see ProgressReporter
   */
  virtual void setProgress(Progress progress) { _progress = progress; }
  /**
   * @see ProgressReporter
   */
  virtual unsigned int getNumSteps() const { return 1; }
  /**
   * @brief setErrorPathname Record the pathname of the error changeset
   * @param path Pathname
   */
  void setErrorPathname(const QString& path) { _changeset.setErrorPathname(path); }
  /**
   * @brief writeErrorFile Writes our the error changeset
   * @return true if the file was written successfully
   */
  bool writeErrorFile() { return _changeset.writeErrorFile(); }

private:
  /**
   * @brief The OsmApiFailureInfo struct
   */
  struct OsmApiFailureInfo
  {
    OsmApiFailureInfo() : success(false), status(0), response("") { }
    bool success;
    int status;
    QString response;
  };
  typedef std::shared_ptr<OsmApiFailureInfo> OsmApiFailureInfoPtr;
  /**
   * @brief _createChangeset Request a changeset ID from the API
   *  see: https://wiki.openstreetmap.org/wiki/API_v0.6#Create:_PUT_.2Fapi.2F0.6.2Fchangeset.2Fcreate
   * @param request - Network request object initialized with OSM API URL
   * @param description - Text description of the changeset to create
   * @param source - Text specifying the source for the edits for this changeset
   * @param hashtags - Semicolon delimited list of hashtags for changeset
   * @return ID of the changeset that was created on the server
   */
  long _createChangeset(HootNetworkRequestPtr request, const QString& description,
                        const QString& source, const QString& hashtags);
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
   * @return Failure info status, success, HTTP status and response indicating if the changeset was uploaded correctly
   */
  OsmApiFailureInfoPtr _uploadChangeset(HootNetworkRequestPtr request, long id, const QString& changeset);
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
   * @brief _resolveIssues Query the OSM API for the element in the changeset and try to fix resolvable errors
   * @param request Network request object initialized with OSM API URL
   * @param changeset Pointer to a changeset with one single failed change
   * @return success Whether or not the function was able to find a resolvable issue
   */
  bool _resolveIssues(HootNetworkRequestPtr request, ChangesetInfoPtr changeset);
  /**
   * @brief _fixConflict
   * @param request
   * @param changeset
   * @param conflictExplanation
   * @return
   */
  bool _fixConflict(HootNetworkRequestPtr request, ChangesetInfoPtr changeset, const QString& conflictExplanation);
  /**
   * @brief _changesetClosed
   * @param x
   * @return
   */
  bool _changesetClosed(const QString& conflictExplanation);
  /**
   * @brief _getNode/Way/Relation Perform HTTP GET request to OSM API to get current node/way/relation by ID
   * @param request Network request object initialized with OSM API URL
   * @param id ID of node/way/relation to query from database
   * @return OSM XML string of node/way/relation as it currently sits in the OSM API database
   */
  QString _getNode(HootNetworkRequestPtr request, long id);
  QString _getWay(HootNetworkRequestPtr request, long id);
  QString _getRelation(HootNetworkRequestPtr request, long id);
  /**
   * @brief _getElement Perform HTTP GET request to OSM API to get current element by ID
   * @param request Network request object initialized with OSM API URL
   * @param endpoint Filled out API_PATH_GET_ELEMENT string with node/way/relation and ID
   * @return XML result of the GET request
   */
  QString _getElement(HootNetworkRequestPtr request, const QString& endpoint);
  /**
   * @brief _changesetThreadFunc Thread function that does the actual work of creating a changeset ID
   *  via the API, pushing the changeset data, closing the changeset, and splitting a failing changeset
   *  if necessary
   * @param index Index into the thread status vector to report the status
   */
  void _changesetThreadFunc(int index);
  /**
   * @brief createNetworkRequest Create a network request object
   * @param requiresAuthentication Authentication flag set to true will cause OAuth credentials,
   *    if present, to be passed to the network request object
   * @return smart pointer to network request object
   */
  HootNetworkRequestPtr createNetworkRequest(bool requiresAuthentication = false);
  /**
   * @brief _threadsAreIdle Checks each thread in the thread pool if it is working or idle
   * @return True if all threads are idle
   */
  bool _threadsAreIdle();
  /**
   * @brief _splitChangeset Split a changeset either in half or split out the element reported
   *  back from the API into a new changeset, then both changesets are pushed back on the queue
   * @param workInfo Pointer to the work element to split
   * @param response String response from the server to help in the splitting process
   * @return True if the changeset was split
   */
  bool _splitChangeset(const ChangesetInfoPtr& workInfo, const QString& response);
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
  /** Status of each working thread, working or idle */
  enum ThreadStatus
  {
    Idle,
    Working
  };
  /** Vector of statuses for each running thread */
  std::vector<ThreadStatus> _threadStatus;
  /** Mutex protecting status vector */
  std::mutex _threadStatusMutex;
  /** Base URL for the target OSM API, including authentication information */
  QUrl _url;
  /** List of pathnames for changeset divided across files */
  QList<QString> _changesets;
  /** Changeset description for all changesets loaded, loaded with 'changeset.description' option */
  QString _description;
  /** Changeset source for all changesets loaded, loaded with 'changeset.source' config option */
  QString _source;
  /** Changeset hashtags for all changesets loaded, loaded with 'changeset.hashtags' config option */
  QString _hashtags;
  /** Maximum number of writer threads processing changesets, loaded with 'changeset.apidb.max.writers' option */
  int _maxWriters;
  /** Soft maximum number of elements per changeset, size could be larger than the soft max in order to include
   *  the entirety of a way or relation, loaded with 'changeset.apidb.size.max' option
   */
  long _maxPushSize;
  /** Maximum size of a changeset read from the API itself.  This allows for the potential of multiple
   *  pushes per changeset to increase the speed of the upload.  That way less requests for new changeset
   *  opens/closes are required but the pushes can be an optimal size still.
   */
  long _maxChangesetSize;
  /** Flag to turn on OSM API writer throttling */
  bool _throttleWriters;
  /** Number of seconds for a writer thread to wait after a successful API writer before continuing, if enabled */
  int _throttleTime;
  /** Queried capabilities from the target OSM API */
  OsmApiCapabilites _capabilities;
  /** Actual statistics */
  QList<SingleStat> _stats;
  /** Progress flag */
  bool _showProgress;
  Progress _progress;
  /** OAuth 1.0 consumer key registered with OpenstreetMap */
  QString _consumerKey;
  /** OAuth 1.0 consumer secred registered with OpenstreetMap */
  QString _consumerSecret;
  /** OAuth 1.0 access token granted through OAuth authorization */
  QString _accessToken;
  /** OAuth 1.0 secret token granted through OAuth authorization */
  QString _secretToken;
  /** Full pathname of the error file changeset, if any errors occur */
  QString _errorPathname;
  /** Number of changesets written to API */
  int _changesetCount;
  /** Mutex for changeset count */
  std::mutex _changesetCountMutex;
  /** For white box testing */
  friend class OsmApiWriterTest;
  /** Default constructor for testing purposes only */
  OsmApiWriter() {}
};

}

#endif  //  OSM_API_WRITER_H

