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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ScriptTest.h"

// hoot
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QThreadPool>
#include <QElapsedTimer>

namespace hoot
{

ScriptTest::ScriptTest(QString script, bool printDiff) : CppUnit::TestCase(script.toStdString()),
  _printDiff(printDiff),
  _script(script)
{

}

QString ScriptTest::_readFile(QString path)
{
  QFile fp(path);

  if (!fp.open(QFile::ReadOnly))
  {
    throw Exception("Error opening file for reading: " + path);
  }

  return QString::fromUtf8(fp.readAll());
}

void ScriptTest::_removeFile(QString path)
{
  QFile f(path);

  if (f.exists())
  {
    if (f.remove() == false)
    {
      throw Exception("Error removing: " + path);
    }
  }
}

QString ScriptTest::_removeIgnoredSubstrings(QString output) const
{
  QStringList outLines;
  QStringList inLines = output.split("\n");

  // takes the form: "09:34:21.635 WARN  src/main/cpp/hoot/core/io/OsmPbfReader.cpp(455) - "
  QRegExp reWarn("[0-9:\\.]+ WARN  .*\\( *-?[0-9]+\\) ");
  QRegExp reError("[0-9:\\.]+ ERROR .*\\( *-?[0-9]+\\) ");

  for (int i = 0; i < inLines.size(); i++)
  {
    bool keep = true;
    if (inLines[i].contains(" INFO ") || inLines[i].contains(" DEBUG ") ||
        inLines[i].contains(" elapsed: ") )
    {
      keep = false;
    }

    if (keep)
    {
      inLines[i].replace(reWarn, "WARN - ");
      inLines[i].replace(reError, "ERROR - ");
      outLines.append(inLines[i]);
    }
  }

  return outLines.join("\n");
}

void ScriptTest::runTest()
{
  // before we start remove any temporary files that we generated last time.
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

  if (QFile(_script + ".stdout").exists() == false || QFile(_script + ".stderr").exists() == false)
  {
    LOG_WARN("STDOUT or STDERR doesn't exist for " + _script +
             "\n*************************\n"
             "  This can be resolved by reviewing the output for correctness and then \n"
             "  creating a new baseline. E.g.\n"
             "  verify: \n"
             "    less " + _script + ".stdout.first\n"
             "    less " + _script + ".stderr.first\n"
             "  ### NOTE: If the test is comparing against a known good file (e.g. .osm\n"
             "  ### then it may be better to update that file. You'll have to look at\n"
             "  ### the source files to be sure.\n"
             "  Make a new baseline:\n"
             "    mv " + _script + ".stdout.first " + _script + ".stdout\n"
             "    mv " + _script + ".stderr.first " + _script + ".stderr\n"
             "*************************\n"
             );

    _baseStderr = "<invalid/>";
    _baseStdout = "<invalid/>";
    _writeFile(_script + ".stdout.first", _stdout);
    _writeFile(_script + ".stderr.first", _stderr);
    _writeFile(_script + ".stdout.first.stripped", _removeIgnoredSubstrings(_stdout));
    _writeFile(_script + ".stderr.first.stripped", _removeIgnoredSubstrings(_stderr));
    CPPUNIT_ASSERT_MESSAGE(QString("STDOUT or STDERR does not exist").toStdString(), false);
  }

  _baseStderr = _readFile(_script + ".stderr");
  _baseStdout = _readFile(_script + ".stdout");

  bool failed = false;

  if (_removeIgnoredSubstrings(_baseStdout) != _removeIgnoredSubstrings(_stdout))
  {
    _writeFile(_script + ".stdout.failed", _stdout);
    _writeFile(_script + ".stdout.failed.stripped", _removeIgnoredSubstrings(_stdout));
    _writeFile(_script + ".stdout.stripped", _removeIgnoredSubstrings(_baseStdout));

    if (_printDiff)
    {
      LOG_WARN("STDOUT does not match for:\n" +
        _script + ".stdout" + " " + _script + ".stdout.failed");
      _runDiff(_script + ".stdout.stripped", _script + ".stdout.failed.stripped");
    }
    else
    {
      LOG_WARN("STDOUT does not match for:\n" +
               _script + ".stdout.failed" + " " + _script + ".stdout\n"
               "\n*************************\n"
               "  This can be resolved by reviewing the output for correctness and then \n"
               "  creating a new baseline. E.g.\n"
               "  verify: \n"
               "    diff " + _script + ".stdout.stripped " + _script + ".stdout.failed.stripped\n"
               "  ### NOTE: If the test is comparing against a known good file (e.g. .osm\n"
               "  ### then it may be better to update that file. You'll have to look at\n"
               "  ### the source files to be sure.\n"
               "  Make a new baseline:\n"
               "    mv " + _script + ".stdout.failed " + _script + ".stdout\n"
               "*************************\n"
               );
    }

    failed = true;
  }

  if (_removeIgnoredSubstrings(_baseStderr) != _removeIgnoredSubstrings(_stderr))
  {
    _writeFile(_script + ".stderr.failed", _stderr);
    _writeFile(_script + ".stderr.failed.stripped", _removeIgnoredSubstrings(_stderr));
    _writeFile(_script + ".stderr.stripped", _removeIgnoredSubstrings(_baseStderr));

    if (_printDiff)
    {
      LOG_WARN("STDERR does not match for:\n" +
        _script + ".stderr" + " " + _script + ".stderr.failed");
      _runDiff(_script + ".stderr.stripped", _script + ".stderr.failed.stripped");
    }
    else
    {
      LOG_WARN("STDERR does not match for:\n" +
               _script + ".stderr.failed" + " " + _script + ".stderr\n"
               "\n*************************\n"
               "  This can be resolved by reviewing the output for correctness and then \n"
               "  creating a new baseline. E.g.\n"
               "  verify: \n"
               "    diff " + _script + ".stderr.stripped " + _script + ".stderr.failed.stripped\n"
               "  ### NOTE: If the test is comparing against a known good file (e.g. .osm\n"
               "  ### then it may be better to update that file. You'll have to look at\n"
               "  ### the source files to be sure.\n"
               "  Make a new baseline:\n"
               "    mv " + _script + ".stderr.failed " + _script + ".stderr\n"
               "*************************\n"
               );
    }

    failed = true;
  }

  if (failed)
  {
    CPPUNIT_ASSERT_MESSAGE(QString("STDOUT or STDERR does not match").toStdString(), false);
  }
}

void ScriptTest::_runDiff(QString file1, QString file2)
{
  QProcess p;
  p.start("diff", QStringList() << file1 << file2, QProcess::ReadOnly);

  while (p.waitForStarted(500) == false)
  {
    LOG_WARN("Waiting for process to start: " + _script);
  }

  const int scriptTestTimeOutSeconds = ConfigOptions(conf()).getScriptTestMaxExecTime();
  bool scriptTimeOutSpecified = false;
  int waitToFinishTime = 30000;
  if (scriptTestTimeOutSeconds != -1)
  {
    waitToFinishTime = scriptTestTimeOutSeconds * 1000;
    scriptTimeOutSpecified = true;
  }

  bool first = true;
  QElapsedTimer timer;
  timer.start();
  while (p.waitForFinished(waitToFinishTime) == false)
  {
    if (first)
    {
      // throw an endl in there so the dots in the test list don't look funny.
      cout << endl;
      LOG_WARN("Waiting for process to finish: " + _script);
      first = false;
    }

    //if the process hangs this will allows us to get out
    const qint64 timeElapsedSeconds = timer.elapsed() / 1000;
    if (scriptTimeOutSpecified && timeElapsedSeconds >= scriptTestTimeOutSeconds)
    {
      LOG_WARN("Forcefully ending test script test after " << timeElapsedSeconds << " seconds.");
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
  QProcess p;
  p.start(_script, QProcess::ReadOnly);

  while (p.waitForStarted(500) == false)
  {
    LOG_WARN("Waiting for process to start: " + _script);
  }

  const int scriptTestTimeOutSeconds = ConfigOptions(conf()).getScriptTestMaxExecTime();
  bool scriptTimeOutSpecified = false;
  int waitToFinishTime = 30000;
  if (scriptTestTimeOutSeconds != -1)
  {
    waitToFinishTime = scriptTestTimeOutSeconds * 1000;
    scriptTimeOutSpecified = true;
  }

  bool first = true;
  QElapsedTimer timer;
  timer.start();
  while (p.waitForFinished(waitToFinishTime) == false)
  {
    if (first && !scriptTimeOutSpecified)
    {
      // throw an endl in there so the dots in the test list don't look funny.
      cout << endl;
      LOG_WARN("Waiting for process to finish: " + _script);
      first = false;
    }

    //if the process hangs this will allows us to get out
    const qint64 timeElapsedSeconds = timer.elapsed() / 1000;
    if (scriptTimeOutSpecified && timeElapsedSeconds >= scriptTestTimeOutSeconds)
    {
      LOG_WARN("Forcefully ending test script test after " << timeElapsedSeconds << " seconds.");
      break;
    }
  }

  _stdout = QString::fromUtf8(p.readAllStandardOutput());
  _stderr = QString::fromUtf8(p.readAllStandardError());
}

void ScriptTest::_writeFile(QString path, QString content)
{
  QFile fp(path);

  if (fp.exists())
  {
    if (fp.remove() == false)
    {
      throw Exception("Error removing file: " + path);
    }
  }

  if (fp.open(QFile::WriteOnly) == false)
  {
    throw Exception("Error opening file for writing: " + path);
  }

  fp.write(content.toUtf8());
}

}
