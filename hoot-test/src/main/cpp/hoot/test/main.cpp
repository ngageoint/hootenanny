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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// GDAL
#include <gdal.h>

// GEOS
#include <geos/geom/Envelope.h>
#include <geos/geom/Geometry.h>
#include <geos/version.h>
using namespace geos::geom;

// CPP Unit
#include <cppunit/Exception.h>
#include <cppunit/SourceLine.h>
#include <cppunit/TestFailure.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextTestResult.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>

// Hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/HootConfig.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/test/ConflateCaseTestSuite.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/SignalCatcher.h>
#include <hoot/core/util/Settings.h>
#include <hoot/js/v8Engine.h>
using namespace hoot;

// Qt
#include <QtGui/QApplication>
#include <QDateTime>
#include <QString>
#include <QStringList>

// Standard
#include <iostream>
#include <vector>
using namespace std;

// Tgs
#include <tgs/System/Time.h>

#include "ScriptTestSuite.h"
#include "ProcessPool.h"

typedef boost::shared_ptr<CppUnit::Test> TestPtr;

enum _TestType
{
  CURRENT,
  QUICK,
  QUICK_ONLY,
  SLOW,
  SLOW_ONLY,
  GLACIAL,
  GLACIAL_ONLY,
  SERIAL,
  ALL
};

enum _TimeOutValue
{
  QUICK_WAIT    = 3,
  SLOW_WAIT     = 30,
  GLACIAL_WAIT  = 900
};

class HootTestListener : public CppUnit::TestListener
{
public:

  HootTestListener(bool showTestName, double testTimeout = QUICK_WAIT, bool showElapsed = true)
    : _success(true),
      _showTestName(showTestName),
      _showElapsed(showElapsed),
      _start(Tgs::Time::getTime()),
      _allStart(_start),
      _testTimeout(testTimeout)
  {
  }

  virtual void addFailure(const CppUnit::TestFailure& failure)
  {
    cout << endl << "Failure: " << failure.failedTest()->getName() << endl
      << "  " << failure.sourceLine().fileName() << "(" << failure.sourceLine().lineNumber() << ") ";
    CppUnit::Exception* e = failure.thrownException();
    if (e != NULL && QString::fromStdString(e->message().details()).trimmed() != "")
    {
      cout << "  " << e->message().details();
    }
    cout.flush();
    _success = false;
  }

  virtual bool getSuccess()
  {
    return _success;
  }

  virtual void endTest(CppUnit::Test * test )
  {
    double elapsed = Tgs::Time::getTime() - _start;
    if (_showTestName)
    {
      cout << test->getName() << " - " << elapsed << endl;
    }
    if (elapsed > _testTimeout && _testTimeout >= 0.0)
    {
      cout << "Test " << test->getName().data() << " ran longer than expected -- " << elapsed <<
              endl;
    }
    cout.flush();

    _start = Tgs::Time::getTime();
  }

  void showTestNames(bool show) { _showTestName = show; }

  virtual void startSuite(CppUnit::Test* suite)
  {
    if (_showTestName)
    {
      cout << "Starting " << suite->getName().data() << endl << flush;
    }
  }

  virtual void endTestRun(CppUnit::Test* /*test*/, CppUnit::TestResult* /*eventManager*/ )
  {
    if (_showElapsed)
    {
      cout << endl;
      cout << "Elapsed: " << Tgs::Time::getTime() - _allStart << endl;
    }
  }

  double getTestTimeout() { return _testTimeout; }

private:
  bool _success;
  bool _showTestName;
  bool _showElapsed;

  double _start;
  double _allStart;
  double _testTimeout;
};

void getTestVector(CppUnit::Test* from, vector<CppUnit::Test*>& to)
{
  for (int i = 0; i < from->getChildTestCount(); i++)
  {
    CppUnit::Test* child = from->getChildTestAt(i);
    if (dynamic_cast<CppUnit::TestComposite*>(child))
      getTestVector(child, to);
    else
      to.push_back(child);
  }
}

void getTestVector(const vector<TestPtr>& from, vector<CppUnit::Test*>& to)
{
  for (vector<TestPtr>::size_type i = 0; i < from.size(); ++i)
  {
    CppUnit::Test* child = from[i].get();
    if (dynamic_cast<CppUnit::TestComposite*>(child))
      getTestVector(child, to);
    else
      to.push_back(child);
  }
}

void filterPattern(const std::vector<CppUnit::Test*> &from, std::vector<CppUnit::Test*> &to,
                   QString pattern, bool includeOnMatch)
{
  QRegExp regex(pattern);

  for (size_t i = 0; i < from.size(); i++)
  {
    CppUnit::Test* child = from[i];
    QString name = QString::fromStdString(child->getName());
    if (regex.exactMatch(name) == includeOnMatch)
      to.push_back(child);
  }
}

CppUnit::Test* findTest(CppUnit::Test* t, std::string name)
{
  if (name == t->getName())
  {
    return t;
  }

  CppUnit::TestSuite* suite = dynamic_cast<CppUnit::TestSuite*>(t);
  if (suite != 0)
  {
    vector<CppUnit::Test*> children = suite->getTests();
    for (size_t i = 0; i < children.size(); ++i)
    {
      CppUnit::Test* result = findTest(children[i], name);
      if (result != 0)
      {
        return result;
      }
    }
  }

  return NULL;
}

CppUnit::Test* findTest(std::vector<TestPtr> vTests, std::string name)
{
  for (size_t i = 0; i < vTests.size(); i++)
  {
    if (name == vTests[i]->getName())
    {
      return vTests[i].get();
    }

    // Check to see if it's a suite of tests...
    CppUnit::TestSuite* suite = dynamic_cast<CppUnit::TestSuite*>(vTests[i].get());
    if (suite != 0)
    {
      vector<CppUnit::Test*> children = suite->getTests();
      for (size_t j = 0; j < children.size(); ++j)
      {
        CppUnit::Test* result = findTest(children[j], name);
        if (result != 0)
        {
          return result;
        }
      }
    }
  }

  return NULL;
}

void getNames(vector<string>& names, CppUnit::Test* t)
{
  CppUnit::TestSuite* suite = dynamic_cast<CppUnit::TestSuite*>(t);
  if (suite != 0)
  {
    vector<CppUnit::Test*> children = suite->getTests();
    for (size_t i = 0; i < children.size(); ++i)
    {
      getNames(names, children[i]);
    }
  }
  else
  {
    names.push_back(t->getName());
  }
}

void getNames(vector<string>& names, const std::vector<TestPtr> &vTests)
{
  for (size_t i = 0; i < vTests.size(); i++)
  {
    // See if our test is really a suite
    CppUnit::TestSuite* suite = dynamic_cast<CppUnit::TestSuite*>(vTests[i].get());
    if (suite != 0)
    {
      vector<CppUnit::Test*> children = suite->getTests();
      for (size_t j = 0; j < children.size(); ++j)
      {
        getNames(names, children[j]);
      }
    }
    else
    {
      names.push_back(vTests[i]->getName());
    }
  }
}

void getNames(std::vector<string>& names, const std::vector<CppUnit::Test*> &vTests)
{
  for (size_t i = 0; i < vTests.size(); i++)
    names.push_back(vTests[i]->getName());
}

void printNames(const std::vector<TestPtr> &vTests)
{
  vector<string> names;
  getNames(names, vTests);
  for (vector<string>::iterator it = names.begin(); it != names.end(); ++it)
    cout << *it << endl;
}

void runSingleTest(CppUnit::Test * pTest, QStringList &args, CppUnit::TextTestResult * pResult)
{
  // clear all user configuration so we have consistent tests.
  conf().clear();
  ConfigOptions::populateDefaults(conf());
  conf().set("HOOT_HOME", getenv("HOOT_HOME"));
  Settings::parseCommonArguments(args);
  pTest->run(pResult);
}

void populateTests(_TestType t, std::vector<TestPtr> &vTests, bool printDiff, bool hideDisableTests = false)
{
  /** This section is a bit verbose but ordering is very important as the order must go as follows:
   *  Default Registry
   *  Script Tests
   *  Named Registries (current, quick, etc.)
   */
  switch(t)
  {
  default:
  case CURRENT:
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/current/", printDiff, QUICK_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest()));
    break;
  case QUICK:
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry().makeTest()));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/current/", printDiff, QUICK_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/quick/", printDiff, QUICK_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("TgsTest").makeTest()));
    break;
  case QUICK_ONLY:
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry().makeTest()));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/quick/", printDiff, QUICK_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("TgsTest").makeTest()));
    break;
  case SLOW:
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry().makeTest()));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/current/", printDiff, QUICK_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/quick/", printDiff, QUICK_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/slow/", printDiff, SLOW_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/slow/serial/", printDiff, SLOW_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ConflateCaseTestSuite("test-files/cases", hideDisableTests)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("TgsTest").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("slow").makeTest()));
    break;
  case SLOW_ONLY:
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/slow/", printDiff, SLOW_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/slow/serial/", printDiff, SLOW_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ConflateCaseTestSuite("test-files/cases", hideDisableTests)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("slow").makeTest()));
    break;
  case GLACIAL:
  case ALL:
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry().makeTest()));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/current/", printDiff, QUICK_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/quick/", printDiff, QUICK_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/slow/", printDiff, SLOW_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/slow/serial/", printDiff, SLOW_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/glacial/", printDiff, GLACIAL_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/glacial/serial/", printDiff, GLACIAL_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ConflateCaseTestSuite("test-files/cases", hideDisableTests)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("TgsTest").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("slow").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("glacial").makeTest()));
    break;
  case GLACIAL_ONLY:
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/glacial/", printDiff, GLACIAL_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/glacial/serial/", printDiff, GLACIAL_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("glacial").makeTest()));
    break;
  case SERIAL:
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/glacial/serial/", printDiff, GLACIAL_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(new ScriptTestSuite("test-files/cmd/slow/serial/", printDiff, SLOW_WAIT, hideDisableTests)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("serial").makeTest()));
    break;
  }
}

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    cout << argv[0] << " Usage:\n"
            "--current - Run the 'current' tests.\n"
            "--quick - Run the quick (unnamed) tests and all above.\n"
            "--slow - Run the 'slow' tests and all above.\n"
            "--glacial - Run the 'glacial' tests and all above.\n"
            "--all - Run all the above tests.\n"
            "--quick-only - Run the quick (unnamed) tests only.\n"
            "--slow-only - Run the 'slow' tests only.\n"
            "--glacial-only - Run the 'glacial' tests only.\n"
            "--single [test name] - Run only the test specified.\n"
            "--names - Show the names of all the tests as they run.\n"
            "--all-names - Only print the names of all the tests.\n"
            "--warn - Show warning messages and above.\n"
            "--info - Show info messages and above.\n"
            "--verbose - Show verbose messages and above.\n"
            "--debug - Show debug messages and above.\n"
            "--trace - Show trace messages and above.\n"
            "--diff - Print diff when a script test fails.\n"
            "--include=[regex] - Include only tests that match the specified regex.\n"
            "--exclude=[regex] - Exclude tests that match the specified regex.\n"
            "--parallel [process count] - Run the specified tests in parallel.\n"
            "\n"
            "See the Hootenanny Developer Guide for more information.\n"
            ;
  }
  else
  {
    Hoot::getInstance().init();

    v8Engine::setPlatformInit();
    v8Engine::getInstance().init();

    QCoreApplication app(argc, argv);

    QStringList args;
    for (int i = 1; i < argc; i++)
    {
      args << argv[i];
    }

    Log::getInstance().setLevel(Log::Warn);
    std::vector<TestPtr> vAllTests;
    std::vector<CppUnit::Test*> vTestsToRun;
    CppUnit::TextTestResult result;

# if HOOT_HAVE_HADOOP
    Hoot::getInstance().loadLibrary("PrettyPipesExample");
# endif

    // initialize OSM Schema so the time expense doesn't print in other tests.
    OsmSchema::getInstance();

    boost::shared_ptr<HootTestListener> listener;

    bool printDiff = args.contains("--diff");

    // Print all names & exit without running anything
    if (args.contains("--all-names"))
    {
      populateTests(ALL, vAllTests, printDiff);
      printNames(vAllTests);
      return 0;
    }

    // Run a single test
    if (args.contains("--single"))
    {
      int i = args.indexOf("--single") + 1;
      if (i >= args.size())
      {
        throw HootException("Expected a test name after --single.");
      }
      QString testName = args[i];

      listener.reset(new HootTestListener(false, -1));
      result.addListener(listener.get());
      Log::getInstance().setLevel(Log::Info);
      populateTests(ALL, vAllTests, printDiff);
      CppUnit::Test* t = findTest(vAllTests, testName.toStdString());
      if (t == NULL)
      {
        cout << "Could not find the specified test: " << testName.toStdString() << endl;
        return -1;
      }

      runSingleTest(t, args, &result);
      return result.failures().size() > 0 ? -1 : 0;
    }
    else if (args.contains("--listen"))
    {
      double slowTest = -1;
      int i = args.indexOf("--listen") + 1;
      if (i < args.size())
        slowTest = args[i].toDouble();

      listener.reset(new HootTestListener(false, slowTest, false));
      if (args.contains("--names"))
        listener->showTestNames(true);
      result.addListener(listener.get());

      string testName;
      cin >> testName;
      while (testName != HOOT_TEST_FINISHED)
      {
        populateTests(ALL, vAllTests, printDiff, true);
        CppUnit::Test* t = findTest(vAllTests, testName);
        if (t != 0)
        {
          runSingleTest(t, args, &result);
          cout << endl << HOOT_TEST_FINISHED << endl;
        }
        else
        {
          cerr << "Could not find the specified test: " <<  testName << endl;
          cout << HOOT_TEST_FINISHED << endl;
        }
        cin >> testName;
      }
      return result.failures().size() > 0 ? -1 : 0;
    }
    else
    {
      if (args.contains("--current"))
      {
        listener.reset(new HootTestListener(true));
        Log::getInstance().setLevel(Log::Info);
        populateTests(CURRENT, vAllTests, printDiff);
      }
      else if (args.contains("--quick"))
      {
        listener.reset(new HootTestListener(false, QUICK_WAIT));
        populateTests(QUICK, vAllTests, printDiff);
      }
      else if (args.contains("--quick-only"))
      {
        listener.reset(new HootTestListener(false, QUICK_WAIT));
        populateTests(QUICK_ONLY, vAllTests, printDiff);
      }
      else if (args.contains("--slow"))
      {
        listener.reset(new HootTestListener(false, SLOW_WAIT));
        populateTests(SLOW, vAllTests, printDiff);
      }
      else if (args.contains("--slow-only"))
      {
        listener.reset(new HootTestListener(false, SLOW_WAIT));
        populateTests(SLOW_ONLY, vAllTests, printDiff);
      }
      else if (args.contains("--all") || args.contains("--glacial"))
      {
        listener.reset(new HootTestListener(false, GLACIAL_WAIT));
        populateTests(GLACIAL, vAllTests, printDiff);
      }
      else if (args.contains("--glacial-only"))
      {
        listener.reset(new HootTestListener(false, GLACIAL_WAIT));
        populateTests(GLACIAL_ONLY, vAllTests, printDiff);
      }

      vector<CppUnit::Test*> vTests;
      getTestVector(vAllTests, vTests);
      bool filtered = false;

      for (int i = 0; i < args.size(); i++)
      {
        if (args[i].startsWith("--exclude="))
        {
          if (vTestsToRun.size() > 0)
          {
            //  On the second (or more) time around exclude from the excluded list
            vTests.swap(vTestsToRun);
            vTestsToRun.clear();
          }
          int equalsPos = args[i].indexOf('=');
          QString regex = args[i].mid(equalsPos + 1);
          LOG_WARN("Excluding pattern: " << regex);
          filterPattern(vTests, vTestsToRun, regex, false);
          filtered = true;
        }
        else if (args[i].startsWith("--include="))
        {
          int equalsPos = args[i].indexOf('=');
          QString regex = args[i].mid(equalsPos + 1);
          LOG_WARN("Including only tests that match: " << regex);
          filterPattern(vTests, vTestsToRun, regex, true);
          filtered = true;
        }
      }

      if (!filtered) // Do all tests
        vTestsToRun.swap(vTests);
      cout << "Running core tests.  Test count: " << vTestsToRun.size() << endl;
    }

    if (args.contains("--parallel"))
    {
      double start = Tgs::Time::getTime();

      int i = args.indexOf("--parallel") + 1;
      if (i >= args.size())
      {
        throw HootException("Expected integer after --parallel.");
      }
      bool ok = false;
      int nproc = args[i].toInt(&ok);
      if (!ok || nproc < 1)
      {
        throw HootException("Expected integer after --parallel");
      }
      ProcessPool pool(nproc, listener->getTestTimeout(),
                       (bool)args.contains("--names"),
                       (bool)args.contains("--diff"));

      //  Get the names of all of the tests to run
      vector<string> allNames;
      getNames(allNames, vTestsToRun);
      set<string> nameCheck;
      for (vector<string>::iterator it = allNames.begin(); it != allNames.end(); ++it)
        nameCheck.insert(*it);

      //  Add all of the jobs that must be done serially and are a part of the selected tests
      vector<TestPtr> serialTests;
      populateTests(SERIAL, serialTests, printDiff, true);
      vector<string> serialNames;
      getNames(serialNames, serialTests);
      for (vector<string>::iterator it = serialNames.begin(); it != serialNames.end(); ++it)
      {
        if (nameCheck.find(*it) != nameCheck.end())
          pool.addJob(QString(it->c_str()), false);
      }

      //  Add all of the remaining jobs in the test suite
      for (vector<string>::iterator it = allNames.begin(); it != allNames.end(); ++it)
        pool.addJob(QString(it->c_str()));

      pool.startProcessing();
      pool.wait();

      cout << endl;
      cout << "Elapsed: " << Tgs::Time::getTime() - start << endl;
      return pool.getFailures() > 0 ? -1 : 0;
    }
    else
    {
      double start = Tgs::Time::getTime();

      if (args.contains("--names"))
        listener->showTestNames(true);

      // clear all user configuration so we have consistent tests.
      conf().clear();
      ConfigOptions::populateDefaults(conf());
      LOG_DEBUG("HOOT_HOME: " + QString(getenv("HOOT_HOME")))
      conf().set("HOOT_HOME", getenv("HOOT_HOME"));

      // allows us to pass config options through HootTest
      Settings::parseCommonArguments(args);

      // set up listener
      result.addListener(listener.get());

      // run all tests
      for (size_t i = 0; i < vTestsToRun.size(); i++)
        vTestsToRun[i]->run(&result);

      cout << endl;
      cout << "Elapsed: " << Tgs::Time::getTime() - start << endl;
      return result.failures().size() > 0 ? -1 : 0;
    }
  }
}
