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

#ifndef __CASETESTFITNESSFUNCTION_H__
#define __CASETESTFITNESSFUNCTION_H__

// Cpp Unit
#include <cppunit/TestListener.h>
#include <cppunit/TestResult.h>

// Hoot
#include <hoot/core/test/ConflateCaseTestSuite.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QTemporaryFile>

// Tgs
#include <tgs/Optimization/FitnessFunction.h>
#include <tgs/Optimization/State.h>
#include <tgs/Statistics/Random.h>

namespace hoot
{

/**
 *
 */
class CaseTestFitnessFunction : public Tgs::FitnessFunction
{

public:

    class SimpleListener : public CppUnit::TestListener
    {

    public:

      SimpleListener() : _failure(false) {}

      virtual void addFailure( const CppUnit::TestFailure & /*failure*/ ) { _failure = true; }
      bool isFailure() const { return _failure; }

    private:

      bool _failure;
    };

    class TempFileName
    {
    public:

      TempFileName()
      {
        do
        {
          int r = Tgs::Random::instance()->generateInt();
          _name = QString("/tmp/HootConflictsTest-%1.conf").arg(r, 10, 10, QChar('0'));
        }
        while (QFile(_name).exists());
      }

      ~TempFileName()
      {
        if (QFile(_name).exists())
        {
          if (!QFile(_name).remove())
          {
            LOG_WARN("Failure removing: " << _name);
          }
        }
      }

      QString getFileName() const { return _name; }

    private:

      QString _name;
    };

    CaseTestFitnessFunction();

    virtual double f(const Tgs::ConstStatePtr& s);

    int getTestCount() { return _testCount; }

    QStringList getFailingTestsForBestRuns() { return _failingTestsForBestRuns; }

private:

    int _testCount;
    int _lowestNumFailingTestsPerRun;
    //list members are one or more test names joined by a ';'
    QStringList _failingTestsForBestRuns;
    boost::shared_ptr<ConflateCaseTestSuite> _testSuite;

    QString _failedTestsToString(const QStringList failedTests) const;
};

}

#endif // __CASETESTFITNESSFUNCTION_H__
