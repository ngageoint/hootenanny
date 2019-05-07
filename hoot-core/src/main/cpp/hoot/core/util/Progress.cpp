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

// standard includes
#include <iostream>
using namespace std;

// QT includes
#include <QtCore/QStringBuilder>

// Hoot includes
#include "Progress.h"


namespace hoot
{

// constructor
Progress::Progress(QString source = "")
{
  _source = source;
  _reportType = "";
  _percentComplete = 0.0;
  _taskStartPercentComplete = 0.0;
  _lastPercentComplete = 0.0;
  _state = "Pending";
  _jobFinished = false;
  _userMessage = "";
}

/////////////////////////////////
// public methods
/////////////////////////////////

// get the message in report type's format
// - this will be called by the polling method
QString Progress::getMessage()
{
  QString msg = "";
  // send message only if exceeds some range (e.g., every 10%)
  if (_readyToSend())
  {
    if (_reportType == "json")
    {
      msg.append( _toJson() );
    }
  }
  return msg;
}

// sets the task weight for the next subtask in range of 0...p(x) where p(x) is progress percentage leftover
//   for completing the current overall task
void Progress::setTaskWeight(float taskWeight)
{
  _taskWeight = taskWeight;
  _taskStartPercentComplete = _percentComplete;
}

// the task method generally uses this setter
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
  if (_reportType == "json")
  {
    QString msg = getMessage();
    if (msg != "")
    {
      cout << getMessage().toStdString() << endl;
    }
  }
}

// set from relative values updates the absolute values automatically
void Progress::setFromRelative(float relativePercentComplete, QString state, bool jobFinished, QString userMessage)
{
  // update absolute percent weight
  _lastPercentComplete = _percentComplete;
  _percentComplete = _taskStartPercentComplete + (relativePercentComplete * _taskWeight);
  _state = state;
  _jobFinished = jobFinished;
  _userMessage = userMessage;
  //temporary force getMessage call
  if (_reportType == "json")
  {
    QString msg = getMessage();
    if (msg != "")
    {
      cout << getMessage().toStdString() << endl;
    }
  }
}

/////////////////////////////////
// protected methods
/////////////////////////////////

// convert progress status to JSON
QString Progress::_toJson()
{
  QString js = "{ ";
    js.append("\"source\": \"" % _source % "\", ");
    js.append("\"status\": { ");
      js.append("\"message\": \"" % _userMessage % "\", ");
      js.append("\"state\": \"" % _state % "\", ");
      js.append("\"jobFinished\": \"" % QString("%1").arg(_jobFinished?"true":"false") % "\", ");
      js.append("\"percentComplete\" : " % QString("%1").arg((int)(_percentComplete*100.)));
    js.append(" }");
  js.append(" }");
  return js;
}


// check if ready to send message or not
bool Progress::_readyToSend()
{
  int last = (int)(_lastPercentComplete*10.);
  int current = (int)(_percentComplete*10.);
  return (current - last > 0? true : false);
}

} // end namespace hoot
