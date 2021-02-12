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
 * @copyright Copyright (C) 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ABSTRACTTESTSUITE_H
#define ABSTRACTTESTSUITE_H

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Abstract base class for hoot test suites
 */
class AbstractTestSuite : public CppUnit::TestSuite
{

public:

  AbstractTestSuite(const QString& dir);

  /**
   * @brief loadDir Loads directory of tests
   * @param dir Directory to load
   * @param confs List of config files, NOTE: passed by value because the copy of `confs`
   *              can be edited without affecting the original value
   */
  virtual void loadDir(const QString& dir, QStringList confs) = 0;
};

}

#endif // ABSTRACTTESTSUITE_H
