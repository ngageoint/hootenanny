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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmApiWriter.h"

//  Hootenanny
#include <hoot/core/info/VersionDefines.h>
#include <hoot/core/io/OsmApiChangeset.h>
#include <hoot/core/io/OsmApiChangesetElement.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

//  Tgs
#include <tgs/System/Timer.h>

//  Qt
#include <QXmlStreamReader>

using namespace std;
using namespace Tgs;

namespace hoot
{

OsmApiWriter::OsmApiWriter(const QUrl &url, const QString &changeset)
  : _description(ConfigOptions().getChangesetDescription()),
    _maxWriters(ConfigOptions().getChangesetApidbWritersMax()),
    _maxChangesetSize(ConfigOptions().getChangesetApidbSizeMax()),
    _throttleWriters(ConfigOptions().getChangesetApidbWritersThrottle()),
    _throttleTime(ConfigOptions().getChangesetApidbWritersThrottleTime()),
    _showProgress(false)
{
  _changesets.push_back(changeset);
  if (isSupported(url))
    _url = url;
}

OsmApiWriter::OsmApiWriter(const QUrl& url, const QList<QString>& changesets)
  : _changesets(changesets),
    _description(ConfigOptions().getChangesetDescription()),
    _maxWriters(ConfigOptions().getChangesetApidbWritersMax()),
    _maxChangesetSize(ConfigOptions().getChangesetApidbSizeMax()),
    _throttleWriters(ConfigOptions().getChangesetApidbWritersThrottle()),
    _throttleTime(ConfigOptions().getChangesetApidbWritersThrottleTime()),
    _showProgress(false),
    _consumerKey(ConfigOptions().getHootOsmAuthConsumerKey()),
    _consumerSecret(ConfigOptions().getHootOsmAuthConsumerSecret()),
    _accessToken(ConfigOptions().getHootOsmAuthAccessToken()),
    _secretToken(ConfigOptions().getHootOsmAuthAccessTokenSecret())
{
  if (isSupported(url))
    _url = url;
}

bool OsmApiWriter::apply()
{
  Timer timer;
  //  Setup the network request object without OAuth for the capabilities call
  HootNetworkRequestPtr request = createNetworkRequest();
  //  Validate API capabilites
  if (!queryCapabilities(request))
  {
    LOG_ERROR("API Capabilities error");
    return false;
  }
  _stats.append(SingleStat("API Capabilites Query Time (sec)", timer.getElapsedAndRestart()));
  //  Setup the network request object with OAuth or with username/password authentication
  request = createNetworkRequest(true);
  //  Validate API permissions
  if (!validatePermissions(request))
  {
    LOG_ERROR("API Permissions error");
    return false;
  }
  _stats.append(SingleStat("API Permissions Query Time (sec)", timer.getElapsedAndRestart()));
  bool success = true;
  //  Load all of the changesets into memory
  _changeset.setMaxSize(_maxChangesetSize);
  for (int i = 0; i < _changesets.size(); ++i)
  {
    LOG_INFO("Loading changeset: " << _changesets[i]);
    _changeset.loadChangeset(_changesets[i]);
    _stats.append(SingleStat(QString("Changeset (%1) Load Time (sec)").arg(_changesets[i]), timer.getElapsedAndRestart()));
  }
  //  Split any ways that need splitting
  _changeset.splitLongWays(_capabilities.getWayNodes());
  //  Fix any changeset input that isn't formatted correctly
  _changeset.fixMalformedInput();
  //  Start the writer threads
  LOG_INFO("Starting " << _maxWriters << " processing threads.");
  for (int i = 0; i < _maxWriters; ++i)
    _threadPool.push_back(thread(&OsmApiWriter::_changesetThreadFunc, this));
  //  Setup the progress indicators
  long total = _changeset.getTotalElementCount();
  float progress = 0.0f;
  float increment = 0.01f;
  //  Setup the increment
  if (total < 100000)
    increment = 0.1f;
  else if (total < 1000000)
    increment = 0.05f;
  //  Iterate all changes until there are no more elements to send
  while (_changeset.hasElementsToSend())
  {
    //  Check the size of the work queue, don't overwork the work queue
    _workQueueMutex.lock();
    int queueSize = (int)_workQueue.size();
    _workQueueMutex.unlock();
    //  Only queue up enough work to keep all the threads busy with times QUEUE_SIZE_MULTIPLIER
    //  so that results can come back and update the changeset for more atomic changesets instead
    //  of a big list of nodes, then ways, then relations.  This will give us fuller more atomic
    //  changesets towards the end of the job
    if (queueSize < QUEUE_SIZE_MULTIPLIER * _maxWriters)
    {
      //  Divide up the changes into atomic changesets
      ChangesetInfoPtr changeset_info(new ChangesetInfo());
      //  Repeat divide until all changes have been committed
      _changesetMutex.lock();
      bool newChangeset = _changeset.calculateChangeset(changeset_info);
      _changesetMutex.unlock();
      //  Add the new work to the queue if there is any
      if (newChangeset)
      {
        _workQueueMutex.lock();
        _workQueue.push(changeset_info);
        _workQueueMutex.unlock();
      }
      else
      {
        //  Allow time for the worker threads to complete some work
        this_thread::sleep_for(chrono::milliseconds(10));
      }
    }
    else
    {
      //  Allow time for the worker threads to complete some work
      this_thread::sleep_for(chrono::milliseconds(10));
    }
    //  Show the progress
    if (_showProgress)
    {
      float percent_complete = _changeset.getProcessedCount() / (float)total;
      //  Actual progress is calculated and once it passes the next increment it is reported
      if (percent_complete >= progress + increment)
      {
        progress = percent_complete - fmod(percent_complete, increment);
        _progress.set(percent_complete, "Uploading changeset...");
      }
    }
  }
  //  Wait for the threads to shutdown
  for (int i = 0; i < _maxWriters; ++i)
    _threadPool[i].join();
  LOG_INFO("Upload progress: 100%");
  //  Keep some stats
  _stats.append(SingleStat("API Upload Time (sec)", timer.getElapsedAndRestart()));
  _stats.append(SingleStat("Total Nodes in Changeset", _changeset.getTotalNodeCount()));
  _stats.append(SingleStat("Total Ways in Changeset", _changeset.getTotalWayCount()));
  _stats.append(SingleStat("Total Relations in Changeset", _changeset.getTotalRelationCount()));
  _stats.append(SingleStat("Total Elements Created", _changeset.getTotalCreateCount()));
  _stats.append(SingleStat("Total Elements Modified", _changeset.getTotalModifyCount()));
  _stats.append(SingleStat("Total Elements Deleted", _changeset.getTotalDeleteCount()));
  _stats.append(SingleStat("Total Errors", _changeset.getFailedCount()));
  //  Return successfully
  return success;
}

void OsmApiWriter::_changesetThreadFunc()
{
  //  Setup the network request object with OAuth or with username/password authentication
  HootNetworkRequestPtr request = createNetworkRequest(true);
  long id = -1;
  //  Iterate until all elements are sent and updated
  while (!_changeset.isDone())
  {
    ChangesetInfoPtr workInfo;
    //  Try to get something off of the work queue
    _workQueueMutex.lock();
    if (!_workQueue.empty())
    {
      workInfo = _workQueue.front();
      _workQueue.pop();
    }
    _workQueueMutex.unlock();

    if (workInfo)
    {
      //  Create the changeset ID if required
      if (id < 1)
        id = _createChangeset(request, _description);
      //  An ID of less than 1 isn't valid, try to fix it
      if (id < 1)
      {
        _workQueueMutex.lock();
        _workQueue.push(workInfo);
        _workQueueMutex.unlock();
        //  Reset the network request object and sleep it off
        request = createNetworkRequest(true);
        LOG_WARN("Bad changeset ID. Resetting network request object.");
        this_thread::sleep_for(chrono::milliseconds(100));
        //  Try a new create changeset request
        continue;
      }
      //  Display the changeset in TRACE mode
      LOG_TRACE("Thread: " << std::this_thread::get_id() << "\n" << _changeset.getChangesetString(workInfo, id));
      //  Upload the changeset
      OsmApiFailureInfoPtr info = _uploadChangeset(request, id, _changeset.getChangesetString(workInfo, id));
      if (info->success)
      {
        //  Display the upload response in TRACE mode
        LOG_TRACE("Thread: " << std::this_thread::get_id() << "\n" << QString(request->getResponseContent()));
        //  Update the changeset with the response
        _changesetMutex.lock();
        _changeset.updateChangeset(QString(request->getResponseContent()));
        _changesetMutex.unlock();
        //  Close the changeset
        _closeChangeset(request, id);
        //  Signal for a new changeset id
        id = -1;
        //  Throttle the input rate if desired
        if (_throttleWriters && !_changeset.isDone())
          this_thread::sleep_for(chrono::seconds(_throttleTime));
      }
      else
      {
        //  Log the error
        LOG_ERROR("Error uploading changeset: " << id << "\t" << request->getErrorString());
        //  Split the changeset on conflict errors
        switch (info->status)
        {
        case 400:   //  Placeholder ID is missing or not unique
        case 404:   //  Diff contains elements where the given ID could not be found
        case 409:   //  Conflict, Split the changeset, push both back on the queue
        case 412:   //  Precondition Failed, Relation with id cannot be saved due to other member
          {
            _changesetMutex.lock();
            ChangesetInfoPtr split = _changeset.splitChangeset(workInfo, info->response);
            _changesetMutex.unlock();
            if (split->size() > 0)
            {
              _workQueueMutex.lock();
              _workQueue.push(split);
              _workQueue.push(workInfo);
              _workQueueMutex.unlock();
            }
            else
            {
              if (!workInfo->getChangesetIssuesResolved())
              {
                //  Set the issues resolved flag
                workInfo->setChangesetIssuesResolved(true);
                //  Try to automatically resolve certain issues, like out of date version
                if (_resolveIssues(request, workInfo))
                {
                  _workQueueMutex.lock();
                  _workQueue.push(workInfo);
                  _workQueueMutex.unlock();
                }
                else
                {
                  //  Set the element in the changeset to failed because the issues couldn't be resolved
                  _changeset.updateFailedChangeset(workInfo);
                }
              }
            }
          }
          break;
        case 405:   //  This shouldn't ever happen, push back on the queue
          _workQueueMutex.lock();
          _workQueue.push(workInfo);
          _workQueueMutex.unlock();
          break;
        default:
          //  This is a big problem, report it and try again
          LOG_ERROR("Changeset upload responded with HTTP status response: " << request->getHttpStatus());
          _workQueueMutex.lock();
          _workQueue.push(workInfo);
          _workQueueMutex.unlock();
          break;
        }
      }
    }
    else
      this_thread::sleep_for(chrono::milliseconds(10));
  }
  //  Close the changeset if one is still open
  if (id != -1)
    _closeChangeset(request, id);
}

void OsmApiWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _description = options.getChangesetDescription();
  _maxChangesetSize = options.getChangesetApidbSizeMax();
  _maxWriters = options.getChangesetApidbWritersMax();
  _throttleWriters = options.getChangesetApidbWritersThrottle();
  _throttleTime = options.getChangesetApidbWritersThrottleTime();
  _consumerKey = options.getHootOsmAuthConsumerKey();
  _consumerSecret = options.getHootOsmAuthConsumerSecret();
  _accessToken = options.getHootOsmAuthAccessToken();
  _secretToken = options.getHootOsmAuthAccessTokenSecret();
}

bool OsmApiWriter::isSupported(const QUrl &url)
{
  if (url.isEmpty() ||
      url.isLocalFile() ||
      url.isRelative() ||
      !url.isValid() ||
      (!url.path().isEmpty() && url.path() != "/") ||
      (url.scheme().toLower() != "http" && url.scheme().toLower() != "https"))
  {
    LOG_WARN("Invalid URL for OSM API endpoint.");
    return false;
  }
  else
    return true;
}

//  https://wiki.openstreetmap.org/wiki/API_v0.6#Capabilities:_GET_.2Fapi.2Fcapabilities
bool OsmApiWriter::queryCapabilities(HootNetworkRequestPtr request)
{
  try
  {
    QUrl capabilities = _url;
    capabilities.setPath(API_PATH_CAPABILITIES);
    request->networkRequest(capabilities);
    QString responseXml = QString::fromUtf8(request->getResponseContent().data());
    LOG_DEBUG("Capabilities: " << capabilities.toString(QUrl::RemoveUserInfo));
    LOG_DEBUG("Response: " << responseXml);
    _capabilities = _parseCapabilities(responseXml);
  }
  catch (const HootException& ex)
  {
    LOG_WARN(ex.what());
    return false;
  }
  return true;
}

//  https://wiki.openstreetmap.org/wiki/API_v0.6#Retrieving_permissions:_GET_.2Fapi.2F0.6.2Fpermissions
bool OsmApiWriter::validatePermissions(HootNetworkRequestPtr request)
{
  bool success = false;
  try
  {
    QUrl permissions = _url;
    permissions.setPath(API_PATH_PERMISSIONS);
    request->networkRequest(permissions);
    QString responseXml = QString::fromUtf8(request->getResponseContent().data());
    success = _parsePermissions(responseXml);
  }
  catch (const HootException& ex)
  {
    LOG_WARN(ex.what());
  }
  return success;
}

OsmApiCapabilites OsmApiWriter::_parseCapabilities(const QString& capabilites)
{
  OsmApiCapabilites caps;
  QXmlStreamReader reader(capabilites);

  while (!reader.atEnd() && !reader.hasError())
  {
    QXmlStreamReader::TokenType type = reader.readNext();
    if (type == QXmlStreamReader::StartDocument)
      continue;
    if (type == QXmlStreamReader::StartElement)
    {
      QStringRef name = reader.name();
      QXmlStreamAttributes attributes = reader.attributes();
      if (name == "version" && attributes.hasAttribute("maximum"))
        caps.setVersion(attributes.value("maximum").toString());
      else if (name == "tracepoints" && attributes.hasAttribute("per_page"))
        caps.setTracepoints(attributes.value("per_page").toString().toLong());
      else if (name == "waynodes" && attributes.hasAttribute("maximum"))
        caps.setWayNodes(attributes.value("maximum").toString().toLong());
      else if (name == "changesets" && attributes.hasAttribute("maximum_elements"))
        caps.setChangesets(attributes.value("maximum_elements").toString().toLong());
      else if (name == "timeout" && attributes.hasAttribute("seconds"))
        caps.setTimeout(attributes.value("seconds").toString().toLong());
      else if (name == "status")
      {
        if (attributes.hasAttribute("database"))
          caps.setDatabaseStatus(_parseStatus(attributes.value("database").toString()));
        if (attributes.hasAttribute("api"))
          caps.setApiStatus(_parseStatus(attributes.value("api").toString()));
        if (attributes.hasAttribute("gpx"))
          caps.setGpxStatus(_parseStatus(attributes.value("gpx").toString()));
      }
    }
  }
  return caps;
}

OsmApiStatus OsmApiWriter::_parseStatus(const QString& status)
{
  if (status == "online")
    return OsmApiStatus::ONLINE;
  else if (status == "readonly")
    return OsmApiStatus::READONLY;
  else
    return OsmApiStatus::OFFLINE;
}

bool OsmApiWriter::_parsePermissions(const QString& permissions)
{
  QXmlStreamReader reader(permissions);

  LOG_DEBUG("Permissions: " << permissions);

  while (!reader.atEnd() && !reader.hasError())
  {
    QXmlStreamReader::TokenType type = reader.readNext();
    if (type == QXmlStreamReader::StartDocument)
      continue;
    if (type == QXmlStreamReader::StartElement)
    {
      QStringRef name = reader.name();
      QXmlStreamAttributes attributes = reader.attributes();
      if (name == "permission" && attributes.hasAttribute("name"))
      {
        if (attributes.value("name") == "allow_write_api")
          return true;
      }
    }
  }
  return false;
}

//  https://wiki.openstreetmap.org/wiki/API_v0.6#Create:_PUT_.2Fapi.2F0.6.2Fchangeset.2Fcreate
long OsmApiWriter::_createChangeset(HootNetworkRequestPtr request, const QString& description)
{
  try
  {
    QUrl changeset = _url;
    changeset.setPath(API_PATH_CREATE_CHANGESET);
    QString xml = QString(
      "<osm>"
      "  <changeset>"
      "    <tag k='created_by' v='%1'/>"
      "    <tag k='comment' v='%2'/>"
      "  </changeset>"
      "</osm>").arg(HOOT_NAME).arg(description);

    request->networkRequest(changeset, QNetworkAccessManager::Operation::PutOperation, xml.toUtf8());

    QString responseXml = QString::fromUtf8(request->getResponseContent().data());

    return responseXml.toLong();
  }
  catch (const HootException& ex)
  {
    LOG_WARN(ex.what());
  }
  return -1;
}

//  https://wiki.openstreetmap.org/wiki/API_v0.6#Close:_PUT_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fclose
void OsmApiWriter::_closeChangeset(HootNetworkRequestPtr request, long id)
{
  try
  {
    QUrl changeset = _url;
    changeset.setPath(API_PATH_CLOSE_CHANGESET.arg(id));
    request->networkRequest(changeset, QNetworkAccessManager::Operation::PutOperation);
    QString responseXml = QString::fromUtf8(request->getResponseContent().data());
    switch (request->getHttpStatus())
    {
    case 404:
      LOG_WARN("Unknown changeset");
      break;
    case 409:
      LOG_WARN("Changeset conflict: " << responseXml);
      break;
    case 200:
      //  Changeset closed successfully
      break;
    default:
      LOG_WARN("Uknown HTTP response code: " << request->getHttpStatus());
      break;
    }
  }
  catch (const HootException& ex)
  {
    LOG_WARN(ex.what());
  }
}

//  https://wiki.openstreetmap.org/wiki/API_v0.6#Diff_upload:_POST_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fupload
/**
 * POSSIBLE HTTP error codes and explanations
 * HTTP status code 400 (Bad Request) - text/plain
 *  When there are errors parsing the XML. A text message explaining the error is returned.
 *   This can also happen if you forget to pass the Content-Length header.
 *  When a changeset ID is missing (unfortunately the error messages are not consistent)
 *  When a node is outside the world
 *  When there are too many nodes for a way
 *  When the version of the provided element does not match the current database version of the element
 * HTTP status code 409 (Conflict) - text/plain
 *  If the changeset in question has already been closed (either by the user itself or as a result of the auto-closing feature).
 *   A message with the format "The changeset #id was closed at #closed_at." is returned
 *  Or if the user trying to update the changeset is not the same as the one that created it
 * HTTP status code 404 (Not Found)
 *  When no element with the given id could be found
 * HTTP status code 412 (Precondition Failed)
 *  When a way has nodes that do not exist or are not visible (i.e. deleted):
 *   "Way #{id} requires the nodes with id in (#{missing_ids}), which either do not exist, or are not visible."
 *  When a relation has elements that do not exist or are not visible:
 *   "Relation with id #{id} cannot be saved due to #{element} with id #{element.id}"
 */
OsmApiWriter::OsmApiFailureInfoPtr OsmApiWriter::_uploadChangeset(HootNetworkRequestPtr request, long id, const QString& changeset)
{
  OsmApiFailureInfoPtr info(new OsmApiFailureInfo());
  //  Don't even attempt if the ID is bad
  if (id < 1)
    return info;
  try
  {
    QUrl change = _url;
    change.setPath(API_PATH_UPLOAD_CHANGESET.arg(id));

    QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
    headers[QNetworkRequest::ContentTypeHeader] = "text/xml";

    request->networkRequest(change, headers, QNetworkAccessManager::Operation::PostOperation, changeset.toUtf8());

    info->response = QString::fromUtf8(request->getResponseContent().data());
    info->status = request->getHttpStatus();

    switch (info->status)
    {
    case 200:
      info->success = true;
      break;
    case 400:
      LOG_WARN("Changeset Upload Error: Error parsing XML changeset\n" << info->response);
      break;
    case 404:
      LOG_WARN("Unknown changeset or elements don't exist");
      break;
    case 409:
      LOG_WARN("Changeset conflict: " << info->response);
      break;
    case 412:
      LOG_WARN("Changeset precondition failed: " << info->response);
      break;
    default:
      LOG_WARN("Uknown HTTP response code: " << info->status);
      break;
    }
  }
  catch (const HootException& ex)
  {
    LOG_WARN(ex.what());
  }
  return info;
}

bool OsmApiWriter::_resolveIssues(HootNetworkRequestPtr request, ChangesetInfoPtr changeset)
{
  bool success = false;
  //  Can only work on changesets of size 1
  if (changeset->size() != 1)
    return false;
  for (int elementType = ElementType::Node; elementType < ElementType::Unknown; ++elementType)
  {
    //  Creates cannot be fixed with this method, skip them here
    for (int changesetType = XmlChangeset::TypeModify; changesetType < XmlChangeset::TypeMax; ++changesetType)
    {
      ChangesetInfo::iterator element = changeset->begin((ElementType::Type)elementType, (XmlChangeset::ChangesetType)changesetType);
      if (element != changeset->end((ElementType::Type)elementType, (XmlChangeset::ChangesetType)changesetType))
      {
        long id = *element;
        QString update = "";
        //  Get the element from the OSM API
        if (elementType == ElementType::Node)
          update = _getNode(request, id);
        else if (elementType == ElementType::Way)
          update = _getWay(request, id);
        else if (elementType == ElementType::Relation)
          update = _getRelation(request, id);
        //  Fix the changeset with the node/way/relation from the OSM API
        success |= _changeset.fixChangeset(update);
      }
    }
  }
  return success;
}

QString OsmApiWriter::_getNode(HootNetworkRequestPtr request, long id)
{
  //  Check for a valid ID to query against
  if (id < 1)
    return "";
  //  Get the node by ID
  return _getElement(request, QString(API_PATH_GET_ELEMENT).arg("node").arg(id));
}

QString OsmApiWriter::_getWay(HootNetworkRequestPtr request, long id)
{
  //  Check for a valid ID to query against
  if (id < 1)
    return "";
  //  Get the way by ID
  return _getElement(request, QString(API_PATH_GET_ELEMENT).arg("way").arg(id));
}

QString OsmApiWriter::_getRelation(HootNetworkRequestPtr request, long id)
{
  //  Check for a valid ID to query against
  if (id < 1)
    return "";
  //  Get the relation by ID
  return _getElement(request, QString(API_PATH_GET_ELEMENT).arg("relation").arg(id));
}

QString OsmApiWriter::_getElement(HootNetworkRequestPtr request, const QString& endpoint)
{
  //  Don't follow an uninitialized URL or empty endpoint
  if (endpoint == API_PATH_GET_ELEMENT || endpoint == "")
    return "";
  try
  {
    QUrl get = _url;
    get.setPath(endpoint);
    request->networkRequest(get);
    if (request->getHttpStatus() == 200)
      return QString::fromUtf8(request->getResponseContent().data());
    else
      LOG_WARN("GET error: " << QString::fromUtf8(request->getResponseContent().data()));
  }
  catch (const HootException& ex)
  {
    LOG_WARN(ex.what());
  }
  return "";
}

HootNetworkRequestPtr OsmApiWriter::createNetworkRequest(bool requiresAuthentication)
{
  HootNetworkRequestPtr request;
  if (!requiresAuthentication)
  {
    //  When the call doesn't require authentication, don't pass in OAuth credentials
    request.reset(new HootNetworkRequest());
  }
  else if (!_consumerKey.isEmpty() &&
      !_consumerSecret.isEmpty() &&
      !_accessToken.isEmpty() &&
      !_secretToken.isEmpty())
  {
    //  When OAuth credentials are present and authentication is requested, pass OAuth crendentials
    request.reset(new HootNetworkRequest(_consumerKey, _consumerSecret, _accessToken, _secretToken));
  }
  else
  {
    //  No OAuth credentials are present, so authentication must be by username/password
    request.reset(new HootNetworkRequest());
  }
  return request;
}

}
