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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

#include "ScriptTest.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/LogColor.h>

#include <hoot/josm/validation/MapValidator.h>

// Qt
#include <QCoreApplication>
#include <QDir>
#include <QElapsedTimer>
#include <QProcess>
#include <QThreadPool>

using namespace std;

namespace hoot
{

QRegularExpression ScriptTest::_regWarn("[0-9:\\.]+ WARN  .*\\( *-?[0-9]+\\) ");
QRegularExpression ScriptTest::_regError("[0-9:\\.]+ ERROR .*\\( *-?[0-9]+\\) ");


ScriptTest::ScriptTest(const QString& script, bool printDiff, bool suppressFailureDetail, int waitToFinishTime)
  : CppUnit::TestCase(script.toStdString()),
    _printDiff(printDiff),
    _suppressFailureDetail(suppressFailureDetail),
    _script(script),
    _waitToFinishTime(waitToFinishTime),
    _scriptTestTimeOutSeconds(ConfigOptions(conf()).getTestScriptMaxExecTime())
{
}

QString ScriptTest::_readFile(const QString& path)
{
  QFile fp(path);

  if (!fp.open(QFile::ReadOnly))
    throw HootException("Error opening file for reading: " + path);

  return QString::fromUtf8(fp.readAll());
}

void ScriptTest::_removeFile(const QString& path)
{
  QFile f(path);

  if (f.exists() && f.remove() == false)
    throw HootException("Error removing: " + path);
}

QString ScriptTest::_removeIgnoredSubstrings(const QString& input) const
{
  QStringList outLines;
  QStringList inLines = input.split("\n");

  for (auto line : qAsConst(inLines))
  {
    if (!line.contains(" STATUS ") && !line.contains(" INFO ") &&
        !line.contains(" DEBUG ") && !line.contains(" elapsed: ") &&
        !line.contains("Time (sec)"))
    {
      line.replace(_regWarn, "WARN - ");
      line.replace(_regError, "ERROR - ");
      outLines.append(line);
    }
  }
  return outLines.join("\n");
}

QString ScriptTest::_removeLogColoring(const QString& input) const
{
  QString output = input;
  output.replace(LogColor::ColorRegex, "");
  return output;
}

void ScriptTest::runTest()
{
  // Before we start, remove any temporary files that we generated last time.
  _removeFile(_script + ".stdout.failed");
  _removeFile(_script + ".stderr.failed");
  _removeFile(_script + ".stdout.failed.stripped");
  _removeFile(_script + ".stderr.failed.stripped");
  _removeFile(_script + ".stdout.first");
  _removeFile(_script + ".stderr.first");
  _removeFile(_script + ".stdout.first.stripped");
  _removeFile(_script + ".stderr.first.stripped");
  _removeFile(_script + ".stdout.stripped");
  _removeFile(_script + ".stderr.stripped");

  _runProcess();

  //  Replace colored text in the standard out
  _stdout.replace(LogColor::ColorRegex, "");

  if (QFile(_script + ".stdout").exists() == false || QFile(_script + ".stderr").exists() == false)
  {
    LOG_ERROR("STDOUT or STDERR doesn't exist for " + _script +
              "\n*************************\n"
              "  This can be resolved by reviewing the output for correctness and then creating a new baseline.\n"
              "  Verify the changes in the output are satisfactory: \n"
              "    less " + _script + ".stdout.first\n"
              "    less " + _script + ".stderr.first\n"
              "  Make a new baseline for the output:\n"
              "    mv " + _script + ".stdout.first " + _script + ".stdout\n"
              "    mv " + _script + ".stderr.first " + _script + ".stderr\n"
              "*************************");

    _baseStderr = "<invalid/>";
    _baseStdout = "<invalid/>";
    _writeFile(_script + ".stdout.first", _stdout);
    _writeFile(_script + ".stderr.first", _stderr);
    //  No need to strip anything from "<invalid/>"
    _writeFile(_script + ".stdout.first.stripped", _stdout);
    _writeFile(_script + ".stderr.first.stripped", _stderr);
    CPPUNIT_ASSERT_MESSAGE("STDOUT or STDERR does not exist", false);
  }

  //  Load standard out and error files
  _baseStderr = _readFile(_script + ".stderr");
  _baseStdout = _readFile(_script + ".stdout");

  //  Strip out non-important lines from log for *.stripped files
  QString base_stdout = _removeIgnoredSubstrings(_baseStdout);
  QString stdout = _removeIgnoredSubstrings(_stdout);
  QString base_stderr = _removeIgnoredSubstrings(_baseStderr);
  QString stderr = _removeIgnoredSubstrings(_stderr);

  bool failed = false;

  if (base_stdout != stdout)
  {
    _writeFile(_script + ".stdout.failed", _stdout);
    _writeFile(_script + ".stdout.failed.stripped", stdout);
    _writeFile(_script + ".stdout.stripped", base_stdout);

    if (!_suppressFailureDetail)
    {
      QString msg = "STDOUT does not match for:\n" + _script + ".stdout" + " " + _script + ".stdout.failed";
      if (_printDiff)
      {
        LOG_ERROR(msg);
        _runDiff(_script + ".stdout.stripped", _script + ".stdout.failed.stripped");
      }
      else
      {
        msg += "\n"
               "\n*************************\n"
               "  This can be resolved by reviewing the output for correctness and then creating a new baseline.\n"
               "  Verify the changes in the output are satisfactory: \n"
               "    diff " + _script + ".stdout.stripped " + _script + ".stdout.failed.stripped\n"
               "  Make a new baseline for the output:\n"
               "    mv " + _script + ".stdout.failed " + _script + ".stdout\n"
               "*************************";
        LOG_ERROR(msg);
      }
    }

    failed = true;
  }

  if (base_stderr != stderr)
  {
    _writeFile(_script + ".stderr.failed", _stderr);
    _writeFile(_script + ".stderr.failed.stripped", stderr);
    _writeFile(_script + ".stderr.stripped", base_stderr);

    if (!_suppressFailureDetail)
    {
      QString msg = "STDERR does not match for:\n" + _script + ".stderr" + " " + _script + ".stderr.failed";
      if (_printDiff)
      {
        LOG_ERROR(msg);
        _runDiff(_script + ".stderr.stripped", _script + ".stderr.failed.stripped");
      }
      else
      {
        msg += "\n"
               "\n*************************\n"
               "  This can be resolved by reviewing the output for correctness and then creating a new baseline.\n"
               "  Verify the changes in the output are satisfactory: \n"
               "    diff " + _script + ".stderr.stripped " + _script + ".stderr.failed.stripped\n"
               "  Make a new baseline for the output:\n"
               "    mv " + _script + ".stderr.failed " + _script + ".stderr\n"
               "*************************";
        LOG_ERROR(msg);
      }
    }
    failed = true;
  }

  if (failed)
    CPPUNIT_ASSERT_MESSAGE("STDOUT or STDERR does not match", false);
}

void ScriptTest::_runDiff(const QString& file1, const QString& file2)
{
  QProcess p;
  p.setProcessChannelMode(QProcess::SeparateChannels);
  p.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
  p.start("diff", QStringList() << file1 << file2, QProcess::ReadOnly);

  while (p.waitForStarted(500) == false)
  {
    LOG_WARN("Waiting for diff process to start for: " + _script);
  }

  bool scriptTimeOutSpecified = false;
  if (_scriptTestTimeOutSeconds != -1)
  {
    _waitToFinishTime = _scriptTestTimeOutSeconds * 1000;
    scriptTimeOutSpecified = true;
  }

  bool first = true;
  QElapsedTimer timer;
  timer.start();
  while (p.waitForFinished(_waitToFinishTime) == false)
  {
    if (first)
    {
      // Throw an endl in there so the dots in the test list don't look funny.
      cout << endl;
      LOG_WARN("Waiting for diff process to finish for: " + _script);
      first = false;
    }

    // If the process hangs, this will allow us to get out.
    const qint64 timeElapsedSeconds = timer.elapsed() / 1000;
    if (scriptTimeOutSpecified && timeElapsedSeconds >= _scriptTestTimeOutSeconds)
    {
      //  Check the state of the process
      if (p.state() == QProcess::NotRunning)
        break;
      if (p.processId() == 0)
        break;
      LOG_ERROR("Forcefully ending diff command for: " << _script << " after " << timeElapsedSeconds << " seconds.");
      //  Terminate the process
      p.terminate();
      break;
    }
  }

  _stdout = QString::fromUtf8(p.readAllStandardOutput());
  _stderr = QString::fromUtf8(p.readAllStandardError());
  cout << _stdout;
  cerr << _stderr;
}

void ScriptTest::_runProcess()
{
  // It would be nice if we could specify Testing.conf here to avoid having to specify it in every
  // test file (#3823).
  QProcess p;
  p.setProcessChannelMode(QProcess::SeparateChannels);
  p.setProcessEnvironment(QProcessEnvironment::systemEnvironment());
  p.start(_script, QProcess::ReadOnly);

  while (p.waitForStarted(500) == false)
  {
    LOG_WARN("Waiting for process to start: " + _script);
  }

  bool scriptTimeOutSpecified = false;
  if (_scriptTestTimeOutSeconds != -1)
  {
    _waitToFinishTime = _scriptTestTimeOutSeconds * 1000;
    scriptTimeOutSpecified = true;
  }

  bool first = true;
  QElapsedTimer timer;
  timer.start();
  while (p.waitForFinished(_waitToFinishTime) == false)
  {
    if (first && !scriptTimeOutSpecified)
    {
      // Throw an endl in there so the dots in the test list don't look funny.
      cout << endl;
      LOG_WARN("Waiting for process to finish: " + _script);
      first = false;
    }

    // If the process hangs, this will allows us to get out.
    const qint64 timeElapsedSeconds = timer.elapsed() / 1000;
    if (scriptTimeOutSpecified && timeElapsedSeconds >= _scriptTestTimeOutSeconds)
    {
      //  Check the state of the process
      if (p.state() == QProcess::NotRunning)
        break;
      if (p.processId() == 0)
        break;
      LOG_ERROR("Forcefully ending test script test after " << timeElapsedSeconds << " seconds.");
      //  Terminate the process
      p.terminate();
      break;
    }
  }

  _stdout = QString::fromUtf8(p.readAllStandardOutput());
  _stderr = QString::fromUtf8(p.readAllStandardError());
}

void ScriptTest::_writeFile(const QString& path, const QString& content)
{
  QFile fp(path);

  if (fp.exists() && fp.remove() == false)
    throw HootException("Error removing file: " + path);

  if (fp.open(QFile::WriteOnly) == false)
    throw HootException("Error opening file for writing: " + path);

  fp.write(content.toUtf8());
}

}
