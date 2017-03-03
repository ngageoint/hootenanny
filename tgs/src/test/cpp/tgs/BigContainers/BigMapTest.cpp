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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// CPP Unit Includes
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard Includes
#include <stdlib.h>
#include <iostream>
#include <limits>
using namespace std;

// TGS Includes
#include <tgs/BigContainers/BigMap.h>
#include <tgs/Statistics/Random.h>
#include <tgs/System/Time.h>

namespace Tgs
{

class BigMapTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BigMapTest);
  CPPUNIT_TEST(simpleTest);
  //CPPUNIT_TEST(benchmarkTest);
  //CPPUNIT_TEST(runStxxlTest);
  CPPUNIT_TEST_SUITE_END();
public:

  void simpleTest()
  {
    BigMap<long, long> t;

    map<long, long> m;

    for (size_t i = 0; i < 100000; i++)
    {
      long k = Tgs::Random::instance()->generateInt();
      long v = Tgs::Random::instance()->generateInt();
      m[k] = v;
      t[k] = v;
    }

    for (map<long, long>::const_iterator it = m.begin(); it != m.end(); ++it)
    {
      CPPUNIT_ASSERT_EQUAL(it->second, t[it->first]);
    }

    BigMap<long, long>& ct = t;
    const map<long, long>& cm = m;
    for (size_t i = 0; i < 100000; i++)
    {
      long k = Tgs::Random::instance()->generateInt();
      CPPUNIT_ASSERT_EQUAL(cm.count(k), ct.count(k));
    }
  }

  struct CompareLess
  {
    bool operator() (const long& a, const long& b) const
    {
      return a < b;
    }

    static long max_value() { return std::numeric_limits<long>::max(); }
  };

//  void runStxxlTest()
//  {
////    stxxl::config* cfg = stxxl::config::get_instance();
////    cfg->
//    const int m = 1;
//    typedef stxxl::map<long, long, CompareLess, 4096 * m, 4096 * m> MapType;
//    MapType foo(1024 * 1024 * 512, 1024 * 1024 * 256);

//    typedef stxxl::VECTOR_GENERATOR<long>::result VectorType;
//    VectorType bar;

//    size_t iterations = 10000000;
//    bool doReadTest = false;
//    map<long, long> bax;
//    vector< pair<long, long> > pairs;
//    if (doReadTest)
//    {
//      pairs.reserve(iterations);
//    }

//    double start = Tgs::Time::getTime();
//    double lastStart = start;
//    int reportInterval = 100000;
//    cout << "Inserting...asdf" << endl;
//    for (size_t i = 0; i < iterations; i++)
//    {
//      long key = (long)Tgs::Random::instance()->generateInt() << 30 | Tgs::Random::instance()->generateInt();
//      long value = (long)Tgs::Random::instance()->generateInt() << 30 | Tgs::Random::instance()->generateInt();
//      if (doReadTest)
//      {
//        pairs.push_back(pair<long, long>(key, value));
//      }
//      //bar.push_back(Tgs::Random::instance()->generateInt());
//      foo.insert(std::pair<long, long>(key, value));
//      //foo[key] = value;
//      //foo.insert(std::pair<int, double>(key, value));
//      //bax[key] = value;


////      map<long, long>::const_iterator it = bax.find(keys[Tgs::Random::instance()->generateInt(keys.size())]);
////      assert(it != bax.end());

//      if (doReadTest)
//      {
//        size_t ki = Tgs::Random::instance()->generateInt(pairs.size());
//        assert(foo[pairs[ki].first] == pairs[ki].second);
//      }

//      if (i % reportInterval == 0)
//      {
//        double now = Tgs::Time::getTime();
//        cout << "Last iteration: " << 1000.0 * (now - lastStart) / (double)reportInterval << "ms" << endl;
//        lastStart = now;
//        cout << i << " since start: " << 1000.0 * (Tgs::Time::getTime() - start) / (double)i << "ms" << endl;
//      }
//    }
//    cout << "...Done " << Tgs::Time::getTime() - start << "sec" << endl;
//    foo.print_statistics(cout);

//    if (doReadTest)
//    {
//      start = Tgs::Time::getTime();
//      lastStart = start;
//      for (size_t i = 0; i < 10000000; i++)
//      {
//  //      map<long, long>::const_iterator it = bax.find(keys[Tgs::Random::instance()->generateInt(keys.size())]);
//  //      assert(it != bax.end());

//        //MapType::const_iterator it = constFoo.find(keys[Tgs::Random::instance()->generateInt(keys.size())]);
//        //assert(it != foo.end());
//        //assert(foo.count(keys[Tgs::Random::instance()->generateInt(keys.size())]) == 1);
//        size_t ki = Tgs::Random::instance()->generateInt(pairs.size());
//        assert(foo[pairs[ki].first] == pairs[ki].second);

//        if (i % reportInterval == 0)
//        {
//          double now = Tgs::Time::getTime();
//          cout << "Last iteration: " << 1000.0 * (now - lastStart) / (double)reportInterval << "ms" << endl;
//          lastStart = now;
//          cout << i << " since start: " << 1000.0 * (Tgs::Time::getTime() - start) / (double)i << "ms" << endl;
//        }
//      }
//      cout << "...Done " << Tgs::Time::getTime() - start << "sec" << endl;
//    }
//  }


//  void benchmarkTest()
//  {
//    size_t iterations = 10000000;
//    bool doReadTest = false;
//    //BigMap<long, long> foo;
//    typedef stxxl::map<long, long, CompareLess, 4096, 4096> MapType;
//    MapType bar(1024 * 1024 * 512, 1024 * 1024 * 256);

//    typedef stxxl::VECTOR_GENERATOR<long>::result VectorType;
//    VectorType v;

//    vector< pair<long, long> > pairs;
//    if (doReadTest)
//    {
//      pairs.reserve(iterations);
//    }

//    double start = Tgs::Time::getTime();
//    double lastStart = start;
//    int reportInterval = 100000;
//    cout << "Inserting...";
//    for (size_t i = 0; i < iterations; i++)
//    {
//      long key = (long)Tgs::Random::instance()->generateInt() << 30 | Tgs::Random::instance()->generateInt();
//      long value = (long)Tgs::Random::instance()->generateInt() << 30 | Tgs::Random::instance()->generateInt();
//      if (doReadTest)
//      {
//        pairs.push_back(pair<long, long>(key, value));
//      }
//      //bar.push_back(Tgs::Random::instance()->generateInt());
//      //foo[key] = value;
//      //foo.insert(key, value);
//      bar.insert(std::pair<long, long>(key, value));
//      //bax[key] = value;

//  //      map<long, long>::const_iterator it = bax.find(keys[Tgs::Random::instance()->generateInt(keys.size())]);
//  //      assert(it != bax.end());

//      if (doReadTest)
//      {
//        size_t ki = Tgs::Random::instance()->generateInt(pairs.size());
//        assert(bar.count(pairs[ki].first) == 1);
//        assert(bar.count(Tgs::Random::instance()->generateInt()) == 0);
//      }

//      if (i % reportInterval == 0)
//      {
//        double now = Tgs::Time::getTime();
//        cout << "Last iteration: " << 1000.0 * (now - lastStart) / (double)reportInterval << "ms" << endl;
//        lastStart = now;
//        cout << i << " since start: " << 1000.0 * (Tgs::Time::getTime() - start) / (double)i << "ms" << endl;
//      }
//    }
//    cout << "...Done " << Tgs::Time::getTime() - start << "sec" << endl;

//    if (doReadTest)
//    {
//      start = Tgs::Time::getTime();
//      lastStart = start;
//      for (size_t i = 0; i < 10000000; i++)
//      {
//        if (Tgs::Random::instance()->generateInt(2) == 0)
//        {
//          size_t ki = Tgs::Random::instance()->generateInt(pairs.size());
//          assert(bar[pairs[ki].first] == pairs[ki].second);
//        }
//        else
//        {
//          assert(bar.count(Tgs::Random::instance()->generateInt()) == 0);
//        }

//        if (i % reportInterval == 0)
//        {
//          double now = Tgs::Time::getTime();
//          cout << "Last iteration: " << 1000.0 * (now - lastStart) / (double)reportInterval << "ms" << endl;
//          lastStart = now;
//          cout << i << " since start: " << 1000.0 * (Tgs::Time::getTime() - start) / (double)i << "ms" << endl;
//        }
//      }
//      cout << "...Done " << Tgs::Time::getTime() - start << "sec" << endl;
//    }
//  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::BigMapTest, "quick");

