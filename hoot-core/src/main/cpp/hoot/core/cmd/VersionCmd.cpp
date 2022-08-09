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
#include <hoot/core/algorithms/optimizer/IntegerProgrammingSolver.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/info/Version.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/OpenCv.h>

#ifdef HOOT_HAVE_STXXL
#include <stxxl/bits/version.h>
#endif

// CPP Unit
#ifdef HOOT_HAVE_LIBCPPUNIT
#include <cppunit/Portability.h>
#endif

// NodeJs
#include <node/node_version.h>

// v8
#include <v8-version-string.h>

// tgs
#include <tgs/System/SystemInfo.h>

// python
#include <patchlevel.h>

// Protobuf
#include <google/protobuf/stubs/common.h>

// PNG
#include <png.h>

//  LibPostgresql
#if HOOT_HAVE_POSTGRESQL
#include <pg_config.h>
#endif

//  LibICU
#if HOOT_HAVE_LIBICUI18N
#include <unicode/uversion.h>
#endif

//  Proj4
#include <proj.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class VersionCmd : public BaseCommand
{
public:

  static QString className() { return "VersionCmd"; }

  VersionCmd() = default;

  QString getName() const override { return "version"; }
  QString getDescription() const override { return "Displays version information"; }

  int runSimple(QStringList& args) override
  {
    if (!args.empty())
    {
      throw HootException(QString("%1 takes no parameters.").arg(getName()));
    }

    cout << Version::getFullVersion() << " Built By: " << Version::getBuiltBy() << endl;
    LOG_DEBUG("GEOS Version:\t" << geosversion());
    LOG_DEBUG("GDAL Version:\t" << GDALVersionInfo("RELEASE_NAME"));
    LOG_DEBUG("GLPK Version:\t" << glp_version());
    LOG_DEBUG("Qt Version:  \t" << qVersion());
    LOG_DEBUG("Boost Version:\t" << QString("%1.%2.%3")
              .arg(BOOST_VERSION / 100000)
              .arg(BOOST_VERSION / 100 % 1000)
              .arg(BOOST_VERSION % 100));
#   ifdef HOOT_HAVE_STXXL
      LOG_DEBUG("STXXL Version:\t" << stxxl::get_version_string());
#   endif
#   ifdef HOOT_HAVE_LIBCPPUNIT
      LOG_DEBUG("CppUnit Version:\t" << CPPUNIT_VERSION);
#   endif
    LOG_DEBUG("NodeJs Version:\t" << NODE_VERSION_STRING);
    LOG_DEBUG("v8 Version:  \t" << V8_VERSION_STRING);
    LOG_DEBUG("Python Version:\t" << PY_VERSION);
    LOG_DEBUG("Protobuf Version:\t" << QString("%1.%2.%3")
              .arg(GOOGLE_PROTOBUF_VERSION / 1000000)
              .arg(GOOGLE_PROTOBUF_VERSION / 1000 % 1000)
              .arg(GOOGLE_PROTOBUF_VERSION % 1000));
    LOG_DEBUG("OpenCv Version:\t" << CV_VERSION);
#   ifdef HOOT_HAVE_POSTGRESQL
      LOG_DEBUG("PostgreSQL Version:\t" << PG_VERSION);
#   endif
    LOG_DEBUG("LibPNG Version:\t" << PNG_LIBPNG_VER_STRING);
#   ifdef HOOT_HAVE_LIBICUI18N
      UVersionInfo info;
      char version[U_MAX_VERSION_STRING_LENGTH] = { '\0' };
      u_getVersion(info);
      u_versionToString(info, version);
      LOG_DEBUG("LibICU Version:\t" << version);
#   endif
    LOG_DEBUG("Proj4 Version:\t" << pj_release);
    //  Finish with the memory usage
    LOG_DEBUG(Tgs::SystemInfo::getCurrentProcessMemoryUsageString());
    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, VersionCmd)

}
