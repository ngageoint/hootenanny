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
 * @copyright Copyright (C) 2015, 2019 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <hoot/core/util/HootException.h>

namespace hoot
{

Progress::Progress(QString source, JobState jobState, float percentComplete, float taskWeight) :
_source(source),
_reportType(ConfigOptions().getProgressReportingFormat()),
_percentComplete(percentComplete),
_taskStartPercentComplete(percentComplete),
_lastPercentComplete(percentComplete),
_taskWeight(taskWeight),
_jobState(jobState),
_jobFinished(false),
_userMessage("")
{
  LOG_VART(_source);
  LOG_VART(_percentComplete);
  LOG_VART(_taskStartPercentComplete);
  LOG_VART(_lastPercentComplete);
  LOG_VART(_taskWeight);
  LOG_VART(_jobState);
}

QString Progress::_getMessage() const
{
  QString msg = "";
  if (_reportType == "text")
  {
    msg.append(_toText());
  }
  // do nothing if report type is empty
  return msg;
}

//void Progress::setTaskWeight(float taskWeight)
//{
//  _taskWeight = taskWeight;
//  LOG_VART(_taskWeight);
//  _taskStartPercentComplete = _percentComplete;
//  LOG_VART(_taskStartPercentComplete);
//}

void Progress::set(float percentComplete, QString userMessage)
{
  set(percentComplete, _jobState, _jobFinished, userMessage);
}

void Progress::set(float percentComplete, JobState jobState, bool jobFinished, QString userMessage)
{
  _lastPercentComplete = _percentComplete;
  _percentComplete = percentComplete;
  // reset the start percent complete value to current value
  _taskStartPercentComplete = percentComplete;
  LOG_VART(_lastPercentComplete);
  LOG_VART(_percentComplete);
  LOG_VART(_taskStartPercentComplete);

  _jobState = jobState;
  _jobFinished = jobFinished;
  _userMessage = userMessage;
  LOG_VART(_userMessage);
  const QString msg = _getMessage();
  if (!msg.trimmed().isEmpty())
  {
    LOG_STATUS(msg);
  }
}

void Progress::setFromRelative(float relativePercentComplete, QString userMessage,
                               bool logAsProgress)
{
  setFromRelative(relativePercentComplete, userMessage, logAsProgress);
}

void Progress::setFromRelative(float relativePercentComplete, JobState jobState, bool jobFinished,
                               QString userMessage, bool logAsProgress)
{
  // update absolute percent weight
  _lastPercentComplete = _percentComplete;
  _percentComplete = _taskStartPercentComplete + (relativePercentComplete * _taskWeight);
  LOG_VART(_taskWeight);
  LOG_VART(relativePercentComplete);
  LOG_VART(_lastPercentComplete);
  LOG_VART(_percentComplete);
  LOG_VART(_taskStartPercentComplete);

  _jobState = jobState;
  _jobFinished = jobFinished;
  _userMessage = userMessage;
  LOG_VART(_userMessage);
  const QString msg = _getMessage();
  if (!msg.trimmed().isEmpty())
  {
    if (!logAsProgress)
    {
      LOG_STATUS(msg);
    }
    else
    {
      PROGRESS_STATUS(msg);
    }
  }
}

//QString Progress::jobStateToString(JobState jobState) const
//{
//  switch (jobState)
//  {
//    case JobState::Pending:
//      return "Pending";
//    case JobState::NotRunning:
//      return "Not Running";
//    case JobState::Running:
//      return "Running";
//    case JobState::Successful:
//      return "Successful";
//    case JobState::Failed:
//      return "Failed";
//    default:
//      throw IllegalArgumentException(QString("Unknown (%1)").arg(jobState));
//  }
//}

QString Progress::_toText() const
{
  QString js = "";
  js.append(_source);
  js.append(" (" % QString("%1").arg((int)(_percentComplete * 100.)) % "%): ");
  js.append(_userMessage);
  return js;
}

}
