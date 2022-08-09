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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef CONFLATECASETESTSUITE_H
#define CONFLATECASETESTSUITE_H

// hoot
#include <hoot/test/AbstractTestSuite.h>

namespace hoot
{

/**
 * @brief The ConflateCaseTestSuite class manages the suite of conflate case tests.
 */
class ConflateCaseTestSuite : public AbstractTestSuite
{

public:

  /**
   * @brief ConflateCaseTestSuite constructor
   * @param dir top level directory for case tests
   * @param suppressFailureDetail if true, detailed test failure information is not displayed
   * @param printValidationReportDiff @if true, for failing tests prints the difference between the
   * baseline validation report and the test validation report output
   * @param hideDisableTests if true, status messages for disabled tests are suppressed
   * @param allowSerial if true, case tests under the serial directory are added to the tests to be
   * run
   */
  ConflateCaseTestSuite(const QString& dir, bool suppressFailureDetail = false, bool printValidationReportDiff = false,
                        bool hideDisableTests = false, bool allowSerial = false);

  /**
   * @see AbstractTestSuite
   */
  virtual void loadDir(const QString& dir, QStringList confs) override;

private:

  bool _hideDisableTests;
  int _numTests;
  bool _suppressFailureDetail;
  bool _printValidationReportDiff;
  bool _allowSerial;
};

}

#endif // CONFLATECASETESTSUITE_H
