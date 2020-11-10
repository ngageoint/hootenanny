/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef MAPREDUCETESTFIXTURE_H
#define MAPREDUCETESTFIXTURE_H

// CPP Unit
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestFixture.h>

// Pretty Pipes
#include <pp/mapreduce/Job.h>

namespace hoot
{

class MapReduceTestFixture : public CppUnit::TestFixture
{
public:
  std::string jobTracker;

  virtual void setUp()
  {
    jobTracker = Job::getDefaultJobTracker();
    Job::setDefaultJobTracker("local");
  }

  virtual void tearDown()
  {
    Job::setDefaultJobTracker(jobTracker);
  }
};

}

#endif // MAPREDUCETESTFIXTURE_H
