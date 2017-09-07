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

using namespace std;

ProcessThread::ProcessThread(QMutex* mutex, std::queue<QString>* jobs)
  : _mutex(mutex), _jobs(jobs), _failures(0), _thread()
{
}

void ProcessThread::start()
{
  _thread.reset(new boost::thread(&ProcessThread::doWork, this));
}

void ProcessThread::join()
{
  if (_thread)
    _thread->join();
}

void ProcessThread::quit()
{
  if (_thread)
    _thread->interrupt();
}

int ProcessThread::getFailures()
{
  return _failures;
}

void ProcessThread::doWork()
{
  QProcessPtr proc(new QProcess());
  proc->setProcessChannelMode(QProcess::MergedChannels);
  proc->start("HootTest --listen");

  bool working = true;
  while (working)
  {
    _mutex->lock();
    if (!_jobs->empty())
    {
      QString test = _jobs->front();
      _jobs->pop();
      _mutex->unlock();
      proc->write(test.toAscii());
      proc->write("\n");
      //  Read all of the output
      QString output;
      proc->waitForReadyRead();
      QString line = QString(proc->readLine());
      line = line.trimmed();
      while (line != HOOT_TEST_FINISHED)
      {
        if (proc->state() != QProcess::Running)
        {
          ++_failures;
          working = false;
          continue;
        }
        if (line != ".")
          ++_failures;
        output.append(line);
        line = QString(proc->readLine());
        line = line.trimmed();
      }
      _mutex->lock();
      cout << output.toStdString();
      cout.flush();
      _mutex->unlock();
    }
    else
    {
      proc->write(QString("%1\n").arg(HOOT_TEST_FINISHED).toAscii());
      proc->waitForFinished();
      working = false;
    }
  }
}

ProcessPool::ProcessPool(int nproc)
  : _failed(0), _finished(0)
{
  for (int i = 0; i < nproc; ++i)
  {
    ProcessThreadPtr thread(new ProcessThread(&_mutex, &_queue));
    _threads.push_back(thread);
  }
}

ProcessPool::~ProcessPool()
{
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
  {
    _threads[i]->quit();
    _threads[i]->join();
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
    _threads[i]->join();
}

void ProcessPool::addJob(const QString& test)
{
  _mutex.lock();
  _queue.push(test);
  _mutex.unlock();
}

int ProcessPool::getFailures()
{
  int failures = 0;
  for (vector<ProcessThreadPtr>::size_type i = 0; i < _threads.size(); ++i)
    failures += _threads[i]->getFailures();
  return failures;
}
