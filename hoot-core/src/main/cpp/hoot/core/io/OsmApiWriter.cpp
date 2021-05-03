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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "OsmApiWriter.h"

//  Hootenanny
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/info/VersionDefines.h>
#include <hoot/core/io/OsmApiChangeset.h>
#include <hoot/core/io/OsmApiChangesetElement.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/HootNetworkUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/OsmApiUtils.h>

//  Tgs
#include <tgs/Statistics/Random.h>
#include <tgs/System/Timer.h>

//  Qt
#include <QUrlQuery>
#include <QXmlStreamReader>

using namespace std;
using namespace Tgs;

namespace hoot
{

OsmApiWriter::OsmApiWriter(const QUrl &url, const QString &changeset)
  : _startFlag(false),
    _description(ConfigOptions().getChangesetDescription()),
    _source(ConfigOptions().getChangesetSource()),
    _hashtags(ConfigOptions().getChangesetHashtags()),
    _maxWriters(ConfigOptions().getChangesetApidbWritersMax()),
    _maxPushSize(ConfigOptions().getChangesetApidbSizeMax()),
    _maxChangesetSize(ConfigOptions().getChangesetMaxSize()),
    _throttleWriters(ConfigOptions().getChangesetApidbWritersThrottle()),
    _throttleTime(ConfigOptions().getChangesetApidbWritersThrottleTime()),
    _throttlePlusMinus(ConfigOptions().getChangesetApidbWritersThrottleTimespan()),
    _showProgress(false),
    _consumerKey(ConfigOptions().getHootOsmAuthConsumerKey()),
    _consumerSecret(ConfigOptions().getHootOsmAuthConsumerSecret()),
    _accessToken(ConfigOptions().getHootOsmAuthAccessToken()),
    _secretToken(ConfigOptions().getHootOsmAuthAccessTokenSecret()),
    _changesetCount(0),
    _debugOutput(ConfigOptions().getChangesetApidbWriterDebugOutput()),
    _debugOutputPath(ConfigOptions().getChangesetApidbWriterDebugOutputPath()),
    _apiId(0),
    _threadsCanExit(false),
    _throttleCgiMap(ConfigOptions().getChangesetApidbWritersThrottleCgimap()),
    _timeout(ConfigOptions().getChangesetApidbTimeout())
{
  _changesets.push_back(changeset);
  if (isSupported(url))
    _url = url;
}

OsmApiWriter::OsmApiWriter(const QUrl& url, const QList<QString>& changesets)
  : _startFlag(false),
    _changesets(changesets),
    _description(ConfigOptions().getChangesetDescription()),
    _source(ConfigOptions().getChangesetSource()),
    _hashtags(ConfigOptions().getChangesetHashtags()),
    _maxWriters(ConfigOptions().getChangesetApidbWritersMax()),
    _maxPushSize(ConfigOptions().getChangesetApidbSizeMax()),
    _maxChangesetSize(ConfigOptions().getChangesetMaxSize()),
    _throttleWriters(ConfigOptions().getChangesetApidbWritersThrottle()),
    _throttleTime(ConfigOptions().getChangesetApidbWritersThrottleTime()),
    _throttlePlusMinus(ConfigOptions().getChangesetApidbWritersThrottleTimespan()),
    _showProgress(false),
    _consumerKey(ConfigOptions().getHootOsmAuthConsumerKey()),
    _consumerSecret(ConfigOptions().getHootOsmAuthConsumerSecret()),
    _accessToken(ConfigOptions().getHootOsmAuthAccessToken()),
    _secretToken(ConfigOptions().getHootOsmAuthAccessTokenSecret()),
    _changesetCount(0),
    _debugOutput(ConfigOptions().getChangesetApidbWriterDebugOutput()),
    _debugOutputPath(ConfigOptions().getChangesetApidbWriterDebugOutputPath()),
    _apiId(0),
    _threadsCanExit(false),
    _throttleCgiMap(ConfigOptions().getChangesetApidbWritersThrottleCgimap()),
    _timeout(ConfigOptions().getChangesetApidbTimeout())
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
  //  Limit the max changeset size to the max from the API, testing found that smaller (1k to 2k elements)
  //  pushed across multiple processing threads out performed larger (50k element) datasets
  if (_maxChangesetSize > _capabilities.getChangesets())
    _maxChangesetSize = _capabilities.getChangesets();
  if (_maxPushSize > _maxChangesetSize)
    _maxPushSize = _maxChangesetSize / 5;
  //  Setup the network request object with OAuth or with username/password authentication
  request = createNetworkRequest(true);
  //  Validate API permissions
  if (!validatePermissions(request))
  {
    LOG_ERROR("API Permissions error");
    return false;
  }
  _stats.append(SingleStat("API Permissions Query Time (sec)", timer.getElapsedAndRestart()));
  //  Throttle uploads when using CGImap if requested
  if (_throttleCgiMap && usingCgiMap(request))
    _throttleWriters = true;
  bool success = true;
  //  Load all of the changesets into memory
  _changeset.setMaxPushSize(_maxPushSize);
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
  _threadIdle.reserve(_maxWriters);
  for (int i = 0; i < _maxWriters; ++i)
  {
    _threadStatus.push_back(ThreadStatus::Working);
    _threadPool.push_back(thread(&OsmApiWriter::_changesetThreadFunc, this, i));
  }
  //  Setup the progress indicators
  long total = _changeset.getTotalElementCount();
  float progress = 0.0f;
  float increment = 0.01f;
  //  Setup the increment
  if (total < 10000)
    increment = 0.1f;
  else if (total < 100000)
    increment = 0.05f;
  //  Iterate all changes until there are no more elements to send
  while (_changeset.hasElementsToSend())
  {
    //  Check the size of the work queue, don't overwork the work queue
    _workQueueMutex.lock();
    int queueSize = (int)_workQueue.size();
    _workQueueMutex.unlock();
    //  If all threads have failed, fail the rest of the changeset and exit
    if (_allThreadsFailed())
    {
      _changeset.failRemainingChangeset();
      _threadsCanExit = true;
      break;
    }
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
        //  Push the new changeset onto the work queue
        _pushChangesets(changeset_info);
      }
      else if (queueSize == 0 && !newChangeset && _changeset.hasElementsToSend() && _threadsAreIdle())
      {
        //  This is an error case, the queue is empty, there are still elements to send but
        //  all of the threads are idle and not waiting for something to come back
        //  There are two things that can be done here, first is to put everything that is
        //  "ready to send" in a changeset and send it OR move everything to the error state
/*
        //  Option #1: Get all of the remaining elements as a single changeset
        _changesetMutex.lock();
        _changeset.calculateRemainingChangeset(changeset_info);
        _changesetMutex.unlock();
        //  Push that changeset
        _pushChangesets(changeset_info);
        //  Let the threads know that the remaining changeset is the "remaining" changeset
        _threadsCanExit = true;
*/
        LOG_STATUS("Apply Changeset: Remaining elements unsendable...");
        //  Option #2: Move everything to the error state and exit
        _changesetMutex.lock();
        _changeset.failRemainingChangeset();
        _changesetMutex.unlock();
        //  Let the threads know that the remaining changeset has failed
        _threadsCanExit = true;
        break;
      }
      else
      {
        //  Indicate to the worker threads that there is work to be done
        _startWork();
        //  Allow time for the worker threads to complete some work
        _yield();
      }
    }
    else
    {
      //  Indicate to the worker threads that there is work to be done
      _startWork();
      //  Allow time for the worker threads to complete some work
      _yield();
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
  //  Indicate to the worker threads that there is work to be done, if they haven't started already
  _startWork();
  //  Wait for the threads to shutdown
  for (int i = 0; i < _maxWriters; ++i)
    _threadPool[i].join();
  //  Check for failed threads
  if (_hasFailedThread())
  {
    LOG_ERROR(_errorMessage);
    _changeset.failRemainingChangeset();
  }
  //  Final write for the error file
  _changeset.writeErrorFile();
  //  Keep some stats
  _stats.append(SingleStat("API Upload Time (sec)", timer.getElapsedAndRestart()));
  _stats.append(SingleStat("Total OSM Changesets Uploaded", _changesetCount));
  _stats.append(SingleStat("Total Nodes in Changeset", _changeset.getTotalNodeCount()));
  _stats.append(SingleStat("Total Ways in Changeset", _changeset.getTotalWayCount()));
  _stats.append(SingleStat("Total Relations in Changeset", _changeset.getTotalRelationCount()));
  _stats.append(SingleStat(" Total Elements Created", _changeset.getTotalCreateCount()));
  _stats.append(SingleStat("          Nodes Created", _changeset.getNodeCountByType(ChangesetType::TypeCreate)));
  _stats.append(SingleStat("           Ways Created", _changeset.getWayCountByType(ChangesetType::TypeCreate)));
  _stats.append(SingleStat("      Relations Created", _changeset.getRelationCountByType(ChangesetType::TypeCreate)));
  _stats.append(SingleStat("Total Elements Modified", _changeset.getTotalModifyCount()));
  _stats.append(SingleStat("         Nodes Modified", _changeset.getNodeCountByType(ChangesetType::TypeModify)));
  _stats.append(SingleStat("          Ways Modified", _changeset.getWayCountByType(ChangesetType::TypeModify)));
  _stats.append(SingleStat("     Relations Modified", _changeset.getRelationCountByType(ChangesetType::TypeModify)));
  _stats.append(SingleStat(" Total Elements Deleted", _changeset.getTotalDeleteCount()));
  _stats.append(SingleStat("          Nodes Deleted", _changeset.getNodeCountByType(ChangesetType::TypeDelete)));
  _stats.append(SingleStat("           Ways Deleted", _changeset.getWayCountByType(ChangesetType::TypeDelete)));
  _stats.append(SingleStat("      Relations Deleted", _changeset.getRelationCountByType(ChangesetType::TypeDelete)));
  _stats.append(SingleStat("Total Errors", _changeset.getFailedCount()));
  //  Return successfully
  return success;
}

void OsmApiWriter::_changesetThreadFunc(int index)
{
  //  Set the status to working
  _updateThreadStatus(index, ThreadStatus::Working);
  //  Setup the network request object with OAuth or with username/password authentication
  HootNetworkRequestPtr request = createNetworkRequest(true);
  long id = -1;
  long changesetSize = 0;
  LastElementInfo last;
  bool stop_thread = false;
  int changeset_failures = 0;
  //  Before working, wait for the signal
  _waitForStart();
  //  Iterate until all elements are sent and updated
  while (!_changeset.isDone() && !stop_thread)
  {
    ChangesetInfoPtr workInfo;
    //  Try to get something off of the work queue
    _workQueueMutex.lock();
    int queueSize = _workQueue.size();
    if (!_workQueue.empty())
    {
      workInfo = _workQueue.front();
      _workQueue.pop();
    }
    _workQueueMutex.unlock();

    if (workInfo)
    {
      //  Set the status to working
      _updateThreadStatus(index, ThreadStatus::Working);
      int create_changeset_status = HttpResponseCode::HTTP_OK;
      //  Create the changeset ID if required
      if (id < 1)
      {
        id = _createChangeset(request, _description, _source, _hashtags, create_changeset_status);
        changesetSize = 0;
      }
      //  An ID of less than 1 isn't valid, try to fix it
      if (id < 1)
      {
        _pushChangesets(workInfo);
        //  Multiple changeset failures
        changeset_failures++;
        if (changeset_failures >= 3)
        {
          //  Set the thread status to failed and report the error message in the main thread
          _updateThreadStatus(index, ThreadStatus::Failed);
          stop_thread = true;
          //  Set the error message
          _errorMessage = "Multiple bad changeset ID errors in a row, is the API functioning correctly?";
        }
        else
        {
          //  Reset the network request object and sleep it off
          request = createNetworkRequest(true);
          LOG_DEBUG("Bad changeset ID. Resetting network request object.");
          //  Sleep between 30 and 60 seconds to allow the database server to recover on service unavailable
          if (create_changeset_status == HttpResponseCode::HTTP_SERVICE_UNAVAILABLE)
            _yield(30 * 1000, 60 * 1000);
        }
        //  Try a new create changeset request
        continue;
      }
      else
      {
        //  Reset the changeset failure count when one is successful
        changeset_failures = 0;
      }
      //  Make sure that the changeset is valid and isn't empty
      if (workInfo->size() < 1)
      {
        LOG_DEBUG("Empty changeset created.");
        //  Jump back to the beginning and release the empty workInfo object
        continue;
      }
      QString changeset = _changeset.getChangesetString(workInfo, id);
      //  Display the changeset in TRACE mode
      LOG_TRACE("Thread: " << std::this_thread::get_id() << "\n" << changeset);
      //  Output the debug request if requested
      int apiId = _getNextApiId();
      if (_debugOutput)
        _writeDebugFile("Request-", changeset, apiId, id);
      //  Upload the changeset
      OsmApiFailureInfoPtr info = _uploadChangeset(request, id, changeset);
      //  Output the debug response if requested
      if (_debugOutput)
        _writeDebugFile("Response", info->response, apiId, id, info->status);
      if (info->success)
      {
        //  Display the upload response in TRACE mode
        LOG_TRACE("Thread: " << std::this_thread::get_id() << "\n" << info->response);
        //  Update the changeset with the response
        _changesetMutex.lock();
        _changeset.updateChangeset(info->response);
        _changesetMutex.unlock();
        //  Update the size of the current changeset that is open
        changesetSize += workInfo->size();
        //  Get the last element from this changeset chunk
        last = _extractLastElement(workInfo);
        //  When the changeset eclipses the 10k max, the API automatically closes the changeset,
        //  reset the id and continue
        if (changesetSize >= _maxChangesetSize)
        {
          //  No need to call _closeChangeset() because it was closed by the API
          //  Signal for a new changeset to be created
          id = -1;
        }
        //  When the changeset is nearing the 10k max (or the specified max), close the changeset
        //  otherwise keep it open and go again
        else if (changesetSize > _maxChangesetSize - (int)(_maxPushSize * 1.5))
        {
          //  Close the changeset
          _closeChangeset(request, id, last);
          //  Signal for a new changeset id
          id = -1;
        }
        //  Throttle the input rate if desired
        if (_throttleWriters && !_changeset.isDone())
          _yield(std::max(_throttleTime - _throttlePlusMinus, 0) * 1000, (_throttleTime + _throttlePlusMinus) * 1000);
      }
      else
      {
        //  Report the status message for this failure
        _statusMessage(info, id);
        //  If this is the last changeset, error it all out and finish working
        if (workInfo->getFinished())
        {
          //  Fail the entire changeset
          _changeset.updateFailedChangeset(workInfo, true);
          //  Let the threads know that the remaining changeset is the "remaining" changeset
          _threadsCanExit = true;
          //  Looping should end the thread because all of the remaining elements have now been set to the failed state
          stop_thread = true;
          continue;
        }
        //  Split the changeset on conflict errors
        switch (info->status)
        {
        case HttpResponseCode::HTTP_CONFLICT:   //  Conflict, check for version conflicts and fix, or split and continue
          if (_changesetClosed(info->response))
          {
            //  The changeset was closed already so set the ID to -1 and reprocess
            id = -1;

            if ((int)workInfo->size() > _maxChangesetSize / 2)
            {
              //  Split the changeset into half so that it is smaller and won't fail
              _splitChangeset(workInfo);
            }
            else
            {
              //  Push the changeset back on the queue
              _pushChangesets(workInfo);
            }
            //  Loop back around to work on the next changeset
            continue;
          }
          else if (_fixConflict(request, workInfo, info->response))
          {
            //  If this changeset has version failed enough times, don't attempt to fix it
            if (!workInfo->canRetryVersion())
            {
              //  Fail the entire changeset
              _changeset.updateFailedChangeset(workInfo, true);
              //  Let the threads know that the remaining changeset is the "remaining" changeset
              _threadsCanExit = true;
              stop_thread = true;
              _updateThreadStatus(index, ThreadStatus::Failed);
              _changeset.failChangeset(workInfo);
              //  Set the error message
              _errorMessage = "Multiple version failures in a row, please refresh your data";
            }
            else
              _pushChangesets(workInfo);
            //  Loop back around to work on the next changeset
            continue;
          }
          //  Fall through here to split the changeset and retry
          //  This includes when the changeset is too big, i.e.:
          //    The changeset <id> was closed at <dtg> UTC
        case HttpResponseCode::HTTP_BAD_REQUEST:          //  Placeholder ID is missing or not unique
        case HttpResponseCode::HTTP_NOT_FOUND:            //  Diff contains elements where the given ID could not be found
        case HttpResponseCode::HTTP_PRECONDITION_FAILED:  //  Precondition Failed, Relation with id cannot be saved due to other member
          if (!_splitChangeset(workInfo, info->response))
          {
            if (!workInfo->getAttemptedResolveChangesetIssues())
            {
              //  Set the attempt issues resolved flag
              workInfo->setAttemptedResolveChangesetIssues(true);
              //  Try to automatically resolve certain issues, like out of date version
              if (_resolveIssues(request, workInfo))
              {
                _pushChangesets(workInfo);
              }
              else
              {
                //  Set the element in the changeset to failed because the issues couldn't be resolved
                _changeset.updateFailedChangeset(workInfo);
              }
            }
          }
          break;
        case HttpResponseCode::HTTP_GONE:                   //  Element has already been deleted, split and retry (error body is blank sometimes)
          //  Where there is only one element left and it is GONE, there is no need to retry it
          if (workInfo->size() == 1)
            workInfo->setAttemptedResolveChangesetIssues(true);
          //  Attempt to split the changeset
          if (!_splitChangeset(workInfo, info->response))
          {
            //  For HTTP_GONE, it could come back false if the element that is gone is removed
            //  successfully and the rest of the changeset needs to be processed
            if (workInfo->size() > 0)
              _pushChangesets(workInfo);
          }
          break;
        case HttpResponseCode::HTTP_INTERNAL_SERVER_ERROR:  //  Internal Server Error, could be caused by the database being saturated
        case HttpResponseCode::HTTP_BAD_GATEWAY:            //  Bad Gateway, there are issues with the gateway, split and retry
        case HttpResponseCode::HTTP_GATEWAY_TIMEOUT:        //  Gateway Timeout, server is taking too long, split and retry
          if (!_splitChangeset(workInfo, info->response))
          {
            //  Splitting failed which means that the changeset only has one element in it,
            //  push it back on the queue and give the API a break
            _pushChangesets(workInfo);
            //  Sleep the thread
            _yield();
          }
          break;
        default:
          //  This is a big problem, report it and try again
          request->logConnectionError();
          //  Fall through
        case HttpResponseCode::HTTP_METHOD_NOT_ALLOWED:
        case HttpResponseCode::HTTP_UNAUTHORIZED:
          //  This shouldn't ever happen, push back on the queue, only process a certain amount of times
          workInfo->retryFailure();
          if (workInfo->canRetryFailure())
            _pushChangesets(workInfo);
          else
            _changeset.updateFailedChangeset(workInfo, true);
          break;
        case HttpResponseCode::HTTP_SERVICE_UNAVAILABLE:
          _pushChangesets(workInfo);
          //  Multiple changeset failures
          changeset_failures++;
          if (changeset_failures >= 3)
          {
            //  Set the thread status to failed and report the error message in the main thread
            _updateThreadStatus(index, ThreadStatus::Failed);
            stop_thread = true;
          }
          else
          {
            //  Sleep between 30 and 60 seconds to allow the database server to recover
            _yield(30 * 1000, 60 * 1000);
          }
          break;
        }
      }
    }
    else
    {
      if (_threadsCanExit)
      {
        stop_thread = true;
        _updateThreadStatus(index, ThreadStatus::Completed);
      }
      else if (!_changeset.isDone() && queueSize == 0)
      {
        if (_threadsAreIdle() &&
            _changeset.getCleanupCount() > 0 &&
            _changeset.getTotalElementCount() <= (_changeset.getProcessedCount() + _changeset.getFailedCount() + _changeset.getCleanupCount()))
        {
          //  At this point all work is done and the threads can exit, if there is anything in the changeset cleanup, do it here
          ChangesetInfoPtr cleanup = _changeset.getCleanupElements();
          if (index == 0 && cleanup)
          {
            _changeset.clearCleanupElements();
            _pushChangesets(cleanup);
          }
        }
        else
        {
          //  This is a bad state where the producer thread says all elements are sent and
          //  waits for all threads to join but the changeset isn't "done".
          //  Set the status to idle and start idle timer
          _threadStatusMutex.lock();
          if (_threadStatus[index] != ThreadStatus::Idle)
          {
            _threadStatus[index] = ThreadStatus::Idle;
            _threadIdle[index].reset();
          }
          else if (id > 0 && _threadIdle[index].getElapsed() > 10 * 1000)
          {
            //  Close the current changeset so all data is "committed"
            _closeChangeset(request, id, last);
            id = -1;
          }
          _threadStatusMutex.unlock();
        }
      }
      else
      {
        //  Set the status to idle
        _updateThreadStatus(index, ThreadStatus::Idle);
        //  Yield the thread
        _yield();
      }
    }
  }
  //  Close the changeset if one is still open
  if (id != -1)
    _closeChangeset(request, id, last);
  //  Update the thread to complete if it didn't fail
  ThreadStatus status = _getThreadStatus(index);
  if (status != ThreadStatus::Failed && status != ThreadStatus::Unknown)
    _updateThreadStatus(index, ThreadStatus::Completed);
}

void OsmApiWriter::_yield(int milliseconds)
{
  //  Sleep for the specified number of milliseconds
  if (milliseconds != 10)
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
  else
    std::this_thread::yield();
}

void OsmApiWriter::_yield(int minimum_ms, int maximum_ms)
{
  //  Yield for a random amount of time between minimum_ms and maximum_ms
  _yield(minimum_ms + Tgs::Random::instance()->generateInt(maximum_ms - minimum_ms));
}

void OsmApiWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _description = options.getChangesetDescription();
  _source = options.getChangesetSource();
  _hashtags = options.getChangesetHashtags();
  _maxPushSize = options.getChangesetApidbSizeMax();
  _maxChangesetSize = options.getChangesetMaxSize();
  _maxWriters = options.getChangesetApidbWritersMax();
  _throttleWriters = options.getChangesetApidbWritersThrottle();
  _throttleTime = options.getChangesetApidbWritersThrottleTime();
  _throttlePlusMinus = options.getChangesetApidbWritersThrottleTimespan();
  _consumerKey = options.getHootOsmAuthConsumerKey();
  _consumerSecret = options.getHootOsmAuthConsumerSecret();
  _accessToken = options.getHootOsmAuthAccessToken();
  _secretToken = options.getHootOsmAuthAccessTokenSecret();
  _debugOutput = options.getChangesetApidbWriterDebugOutput();
  _debugOutputPath = options.getChangesetApidbWriterDebugOutputPath();
  _throttleCgiMap = options.getChangesetApidbWritersThrottleCgimap();
  _timeout = options.getChangesetApidbTimeout();
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
    capabilities.setPath(OsmApiEndpoints::API_PATH_CAPABILITIES);
    request->networkRequest(capabilities, _timeout);
    QString responseXml = QString::fromUtf8(request->getResponseContent().data());
    QString printableUrl = capabilities.toString(QUrl::RemoveUserInfo);
    HootNetworkRequest::removeIpFromUrlString(printableUrl, capabilities);
    LOG_TRACE("Capabilities: " << printableUrl);
    LOG_TRACE("Response: " << responseXml);
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
    permissions.setPath(OsmApiEndpoints::API_PATH_PERMISSIONS);
    request->networkRequest(permissions, _timeout);
    QString responseXml = QString::fromUtf8(request->getResponseContent().data());
    success = _parsePermissions(responseXml);
  }
  catch (const HootException& ex)
  {
    LOG_WARN(ex.what());
  }
  return success;
}

bool OsmApiWriter::usingCgiMap(HootNetworkRequestPtr request)
{
  bool cgimap = false;
  try
  {
    QUrl map = _url;
    map.setPath(OsmApiEndpoints::API_PATH_MAP);
    QUrlQuery query(map);
    //  Use the correct type of bbox for this query
    geos::geom::Envelope envelope(-77.42249541, -77.42249539, 38.96003149, 38.96003151);
    QString bboxQuery = GeometryUtils::toConfigString(envelope);
    query.addQueryItem("bbox", bboxQuery);
    map.setQuery(query);
    request->networkRequest(map, _timeout);
    QString responseXml = QString::fromUtf8(request->getResponseContent().data());
    QRegExp regex("generator=(\"|')CGImap", Qt::CaseInsensitive);
    cgimap = responseXml.contains(regex);
  }
  catch (HootException& ex)
  {
    LOG_WARN(ex.what());
  }
  return cgimap;
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

  LOG_TRACE("Permissions: " << permissions);

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
long OsmApiWriter::_createChangeset(HootNetworkRequestPtr request,
                                    const QString& description,
                                    const QString& source,
                                    const QString& hashtags,
                                    int& http_status)
{
  try
  {
    QUrl changeset = _url;
    changeset.setPath(OsmApiEndpoints::API_PATH_CREATE_CHANGESET);
    QString xml = QString(
      "<osm>"
      "  <changeset>"
      "    <tag k='created_by' v='%1'/>"
      "    <tag k='comment' v='%2'/>"
      "    <tag k='source' v='%3'/>"
      "    <tag k='hashtags' v='%4'/>"
      "    <tag k='bot' v='yes'/>"
      "  </changeset>"
      "</osm>").arg(HOOT_NAME).arg(description).arg(source).arg(hashtags);

    QByteArray content = xml.toUtf8();
    QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
    headers[QNetworkRequest::ContentTypeHeader] = HootNetworkUtils::CONTENT_TYPE_XML;
    headers[QNetworkRequest::ContentLengthHeader] = content.length();

    request->networkRequest(changeset, _timeout, QNetworkAccessManager::Operation::PutOperation, content);

    QString responseXml = QString::fromUtf8(request->getResponseContent().data());
    http_status = request->getHttpStatus();

    //  Only return the parsed response from HTTP 200 OK
    if (http_status == HttpResponseCode::HTTP_OK)
      return responseXml.toLong();
  }
  catch (const HootException& ex)
  {
    LOG_WARN(ex.what());
  }
  return -1;
}

//  https://wiki.openstreetmap.org/wiki/API_v0.6#Close:_PUT_.2Fapi.2F0.6.2Fchangeset.2F.23id.2Fclose
void OsmApiWriter::_closeChangeset(HootNetworkRequestPtr request, long changeset_id, LastElementInfo& last)
{
  try
  {
    QUrl changeset = _url;
    changeset.setPath(QString(OsmApiEndpoints::API_PATH_CLOSE_CHANGESET).arg(changeset_id));
    request->networkRequest(changeset, _timeout, QNetworkAccessManager::Operation::PutOperation);
    QString responseXml = QString::fromUtf8(request->getResponseContent().data());
    switch (request->getHttpStatus())
    {
    case HttpResponseCode::HTTP_NOT_FOUND:
      LOG_WARN("Unknown changeset");
      break;
    case HttpResponseCode::HTTP_CONFLICT:
      LOG_WARN("Changeset conflict: " << responseXml);
      break;
    case HttpResponseCode::HTTP_OK:
      //  Changeset closed successfully
      _changesetCountMutex.lock();
      _changesetCount++;
      //  Keep track of the last element information
      if (last.isValid())
      {
        _lastElement = last;
        last = LastElementInfo();
      }
      _changesetCountMutex.unlock();
      break;
    default:
      request->logConnectionError();
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
    change.setPath(QString(OsmApiEndpoints::API_PATH_UPLOAD_CHANGESET).arg(id));

    QByteArray content = changeset.toUtf8();
    QMap<QNetworkRequest::KnownHeaders, QVariant> headers;
    headers[QNetworkRequest::ContentTypeHeader] = HootNetworkUtils::CONTENT_TYPE_XML;
    headers[QNetworkRequest::ContentLengthHeader] = content.length();

    request->networkRequest(change, _timeout, headers, QNetworkAccessManager::Operation::PostOperation, content);

    info->response = QString::fromUtf8(request->getResponseContent().data());
    info->status = request->getHttpStatus();

    switch (info->status)
    {
    case HttpResponseCode::HTTP_OK:
      info->success = true;
      break;
    case HttpResponseCode::HTTP_BAD_REQUEST:
      LOG_WARN("Changeset Upload Error: Error parsing XML changeset - " << info->response);
      break;
    case HttpResponseCode::HTTP_NOT_FOUND:
      LOG_WARN("Unknown changeset or elements don't exist");
      break;
    case HttpResponseCode::HTTP_CONFLICT:
      LOG_WARN("Changeset conflict: " << info->response);
      break;
    case HttpResponseCode::HTTP_PRECONDITION_FAILED:
      LOG_WARN("Changeset precondition failed: " << info->response);
      break;
    default:
      request->logConnectionError();
      break;
    }
  }
  catch (const HootException& ex)
  {
    LOG_WARN(ex.what());
  }
  return info;
}

bool OsmApiWriter::_fixConflict(HootNetworkRequestPtr request, ChangesetInfoPtr changeset, const QString& conflictExplanation)
{
  bool success = false;
  long element_id = 0;
  ElementType::Type element_type = ElementType::Unknown;
  long version_old = 0;
  long version_new = 0;
  //  Match the error and parse the error information
  if (_changeset.getFailureCheck().matchesChangesetConflictVersionMismatchFailure(conflictExplanation, element_id, element_type, version_old, version_new))
  {
    //  Increment the retry version count
    changeset->retryVersion();
    //  Iterate the changeset types looking for the element, no need to check XmlChangeset::TypeCreate
    for (int changesetType = ChangesetType::TypeModify; changesetType < ChangesetType::TypeMax; ++changesetType)
    {
      ChangesetType type = static_cast<ChangesetType>(changesetType);
      ChangesetInfo::iterator element = changeset->begin(element_type, type);
      if (element != changeset->end(element_type, type))
      {
        QString update = "";
        //  Get the element from the OSM API
        if (element_type == ElementType::Node)
          update = _getNode(request, element_id);
        else if (element_type == ElementType::Way)
          update = _getWay(request, element_id);
        else if (element_type == ElementType::Relation)
          update = _getRelation(request, element_id);
        //  Fix the changeset with the node/way/relation from the OSM API
        success |= _changeset.fixChangeset(update);
      }
    }
  }
  return success;
}

bool OsmApiWriter::_changesetClosed(const QString &conflictExplanation)
{
  return _changeset.getFailureCheck().matchesChangesetClosedFailure(conflictExplanation);
}

bool OsmApiWriter::_resolveIssues(HootNetworkRequestPtr request, ChangesetInfoPtr changeset)
{
  bool success = false;
  //  Can only work on changesets of size 1
  if (changeset->size() != 1)
    return false;
  for (int elementType = ElementType::Node; elementType < ElementType::Unknown; ++elementType)
  {
    ElementType::Type e_type = static_cast<ElementType::Type>(elementType);
    //  Creates cannot be fixed with this method, skip them here
    for (int changesetType = ChangesetType::TypeModify; changesetType < ChangesetType::TypeMax; ++changesetType)
    {
      ChangesetType c_type = static_cast<ChangesetType>(changesetType);
      ChangesetInfo::iterator element = changeset->begin(e_type, c_type);
      if (element != changeset->end(e_type, c_type))
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
  return _getElement(request, QString(OsmApiEndpoints::API_PATH_GET_ELEMENT).arg("node").arg(id));
}

QString OsmApiWriter::_getWay(HootNetworkRequestPtr request, long id)
{
  //  Check for a valid ID to query against
  if (id < 1)
    return "";
  //  Get the way by ID
  return _getElement(request, QString(OsmApiEndpoints::API_PATH_GET_ELEMENT).arg("way").arg(id));
}

QString OsmApiWriter::_getRelation(HootNetworkRequestPtr request, long id)
{
  //  Check for a valid ID to query against
  if (id < 1)
    return "";
  //  Get the relation by ID
  return _getElement(request, QString(OsmApiEndpoints::API_PATH_GET_ELEMENT).arg("relation").arg(id));
}

QString OsmApiWriter::_getElement(HootNetworkRequestPtr request, const QString& endpoint)
{
  //  Don't follow an uninitialized URL or empty endpoint
  if (endpoint == OsmApiEndpoints::API_PATH_GET_ELEMENT || endpoint == "")
    return "";
  try
  {
    QUrl get = _url;
    get.setPath(endpoint);
    request->networkRequest(get, _timeout);
    if (request->getHttpStatus() == HttpResponseCode::HTTP_OK)
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

bool OsmApiWriter::_threadsAreIdle()
{
  bool response = true;
  //  Lock the thread status mutex only once
  _threadStatusMutex.lock();
  for (vector<ThreadStatus>::iterator it = _threadStatus.begin(); it != _threadStatus.end(); ++it)
  {
    //  It only takes one thread working to return false
    if (*it == ThreadStatus::Working)
    {
      response = false;
      break;
    }
  }
  //  Unlock the thread status mutex
  _threadStatusMutex.unlock();
  return response;
}

bool OsmApiWriter::_splitChangeset(const ChangesetInfoPtr& workInfo, const QString& response)
{
  //  Try to split the changeset in half
  _changesetMutex.lock();
  ChangesetInfoPtr split = _changeset.splitChangeset(workInfo, response);
  _changesetMutex.unlock();
  if (split->size() > 0)
  {
    //  Fail the split changeset if the error flag is set
    if (split->getError())
    {
      _changeset.failChangeset(split);
      _pushChangesets(workInfo);
      return true;
    }
    else
    {
      //  Push both of the changesets onto the queue
      _pushChangesets(workInfo, split);
      return true;
    }
  }
  //  Nothing was split out, return false
  return false;
}

void OsmApiWriter::_writeDebugFile(const QString& type, const QString& data, int file_id, long changeset_id, int status)
{
  //  Setup the path including the changeset and file IDs, type and HTTP status
  QString path = QString("%1/OsmApiWriter-%2-%3-%4-%5.osc")
      .arg(_debugOutputPath)
      .arg(QString::number(file_id), 6, '0')
      .arg(QString::number(changeset_id), 5, '0')
      .arg(type)
      .arg(QString::number(status), 3, '0');
  //  Write the data to the file
  FileUtils::writeFully(path, data);
}

int OsmApiWriter::_getNextApiId()
{
  //  Lock the mutex and increment the API ID counter
  std::lock_guard<std::mutex> lock(_apiIdMutex);
  return ++_apiId;
}

void OsmApiWriter::_pushChangesets(ChangesetInfoPtr changeset, ChangesetInfoPtr changeset2)
{
  std::lock_guard<std::mutex> lock(_workQueueMutex);
  if (changeset)
    _workQueue.push(changeset);
  if (changeset2)
    _workQueue.push(changeset2);
}

void OsmApiWriter::_startWork()
{
  std::lock_guard<std::mutex> lock(_startMutex);
  _startFlag = true;
  _start.notify_all();
}

void OsmApiWriter::_waitForStart()
{
  std::unique_lock<std::mutex> lock(_startMutex);
  _start.wait(lock, [this]{ return _startFlag; });
}

OsmApiWriter::ThreadStatus OsmApiWriter::_getThreadStatus(int thread_index)
{
  //  Validate the index
  if (thread_index < 0 || thread_index >= (int)_threadStatus.size())
    return ThreadStatus::Unknown;
  //  Lock the mutex and return the status
  std::lock_guard<std::mutex> lock(_threadStatusMutex);
  return _threadStatus[thread_index];
}


void OsmApiWriter::_updateThreadStatus(int thread_index, ThreadStatus status)
{
  //  Validate the index
  if (thread_index < 0 || thread_index >= (int)_threadStatus.size())
    return;
  //  Lock the mutex and update the status
  std::lock_guard<std::mutex> lock(_threadStatusMutex);
  _threadStatus[thread_index] = status;
}

bool OsmApiWriter::_allThreadsFailed()
{
  std::lock_guard<std::mutex> lock(_threadStatusMutex);
  for (size_t i = 0; i < _threadStatus.size(); ++i)
  {
    //  Short circuit the loop if a thread isn't in the failed state
    if (_threadStatus[i] != ThreadStatus::Failed)
      return false;
  }
  //  All threads are in the failed state
  return true;
}

bool OsmApiWriter::_hasFailedThread()
{
  std::lock_guard<std::mutex> lock(_threadStatusMutex);
  for (size_t i = 0; i < _threadStatus.size(); ++i)
  {
    //  Short circuit the loop if a thread is in the failed state
    if (_threadStatus[i] == ThreadStatus::Failed)
      return true;
  }
  //  All threads are in the non-failed state
  return false;
}

void OsmApiWriter::_statusMessage(OsmApiFailureInfoPtr info, long changesetId)
{
  //  Log the error as a status message
  switch (info->status)
  {
  /** These error states are understandable and aren't "errors" per-se */
  case HttpResponseCode::HTTP_CONFLICT:               //  Conflict, check for version conflicts and fix, or split and continue
    LOG_STATUS("Version conflict in changeset " << changesetId << " upload. Updating element.");
    break;
  case HttpResponseCode::HTTP_NOT_FOUND:              //  Diff contains elements where the given ID could not be found
    LOG_STATUS("Element with given ID not found in changeset " << changesetId << ", moving element to manual changeset.");
    break;
  case HttpResponseCode::HTTP_PRECONDITION_FAILED:    //  Precondition Failed, Relation with id cannot be saved due to other member
    LOG_STATUS("Required precondition not met in changeset " << changesetId << ", moving element to manual changeset.");
    break;
  case HttpResponseCode::HTTP_GONE:                   //  Element has already been deleted, split and retry (error body is blank sometimes)
    LOG_STATUS("Element to be deleted already deleted, removing from changeset " << changesetId);
    break;
  case HttpResponseCode::HTTP_BAD_REQUEST:            //  Placeholder ID is missing or not unique
    LOG_STATUS("Element placehoder ID is missing or not unique in changeset " << changesetId << ", moving element the manual changeset.");
    break;
  /** These error states are actual errors and the user should report them */
  case HttpResponseCode::HTTP_INTERNAL_SERVER_ERROR:  //  Internal Server Error, could be caused by the database being saturated
    LOG_STATUS("API server responded with internal server error, API is not stable.  Changeset " << changesetId);
    break;
  case HttpResponseCode::HTTP_BAD_GATEWAY:            //  Bad Gateway, there are issues with the gateway, split and retry
    LOG_STATUS("API server responded with bad gateway error, API is not stable.  Changeset " << changesetId);
    break;
  case HttpResponseCode::HTTP_GATEWAY_TIMEOUT:        //  Gateway Timeout, server is taking too long, split and retry
    LOG_STATUS("API server responded with gateway timeout, API is not stable.  Changeset " << changesetId);
    break;
  case HttpResponseCode::HTTP_SERVICE_UNAVAILABLE:
    LOG_STATUS("API server responded with service unavailable, API is not stable.  Changeset " << changesetId);
    break;
  case HttpResponseCode::HTTP_METHOD_NOT_ALLOWED:
    LOG_STATUS("API server responded with method not allowed error.  Changeset " << changesetId);
    break;
  case HttpResponseCode::HTTP_UNAUTHORIZED:
    LOG_STATUS("User not authorized to apply changesets to API.");
    break;
  /** Unknown errors are bad */
  default:
    LOG_STATUS("Unknown network error occurred while uploading changeset " << changesetId);
    break;
  }
}

LastElementInfo OsmApiWriter::_extractLastElement(const ChangesetInfoPtr& workInfo)
{
  //  Get the last element uploaded
  LastElementInfo last = workInfo->getLastElement();
  //  Update the structure with positive ID for create and version of element
  _changeset.updateLastElement(last);
  return last;
}

}
