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
 * @copyright Copyright (C) 2015, 2016, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Standard
#include <iostream>
using namespace std;

// TGS
#include <tgs/BigContainers/BasicBloomFilter.h>
#include <tgs/Statistics/Random.h>
#include <tgs/StreamUtils.h>

namespace Tgs
{

class BasicBloomFilterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(BasicBloomFilterTest);
  CPPUNIT_TEST(simpleTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void simpleTest()
  {
    using Bloom =
      boost::bloom_filters::basic_bloom_filter<long, 1000 * 1000 * 120, boost::mpl::vector<
      boost::bloom_filters::boost_hash<size_t, 0x327B23C66B8B4567>,
      boost::bloom_filters::boost_hash<size_t, 0x66334873643C9869>,
      boost::bloom_filters::boost_hash<size_t, 0x19495CFF74B0DC51>,
      boost::bloom_filters::boost_hash<size_t, 0x625558EC2AE8944A>,
      boost::bloom_filters::boost_hash<size_t, 0x46E87CCD238E1F29>,
      boost::bloom_filters::boost_hash<size_t, 0x507ED7AB3D1B58BA>,
      boost::bloom_filters::boost_hash<size_t, 0x41B71EFB2EB141F2>,
      boost::bloom_filters::boost_hash<size_t, 0x7545E14679E2A9E3>,
      boost::bloom_filters::boost_hash<size_t, 0x5BD062C2515F007C>,
      boost::bloom_filters::boost_hash<size_t, 0x4DB127F812200854>>>;
    //  NOTE: Bloom must be created on the heap and not the stack, thus the "new Bloom()"
    std::shared_ptr<Bloom> b = std::make_shared<Bloom>();

    b->insert(5);
    CPPUNIT_ASSERT_EQUAL(true, b->probably_contains(5));
    CPPUNIT_ASSERT_EQUAL(false, b->probably_contains(7));

    vector<long> ks;
    set<long> s;
    for (size_t i = 0; i < 100000; i++)
    {
      long k = Tgs::Random::instance()->generateInt();
      ks.push_back(k);
      s.insert(k);
      b->insert(k);
    }

    for (size_t i = 0; i < ks.size(); i++)
    {
      CPPUNIT_ASSERT_EQUAL(true, b->probably_contains(ks[i]));
    }
  }
};

}

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(Tgs::BasicBloomFilterTest, "quick");

