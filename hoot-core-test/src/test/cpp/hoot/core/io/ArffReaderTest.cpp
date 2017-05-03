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

// Boost
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

// Hoot
#include <hoot/core/io/ArffReader.h>
#include <hoot/core/scoring/DataSamples.h>
#include <hoot/core/scoring/MatchFeatureExtractor.h>

// Qt
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class ArffReaderTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(ArffReaderTest);
  CPPUNIT_TEST(runBasicTest);
  CPPUNIT_TEST(runBz2Test);
  CPPUNIT_TEST_SUITE_END();

public:

  void runBasicTest()
  {
    DataSamples samples;

    ArffReader uut("test-files/io/ArffReaderTest/runBasic.arff");
    boost::shared_ptr<DataSamples> ds = uut.read();

    // check for consistency with previous versions.
    HOOT_STR_EQUALS(
      "[3]{[3]{(a, 1.1), (b, 1.2), (class, 1)}, [4]{(a, 2.1), (b, 2.2), (c, 2.3), (class, 0)}, [2]{(b, 3.2), (class, 2)}}",
      *ds);
  }

  void runBz2Test()
  {
    DataSamples samples;

    ArffReader uut("test-files/io/ArffReaderTest/runBz2.arff.bz2");
    boost::shared_ptr<DataSamples> ds = uut.read();

    // check for consistency with previous versions.
    HOOT_STR_EQUALS(
      "[3]{[3]{(a, 1.1), (b, 1.2), (class, 1)}, [4]{(a, 2.1), (b, 2.2), (c, 2.3), (class, 0)}, [2]{(b, 3.2), (class, 2)}}",
      *ds);
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ArffReaderTest, "quick");

}
