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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef CONFLATECASETEST_H
#define CONFLATECASETEST_H

// Hoot
#include <hoot/test/AbstractTest.h>

namespace hoot
{

/**
 * @brief The ConflateCaseTest class is a conflate case test to be run as part of the hoot testing
 * suite.
 */
class ConflateCaseTest : public AbstractTest
{

public:

  /**
   * @brief ConflateCaseTest constructor
   * @param d the directory containing the test files
   * @param confs configuration files to be used by the test
   * @param suppressFailureDetail if true, detailed test failure information is not displayed
   * @param printValidationReportDiff if true, for failing tests prints the difference between the
   * baseline validation report and the test validation report output
   */
  ConflateCaseTest(QDir d, QStringList confs, bool suppressFailureDetail = false,
                   bool printValidationReportDiff = false);

  /**
   * @see AbstractTest
   */
  virtual void runTest();

private:

  bool _suppressFailureDetail;
  bool _printValidationReportDiff;

  void _runConflateCmd() const;
  void _runMultiaryConflateCmd() const;
};

}

#endif // CONFLATECASETEST_H
