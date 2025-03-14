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
 * @copyright Copyright (C) 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef PROCESS_POOL_H
#define PROCESS_POOL_H

// Hoot
#include <hoot/core/util/Log.h>

//  Standard
#include <memory>
#include <mutex>
#include <set>
#include <vector>

//  QT
#include <QString>
#include <QProcess>
#include <QThread>
#include <QWaitCondition>
using QProcessPtr = std::shared_ptr<QProcess>;

#define HOOT_TEST_FINISHED "HOOT_TEST_FINISHED"

/**
 * @brief The JobQueue class that is thread-safe queue class implemented with a std::set so it can
 * be searched in non-linear time for the contains() method
 *
 */
class JobQueue
{
public:

  /** Standard constructor */
  JobQueue() = default;

  /**
   * @brief basic empty method on queue class, thread-safe
   * @return true if the set is empty
   */
  bool empty();

  /**
   * @brief basic contains method for searching the queue, thread-safe
   * @param job - name of job to check the queue for
   * @return true if job is found in the queue
   */
  bool contains(const QString& job);

  /**
   * @brief atomic pop/top method in one in order to be thread-safe
   * @return top job name on the queue
   */
  QString pop();

  /**
   * @brief basic push method, thread-safe
   * @param job - name of job to add to back of queue
   */
  void push(const QString& job);

  QString toString() const;

  QString getName() const { return _name; }

  void setName(const QString& name) { _name = name; }

private:

  /** Mutex for locking _jobs */
  std::mutex _mutex;
  /** std::set of job names to be processed */
  std::set<QString> _jobs;
  // job queue name
  QString _name;
};


/**
 * @brief The ProcessThread class is a process manager class that runs in a separate execution
 * thread so that it can wait on output from the thread without holding up other test threads or
 * without requiring polling of all processes
 */
class ProcessThread : public QThread
{
public:
  /**
   * @brief ProcessThread constructor
   * @param threadId ID of this thread to output if '--names' is set
   * @param maxThreads Maximum number of threads in the pool
   * @param showTestName boolean flag indicating if the thread should pass the '--names' flag to
   * the process
   * @param suppressFailureDetail If true, only the failing test name gets logged and none of the
   * failure detail is logged.
   * @param printDiff boolean flag indicating if the thread should pass the '--diff' flag to the
   * process
   * @param disableFailureRetries If true, tests will never be run a subsequent time after a
   * failure; useful for debugging
   * @param waitTime number of seconds to wait before reporting that a test took too long
   * @param outMutex mutex for preserving output ordering to standard out
   * @param parallelJobs JobQueue object that contains a set of jobs that can all be run in
   * parallel
   * @param casesJobs JobQueue object that contains a set of conflate cases that must be run last
   * @param serialJobs JobQueue object (nullptr for all threads but one) that contains a set of
   * all jobs that cannot be run in parallel but must be run serially
   * @param serialCasesJobs JobQueue object (nullptr for all threads but one) that contains a set
   * of conflate cases that must be run serially
   */
  ProcessThread(int threadId, int maxThreads, bool showTestName, bool suppressFailureDetail, bool printDiff,
                bool disableFailureRetries, double waitTime, std::mutex* outMutex, JobQueue* parallelJobs,
                JobQueue* casesJobs, JobQueue* serialJobs, JobQueue* serialCasesJobs);
  virtual ~ProcessThread() = default;

  /**
   * @brief run method for thread, called by ::start()
   */
  void run();

  /**
   * @brief getFailures accessor function of _failures count
   * @return return number of failed tests
   */
  int getFailures() const { return _failures; }

private:

  /**
   * @brief createProcess actually creates a HootTest process that is listening for tests on
   * standard in to run
   * @return pointer to the process created, ownership is passed back
   */
  QProcess* createProcess();

  /**
   * @brief resetProcess shutsdown the current process and starts a new one in its place for error
   * handling
   */
  void resetProcess();

  /**
   * @brief processJobs abstracted function to process all jobs in the shared job queue
   * @param queue pointer to the job queue to begin work on
   */
  void processJobs(JobQueue* queue);

  /** Thread ID for current process */
  int _threadId;
  /** Maximum number of threads in the pool */
  int _maxThreads;
  /** Flag for showing test names in output */
  bool _showTestName;
  /** If true, only the failing test name gets logged and none of the failure detail is logged. */
  bool _suppressFailureDetail;
  /** Flag for showing diff for script test failures in output */
  bool _printDiff;
  /** Disables the rerunning of tests when they fail **/
  bool _disableFailureRetries;
  /** Time (in seconds) to wait before reporting a tests is "taking too long" */
  double _waitTime;
  /** Mutex guarding standard out so that output messages aren't scrambled */
  std::mutex* _outMutex;
  /** Pointer to shared job queue that contains names of jobs that can all be run in parallel */
  JobQueue* _parallelJobs;
  /** Pointer to shared job queue that contains only names of conflate case jobs */
  JobQueue* _casesJobs;
  /** Pointer to job queue that contains only names of jobs that must be run serially */
  JobQueue* _serialJobs;
  /** Pointer to job queue that contains only names of conflate case jobs that must be run serially */
  JobQueue* _serialCasesJobs;
  /** Number of failed tests */
  int _failures;
  /** Shared pointer containing ownership of the process pointer from createProcess() */
  QProcessPtr _proc;
};

using ProcessThreadPtr = std::shared_ptr<ProcessThread>;

/**
 * @brief The ProcessPool class that encapsulates the division of work amongs processes.  Tests
 *  that must be run serially are passed off to the first process in the pool while the others
 *  work on jobs that can be run in parallel.  If there are any jobs left after that the first
 *  process begins working on those tests with all of the rest of the processes in the pool.
 */
class ProcessPool
{
public:

  /**
   * @brief ProcessPool constructor
   * @param nproc number of threads/processes to add to the pool
   * @param waitTime number of seconds to wait before reporting that a test took too long
   * @param showTestName boolean flag indicating if the thread should pass the '--names' flag to
   * the process
   * @param suppressFailureDetail If true, only the failing test name gets logged and none of the
   * failure detail is logged.
   * @param printDiff boolean flag indicating if the thread should pass the '--diff' flag to the
   * process
   * @param disableFailureRetries If true, tests will never be run a subsequent time after a
   * failure; useful for debugging
   */
  ProcessPool(int nproc, double waitTime, bool showTestName, bool suppressFailureDetail,
              bool printDiff, bool disableFailureRetries);
  /** Destructor */
  ~ProcessPool();

  enum JobType : int
  {
    SerialJob   = 0x01,
    ParallelJob = 0x02,
    ConflateJob = 0x04
  };

  /**
   * @brief addJob method to add a new job to one of the job queues
   * @param test - name of job to add
   * @param parallel - boolean flag indicating if this test is able to be run in parallel
   */
  void addJob(const QString& test, JobType job_type = ParallelJob);

  /**
   * @brief startProcessing starts each thread and process in order
   */
  void startProcessing();

  /**
   * @brief getFailures gets the unit test failure count
   * @return number of unit tests that failed
   */
  int getFailures();

  /**
   * @brief wait for all threads (and processes) to finish working
   */
  void wait();

  /**
   * @brief jobQueueToString prints the contents of a job queue
   * @param name name of the job queue to print; valid values are: parallel, serial, or case
   * @return a string
   */
  QString jobQueueToString(const QString& name) const;

private:

  /** vector of n threads to do the processing */
  std::vector<ProcessThreadPtr> _threads;
  /** queue of jobs that must be run in serial */
  JobQueue _serialJobs;
  /** queue of jobs that can be run in parallel */
  JobQueue _parallelJobs;
  /** queue of conflate case jobs */
  JobQueue _caseJobs;
  /** queue of serial conflate case jobs */
  JobQueue _serialCaseJobs;
  /** mutex protecting standard out to keep all output for each single test together */
  std::mutex _mutex;
  /** count of failed unit tests */
  int _failed;
};

#endif  //  PROCESS_POOL_H
