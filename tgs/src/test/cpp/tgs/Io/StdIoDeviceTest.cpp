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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Qt
#include <QDataStream>

// Standard
#include <sstream>
using namespace std;

#include <tgs/Io/StdIoDevice.h>

namespace Tgs
{

class StdIoDeviceTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(StdIoDeviceTest);
  CPPUNIT_TEST(runToyTest);
  CPPUNIT_TEST(runDataStreamTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void runToyTest()
  {
    const char* const data = "Hello World.";
    size_t dataSize = strlen(data) + 1;

    string s;
    s.resize(dataSize);
    memcpy((char*)s.data(), data, dataSize);
    stringstream ss(s, stringstream::in);

    StdIoDevice dev(ss);

    int count = 0;
    string result;
    char buffer[5];
    while (dev.atEnd() == false)
    {
      int s = dev.readData(buffer, 5);
      result.append(buffer, buffer + s);
      count += s;
    }
    CPPUNIT_ASSERT_EQUAL(s, result);
    CPPUNIT_ASSERT_EQUAL((int)s.size(), count);
  }

  void runDataStreamTest()
  {
    QByteArray arr;
    QDataStream ds(&arr, QIODevice::WriteOnly);
    QString hello("hello world.");
    ds << hello;

    string s;
    s.resize(arr.size());
    memcpy((char*)s.data(), arr.data(), arr.size());
    stringstream ss(s, stringstream::in);

    StdIoDevice dev(ss);
    QDataStream in(&dev);
    QString str;
    in >> str;
    CPPUNIT_ASSERT_EQUAL(hello.toStdString(), str.toStdString());
  }
};

//CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StdIoDeviceTest, "current");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(StdIoDeviceTest, "quick");

}
