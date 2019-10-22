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
 * @copyright Copyright (C) 2015, 2017, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SCRIPTTEST_H
#define SCRIPTTEST_H

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QRunnable>
#include <QString>

namespace hoot
{

class ScriptTest : public CppUnit::TestCase
{
public:

  ScriptTest(QString script, bool printDiff, bool suppressFailureDetail = false,
             int waitToFinishTime = 30000);

  virtual void runTest();

private:

  bool _printDiff;
  bool _suppressFailureDetail;
  QString _script;
  QString _stderr, _baseStderr;
  QString _stdout, _baseStdout;
  /** Timeout before emitting warning "Waiting for process to finish" in msecs */
  int _waitToFinishTime;

  QString _readFile(QString path);
  void _writeFile(QString path, QString content);

  void _removeFile(QString path);

  /**
   * Given an input string remove any strings that aren't relevant to testing. Examples include
   * LOG statements of info or debug.
   */
  QString _removeIgnoredSubstrings(QString input) const;

  void _runDiff(QString file1, QString file2);

  void _runProcess();
};

}

#endif // SCRIPTTEST_H
