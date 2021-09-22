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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "ScriptTest.h"

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/LogColor.h>
#include <hoot/core/util/StringUtils.h>

#include <hoot/test/validation/TestOutputValidator.h>

// Qt
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QThreadPool>
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

ScriptTest::ScriptTest(
  const QString& script, bool printDiff, bool suppressFailureDetail, int waitToFinishTime) :
CppUnit::TestCase(script.toStdString()),
_printDiff(printDiff),
_suppressFailureDetail(suppressFailureDetail),
_script(script),
_waitToFinishTime(waitToFinishTime)
{
}

QString ScriptTest::_readFile(const QString& path)
{
  QFile fp(path);

  if (!fp.open(QFile::ReadOnly))
  {
    throw HootException("Error opening file for reading: " + path);
  }

  return QString::fromUtf8(fp.readAll());
}

void ScriptTest::_removeFile(const QString& path)
{
  QFile f(path);

  if (f.exists())
  {
    if (f.remove() == false)
    {
      throw HootException("Error removing: " + path);
    }
  }
}

QString ScriptTest::_removeIgnoredSubstrings(const QString& input) const
{
  QStringList outLines;
  QStringList inLines = input.split("\n");

  // takes the form: "09:34:21.635 WARN  src/main/cpp/hoot/core/io/OsmPbfReader.cpp(455) - "
  QRegExp reWarn("[0-9:\\.]+ WARN  .*\\( *-?[0-9]+\\) ");
  QRegExp reError("[0-9:\\.]+ ERROR .*\\( *-?[0-9]+\\) ");

  for (int i = 0; i < inLines.size(); i++)
  {
    bool keep = true;
    if (inLines[i].contains(" STATUS ") || inLines[i].contains(" INFO ") ||
        inLines[i].contains(" DEBUG ") || inLines[i].contains(" elapsed: ") ||
        inLines[i].contains("Time (sec)"))
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

QString ScriptTest::_removeLogColoring(const QString& input) const
{
  QString output = input;
  output.replace(LogColor::ColorRegex, "");
  return output;
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

  // Run validation on test output if configured for it and a file to validate was specified in the
  // script.
# ifdef HOOT_HAVE_JOSM
  LOG_VART(ConfigOptions().getTestValidationEnable());
  if (ConfigOptions().getTestValidationEnable())
  {
    _runValidation();
  }
# endif

  //  Replace colored text in the standard out
  _stdout.replace(LogColor::ColorRegex, "");

  if (QFile(_script + ".stdout").exists() == false || QFile(_script + ".stderr").exists() == false)
  {
    LOG_ERROR("STDOUT or STDERR doesn't exist for " + _script +
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
              "*************************");

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

    QString msg =
      "STDOUT does not match for:\n" + _script + ".stdout" + " " + _script + ".stdout.failed";
    if (_suppressFailureDetail)
    {
      LOG_ERROR(msg);
    }
    else if (_printDiff)
    {
      LOG_ERROR(msg);
      _runDiff(_script + ".stdout.stripped", _script + ".stdout.failed.stripped");
    }
    else
    {
      msg += "\n"
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
             "*************************";
      LOG_ERROR(msg);
    }

    failed = true;
  }

  if (_removeIgnoredSubstrings(_baseStderr) != _removeIgnoredSubstrings(_stderr))
  {
    _writeFile(_script + ".stderr.failed", _stderr);
    _writeFile(_script + ".stderr.failed.stripped", _removeIgnoredSubstrings(_stderr));
    _writeFile(_script + ".stderr.stripped", _removeIgnoredSubstrings(_baseStderr));

    QString msg =
      "STDERR does not match for:\n" + _script + ".stderr" + " " + _script + ".stderr.failed";
    if (_suppressFailureDetail)
    {
      LOG_ERROR(msg);
    }
    else if (_printDiff)
    {
      LOG_ERROR(msg);
      _runDiff(_script + ".stderr.stripped", _script + ".stderr.failed.stripped");
    }
    else
    {
      msg += "\n"
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
             "*************************";
      LOG_ERROR(msg);
    }

    failed = true;
  }

  if (failed)
  {
    CPPUNIT_ASSERT_MESSAGE(QString("STDOUT or STDERR does not match").toStdString(), false);
  }
}

void ScriptTest::_runDiff(const QString& file1, const QString& file2)
{
  QProcess p;
  p.start("diff", QStringList() << file1 << file2, QProcess::ReadOnly);

  while (p.waitForStarted(500) == false)
  {
    LOG_WARN("Waiting for process to start: " + _script);
  }

  const int scriptTestTimeOutSeconds = ConfigOptions(conf()).getTestScriptMaxExecTime();
  bool scriptTimeOutSpecified = false;
  if (scriptTestTimeOutSeconds != -1)
  {
    _waitToFinishTime = scriptTestTimeOutSeconds * 1000;
    scriptTimeOutSpecified = true;
  }

  bool first = true;
  QElapsedTimer timer;
  timer.start();
  while (p.waitForFinished(_waitToFinishTime) == false)
  {
    if (first)
    {
      // throw an endl in there so the dots in the test list don't look funny.
      cout << endl;
      LOG_WARN("Waiting for process to finish: " + _script);
      first = false;
    }

    // If the process hangs this will allows us to get out.
    const qint64 timeElapsedSeconds = timer.elapsed() / 1000;
    if (scriptTimeOutSpecified && timeElapsedSeconds >= scriptTestTimeOutSeconds)
    {
      LOG_ERROR("Forcefully ending test script test after " << timeElapsedSeconds << " seconds.");
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
  // TODO: It would be nice if we could specify Testing.conf here to avoid having to specify it in
  // every test file (#3823).
  QProcess p;
  p.start(_script, QProcess::ReadOnly);

  while (p.waitForStarted(500) == false)
  {
    LOG_WARN("Waiting for process to start: " + _script);
  }

  const int scriptTestTimeOutSeconds = ConfigOptions(conf()).getTestScriptMaxExecTime();
  bool scriptTimeOutSpecified = false;
  if (scriptTestTimeOutSeconds != -1)
  {
    _waitToFinishTime = scriptTestTimeOutSeconds * 1000;
    scriptTimeOutSpecified = true;
  }

  bool first = true;
  QElapsedTimer timer;
  timer.start();
  while (p.waitForFinished(_waitToFinishTime) == false)
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
      LOG_ERROR("Forcefully ending test script test after " << timeElapsedSeconds << " seconds.");
      break;
    }
  }

  _stdout = QString::fromUtf8(p.readAllStandardOutput());
  _stderr = QString::fromUtf8(p.readAllStandardError());
}

void ScriptTest::_runValidation()
{
  QStringList filesToValidate;
  QStringList goldValidationReports;
  if (!_validateScriptForValidation(filesToValidate, goldValidationReports))
  {
    return;
  }

  assert(filesToValidate.size() == goldValidationReports.size());
  const QString testName = QFileInfo(_script).completeBaseName();
  for (int i = 0; i < filesToValidate.size(); i++)
  {
    TestOutputValidator::validate(testName, filesToValidate.at(i), goldValidationReports.at(i));
  }
}

bool ScriptTest::_validateScriptForValidation(
  QStringList& filesToValidate, QStringList& goldValidationReports)
{
  const QString fileToValidatePropertyKey = "TO_VALIDATE";
  const QString validationReportGoldPropertyKey = "VALIDATION_REPORT_GOLD";
  const QStringList scriptFileLines = FileUtils::readFileToLines(_script);

  // Check for typo duplications of validation variables to save some tedious debugging later on.
  if (StringUtils::containsDuplicatePropertyKeys(scriptFileLines, fileToValidatePropertyKey, "="))
  {
    _stderr +=
      "Script test: " + _script + " has duplicated " + fileToValidatePropertyKey +
      " property keys." + "\n";
    return false;
  }
  if (StringUtils::containsDuplicatePropertyKeys(
        scriptFileLines, validationReportGoldPropertyKey, "="))
  {
    _stderr +=
      "Script test: " + _script + " has duplicated " + validationReportGoldPropertyKey +
      " property keys." + "\n";
    return false;
  }
  if (StringUtils::containsDuplicatePropertyValues(scriptFileLines, fileToValidatePropertyKey, "="))
  {
    _stderr +=
      "Script test: " + _script + " has duplicated " + fileToValidatePropertyKey +
      " property values." + "\n";
    return false;
  }
  if (StringUtils::containsDuplicatePropertyValues(
        scriptFileLines, validationReportGoldPropertyKey, "="))
  {
    _stderr +=
      "Script test: " + _script + " has duplicated " + validationReportGoldPropertyKey +
      " property values." + "\n";
    return false;
  }
  // Since script tests can support multiple outputs and validations for each, we require that a
  // numeric index be appended to each property key.
  const QString propKeyNonNumericEndErrorMsg = "property keys must end with a numeric character.";
  if (!StringUtils::allEndWithNumericChar(
        StringUtils::filterToPropertyKeysContaining(
          scriptFileLines, fileToValidatePropertyKey, "=")))
  {
    _stderr += fileToValidatePropertyKey + " " + propKeyNonNumericEndErrorMsg + "\n";
    return false;
  }
  if (!StringUtils::allEndWithNumericChar(
        StringUtils::filterToPropertyKeysContaining(
          scriptFileLines, validationReportGoldPropertyKey, "=")))
  {
    _stderr += validationReportGoldPropertyKey + " " + propKeyNonNumericEndErrorMsg + "\n";
    return false;
  }

  // If a test script contains TO_VALIDATE_<index>, that indicates it wants to perform validation on
  // a test output file. Validation is optional.
  filesToValidate =
    StringUtils::filterToPropertyValuesContaining(scriptFileLines, fileToValidatePropertyKey, "=");
  LOG_VART(filesToValidate);
  if (!filesToValidate.empty())
  {
    // A validation report to compare with must also be specified with
    // VALIDATION_REPORT_GOLD_<index>. Both sets of file names are sorted so the indexes between the
    // two file lists will match up correctly if they were written in the script correctly.
    goldValidationReports =
      StringUtils::filterToPropertyValuesContaining(
        scriptFileLines, validationReportGoldPropertyKey, "=");
    LOG_VART(goldValidationReports);
    const QString testName = QFileInfo(_script).completeBaseName();
    if (filesToValidate.size() != goldValidationReports.size())
    {
      _stderr +=
        "In script test: " + testName + " the number of files to validate specified by " +
        fileToValidatePropertyKey + ": " + QString::number(filesToValidate.size()) +
        " does not match the number of validation report gold files specified by " +
        validationReportGoldPropertyKey + ": " + QString::number(goldValidationReports.size()) +
        "\n";
      return false;
    }
  }

  return true;
}

void ScriptTest::_writeFile(const QString& path, const QString& content)
{
  QFile fp(path);

  if (fp.exists())
  {
    if (fp.remove() == false)
    {
      throw HootException("Error removing file: " + path);
    }
  }

  if (fp.open(QFile::WriteOnly) == false)
  {
    throw HootException("Error opening file for writing: " + path);
  }

  fp.write(content.toUtf8());
}

}
