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

// Boost
#include <boost/version.hpp>

// GDAL
#include <gdal.h>

// GEOS
// contains geosversion()
#include <geos/geom/Geometry.h>
#include <geos/version.h>

// Hoot
#include <hoot/core/HootConfig.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/Version.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/algorithms/optimizer/IntegerProgrammingSolver.h>

// Qt
#include <Qt>
#include <QStringList>

#ifdef HOOT_HAVE_STXXL
#include <stxxl/bits/version.h>
#endif

// CPP Unit
#ifdef HOOT_HAVE_LIBCPPUNIT
#include <cppunit/Portability.h>
#endif

// tgs
#include <tgs/System/SystemInfo.h>

namespace hoot
{

class VersionCmd : public BaseCommand
{
public:
  static string className() { return "hoot::VersionCmd"; }

  VersionCmd() {}

  virtual QString getName() const { return "version"; }

  int runSimple(QStringList args)
  {
    if (args.size() != 0)
    {
      throw HootException(QString("%1 takes no parameters.").arg(getName()));
    }

    cout << Version::getFullVersion() << " Built By: " << Version::getBuiltBy() << endl;
    LOG_DEBUG("GEOS Version: " << geosversion());
    LOG_DEBUG("GDAL Version: " << GDALVersionInfo("RELEASE_NAME"));
    LOG_DEBUG("GLPK Version: " << glp_version());
    LOG_DEBUG("Qt Version: " << qVersion());
    LOG_DEBUG("Boost Version: " << QString("%1.%2.%3")
              .arg(BOOST_VERSION / 100000)
              .arg(BOOST_VERSION / 100 % 1000)
              .arg(BOOST_VERSION % 100));
#   ifdef HOOT_HAVE_STXXL
      LOG_DEBUG("STXXL Version: " << stxxl::get_version_string());
#   endif
#   ifdef HOOT_HAVE_LIBCPPUNIT
      LOG_DEBUG("CppUnit Version: " << CPPUNIT_VERSION);
#   endif

    LOG_DEBUG(Tgs::SystemInfo::getMemoryUsageString());

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, VersionCmd)

}
