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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Hoot
#include <hoot/core/algorithms/optimizer/IntegerProgrammingSolver.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDir>

// Standard
#include <fstream>
#include <list>
#include <cassert>

namespace hoot
{

class IntegerProgrammingSolverTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(IntegerProgrammingSolverTest);
  CPPUNIT_TEST(runTest);
  CPPUNIT_TEST(runTest2);
  CPPUNIT_TEST_SUITE_END();

public:

  void runTest()
  {
    double z, x1, x2, x3;

    IntegerProgrammingSolver solver;
    solver.setProblemName("sample");
    solver.setObjectiveDirection(GLP_MAX);
    solver.addRows(2);

    solver.setRowName(1, "p");
    solver.setRowBounds(1, GLP_DB, 0.0, 1.0);
    solver.setRowName(2, "q");
    solver.setRowBounds(2, GLP_DB, 0.0, 1.0);

    solver.addColumns(3);
    solver.setColumnName(1, "x1");
    solver.setColumnKind(1, GLP_BV);
    solver.setColumnBounds(1, GLP_DB, 0.0, 1.0);
    solver.setObjectiveCoefficient(1, 0.4);

    solver.setColumnName(2, "x2");
    solver.setColumnKind(2, GLP_BV);
    solver.setColumnBounds(2, GLP_DB, 0.0, 1.0);
    solver.setObjectiveCoefficient(2, 0.9);

    solver.setColumnName(3, "x3");
    solver.setColumnKind(3, GLP_BV);
    solver.setColumnBounds(3, GLP_DB, 0.0, 1.0);
    solver.setObjectiveCoefficient(3, 0.6);

    vector<int> ia, ja;
    vector<double> ar;
    ia.push_back(-1);
    ja.push_back(-1);
    ar.push_back(-1);

    ia.push_back(1); ja.push_back(1); ar.push_back(1.0);
    ia.push_back(1); ja.push_back(2); ar.push_back(1.0);
    ia.push_back(1); ja.push_back(3); ar.push_back(0.0);

    //ia.push_back(2); ja.push_back(1); ar.push_back(0.0);
    ia.push_back(2); ja.push_back(2); ar.push_back(1.0);
    ia.push_back(2); ja.push_back(3); ar.push_back(1.0);
    solver.loadMatrix(ia, ja, ar);
    solver.solve();

    z = solver.getObjectiveValue();
    x1 = solver.getColumnPrimalValue(1);
    x2 = solver.getColumnPrimalValue(2);
    x3 = solver.getColumnPrimalValue(3);

    LOG_TRACE("z: " << z << " x1: " << x1 << " x2: " << x2 << " x3: " << x3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, z, 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, x1, 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, x2, 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, x3, 1e-5);

    solver.setObjectiveCoefficient(3, 0.4);
    solver.solve();

    z = solver.getObjectiveValue();
    x1 = solver.getColumnPrimalValue(1);
    x2 = solver.getColumnPrimalValue(2);
    x3 = solver.getColumnPrimalValue(3);

    LOG_TRACE("z: " << z << " x1: " << x1 << " x2: " << x2 << " x3: " << x3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.9, z, 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, x1, 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, x2, 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, x3, 1e-5);
  }

  void runTest2()
  {
    double z, x1, x2, x3;

    IntegerProgrammingSolver solver;
    solver.setProblemName("sample");
    solver.setObjectiveDirection(GLP_MAX);
    solver.addRows(1);

    // 0.0 <= p <= 1.1
    solver.setRowName(1, "p");
    // make sure it properly solves an integer problem.
    solver.setRowBounds(1, GLP_DB, 0.0, 1.1);

    // z = x1 + 2 * x2 + x3
    solver.addColumns(3);
    solver.setColumnName(1, "x1");
    solver.setColumnKind(1, GLP_BV);
    solver.setColumnBounds(1, GLP_DB, 0.0, 1.0);
    solver.setObjectiveCoefficient(1, 1.0);

    solver.setColumnName(2, "x2");
    solver.setColumnKind(2, GLP_BV);
    solver.setColumnBounds(2, GLP_DB, 0.0, 1.0);
    solver.setObjectiveCoefficient(2, 2.0);

    solver.setColumnName(3, "x3");
    solver.setColumnKind(3, GLP_BV);
    solver.setColumnBounds(3, GLP_DB, 0.0, 1.0);
    solver.setObjectiveCoefficient(3, 1.0);

    vector<int> ia, ja;
    vector<double> ar;
    ia.push_back(-1);
    ja.push_back(-1);
    ar.push_back(-1);

    // subject to constraint:
    // p = x1 + x2 + 0 * x3
    ia.push_back(1); ja.push_back(1); ar.push_back(1.0);
    ia.push_back(1); ja.push_back(2); ar.push_back(1.0);
    ia.push_back(1); ja.push_back(3); ar.push_back(0.0);

    solver.loadMatrix(ia, ja, ar);
    solver.solve();

    z = solver.getObjectiveValue();
    x1 = solver.getColumnPrimalValue(1);
    x2 = solver.getColumnPrimalValue(2);
    x3 = solver.getColumnPrimalValue(3);

    LOG_TRACE("z: " << z << " x1: " << x1 << " x2: " << x2 << " x3: " << x3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(3.0, z, 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, x1, 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, x2, 1e-5);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, x3, 1e-5);
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IntegerProgrammingSolverTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(IntegerProgrammingSolverTest, "quick");

}
