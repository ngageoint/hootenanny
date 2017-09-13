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
  : _mutex(mutex), _jobs(jobs), _failures(0)
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
  proc->start("HootTest --listen");
  return proc;
}

void ProcessThread::run()
{
  QProcessPtr proc(createProcess());

  int restart_count = 0;
  const int MAX_RESTART = 3;
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
      QString line2 = QString(proc->readLine());
      QString line = line2.trimmed();
      while (line != HOOT_TEST_FINISHED)
      {
        if (proc->state() == QProcess::NotRunning)
        {
          //  Put this job that failed back on the queue
          _mutex->lock();
          _jobs->push(test);
          _mutex->unlock();
          ++restart_count;
          //  Restart the process if there was a process failure
          if (restart_count < MAX_RESTART)
            proc.reset(createProcess());
          else
            working = false;
          break;
        }
        else if (line == "")
        {
/*
          //  Kill the process
          proc->write(HOOT_TEST_FINISHED);
          proc->write("\n");
          proc->waitForFinished();
          //  Start a new process
          proc.reset(createProcess());
          proc->write(test.toAscii());
          proc->write("\n");
/*
          ++_failures;
          QProcess::ProcessError error = proc->error();
          QString errorMessage = proc->errorString();
          cout << error << ": " << errorMessage.toStdString() << endl;
*/
        }
        else if (line.contains(" ERROR "))
        {
          ++_failures;
          //  Kill the process
          proc->write(HOOT_TEST_FINISHED);
          proc->write("\n");
          proc->waitForFinished();
          //  Start a new process
          proc.reset(createProcess());
          line = HOOT_TEST_FINISHED;
//          continue;
        }
        output.append(line);
        line2 = QString(proc->readLine());
        line = line2.trimmed();
      }
      _mutex->lock();
      cout << output.toStdString();
      cout.flush();
      _mutex->unlock();
    }
    else
    {
      _mutex->unlock();
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
