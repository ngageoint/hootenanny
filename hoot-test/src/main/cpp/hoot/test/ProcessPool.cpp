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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "ProcessPool.h"

// Hoot
#include <hoot/core/util/StringUtils.h>

#include <hoot/js/v8Engine.h>

// std
#include <iostream>

using namespace std;

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
  QString job;
  //  Don't try to erase a job from an empty queue.
  if (!_jobs.empty())
  {
    job = *_jobs.begin();
    _jobs.erase(_jobs.begin());
  }
  _mutex.unlock();
  return job;
}

void JobQueue::push(const QString& job)
{
  _mutex.lock();
  _jobs.insert(job);
  _mutex.unlock();
}

QString JobQueue::toString() const
{
  return
    "Name: " + _name + "; Size: " + QString::number(_jobs.size()) + "; Jobs: " +
    hoot::StringUtils::setToString(_jobs);
}

ProcessThread::ProcessThread(int threadId, int maxThreads, bool showTestName, bool suppressFailureDetail, bool printDiff,
                             bool disableFailureRetries, double waitTime, QMutex* outMutex, JobQueue* parallelJobs,
                             JobQueue* casesJobs, JobQueue* serialJobs, JobQueue* serialCasesJobs)
  : _threadId(threadId),
    _maxThreads(maxThreads),
    _showTestName(showTestName),
    _suppressFailureDetail(suppressFailureDetail),
    _printDiff(printDiff),
    _disableFailureRetries(disableFailureRetries),
    _waitTime(waitTime),
    _outMutex(outMutex),
    _parallelJobs(parallelJobs),
    _casesJobs(casesJobs),
    _serialJobs(serialJobs),
    _serialCasesJobs(serialCasesJobs),
    _failures(0),
    _proc()
{
}

void ProcessThread::resetProcess()
{
  //  Kill the process
  _proc->write(QString("%1\n").arg(HOOT_TEST_FINISHED).toLatin1());
  _proc->waitForFinished();
  //  Start a new process
  _proc.reset(createProcess());
}

QProcess* ProcessThread::createProcess()
{
  QProcess* proc = new QProcess();
  proc->setProcessChannelMode(QProcess::MergedChannels);
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  proc->setProcessEnvironment(env);
  QString names = (_showTestName ? "--names" : "");
  QString suppressFailureDetail = (_suppressFailureDetail ? "--suppress-failure-detail" : "");
  QString diff = (_printDiff ? "--diff" : "");

  proc->start(
    QString("HootTest %1 %2 %3 --listen %4")
      .arg(names).arg(suppressFailureDetail).arg(diff).arg((int)_waitTime));
  return proc;
}

void ProcessThread::run()
{
  _proc.reset(createProcess());
  if (_serialJobs != nullptr)
  {
    processJobs(_serialJobs);
    processJobs(_serialCasesJobs);
    //  Reset the process between queues
    resetProcess();
  }
  processJobs(_parallelJobs);
  processJobs(_casesJobs);

  _proc->write(QString("%1\n").arg(HOOT_TEST_FINISHED).toLatin1());
  _proc->waitForFinished();
}

void ProcessThread::processJobs(JobQueue* queue)
{
  int restart_count = 0;
  const int MAX_RESTART = 3;
  const int READ_TIMEOUT = 500;
  bool working = true;
  while (working)
  {
    if (!queue->empty())
    {
      QString test = queue->pop();
      //  Empty strings can be ignored
      if (test.isEmpty())
        continue;
      _proc->write(QString("%1\n").arg(test).toLatin1());
      //  Read all of the output
      QString output;
      _proc->waitForReadyRead(READ_TIMEOUT);
      QString line = QString(_proc->readLine()).trimmed();
      while (line != HOOT_TEST_FINISHED)
      {
        if (_proc->state() == QProcess::NotRunning && !_disableFailureRetries)
        {
          //  Put this job that failed back on the queue
          queue->push(test);
          ++restart_count;
          //  Restart the process if there was a process failure
          if (restart_count < MAX_RESTART)
          {
            _proc.reset(createProcess());
            output.clear();
          }
          else
          {
            working = false;
            _outMutex->lock();
            cout << test.toStdString() << " failed to execute, exiting thread." << endl;
            _outMutex->unlock();
          }
          break;
        }
        else if (line == "")
        {
          if (output != "" && output != "." && !output.endsWith("\n"))
            output.append("\n");
        }
        else if ((line.contains(" ERROR ") || line.contains("Failure: ")) &&
                 !_disableFailureRetries)
        {
          ++_failures;
          line.append("\n");
          //  If the process is still running, then wait for it to finish
          if (_proc->state() == QProcess::Running)
          {
            QString next;
            while (next != HOOT_TEST_FINISHED)
            {
              if (_proc->bytesAvailable() < 1)
                _proc->waitForReadyRead(READ_TIMEOUT);
              next = QString(_proc->readLine()).trimmed();
              if (next != HOOT_TEST_FINISHED && !next.isEmpty())
                line.append(next.append("\n"));
              //  If the process ends here before writing HOOT_TEST_FINISH, break out of the loop
              if (_proc->state() != QProcess::Running)
                break;
            }
          }
          output.append(line);
          //  Reset the process
          resetProcess();
          line = HOOT_TEST_FINISHED;
          continue;
        }
        else if (line != ".")
          line.append("\n");
        output.append(line);
        if (_proc->bytesAvailable() < 1)
          _proc->waitForReadyRead(READ_TIMEOUT);
        line = QString(_proc->readLine()).trimmed();
      }
      output = output.replace("\n\n\n", "\n").replace("\n\n", "\n");
      if (_showTestName)
        output = QString("(%1) %2").arg(_threadId, (_maxThreads < 10) ? 1 : 2).arg(output);
      _outMutex->lock();
      cout << output.toStdString();
      cout.flush();
      _outMutex->unlock();
    }
    else
      working = false;
  }
}

ProcessPool::ProcessPool(int nproc, double waitTime, bool showTestName, bool suppressFailureDetail,
                         bool printDiff, bool disableFailureRetries)
  : _failed(0)
{
  _parallelJobs.setName("parallel");
  _serialJobs.setName("serial");
  _caseJobs.setName("case");
  _serialCaseJobs.setName("serial_case");

  for (int i = 0; i < nproc; ++i)
  {
    //  First process gets the serial jobs
    JobQueue* serial = (i == 0) ? &_serialJobs : nullptr;
    JobQueue* serialCases = (i == 0) ? &_serialCaseJobs : nullptr;
    ProcessThreadPtr thread =
      std::make_shared<ProcessThread>(
        i, nproc, showTestName, suppressFailureDetail, printDiff, disableFailureRetries, waitTime,
        &_mutex, &_parallelJobs, &_caseJobs, serial, serialCases);
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

QString ProcessPool::jobQueueToString(const QString& name) const
{
  if (name == "parallel")
    return _parallelJobs.toString();
  else if (name == "serial")
    return _serialJobs.toString();
  else if (name == "case")
    return _caseJobs.toString();
  else if (name == "serial_case")
    return _serialCaseJobs.toString();
  else
    return "";
}

void ProcessPool::startProcessing()
{
  for (size_t i = 0; i < _threads.size(); ++i)
    _threads[i]->start();
}

void ProcessPool::wait()
{
  for (size_t i = 0; i < _threads.size(); ++i)
    _threads[i]->wait();
}

void ProcessPool::addJob(const QString& test, JobType job_type)
{
  if (job_type == ParallelJob && !_serialJobs.contains(test) && !_caseJobs.contains(test) && !_serialCaseJobs.contains(test))
    _parallelJobs.push(test);
  else if (job_type == (SerialJob | ConflateJob))
    _serialCaseJobs.push(test);
  else if (job_type == SerialJob)
    _serialJobs.push(test);
  else if (job_type == ConflateJob)
    _caseJobs.push(test);
}

int ProcessPool::getFailures()
{
  int failures = 0;
  for (size_t i = 0; i < _threads.size(); ++i)
    failures += _threads[i]->getFailures();
  return failures;
}
