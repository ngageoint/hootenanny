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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Progress.h"

// std
#include <iostream>
using namespace std;

// Qt
#include <QtCore/QStringBuilder>

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

Progress::Progress(QString source) :
_source(source),
_reportType(ConfigOptions().getProgressReportingFormat()),
_percentComplete(0.0),
_taskStartPercentComplete(0.0),
_lastPercentComplete(0.0),
_state("Pending"),
_jobFinished(false),
_userMessage(""),
_progressReportingGranularity(1.0)
{
}

QString Progress::getMessage() const
{
  QString msg = "";
  // send message only if exceeds some range (e.g., every 10%)
  //if (_readyToSend())
  //{
    if (_reportType == "json")
    {
      msg.append(_toJson());
    }
    else if (_reportType == "text") //TODO: update config opts with this
    {
      msg.append(_toText());
    }
    else
    {
      // do nothing
    }
  //}
  return msg;
}

void Progress::setTaskWeight(float taskWeight)
{
  _taskWeight = taskWeight;
  _taskStartPercentComplete = _percentComplete;
}

void Progress::set(float percentComplete, QString state, bool jobFinished, QString userMessage)
{
  _lastPercentComplete = _percentComplete;
  _percentComplete = percentComplete;
  // reset the start percent complete value to current value
  _taskStartPercentComplete = percentComplete;
  _state = state;
  _jobFinished = jobFinished;
  _userMessage = userMessage;
  //temporary force getMessage call
  //if (_reportType == "json")
  //{
    QString msg = getMessage();
    if (msg != "")
    {
      //cout << getMessage().toStdString() << endl;
      LOG_STATUS(getMessage());
    }
  //}
}

void Progress::setFromRelative(float relativePercentComplete, QString state, bool jobFinished,
                               QString userMessage)
{
  // update absolute percent weight
  _lastPercentComplete = _percentComplete;
  _percentComplete = _taskStartPercentComplete + (relativePercentComplete * _taskWeight);
  _state = state;
  _jobFinished = jobFinished;
  _userMessage = userMessage;
  //temporary force getMessage call
  //if (_reportType == "json")
  //{
    QString msg = getMessage();
    if (msg != "")
    {
      //cout << getMessage().toStdString() << endl;
      LOG_STATUS(getMessage());
    }
  //}
}

QString Progress::_toJson() const
{
  QString js = "{ ";
    js.append("\"source\": \"" % _source % "\", ");
    js.append("\"status\": { ");
      js.append("\"message\": \"" % _userMessage % "\", ");
      js.append("\"state\": \"" % _state % "\", ");
      js.append("\"jobFinished\": \"" % QString("%1").arg(_jobFinished ? "true" : "false") % "\", ");
      js.append("\"percentComplete\" : " % QString("%1").arg((int)(_percentComplete * 100.)));
    js.append(" }");
  js.append(" }");
  return js;
}

QString Progress::_toText() const
{
  QString js = "";
    js.append(_source % ": ");
      js.append(_userMessage % "; ");
      //js.append("state: " % _state % ", ");
      js.append(QString("%1").arg((int)(_percentComplete * 100.)) % "% complete");
  return js;
}

bool Progress::_readyToSend() const
{
  int last = (int)(_lastPercentComplete * _progressReportingGranularity);
  //LOG_VARS(last);
  int current = (int)(_percentComplete * _progressReportingGranularity);
  //LOG_VARS(current);
  return (current - last > 0 ? true : false);
}

}
