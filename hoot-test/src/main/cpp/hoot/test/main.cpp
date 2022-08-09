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
#include <hoot/core/TestUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/SignalCatcher.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/StringUtils.h>

#include <hoot/js/v8Engine.h>

#include <hoot/test/ConflateCaseTestSuite.h>
#include <hoot/test/ProcessPool.h>
#include <hoot/test/ScriptTestSuite.h>
#include <hoot/test/cmd/TestCommand.h>

// Qt
#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QString>
#include <QStringList>

// Standard
#include <iostream>
#include <map>
#include <unistd.h>
#include <vector>

// Tgs
#include <tgs/System/Time.h>

using namespace geos::geom;
using namespace hoot;
using namespace std;

using TestPtr = std::shared_ptr<CppUnit::Test>;

enum _TestType
{
  NONE           = 0x00,
  CURRENT        = 0x01,
  QUICK_ONLY     = 0x02,
  QUICK          = CURRENT | QUICK_ONLY,
  SERIAL         = 0x04,
  CASE_TESTS     = 0x08,
  CASE_ONLY      = CASE_TESTS | SERIAL,
  VALIDATED_ONLY = 0x10,
  SLOW_TESTS     = 0x20,
  SLOW_ONLY      = SLOW_TESTS | SERIAL | CASE_ONLY,
  SLOW           = SLOW_ONLY | QUICK,
  GLACIAL_TESTS  = 0x40,
  GLACIAL_ONLY   = GLACIAL_TESTS | SERIAL,
  GLACIAL        = GLACIAL_ONLY | SLOW
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

  void addFailure(const CppUnit::TestFailure& failure) override
  {
    cout << endl << "Failure: " << failure.failedTest()->getName() << endl;
    if (!_suppressFailureDetail)
    {
      cout << "  " << failure.sourceLine().fileName() << "("
           << failure.sourceLine().lineNumber() << ") ";
      CppUnit::Exception* e = failure.thrownException();
      if (e != nullptr && QString::fromStdString(e->message().details()).trimmed() != "")
        cout << "  " << e->message().details();
    }
    cout.flush();
    _success = false;
  }

  bool getSuccess() const
  {
    return _success;
  }

  void endTest(CppUnit::Test* test) override
  {
    double elapsed = Tgs::Time::getTime() - _start;
    if (_showTestName)
      cout << test->getName() << " - " << elapsed << endl;
    if (elapsed > _testTimeout && _testTimeout >= 0.0)
    {
      cout << "Test " << test->getName().data() << " ran longer than expected -- "
           << elapsed << endl;
    }
    cout.flush();

    _start = Tgs::Time::getTime();
  }

  void showTestNames(bool show) { _showTestName = show; }

  void startSuite(CppUnit::Test* suite) override
  {
    if (_showTestName)
      cout << "Starting " << suite->getName().data() << endl << flush;
  }

  void endTestRun(CppUnit::Test* /*test*/, CppUnit::TestResult* /*eventManager*/) override
  {
    if (_showElapsed)
      cout << endl << "Elapsed: " << Tgs::Time::getTime() - _allStart << endl;
  }

  double getTestTimeout() const { return _testTimeout; }

private:

  bool _success;
  bool _showTestName;
  bool _suppressFailureDetail;
  bool _showElapsed;

  double _start;
  double _allStart;
  double _testTimeout;
};

void init()
{
  v8Engine::getInstance();
  // initialize OSM Schema so the time expense doesn't print in other tests.
  OsmSchema::getInstance();
}

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
  for (const auto& test : from)
  {
    CppUnit::Test* child = test.get();
    if (dynamic_cast<CppUnit::TestComposite*>(child))
      getTestVector(child, to);
    else
      to.push_back(child);
  }
}

void filterPattern(const std::vector<CppUnit::Test*>& from,
                   std::vector<CppUnit::Test*>& to, QString pattern,
                   bool includeOnMatch)
{
  QRegExp regex(pattern);

  for (const auto& child : from)
  {
    QString name = QString::fromStdString(child->getName());
    if (regex.exactMatch(name) == includeOnMatch)
      to.push_back(child);
  }
}

void includeExcludeTests(const QStringList& args, vector<CppUnit::Test*>& vTests)
{
  vector<CppUnit::Test*> vTestsToRun;
  bool filtered = false;

  for (const auto& arg : args)
  {
    if (arg.startsWith("--exclude="))
    {
      if (vTestsToRun.size() > 0)
      {
        //  On the second (or more) time around exclude from the excluded list
        vTests.swap(vTestsToRun);
        vTestsToRun.clear();
      }
      int equalsPos = arg.indexOf('=');
      QString regex = arg.mid(equalsPos + 1);
      LOG_INFO("Excluding pattern: " << regex);
      filterPattern(vTests, vTestsToRun, regex, false);
      filtered = true;
    }
    else if (arg.startsWith("--include="))
    {
      int equalsPos = arg.indexOf('=');
      QString regex = arg.mid(equalsPos + 1);
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
  if (name == t->getName())
    return t;

  CppUnit::TestSuite* suite = dynamic_cast<CppUnit::TestSuite*>(t);
  if (suite != 0)
  {
    vector<CppUnit::Test*> children = suite->getTests();
    for (auto child : children)
    {
      CppUnit::Test* result = findTest(child, name);
      if (result != 0)
        return result;
    }
  }

  return nullptr;
}

CppUnit::Test* findTest(std::vector<TestPtr> vTests, std::string name)
{
  for (const auto& test : vTests)
  {
    if (name == test->getName())
      return test.get();
    // Check to see if it's a suite of tests...
    CppUnit::TestSuite* suite = dynamic_cast<CppUnit::TestSuite*>(test.get());
    if (suite != 0)
    {
      vector<CppUnit::Test*> children = suite->getTests();
      for (auto child : children)
      {
        CppUnit::Test* result = findTest(child, name);
        if (result != 0)
          return result;
      }
    }
  }
  return nullptr;
}

void getNames(vector<string>& names, CppUnit::Test* t)
{
  CppUnit::TestSuite* suite = dynamic_cast<CppUnit::TestSuite*>(t);
  if (suite != nullptr)
  {
    vector<CppUnit::Test*> children = suite->getTests();
    for (auto child : children)
      getNames(names, child);
  }
  else if (t != nullptr)
    names.push_back(t->getName());
}

void getNames(vector<string>& names, const std::vector<CppUnit::Test*>& vTests)
{
  for (auto test : vTests)
    getNames(names, test);
}

void printNames(const std::vector<CppUnit::Test*>& vTests)
{
  vector<string> names;
  getNames(names, vTests);
  for (const auto& test : names)
    cout << test << endl;
}

const QMap<QString, QMap<QString, QString>>& getAllOptions()
{
  static QMap<QString, QMap<QString, QString>> options;

  if (options.size() == 0)
  {
    QMap<QString, QString> log_level;
    log_level["--debug"] = "Show debug level log messages and above";
    log_level["--error"] = "Show error log level messages and above";
    log_level["--fatal"] = "Show fatal error log level messages only";
    log_level["--info"] = "Show info log level messages and above";
    log_level["--status"] = "Show status log level messages and above";
    log_level["--trace"] = "Show trace log level messages and above";
    log_level["--verbose"] = "Show verbose log level messages and above";
    log_level["--warn"] = "Show warning log level messages and above";
    options["Log Level"] = log_level;

    QMap<QString, QString> test_list;
    test_list["--all"] = "Run all the tests";
    test_list["--case-only"] = "Run the conflate case tests only";
    test_list["--current"] = "Run the 'current' level tests";
    test_list["--glacial"] = "Run 'glacial' level tests and below";
    test_list["--glacial-only"] = "Run the 'glacial' level tests only";
    test_list["--quick"] = "Run the 'quick' level' tests";
    test_list["--quick-only"] = "Run the 'quick' level tests only";
    test_list["--slow"] = "Run the 'slow' level tests and above";
    test_list["--slow-only"] = "Run the 'slow' level tests only";
    // Don't show this option if we're not configured to validate test output.
    if (ConfigOptions().getTestValidationEnable())
    {
      test_list["--validated-only"] = "Run only tests where some or all of the output is validated";
    }
    test_list["--single [test name]"] = "Run only the test specified";
    test_list["--exclude=[regex]"] =
      "Exclude tests that match the specified regex; e.g. HootTest '--exclude=.*building.*'";
    test_list["--include=[regex]"] =
      "Include only tests that match the specified regex; e.g. HootTest '--include=.*building.*'";
    options["Test List"] = test_list;

    QMap<QString, QString> run_parallel;
    run_parallel["--parallel [process count]"] =
      "Run the specified tests in parallel with the specified number of processes";
    options["Run in parallel"] = run_parallel;

    QMap<QString, QString> advanced;
    advanced["--all-names"] = "Print the names of all the tests without running them";
    advanced["--check-env"] = "Check the environment before and after the test is run, good for debugging";
    advanced["--diff"] = "Print a diff when a script test fails";
    advanced["--disable-failure-retries"] = "Disables retrying test runs when they fail in parallel";
    advanced["--names"] = "Show the names of all the tests as they run";
    advanced["--suppress-failure-detail"] =
      "Do not show test failure detailed messages; disables --diff for script tests";
    options["Advanced"] = advanced;
  }
  return options;
}

QMap<QString, QString> getAllowedOptions()
{
  const QMap<QString, QMap<QString, QString>>& all_options = getAllOptions();
  QMap<QString, QString> options;
  for (const auto& category : all_options)
  {
    for (auto entry = category.begin(); entry != category.end(); ++entry)
      options.insert(entry.key(), entry.value());
  }
  return options;
}

QStringList getAllowedOptionNames()
{
  QStringList optionNames;
  const QMap<QString, QString> options = getAllowedOptions();
  for (auto option = options.begin(); option != options.end(); ++option)
    optionNames.append(option.key().split(QRegExp("\\s+|="))[0]);
  return optionNames;
}

void setupTestingConfig(QStringList& args)
{
  //  Clear all user configuration so we have consistent tests.
  conf().clear();
  ConfigOptions::populateDefaults(conf());
  conf().set("HOOT_HOME", getenv("HOOT_HOME"));
  StringUtils::removeAllContaining(args, "hoottest");
  LOG_VART(args);
  // Settings doesn't recognize all HootTest specific options, so pass in a list from here to
  // ignore. Don't worry about getAllowedOptionNames() returning the log level options to be
  // ignored...the Settings class will parse them anyway.
  Settings::parseCommonArguments(args, getAllowedOptionNames());
  LOG_VART(args);
  // We require that all tests use Testing.conf as a starting point and any conf values specified by
  // it may be overridden when necessary.
  conf().loadJson(ConfPath::search("Testing.conf"));
}

void runSingleTest(CppUnit::Test* pTest, QStringList& args, CppUnit::TextTestResult* pResult)
{
  //  Setup the testing config
  setupTestingConfig(args);
  //  before running the test
  pTest->run(pResult);
}

void populateTests(_TestType t, std::vector<TestPtr>& vTests, bool printDiff,
                   bool suppressFailureDetail, bool hideDisableTests = false)
{
  bool printValidationReportDiff = false;
  if (ConfigOptions().getTestValidationEnable() && printDiff)
    printValidationReportDiff = true;

  //  Add glacial tests if the bit flag is set
  if (t & GLACIAL_TESTS)
  {
    vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/glacial/", printDiff, GLACIAL_WAIT,
                                                       hideDisableTests, suppressFailureDetail, false));
    if (t & SERIAL)
    {
      vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/glacial/serial/", printDiff, GLACIAL_WAIT,
                                                         hideDisableTests, suppressFailureDetail, false));
    }
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("glacial").makeTest()));
  }
  //  Slow tests are included in SLOW and GLACIAL; Add slow tests if the bit flag is set
  if (t & SLOW_TESTS)
  {
    vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/slow/", printDiff, SLOW_WAIT,
                                                       hideDisableTests, suppressFailureDetail, false));
    if (t & SERIAL)
    {
      vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/slow/serial/", printDiff, SLOW_WAIT,
                                                         hideDisableTests, suppressFailureDetail, false));
    }
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("slow").makeTest()));
  }
  //  Quick tests are included in QUICK, SLOW, and GLACIAL; Add quick tests if the bit flag is set
  if (t & QUICK_ONLY)
  {
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry().makeTest()));
    vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/quick/", printDiff, QUICK_WAIT,
                                                       hideDisableTests, suppressFailureDetail, false));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest()));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("TgsTest").makeTest()));
  }
  //  Current tests are included in CURRENT, QUICK, SLOW, and GLACIAL; Add current tests if the bit
  //  flag is set
  if (t & CURRENT)
  {
    vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/current/", printDiff, QUICK_WAIT,
                                                       hideDisableTests, suppressFailureDetail, false));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest()));
  }
  //  Add serial tests if the bit flag is set
  if (t == SERIAL)
  {
    vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/glacial/serial/", printDiff, GLACIAL_WAIT,
                                                       hideDisableTests, suppressFailureDetail, false));
    vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/slow/serial/", printDiff, SLOW_WAIT,
                                                       hideDisableTests, suppressFailureDetail, false));
    vTests.push_back(TestPtr(CppUnit::TestFactoryRegistry::getRegistry("serial").makeTest()));
    vTests.push_back(std::make_shared<ConflateCaseTestSuite>("test-files/cases/serial", suppressFailureDetail,
                                                             printValidationReportDiff, hideDisableTests, true));
  }
  //  Test cases go last because of the way they change the environment.
  if (t & CASE_TESTS)
  {
    vTests.push_back(std::make_shared<ConflateCaseTestSuite>("test-files/cases", suppressFailureDetail,
                                                             printValidationReportDiff, hideDisableTests));
    if (t & SERIAL)
    {
      vTests.push_back(std::make_shared<ConflateCaseTestSuite>("test-files/cases/serial", suppressFailureDetail,
                                                               printValidationReportDiff, hideDisableTests, true));
    }
  }
  // If test output validation is enabled, then we want to enable only those tests that use it. At
  // this time that includes all case tests (includes serial) and selected script tests from the
  // slow and glacial suites (none of which happen to be serial tests). As script tests are
  // updated to run output validation, this may need to be updated. Unfortunately, the displayed
  // test count won't be accurate since ScriptTestSuite filters out tests that don't run validation.
  // No need to check test.validation.enable here as the --validated-only command line option is
  // suppressed from availability before this point if the option isn't enabled.
  if (t == VALIDATED_ONLY)
  {
    printValidationReportDiff = printDiff;
    vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/glacial/", printValidationReportDiff, GLACIAL_WAIT,
                                                       hideDisableTests, suppressFailureDetail, true));
    vTests.push_back(std::make_shared<ScriptTestSuite>("test-files/cmd/slow/", printValidationReportDiff, SLOW_WAIT,
                                                       hideDisableTests, suppressFailureDetail, true));
    // Run case tests last for the reason noted previously.
    vTests.push_back(std::make_shared<ConflateCaseTestSuite>("test-files/cases", suppressFailureDetail,
                                                             printValidationReportDiff, hideDisableTests));
    vTests.push_back(std::make_shared<ConflateCaseTestSuite>("test-files/cases/serial", suppressFailureDetail,
                                                             printValidationReportDiff, hideDisableTests, true));
  }
}

int largestOptionNameSize()
{
  int largest = 0;
  const QMap<QString, QString> options = getAllowedOptions();
  for (auto it = options.begin(); it != options.end(); ++it)
  {
    if (it.key().size() > largest)
      largest = it.key().size();
  }
  return largest;
}

void usage(char* argv)
{
  QString commandName(argv);
  QFileInfo commandInfo(commandName);
  commandName = commandInfo.baseName();
  cout << commandName << " Usage:" << endl << endl;
  cout << commandName << " [log level] [test list] [--parallel] [advanced]" << endl << endl;
  const QMap<QString, QMap<QString, QString>>& options = getAllOptions();
  const int largestSize = largestOptionNameSize();
  for (auto category = options.begin(); category != options.end(); ++category)
  {
    cout << category.key() << ":" << endl;
    for (auto entry = category->begin(); entry != category->end(); ++entry)
    {
      cout << "  " << entry.key();
      const int numSpaces = largestSize - entry.key().size();
      for (int i = 0; i < numSpaces; i++)
        cout << " ";
      cout << " - " << entry.value() << endl;
    }
    cout << endl;
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
  else if (args.contains("--glacial-only"))
    return GLACIAL_ONLY;
  else if (args.contains("--case-only"))
    return CASE_ONLY;
  else if (args.contains("--validated-only"))
    return VALIDATED_ONLY;
  else // --all or --glacial
    return GLACIAL;
}

_TimeOutValue getTimeoutValue(_TestType type)
{
  if (type & GLACIAL_ONLY)
    return GLACIAL_WAIT;
  else if (type & SLOW_ONLY)
    return SLOW_WAIT;
  else if (type == VALIDATED_ONLY)
    return GLACIAL_WAIT;
  else
    return QUICK_WAIT;
}

void reportFailedTests(int failedTests, int totalTests)
{
  if (totalTests < 1 || failedTests > totalTests || failedTests == 0)
    return;
  //  Report the number of failed tests
  cout << failedTests << " of " << totalTests << " tests failed" << endl;
}

void verifyValidationConfig()
{
  // Script tests require a dummy file written out so that they know its ok to run validation, since
  // they can't do the compile time check.
  const QString testValidationEnabledFile = "test-output/test-validation-enabled";
  if (ConfigOptions().getTestValidationEnable())
  {
    if (!QFile::exists(testValidationEnabledFile))
      FileUtils::writeFully(testValidationEnabledFile, "");
  }
  else
  {
    if (QFile::exists(testValidationEnabledFile) && !QFile::remove(testValidationEnabledFile))
      throw TestConfigurationException("Unable to remove: " + testValidationEnabledFile);
  }
}

std::shared_ptr<TestCommand> parseTestCommand(char* argv[])
{
  vector<QString> cmds = Factory::getInstance().getObjectNamesByBase(TestCommand::className());
  std::shared_ptr<TestCommand> command;
  for (size_t i = 0; i < cmds.size(); i++)
  {
    command = Factory::getInstance().constructObject<TestCommand>(cmds[i]);
    QString argName = command->getName();
    if (QString(argv[1]) == argName)
      break;
    else
      command.reset();
  }
  return command;
}

int main(int argc, char* argv[])
{
  if (argc == 1)
  {
    usage(argv[0]);
    return 1;
  }
  else
  {
    // Set the Qt hash seed to 0 for consistent test results.
    conf().set(ConfigOptions().getHashSeedZeroKey(), true);
    qSetGlobalQHashSeed(0);

    // Init hoot, as it handles loading any configured submodules.
    Hoot::getInstance();
    Log::getInstance().setLevel(Log::Warn);

    QCoreApplication app(argc, argv);

    // HootTest has some utility commands that don't actually run the tests. Check for those first
    // before running the tests. Each command will set up its own config.
    std::shared_ptr<TestCommand> command = parseTestCommand(argv);
    if (command != nullptr)
    {
      try
      {
        return command->run(argv, argc);
      }
      catch (const std::exception& e)
      {
        cerr << "Error running " << command->getName().toStdString() << ":" << endl;
        cerr << e.what() << endl;
        return -1;
      }
    }

    // Determine whether we validating any test output or not and set the environment up
    // appropriately.
    verifyValidationConfig();

    QStringList args = BaseCommand::toQStringList(argv, argc);
    LOG_VART(args);

    // This will run on a subsequent pass as part of --listen if --parallel was used.
    if (!args.contains("--parallel"))
      init();

    const QStringList allowedOptions = getAllowedOptionNames();
    for (auto it = args.begin(); it != args.end(); ++it)
    {
      const QString arg = it->split("=")[0];
      if (arg.startsWith("--") && arg != "--listen" && !allowedOptions.contains(arg))
      {
        LOG_ERROR("Invalid HootTest option: " << arg);
        return 1;
      }
    }

    std::vector<TestPtr> vAllTests;
    std::vector<CppUnit::Test*> vTestsToRun;
    CppUnit::TextTestResult result;

    std::shared_ptr<HootTestListener> listener;

    bool printDiff = args.contains("--diff");

    bool suppressFailureDetail = false;
    if (args.contains("--suppress-failure-detail"))
    {
      suppressFailureDetail = true;
      Log::getInstance().setLevel(Log::Error);
    }

    // Print all names & exit without running anything.
    if (args.contains("--all-names"))
    {
      populateTests(getTestType(args), vAllTests, printDiff, suppressFailureDetail);
      getTestVector(vAllTests, vTestsToRun);
      includeExcludeTests(args, vTestsToRun);
      cout << "Test count: " << vTestsToRun.size() << endl;
      printNames(vTestsToRun);
      return 0;
    }

    // Setup the environment checks if requested.
    if (args.contains("--check-env"))
      HootTestFixture::setCompareEnv(true);

    // Run a single test
    if (args.contains("--single"))
    {
      int i = args.indexOf("--single") + 1;
      if (i >= args.size())
        throw HootException("Expected a test name after --single.");

      QString testName = args[i];

      listener = std::make_shared<HootTestListener>(false, suppressFailureDetail, -1);
      result.addListener(listener.get());
      Log::getInstance().setLevel(Log::Status);
      populateTests(GLACIAL, vAllTests, printDiff, suppressFailureDetail, true);
      CppUnit::Test* t = findTest(vAllTests, testName.toStdString());
      if (t == nullptr)
      {
        cout << "Could not find the specified test: " << testName.toStdString() << endl;
        return -1;
      }

      runSingleTest(t, args, &result);
      if (result.failures().size() == 0)
        cout << endl;
      return result.failures().size() > 0 ? -1 : 0;
    }
    else if (args.contains("--listen"))
    {
      double slowTest = -1;
      int i = args.indexOf("--listen") + 1;
      if (i < args.size())
        slowTest = args[i].toDouble();

      listener = std::make_shared<HootTestListener>(false, suppressFailureDetail, slowTest, false);
      if (args.contains("--names"))
        listener->showTestNames(true);
      result.addListener(listener.get());

      string testName;
      cin >> testName;
      while (testName != HOOT_TEST_FINISHED)
      {
        populateTests(GLACIAL, vAllTests, printDiff, suppressFailureDetail, true);
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
        listener = std::make_shared<HootTestListener>(true, suppressFailureDetail, timeout);
        Log::getInstance().setLevel(Log::Status);
      }
      else
        listener = std::make_shared<HootTestListener>(false, suppressFailureDetail, timeout);
      //  Populate the list of tests
      populateTests(type, vAllTests, printDiff, suppressFailureDetail);

      vector<CppUnit::Test*> vTests;
      getTestVector(vAllTests, vTestsToRun);
      //  Include or exclude tests
      includeExcludeTests(args, vTestsToRun);

      cout << "Running core tests.  Test count: " << vTestsToRun.size() << endl;
    }

    // Error out here as there is no HootTestListener created by this point.
    if (!listener)
    {
      usage(argv[0]);
      return 1;
    }

    if (args.contains("--parallel"))
    {
      double start = Tgs::Time::getTime();

      int nproc = 1;
      //  With no number after --parallel, use the number of online processors.
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
                       (bool)args.contains("--diff"),
                       (bool)args.contains("--disable-failure-retries"));

      //  Get the names of all of the tests to run
      vector<string> allNames;
      getNames(allNames, vTestsToRun);
      set<string> nameCheck(allNames.begin(), allNames.end());

      //  Get a list of all conflate cases jobs that must be processed last.
      vector<TestPtr> conflateCases;
      populateTests(CASE_ONLY, conflateCases, printDiff, suppressFailureDetail, true);
      vector<CppUnit::Test*> vConflateCases;
      getTestVector(conflateCases, vConflateCases);
      vector<string> casesNames;
      getNames(casesNames, vConflateCases);
      set<string> casesSet(casesNames.begin(), casesNames.end());

      //  Add all of the jobs that must be done serially and are a part of the selected tests.
      vector<TestPtr> serialTests;
      populateTests(SERIAL, serialTests, printDiff, suppressFailureDetail, true);
      vector<CppUnit::Test*> vSerialTests;
      getTestVector(serialTests, vSerialTests);
      vector<string> serialNames;
      getNames(serialNames, vSerialTests);
      for (const auto& name : serialNames)
      {
        if (nameCheck.find(name) != nameCheck.end())
        {
          //  Insert the serial job or serial case job into the pool
          ProcessPool::JobType t = ProcessPool::SerialJob;
          if (casesSet.find(name) != casesSet.end())
            t = static_cast<ProcessPool::JobType>(ProcessPool::SerialJob | ProcessPool::ConflateJob);
          pool.addJob(QString(name.c_str()), t);
        }
      }
      //  Add all of the remaining non-case jobs in the test suite.
      for (const auto& name : allNames)
      {
        const QString testName = QString(name.c_str());
        //  Jobs in the casesSet are ConflateJob's and the rest are ParallelJob's.
        pool.addJob(testName, (casesSet.find(name) != casesSet.end()) ? ProcessPool::ConflateJob : ProcessPool::ParallelJob);
      }

      pool.startProcessing();
      pool.wait();

      cout << endl;
      reportFailedTests(pool.getFailures(), allNames.size());
      cout << "Elapsed: " << Tgs::Time::getTime() - start << endl;
      return pool.getFailures() > 0 ? -1 : 0;
    }
    else
    {
      double start = Tgs::Time::getTime();

      if (args.contains("--names"))
        listener->showTestNames(true);

      //  Setup the testing configs.
      setupTestingConfig(args);

      // set up listener
      result.addListener(listener.get());

      // run all tests
      for (size_t i = 0; i < vTestsToRun.size(); i++)
        vTestsToRun[i]->run(&result);

      cout << endl;
      reportFailedTests(result.failures().size(), vTestsToRun.size());
      cout << "Elapsed: " << Tgs::Time::getTime() - start << endl;
      return result.failures().size() > 0 ? -1 : 0;
    }
  }
}
