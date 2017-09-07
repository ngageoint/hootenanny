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
#include <map>
#include <queue>

//  Boost
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#define HOOT_TEST_FINISHED "HOOT_TEST_FINISHED"

typedef boost::shared_ptr<QProcess> QProcessPtr;

class ProcessThread
{
public:
  ProcessThread(QMutex* mutex, std::queue<QString>* jobs);

  void start();

  void join();

  void quit();

  int getFailures();

private:
  void doWork();

  QMutex* _mutex;
  std::queue<QString>* _jobs;
  int _failures;
  boost::shared_ptr<boost::thread> _thread;
};

typedef boost::shared_ptr<ProcessThread> ProcessThreadPtr;

class ProcessPool
{
public:
  explicit ProcessPool(int nproc);

  virtual ~ProcessPool();

  void addJob(const QString& test);

  void startProcessing();

  int getFailures();

  void wait();
private:
  std::vector<ProcessThreadPtr> _threads;
  std::queue<QString> _queue;
  QMutex _mutex;
  QWaitCondition _wait;
  int _failed;
  int _finished;
};

#endif  //  PROCESS_POOL_H
