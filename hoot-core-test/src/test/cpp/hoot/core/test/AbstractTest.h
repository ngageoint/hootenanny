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
 * @copyright Copyright (C) 2017 Maxar (http://www.maxar.com/)
 */
#ifndef ABSTRACTTEST_H
#define ABSTRACTTEST_H

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDir>
#include <QStringList>

namespace hoot
{

/**
 * An abstract test to be run as part of a hoot test suite
 */
class AbstractTest : public CppUnit::TestCase
{

public:

  AbstractTest(QDir d, QStringList confs);

  void addConfig(QString conf) { _confs << conf; }

  virtual void runTest() = 0;

protected:

  QDir _d;
  QStringList _confs;
};

}

#endif // ABSTRACTTEST_H
