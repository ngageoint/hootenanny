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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef HOOT_PROGRESS_H
#define HOOT_PROGRESS_H

// Qt
#include <QString>

namespace hoot
{

/**
 * Allows for job progress reporting across the application
 */
class Progress
{
public:

  typedef enum
  {
    Pending = 1,
    NotRunning,
    Running,
    Successful,
    Failed
  } JobState;

  Progress(QString source = "", JobState jobState = JobState::Pending, float percentComplete = 0.0,
           float taskWeight = 0.0);

  /**
   *
   *
   * @param percentComplete
   * @param state
   * @param jobFinished
   * @param userMessage
   */
  void set(float percentComplete, JobState state, bool jobFinished, QString userMessage);

  /**
   *
   *
   * @param percentComplete
   * @param userMessage
   */
  void set(float percentComplete, QString userMessage);

  // method to do the absolute percent complete from the relative percent;
  // set from relative values updates the absolute values automatically
  /**
   *
   *
   * @param relativePercentComplete
   * @param state
   * @param jobFinished
   * @param userMessage
   * @param logAsProgress
   */
  void setFromRelative(float relativePercentComplete, JobState state, bool jobFinished,
                       QString userMessage, bool logAsProgress = false);

  /**
   *
   *
   * @param relativePercentComplete
   * @param userMessage
   * @param logAsProgress
   */
  void setFromRelative(float relativePercentComplete, QString userMessage,
                       bool logAsProgress = false);

  //QString getSource() const { return _source; }
  //QString getReportType() const { return _reportType; }
  float getPercentComplete() const { return _percentComplete; }
  float getTaskWeight() const { return _taskWeight; }
  JobState getState() const { return _jobState; }
  bool getJobFinished() const { return _jobFinished; }
  //QString getUserMessage() const { return _userMessage; }

  void setSource(QString source) { _source = source; }
  //void setReportType(QString reportType) { _reportType = reportType.toLower(); }
  //void setPercentComplete(float percentComplete) { _percentComplete = percentComplete; }
  // sets the task weight for the next subtask in range of 0...p(x) where p(x) is progress
  // percentage leftover for completing the current overall task
  //void setTaskWeight(float taskWeight);
  void setState(JobState state) { _jobState = state; }
  //void setJobFinished(bool jobFinished) { _jobFinished = jobFinished; }
  //void setUserMessage(QString userMessage) { _userMessage = userMessage; }

  //QString jobStateToString(JobState jobState) const;

private:

  // type of job; set at the command level
  QString _source;

  // reporting format (text)
  QString _reportType;

  // progress state for current task
  float _percentComplete;
  float _taskStartPercentComplete;
  float _lastPercentComplete;

  // when progress is passed from parent to child, what percentage of the child's entire task makes
  // up the subset of its parent's task
  float _taskWeight;

  JobState _jobState;

  bool _jobFinished;

  QString _userMessage;

  // the polling method calls this getter; get the message in report type's format
  // - this will be called by the polling method
  QString _getMessage() const;
  QString _toText() const;
};

}

#endif // HOOT_PROGRESS_H
