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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef HOOT_PROGRESS_H
#define HOOT_PROGRESS_H

namespace hoot
{

/**
 * Allows for job progress reporting across the application
 */
class Progress
{
public:

  enum JobState
  {
    Pending = 1,
    NotRunning,
    Running,
    Successful,
    Failed
  };

  Progress(QString jobId = "", QString source = "", JobState jobState = JobState::Pending,
           float percentComplete = 0.0, float taskWeight = 0.0);

  /**
   * Logs a progress status message
   *
   * @param percentComplete completion percentage of the job
   * @param state the job's state
   * @param userMessage descriptive message for the job
   * @param logAsProgress if true; uses status logging which allows for re-uses the same output
   * line in the log display
   */
  void set(float percentComplete, JobState state, QString userMessage, bool logAsProgress = false);

  /**
   * Logs a progress status message
   *
   * @param percentComplete completion percentage of the job
   * @param userMessage descriptive message for the job
   * @param logAsProgress if true; uses status logging which allows for re-uses the same output
   * line in the log display
   */
  void set(float percentComplete, QString userMessage, bool logAsProgress = false);

  /**
   * Logs a progress status message as a child subtask of a job
   *
   * @param relativePercentComplete the starting completion percentage point the subtask should
   * calculate its completion percentage from
   * @param state the job's state
   * @param userMessage whether the job has completed or not
   * @param logAsProgress if true; uses status logging which allows for re-uses the same output
   * line in the log display
   */
  void setFromRelative(float relativePercentComplete, JobState state, QString userMessage,
                       bool logAsProgress = false);

  /**
   * Logs a progress status message as a child subtask of a job
   *
   * @param relativePercentComplete the starting completion percentage point the subtask should
   * calculate its completion percentage from
   * @param userMessage whether the job has completed or not
   * @param logAsProgress if true; uses status logging which allows for re-uses the same output
   * line in the log display
   */
  void setFromRelative(float relativePercentComplete, QString userMessage,
                       bool logAsProgress = false);

  /**
   * Determines if a job is finished
   *
   * @return true if the job is finished; false otherwise
   */
  bool getJobFinished() const;

  float getPercentComplete() const { return _percentComplete; }
  float getTaskWeight() const { return _taskWeight; }
  JobState getState() const { return _jobState; }

  void setJobId(QString id) { _jobId = id; }
  void setSource(QString source) { _source = source; }
  void setState(JobState state) { _jobState = state; }

private:

  // unique ID of the job; currently only applicable when jobs use the Hoot API DB as a data source
  QString _jobId;

  // type of job; set at the command level
  QString _source;

  // progress state for current task
  float _percentComplete;
  float _taskStartPercentComplete;
  float _lastPercentComplete;

  // when progress is passed from parent to child, what percentage of the child's entire task makes
  // up the subset of its parent's task
  float _taskWeight;

  JobState _jobState;

  QString _userMessage;

  // the polling method calls this getter; get the message in report type's format
  // - this will be called by the polling method
  QString _getMessage() const;
  QString _toText() const;
};

}

#endif // HOOT_PROGRESS_H
