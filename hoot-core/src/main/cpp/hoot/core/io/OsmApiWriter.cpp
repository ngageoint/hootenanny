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

#include "OsmApiWriter.h"

//  Hootenanny
#include <hoot/core/VersionDefines.h>
#include <hoot/core/io/OsmApiChangeset.h>
#include <hoot/core/io/OsmApiChangesetElement.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

//  Qt
#include <QXmlStreamReader>

using namespace std;

namespace hoot
{

OsmApiWriter::OsmApiWriter(const QUrl &url, const QString &changeset)
  : _description(ConfigOptions().getChangesetDescription()),
    _maxWriters(ConfigOptions().getChangesetApidbMaxWriters()),
    _maxChangesetSize(ConfigOptions().getChangesetApidbMaxSize())
{
  _changesets.push_back(changeset);
  if (isSupported(url))
    _url = url;
}

OsmApiWriter::OsmApiWriter(const QUrl& url, const QList<QString>& changesets)
  : _changesets(changesets),
    _description(ConfigOptions().getChangesetDescription()),
    _maxWriters(ConfigOptions().getChangesetApidbMaxWriters()),
    _maxChangesetSize(ConfigOptions().getChangesetApidbMaxSize())
{
  if (isSupported(url))
    _url = url;
}

bool OsmApiWriter::apply()
{
  OsmApiNetworkRequestPtr request(new OsmApiNetworkRequest());
  //  Validate API capabilites
  if (!queryCapabilities(request))
  {
    LOG_ERROR("API Capabilities error");
    return false;
  }
  //  Validate API permissions
  if (!validatePermissions(request))
  {
    LOG_ERROR("API Permissions error");
    return false;
  }
  bool success = true;
  //  Load all of the changesets into memory
  _changeset.setMaxSize(_maxChangesetSize);
  for (int i = 0; i < _changesets.size(); ++i)
  {
    LOG_INFO("Loading changeset: " << _changesets[i]);
    _changeset.loadChangeset(_changesets[i]);
  }
  //  Start the writer threads
  LOG_INFO("Starting " << _maxWriters << " processing threads.");
  for (int i = 0; i < _maxWriters; ++i)
    _threadPool.push_back(thread(&OsmApiWriter::_changesetThreadFunc, this));
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
  }
  //  Wait for the threads to shutdown
  for (int i = 0; i < _maxWriters; ++i)
    _threadPool[i].join();
  return success;
}

void OsmApiWriter::_changesetThreadFunc()
{
  OsmApiNetworkRequestPtr request(new OsmApiNetworkRequest());
  //
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
      //  Create the changeset for the first changeset
      if (id == -1)
        id = _createChangeset(request, _description);
      //  Display the changeset in TRACE mode
      LOG_TRACE("Thread: " << std::this_thread::get_id() << "\n" << _changeset.getChangesetString(workInfo, id));
      //  Upload the changeset
      if (_uploadChangeset(request, id, _changeset.getChangesetString(workInfo, id)))
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
      }
      else
      {
        //  Log the error
        LOG_ERROR("Error uploading changeset: " << id << "\t" << request->getErrorString());
        //  Split the changeset on conflict errors
        switch (request->getHttpStatus())
        {
        case 400:   //  Placeholder ID is missing or not unique
        case 404:   //  Diff contains elements where the given ID could not be found
        case 409:   //  Conflict, Split the changeset, push both back on the queue
        case 412:   //  Precondition Failed, Relation with id cannot be saved due to other member
          {
            _changesetMutex.lock();
            ChangesetInfoPtr split = _changeset.splitChangeset(workInfo);
            _changesetMutex.unlock();
            if (split->size() > 0)
            {
              _workQueueMutex.lock();
              _workQueue.push(split);
              _workQueue.push(workInfo);
              _workQueueMutex.unlock();
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
  _description = ConfigOptions(conf).getChangesetDescription();
  _maxChangesetSize = ConfigOptions(conf).getChangesetApidbMaxSize();
  _maxWriters = ConfigOptions(conf).getChangesetApidbMaxWriters();
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
bool OsmApiWriter::queryCapabilities(OsmApiNetworkRequestPtr request)
{
  try
  {
    QUrl capabilities = _url;
    capabilities.setPath(API_PATH_CAPABILITIES);
    request->networkRequest(capabilities);
    QString responseXml = QString::fromUtf8(request->getResponseContent().data());
    LOG_DEBUG("Capabilities: " << capabilities.toString());
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
bool OsmApiWriter::validatePermissions(OsmApiNetworkRequestPtr request)
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
long OsmApiWriter::_createChangeset(OsmApiNetworkRequestPtr request, const QString& description)
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
void OsmApiWriter::_closeChangeset(OsmApiNetworkRequestPtr request, long id)
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
bool OsmApiWriter::_uploadChangeset(OsmApiNetworkRequestPtr request, long id, const QString& changeset)
{
  bool success = false;
  try
  {
    QUrl change = _url;
    change.setPath(API_PATH_UPLOAD_CHANGESET.arg(id));

    request->networkRequest(change, QNetworkAccessManager::Operation::PostOperation, changeset.toUtf8());

    QString responseXml = QString::fromUtf8(request->getResponseContent().data());

    switch (request->getHttpStatus())
    {
    case 200:
      success = true;
      break;
    case 400:
      LOG_WARN("Changeset Upload Error: Error parsing XML changeset\n" << responseXml);
      break;
    case 404:
      LOG_WARN("Unknown changeset or elements don't exist");
      break;
    case 409:
      LOG_WARN("Changeset conflict: " << responseXml);
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
  return success;
}

}
