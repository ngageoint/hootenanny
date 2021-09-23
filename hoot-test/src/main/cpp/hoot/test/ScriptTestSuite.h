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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef SCRIPTTESTSUITE_H
#define SCRIPTTESTSUITE_H

// hoot
#include <hoot/test/ScriptTest.h>

// Standard
#include <memory>
#include <vector>

namespace hoot
{

class ScriptTestSuite : public CppUnit::TestSuite
{
public:

  /**
   * @brief ScriptTestSuite constructor
   * @param dir top level directory for script tests
   * @param printDiff if enabled, any differences between baseline and actual test output are
   * displayed
   * @param waitTime maximum time, in seconds, to wait for script to finish execution
   * @param hideDisableTests if true, status messages for disabled tests are suppressed
   * @param suppressFailureDetail if true, detailed test failure information is not displayed
   * @param validatedOnly if true, only script tests which validate any output will be run
   */
  ScriptTestSuite(
    QString dir, bool printDiff, double waitTime, bool hideDisableTests,
    bool suppressFailureDetail, bool validatedOnly);

private:

  std::vector<std::shared_ptr<ScriptTest>> _tests;

  /**
   * @brief _scriptValidatesAnyOutput Determines if a script test runs validation with a simple text
   * comparison
   * @param scriptPath path to the test's script
   * @return true if the test runs the validate command; false otherwise
   */
  bool _scriptValidatesAnyOutput(const QString& scriptPath) const;
};

}

#endif // SCRIPTTESTSUITE_H
