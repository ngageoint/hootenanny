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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_PROGRESS_H
#define HOOT_PROGRESS_H

namespace hoot
{

class Progress
{
public:
  Progress(QString source);

  // getters
  QString getSource() { return _source; }
  QString getReportType() { return _reportType; }
  float getPercentComplete() { return _percentComplete; }
  float getTaskWeight() { return _taskWeight; }
  QString getState() { return _state; }
  bool getJobFinished() { return _jobFinished; }
  QString getUserMessage() { return _userMessage; }

  // the polling method calls this getter
  QString getMessage();

  // setters
  void setSource(QString source) { _source = source; }
  void setReportType(QString reportType) { _reportType = reportType; }
  void setPercentComplete(float percentComplete) { _percentComplete = percentComplete; }
  void setTaskWeight(float taskWeight);
  void setState(QString state) { _state = state; }
  void setJobFinished(bool jobFinished) { _jobFinished = jobFinished; }
  void setUserMessage(QString userMessage) { _userMessage = userMessage; }

  // the task method generally uses this setter
  void set(float percentComplete, QString state, bool jobFinished, QString userMessage);

  // really cool method to do the absolute percent complete from the relative percent
  void setFromRelative(float relativePercentComplete, QString state, bool jobFinished,
                       QString userMessage);

protected:
  // source command
  QString _source;

  // report format (json, ...)
  QString _reportType;

  // progress for current task & extra storage variables
  float _percentComplete;
  float _taskStartPercentComplete;
  float _lastPercentComplete;

  // task weight
  float _taskWeight;

  // task's state (Pending, NotRunning, Running, Successful, Failed)
  QString _state;

  // task's finish status
  bool _jobFinished;

  // resulting message
  QString _userMessage;

  // methods
  // output JSON version of message
  QString _toJson();

  // processing for getMessage() to send messages when a metric met (for now every 0.1 units)
  bool _readyToSend();
};

} // end namespace hoot

#endif // HOOT_PROGRESS_H
