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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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
#include <hoot/test/ProcessPool.h>
#include <hoot/test/ScriptTestSuite.h>
using namespace hoot;

// Qt
#include <QCoreApplication>
#include <QDateTime>
#include <QString>
#include <QStringList>

// Standard
#include <iostream>
#include <unistd.h>
#include <vector>
using namespace std;

// Tgs
#include <tgs/System/Time.h>

using TestPtr = std::shared_ptr<CppUnit::Test>;

enum _TestType
{
  NONE          = 0x00,
  CURRENT       = 0x01,
  QUICK_ONLY    = 0x02,
  QUICK         = 0x03,
  CASE_ONLY     = 0x04,
  SLOW_ONLY     = 0x08,
  SLOW          = 0x0f,
  GLACIAL_ONLY  = 0x10,
  GLACIAL       = 0x1f,
  SERIAL        = 0x20,
  ALL           = 0x3f
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

  HootTestListener(bool showTestName, bool suppressFailureDetail = false,
                   double testTimeout = QUICK_WAIT, bool showElapsed = true)
    : _success(true),
      _showTestName(showTestName),
      _suppressFailureDetail(suppressFailureDetail),
      _showElapsed(showElapsed),
      _start(Tgs::Time::getTime()),
      _allStart(_start),
      _testTimeout(testTimeout)
  {
  }

  virtual void addFailure(const CppUnit::TestFailure& failure)
  {
    cout << endl << "Failure: " << failure.failedTest()->getName() << endl;
    if (!_suppressFailureDetail)
    {
      cout  << "  " << failure.sourceLine().fileName() << "(" <<
        failure.sourceLine().lineNumber() << ") ";
      CppUnit::Exception* e = failure.thrownException();
      if (e != nullptr && QString::fromStdString(e->message().details()).trimmed() != "")
      {
        cout << "  " << e->message().details();
      }
    }
    cout.flush();
    _success = false;
  }

  virtual bool getSuccess()
  {
    return _success;
  }

//  virtual void startTest(CppUnit::Test* test)
//  {
//    // for debugging only
//    cout << "Starting " << test->getName() << endl/* << flush*/;
//  }

  virtual void endTest(CppUnit::Test* test)
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

  virtual void endTestRun(CppUnit::Test* /*test*/, CppUnit::TestResult* /*eventManager*/)
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
  bool _suppressFailureDetail;
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

void filterPattern(const std::vector<CppUnit::Test*>& from, std::vector<CppUnit::Test*>& to,
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

void includeExcludeTests(const QStringList& args, vector<CppUnit::Test*>& vTests)
{
  vector<CppUnit::Test*> vTestsToRun;
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
      LOG_INFO("Excluding pattern: " << regex);
      filterPattern(vTests, vTestsToRun, regex, false);
      filtered = true;
    }
    else if (args[i].startsWith("--include="))
    {
      int equalsPos = args[i].indexOf('=');
      QString regex = args[i].mid(equalsPos + 1);
      LOG_INFO("Including only tests that match: " << regex);
      filterPattern(vTests, vTestsToRun, regex, true);
      filtered = true;
    }
  }
  //  Swap the filtered tests into the test list
  if (filtered)
    vTests.swap(vTestsToRun);
}

CppUnit::Test* findTest(CppUnit::Test* t, std::string name)
{
  //cout << t->getName() << endl;
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

  return nullptr;
}

CppUnit::Test* findTest(std::vector<TestPtr> vTests, std::string name)
{
  for (size_t i = 0; i < vTests.size(); i++)
  {
    //cout << vTests[i]->getName() << endl;
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

  return nullptr;
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

void getNames(vector<string>& names, const std::vector<CppUnit::Test*>& vTests)
{
  for (size_t i = 0; i < vTests.size(); i++)
  {
    // See if our test is really a suite
    CppUnit::TestSuite* suite = dynamic_cast<CppUnit::TestSuite*>(vTests[i]);
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

void printNames(const std::vector<CppUnit::Test*>& vTests)
{
  vector<string> names;
  getNames(names, vTests);
  for (vector<string>::iterator it = names.begin(); it != names.end(); ++it)
    cout << *it << endl;
}

void runSingleTest(CppUnit::Test* pTest, QStringList& args, CppUnit::TextTestResult* pResult)
{
  // clear all user configuration so we have consistent tests.
  conf().clear();
  ConfigOptions::populateDefaults(conf());
  conf().set("HOOT_HOME", getenv("HOOT_HOME"));
  Settings::parseCommonArguments(args);
  pTest->run(pResult);
}

void populateTests(_TestType t, std::vector<TestPtr>& vTests, bool printDiff,
                   bool suppressFailureDetail, bool hideDisableTests = false)
{
  //  Current tests are included in CURRENT, QUICK, SLOW, and GLACIAL
  //  Add current tests if the bit flag is set
  if (t & CURRENT)
  {
    vTests.push_back(
      TestPtr(
        new ScriptTestSuite(
          "test-files/cmd/current/", printDiff, QUICK_WAIT, hideDisableTests,
          suppressFailureDetail)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest()));
  }
  //  Quick tests are included in QUICK, SLOW, and GLACIAL
  //  Add quick tests if the bit flag is set
  if (t & QUICK_ONLY)
  {
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry().makeTest()));
    vTests.push_back(
      TestPtr(
        new ScriptTestSuite(
          "test-files/cmd/quick/", printDiff, QUICK_WAIT, hideDisableTests, suppressFailureDetail)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("TgsTest").makeTest()));
  }
  //  Slow tests are included in SLOW and GLACIAL
  //  Add slow tests if the bit flag is set
  if (t & SLOW_ONLY)
  {
    vTests.push_back(
      TestPtr(
        new ScriptTestSuite(
          "test-files/cmd/slow/", printDiff, SLOW_WAIT, hideDisableTests, suppressFailureDetail)));
    vTests.push_back(
      TestPtr(
        new ScriptTestSuite(
          "test-files/cmd/slow/serial/", printDiff, SLOW_WAIT, hideDisableTests,
          suppressFailureDetail)));
    vTests.push_back(TestPtr(new ConflateCaseTestSuite("test-files/cases", hideDisableTests)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("slow").makeTest()));
  }
  //  Add glacial tests if the bit flag is set
  if (t & GLACIAL_ONLY)
  {
    vTests.push_back(
      TestPtr(
        new ScriptTestSuite(
          "test-files/cmd/glacial/", printDiff, GLACIAL_WAIT, hideDisableTests,
          suppressFailureDetail)));
    vTests.push_back(
      TestPtr(
        new ScriptTestSuite(
          "test-files/cmd/glacial/serial/", printDiff, GLACIAL_WAIT, hideDisableTests,
          suppressFailureDetail)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("glacial").makeTest()));
  }
  //  Add serial tests if the bit flag is set
  if (t == SERIAL)
  {
    vTests.push_back(
      TestPtr(
        new ScriptTestSuite(
          "test-files/cmd/glacial/serial/", printDiff, GLACIAL_WAIT, hideDisableTests,
          suppressFailureDetail)));
    vTests.push_back(
      TestPtr(
        new ScriptTestSuite(
          "test-files/cmd/slow/serial/", printDiff, SLOW_WAIT, hideDisableTests,
          suppressFailureDetail)));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("serial").makeTest()));
  }

  if (t == CASE_ONLY)
  {
    vTests.push_back(TestPtr(new ConflateCaseTestSuite("test-files/cases", hideDisableTests)));
  }
}

QMap<QString, QString> getAllowedOptions()
{
  // keep this list alphabetized
  QMap<QString, QString> options;
  options["--all"] = "Run all the tests";
  options["--all-names"] = "Print the names of all the tests without running them";
  options["--case-only"] = "Run the conflate case tests only";
  options["--current"] = "Run the 'current' level tests";
  options["--debug"] = "Show debug level log messages and above";
  options["--diff"] = "Print a diff when a script test fails";
  options["--error"] = "Show error log level messages and above";
  options["--exclude=[regex]"] =
    "Exclude tests that match the specified regex; e.g. HootTest '--exclude=.*building.*'";
  options["--fatal"] = "Show fatal error log level messages only";
  options["--glacial"] = "Run 'glacial' level tests and below";
  options["--glacial-only"] = "Run the 'glacial' level tests only";
  options["--include=[regex]"] =
    "Include only tests that match the specified regex; e.g. HootTest '--include=.*building.*'";
  options["--info"] = "Show info log level messages and above";
  options["--quick"] = "Run the 'quick' level' tests";
  options["--quick-only"] = "Run the 'quick' level tests only";
  options["--names"] = "Show the names of all the tests as they run";
  options["--parallel [process count]"] =
    "Run the specified tests in parallel with the specified number of processes";
  options["--single [test name]"] = "Run only the test specified";
  options["--slow"] = "Run the 'slow' level tests and above";
  options["--slow-only"] = "Run the 'slow' level tests only";
  options["--status"] = "Show status log level messages and above";
  options["--suppress-failure-detail"] =
    "Do not show test failure detailed messages; disables --diff for script tests";
  options["--trace"] = "Show trace log level messages and abov";
  options["--verbose"] = "Show verbose log level messages and above";
  options["--warn"] = "Show warning log level messages and above";
  return options;
}

QStringList getAllowedOptionNames()
{
  QStringList optionNames;
  const QMap<QString, QString> options = getAllowedOptions();
  for (QMap<QString, QString>::const_iterator it = options.begin(); it != options.end(); ++it)
  {
    optionNames.append(it.key().split(QRegExp("\\s+|="))[0]);
  }
  return optionNames;
}

int largestOptionNameSize()
{
  int largest = 0;
  const QMap<QString, QString> options = getAllowedOptions();
  for (QMap<QString, QString>::const_iterator it = options.begin(); it != options.end(); ++it)
  {
    if (it.key().size() > largest)
    {
      largest = it.key().size();
    }
  }
  return largest;
}

void usage(char* argv)
{
  cout << argv << " Usage:" << endl;
  const QMap<QString, QString> options = getAllowedOptions();
  const int largestSize = largestOptionNameSize();
  for (QMap<QString, QString>::const_iterator it = options.begin(); it != options.end(); ++it)
  {
    cout << "  " << it.key();
    const int numSpaces = largestSize - it.key().size();
    for (int i = 0; i < numSpaces; i++)
    {
      cout << " ";
    }
    cout << " - " << it.value() << endl;
  }
  cout << endl << "See the Hootenanny Developer Guide for more information." << endl;
}

_TestType getTestType(const QStringList& args)
{
  if (args.contains("--current"))
    return CURRENT;
  else if (args.contains("--quick"))
    return QUICK;
  else if (args.contains("--quick-only"))
    return QUICK_ONLY;
  else if (args.contains("--slow"))
    return SLOW;
  else if (args.contains("--slow-only"))
    return SLOW_ONLY;
  else if (args.contains("--all") || args.contains("--glacial"))
    return GLACIAL;
  else if (args.contains("--glacial-only"))
    return GLACIAL_ONLY;
  else if (args.contains("--case-only"))
    return CASE_ONLY;
  else
    return ALL;
}

_TimeOutValue getTimeoutValue(_TestType type)
{
  if (type & GLACIAL)
      return GLACIAL_WAIT;
  else if (type & SLOW)
    return SLOW_WAIT;
  else
    return QUICK_WAIT;
}

int main(int argc, char* argv[])
{
  // set the Qt hash seed to 0 for consistent test results
  conf().set(ConfigOptions().getHashSeedZeroKey(), true);
  qSetGlobalQHashSeed(0);

  if (argc == 1)
  {
    usage(argv[0]);
    return 1;
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

    const QStringList allowedOptions = getAllowedOptionNames();
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args.at(i).split("=")[0];
      if (arg.startsWith("--") && arg != "--listen" && !allowedOptions.contains(arg))
      {
        LOG_ERROR("Invalid HootTest option: " << arg);
        return 1;
      }
    }

    Log::getInstance().setLevel(Log::Warn);
    std::vector<TestPtr> vAllTests;
    std::vector<CppUnit::Test*> vTestsToRun;
    CppUnit::TextTestResult result;

    // initialize OSM Schema so the time expense doesn't print in other tests.
    OsmSchema::getInstance();

    std::shared_ptr<HootTestListener> listener;

    bool printDiff = args.contains("--diff");

    bool suppressFailureDetail = false;
    if (args.contains("--suppress-failure-detail"))
    {
      suppressFailureDetail = true;
      Log::getInstance().setLevel(Log::Error);
    }

    // Print all names & exit without running anything
    if (args.contains("--all-names"))
    {
      populateTests(getTestType(args), vAllTests, printDiff, suppressFailureDetail);
      getTestVector(vAllTests, vTestsToRun);
      includeExcludeTests(args, vTestsToRun);
      cout << "Test count: " << vTestsToRun.size() << endl;
      printNames(vTestsToRun);
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

      listener.reset(new HootTestListener(false, suppressFailureDetail, -1));
      result.addListener(listener.get());
      Log::getInstance().setLevel(Log::Status);
      populateTests(ALL, vAllTests, printDiff, suppressFailureDetail, true);
      CppUnit::Test* t = findTest(vAllTests, testName.toStdString());
      if (t == nullptr)
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

      listener.reset(new HootTestListener(false, suppressFailureDetail, slowTest, false));
      if (args.contains("--names"))
        listener->showTestNames(true);
      result.addListener(listener.get());

      string testName;
      cin >> testName;
      while (testName != HOOT_TEST_FINISHED)
      {
        populateTests(ALL, vAllTests, printDiff, suppressFailureDetail, true);
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
      _TestType type = getTestType(args);
      _TimeOutValue timeout = getTimeoutValue(type);
      if (type == CURRENT)
      {
        listener.reset(new HootTestListener(true, suppressFailureDetail, timeout));
        Log::getInstance().setLevel(Log::Status);
      }
      else
        listener.reset(new HootTestListener(false, suppressFailureDetail, timeout));
      //  Populate the list of tests
      populateTests(type, vAllTests, printDiff, suppressFailureDetail);

      vector<CppUnit::Test*> vTests;
      getTestVector(vAllTests, vTestsToRun);
      //  Include or exclude tests
      includeExcludeTests(args, vTestsToRun);

      cout << "Running core tests.  Test count: " << vTestsToRun.size() << endl;
    }

    //  Error out here is there is no HootTestListener created by this point
    if (!listener)
    {
      usage(argv[0]);
      return 1;
    }

    if (args.contains("--parallel"))
    {
      double start = Tgs::Time::getTime();

      int nproc = 1;
      //  With no number after --parallel use the number of online processors
      int nprocs_available = sysconf(_SC_NPROCESSORS_ONLN);
      int i = args.indexOf("--parallel") + 1;
      if (i >= args.size())
        nproc = nprocs_available;
      else
      {
        bool ok = false;
        nproc = args[i].toInt(&ok);
        if (!ok || nproc < 1)
          nproc = nprocs_available;
      }
      ProcessPool pool(nproc, listener->getTestTimeout(),
                       (bool)args.contains("--names"),
                       (bool)args.contains("--suppress-failure-detail"),
                       (bool)args.contains("--diff"));

      //  Get the names of all of the tests to run
      vector<string> allNames;
      getNames(allNames, vTestsToRun);
      set<string> nameCheck;
      for (vector<string>::iterator it = allNames.begin(); it != allNames.end(); ++it)
        nameCheck.insert(*it);

      //  Add all of the jobs that must be done serially and are a part of the selected tests
      vector<TestPtr> serialTests;
      populateTests(SERIAL, serialTests, printDiff, suppressFailureDetail, true);
      vector<CppUnit::Test*> vSerialTests;
      getTestVector(serialTests, vSerialTests);
      vector<string> serialNames;
      getNames(serialNames, vSerialTests);
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
