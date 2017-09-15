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
#include "ProcessPool.h"

#include <iostream>

using namespace std;


JobQueue::JobQueue()
{
}

bool JobQueue::empty()
{
  _mutex.lock();
  bool e = _jobs.empty();
  _mutex.unlock();
  return e;
}

bool JobQueue::contains(const QString& job)
{
  _mutex.lock();
  bool c = _jobs.find(job) != _jobs.end();
  _mutex.unlock();
  return c;
}

QString JobQueue::pop()
{
  _mutex.lock();
  QString job = *_jobs.begin();
  _jobs.erase(_jobs.begin());
  _mutex.unlock();
  return job;
}

void JobQueue::push(const QString& job)
{
  _mutex.lock();
  _jobs.insert(job);
  _mutex.unlock();
}

ProcessThread::ProcessThread(double waitTime, QMutex* outMutex, JobQueue* asyncJobs, JobQueue* syncJobs)
  : _waitTime(waitTime), _outMutex(outMutex), _asyncJobs(asyncJobs), _syncJobs(syncJobs), _failures(0), _proc(createProcess())
{
}

int ProcessThread::getFailures()
{
  return _failures;
}

QProcess* ProcessThread::createProcess()
{
  QProcess* proc = new QProcess();
  proc->setProcessChannelMode(QProcess::MergedChannels);
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  proc->setProcessEnvironment(env);
  proc->start(QString("HootTest --listen %1").arg((int)_waitTime));
  return proc;
}

void ProcessThread::run()
{
  if (_syncJobs != NULL)
    processJobs(_syncJobs);
  processJobs(_asyncJobs);

  _proc->write(QString("%1\n").arg(HOOT_TEST_FINISHED).toAscii());
  _proc->waitForFinished();
}

void ProcessThread::processJobs(JobQueue* queue)
{
  int restart_count = 0;
  const int MAX_RESTART = 3;
  bool working = true;
  while (working)
  {
    if (!queue->empty())
    {
      QString test = queue->pop();
      _proc->write(test.toAscii());
      _proc->write("\n");
      //  Read all of the output
      QString output;
      _proc->waitForReadyRead();
      QString line = QString(_proc->readLine()).trimmed();
      while (line != HOOT_TEST_FINISHED)
      {
        if (_proc->state() == QProcess::NotRunning)
        {
          //  Put this job that failed back on the queue
          queue->push(test);
          ++restart_count;
          //  Restart the process if there was a process failure
          if (restart_count < MAX_RESTART)
            _proc.reset(createProcess());
          else
            working = false;
          break;
        }
        else if (line == "")
        {
          if (output != "." && !output.endsWith("\n"))
            output.append("\n");
        }
        else if (line.contains(" ERROR "))
        {
          ++_failures;
          output.append(line);
          output.append("\n");
          //  Kill the process
          _proc->write(HOOT_TEST_FINISHED);
          _proc->write("\n");
          _proc->waitForFinished();
          //  Start a new process
          _proc.reset(createProcess());
          line = HOOT_TEST_FINISHED;
          continue;
        }
        else if (line != ".")
          line.append("\n");
        output.append(line);
        if (_proc->bytesAvailable() < 1)
          _proc->waitForReadyRead();
        line = QString(_proc->readLine()).trimmed();
      }
      output = output.replace("\n\n\n", "\n").replace("\n\n", "\n");
      _outMutex->lock();
      cout << output.toStdString();
      cout.flush();
      _outMutex->unlock();
    }
    else
      working = false;
  }
}

ProcessPool::ProcessPool(int nproc, double waitTime)
  : _failed(0), _finished(0)
{
  for (int i = 0; i < nproc; ++i)
  {
    JobQueue* sync = (i == 0) ? &_syncJobs : NULL;
    ProcessThreadPtr thread(new ProcessThread(waitTime, &_mutex, &_asyncJobs, sync));
    _threads.push_back(thread);
  }
}

ProcessPool::~ProcessPool()
{
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
  {
    _threads[i]->quit();
    _threads[i]->wait();
  }
}

void ProcessPool::startProcessing()
{
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
    _threads[i]->start();
}

void ProcessPool::wait()
{
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
    _threads[i]->wait();
}

void ProcessPool::addJob(const QString& test, bool async)
{
  if (async && !_syncJobs.contains(test))
    _asyncJobs.push(test);
  else if (!async)
    _syncJobs.push(test);
}

int ProcessPool::getFailures()
{
  int failures = 0;
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
    failures += _threads[i]->getFailures();
  return failures;
}
