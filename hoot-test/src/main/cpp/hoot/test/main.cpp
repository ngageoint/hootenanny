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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <cppunit/ui/text/TestRunner.h>

// Hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/SignalCatcher.h>
#include <hoot/core/util/Settings.h>
using namespace hoot;

// Qt
#include <QLibrary>
#include <QtGui/QApplication>
#include <QString>
#include <QStringList>
#include <QDateTime>

// Standard
#include <iostream>
using namespace std;

// Tgs
#include <tgs/System/Time.h>

#include "ConflateCaseTestSuite.h"
#include "ScriptTestSuite.h"
#include <hoot/core/HootConfig.h>

class HootTestListener : public CppUnit::TestListener
{
public:

  HootTestListener(bool showTestName, double slowTest = 2.0)
  {
    _showTestName = showTestName;
    _slowTest = slowTest;
    _start = Tgs::Time::getTime();
    _allStart = _start;
    _success = true;
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
    if (elapsed > _slowTest && _slowTest >= 0.0)
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
    cout << endl;
    cout << "Elapsed: " << Tgs::Time::getTime() - _allStart << endl;
  }

private:
  bool _success;
  bool _showTestName;

  double _start;
  double _allStart;
  double _slowTest;
};

void filterPattern(CppUnit::Test* from, CppUnit::TestSuite* to, QString pattern,
  bool includeOnMatch)
{
  QRegExp regex(pattern);

  for (int i = 0; i < from->getChildTestCount(); i++)
  {
    CppUnit::Test* child = from->getChildTestAt(i);
    QString name = QString::fromStdString(child->getName());
    if (dynamic_cast<CppUnit::TestComposite*>(child))
    {
      filterPattern(child, to, pattern, includeOnMatch);
    }
    else if (regex.exactMatch(name) == includeOnMatch)
    {
      to->addTest(child);
    }
  }
}

CppUnit::Test* findTest(CppUnit::Test* t, QString name)
{
  if (QString::fromStdString(t->getName()) == name)
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

  return 0;
}

void printNames(CppUnit::Test* t)
{
  CppUnit::TestSuite* suite = dynamic_cast<CppUnit::TestSuite*>(t);
  if (suite != 0)
  {
    vector<CppUnit::Test*> children = suite->getTests();
    for (size_t i = 0; i < children.size(); ++i)
    {
      printNames(children[i]);
    }
  }
  else
  {
    cout << t->getName() << endl;
  }
}

void populateAllTests(CppUnit::TestSuite *suite, bool printDiff)
{
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  suite->addTest(registry.makeTest());
  suite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/current/", printDiff));
  suite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/quick/", printDiff));
  suite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/slow/", printDiff));
  suite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/glacial/", printDiff));
  suite->addTest(new ConflateCaseTestSuite("test-files/cases"));
  suite->addTest(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest());
  suite->addTest(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest());
  suite->addTest(CppUnit::TestFactoryRegistry::getRegistry("slow").makeTest());
  suite->addTest(CppUnit::TestFactoryRegistry::getRegistry("glacial").makeTest());
}

int main(int argc, char *argv[])
{
  Hoot::getInstance().init();

  QCoreApplication app(argc, argv);

  QStringList args;
  for (int i = 1; i < argc; i++)
  {
    args << argv[i];
  }

  Log::getInstance().setLevel(Log::Warn);
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestSuite *rootSuite = new CppUnit::TestSuite( "All tests" );

  // initialize OSM Schema so the time expense doesn't print in other tests.
  OsmSchema::getInstance();

  if (args.contains("--core") == false)
  {
#   if HOOT_HAVE_HADOOP
      // force load hoot hadoop if it is available.
      QLibrary libHootHadoop("HootHadoop");
      if (libHootHadoop.load() == false && getenv("HADOOP_HOME") != NULL)
      {
        // no biggie.
        LOG_WARN(libHootHadoop.errorString().toStdString());
      }
      // force load pretty pipes if it is available.
      QLibrary libPp("PrettyPipesExample");
      if (libPp.load() == false && getenv("HADOOP_HOME") != NULL)
      {
        // no biggie.
        LOG_WARN(libPp.errorString().toStdString());
      }
#   else
      LOG_WARN("Hadoop tests disabled.");
#   endif
    // force load R&D if it is available.
    QLibrary libHootRnd("HootRnd");
    if (libHootRnd.load() == false)
    {
      // no biggie.
      LOG_WARN(libHootRnd.errorString().toStdString());
    }
# ifdef HOOT_HAVE_NODEJS
    QLibrary libJs("HootJs");
    if (libJs.load() == false)
    {
      // no biggie.
      LOG_WARN(libJs.errorString().toStdString());
    }
#   else
      LOG_WARN("HootJs tests disabled.");
#endif
  }

  if (argc == 1)
  {
    cout << argv[0] << " Usage:\n"
            "--current - Run the 'current' tests.\n"
            "--quick - Run the quick (unnamed) tests and all above.\n"
            "--slow - Run the 'slow' tests and all above.\n"
            "--glacial - Run the 'glacial' tests and all above.\n"
            "--all - Run all the above tests.\n"
            "--core - Only load tests in hoot-core-test.\n"
            "--single [test name] - Run only the test specified.\n"
            "--names - Show the names of all the tests as they run.\n"
            "--all-names - Only print the names of all the tests.\n"
            "--warn - Show warning messages and above.\n"
            "--info - Show info messages and above.\n"
            "--debug - Show debug messages and above.\n"
            "--diff - Print diff when a script test fails.\n"
            "--include=[regex] - Include only tests that match the specified regex.\n"
            "--exclude=[regex] - Exclude tests that match the specified regex.\n"
            "\n"
            "See https://insightcloud.digitalglobe.com/redmine/projects/hootenany/wiki/Developer_-_Code_Testing\n"
            ;
  }
  else
  {
    HootTestListener* listener;

    bool printDiff = args.contains("--diff");

    CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    CppUnit::TestSuite *searchSuite = new CppUnit::TestSuite( "Search Tests" );
    if (args.contains("--all-names"))
    {
      populateAllTests(searchSuite, printDiff);
      printNames(searchSuite);
      delete searchSuite;
      return 0;
    }
    else if (args.contains("--single"))
    {
      int i = args.indexOf("--single") + 1;
      if (i >= args.size())
      {
        throw HootException("Expected a test name after --single.");
      }
      QString testName = args[i];

      listener = new HootTestListener(false, -1);
      Log::getInstance().setLevel(Log::Info);
      CppUnit::TestSuite *searchSuite = new CppUnit::TestSuite( "Search Tests" );
      populateAllTests(searchSuite, printDiff);

      CppUnit::Test* t = findTest(searchSuite, testName);
      if (t == 0)
      {
        throw HootException("Could not find the specified test: " + testName);
      }

      runner.addTest(t);
    }
    else
    {
      if (args.contains("--current"))
      {
        listener = new HootTestListener(true);
        Log::getInstance().setLevel(Log::Info);
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest());
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/current/",
          printDiff));
      }
      if (args.contains("--quick"))
      {
        listener = new HootTestListener(false, 1.0);
        rootSuite->addTest(registry.makeTest());
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/current/",
                                               printDiff));
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/quick/",
                                               printDiff));
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest());
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest());
      }
      if (args.contains("--slow"))
      {
        listener = new HootTestListener(false, 30.0);
        rootSuite->addTest(registry.makeTest());
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/current/",
                                               printDiff));
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/quick/",
                                               printDiff));
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/slow/",
                                               printDiff));
        rootSuite->addTest(new ConflateCaseTestSuite("test-files/cases"));
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest());
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest());
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("slow").makeTest());
      }
      if (args.contains("--all") || args.contains("--glacial"))
      {
        listener = new HootTestListener(false, 900.0);
        rootSuite->addTest(registry.makeTest());
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/current/",
                                               printDiff));
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/quick/",
                                               printDiff));
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/slow/",
                                               printDiff));
        rootSuite->addTest(new ScriptTestSuite("hoot-core-test/src/test/resources/cmd/glacial/",
                                               printDiff));
        rootSuite->addTest(new ConflateCaseTestSuite("test-files/cases"));
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("current").makeTest());
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("quick").makeTest());
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("slow").makeTest());
        rootSuite->addTest(CppUnit::TestFactoryRegistry::getRegistry("glacial").makeTest());
      }

      for (int i = 0; i < args.size(); i++)
      {
        if (args[i].startsWith("--exclude="))
        {
          CppUnit::TestSuite *newSuite = new CppUnit::TestSuite( "All tests" );
          int equalsPos = args[i].indexOf('=');
          QString regex = args[i].mid(equalsPos + 1);
          LOG_WARN("Excluding pattern: " << regex);
          filterPattern(rootSuite, newSuite, regex, false);
          rootSuite = newSuite;
        }
        else if (args[i].startsWith("--include="))
        {
          CppUnit::TestSuite *newSuite = new CppUnit::TestSuite( "All tests" );
          int equalsPos = args[i].indexOf('=');
          QString regex = args[i].mid(equalsPos + 1);
          LOG_WARN("Including only tests that match: " << regex);
          filterPattern(rootSuite, newSuite, regex, true);
          rootSuite = newSuite;
        }
      }

      runner.addTest(rootSuite);
      cout << "Test count: " << rootSuite->countTestCases() << endl;
    }

    CppUnit::TextTestResult result;

    if (args.contains("--debug"))
    {
      Log::getInstance().setLevel(Log::Debug);
    }
    if (args.contains("--info"))
    {
      Log::getInstance().setLevel(Log::Info);
    }
    if (args.contains("--warn"))
    {
      Log::getInstance().setLevel(Log::Warn);
    }
    if (args.contains("--names"))
    {
      listener->showTestNames(true);
    }

    // clear all user configuration so we have consistent tests.
    conf().clear();
    ConfigOptions::populateDefaults(conf());
    LOG_DEBUG("HOOT_HOME: " + QString(getenv("HOOT_HOME")))
    conf().set("HOOT_HOME", getenv("HOOT_HOME"));

    result.addListener(listener);
    runner.run(result);
    delete searchSuite;
    delete listener;
    return result.failures().size() > 0 ? -1 : 0;
  }
}
