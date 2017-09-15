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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PROCESS_POOL_H
#define PROCESS_POOL_H

//  QT
#include <QMutex>
#include <QProcess>
#include <QThread>
#include <QWaitCondition>

//  Standard
#include <set>
#include <vector>

//  Boost
#include <boost/shared_ptr.hpp>

#define HOOT_TEST_FINISHED "HOOT_TEST_FINISHED"

typedef boost::shared_ptr<QProcess> QProcessPtr;

class JobQueue
{
public:
  JobQueue();

  bool empty();

  bool contains(const QString& job);

  QString pop();

  void push(const QString& job);

private:
  QMutex _mutex;
  std::set<QString> _jobs;
};

class ProcessThread : public QThread
{
public:
  ProcessThread(double waitTime, QMutex* outMutex, JobQueue* asyncJobs, JobQueue* syncJobs = NULL);

  void run();

  int getFailures();

private:
  QProcess* createProcess();

  void processJobs(JobQueue* queue);

  double _waitTime;
  QMutex* _outMutex;
  JobQueue* _asyncJobs;
  JobQueue* _syncJobs;
  int _failures;
  QProcessPtr _proc;
};

typedef boost::shared_ptr<ProcessThread> ProcessThreadPtr;

class ProcessPool
{
public:
  explicit ProcessPool(int nproc, double waitTime);

  virtual ~ProcessPool();

  void addJob(const QString& test, bool async = true);

  void startProcessing();

  int getFailures();

  void wait();
private:
  std::vector<ProcessThreadPtr> _threads;
  JobQueue _syncJobs;
  JobQueue _asyncJobs;
  QMutex _mutex;
  int _failed;
  int _finished;
};

#endif  //  PROCESS_POOL_H
