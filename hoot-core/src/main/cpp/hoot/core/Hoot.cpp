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
#include "Hoot.h"

// GDAL
#include <gdal.h>

// GEOS
// contains geosversion()
#include <geos/geom/Geometry.h>
#include <geos/version.h>

// hoot
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/SignalCatcher.h>

// Qt
#include <QString>

// System
#include <sys/resource.h>

// Tgs
#include <tgs/TgsConfig.h>
#ifdef TGS_HAVE_LIBSTXXL
# include <tgs/BigContainers/Stxxl.h>
#endif

namespace hoot
{

auto_ptr<Hoot> Hoot::_theInstance;

Hoot::Hoot()
{
  _init();
}

Hoot& Hoot::getInstance()
{
  if (_theInstance.get() == 0)
  {
    _theInstance.reset(new Hoot());
  }
  return *_theInstance;
}

void Hoot::_init()
{
# ifdef TGS_HAVE_LIBSTXXL
    // initialize the environment variable for loading STXXL configuration. If the environment
    // variable has already been set then don't overwrite it (that is the 0 at the end).
    QString stxxlConf = QString(getenv("HOOT_HOME")) + "/conf/stxxl.conf";
    Tgs::Stxxl::getInstance().setConfig(stxxlConf);
# endif

  SignalCatcher::registerHandlers();

  Log::getInstance().setLevel(Log::Info);

  // make sure our GDAL versions are consistent.
  const char* gdalVersion = GDALVersionInfo("RELEASE_NAME");
  if (gdalVersion != QString(GDAL_RELEASE_NAME))
  {
    LOG_ERROR("GDAL Version mismatch. Runtime: " << gdalVersion << " Build time: " <<
              GDAL_RELEASE_NAME);
  }

  std::string geosVersion = geos::geom::geosversion();
  if (QString::fromStdString(geosVersion) != QString(GEOS_VERSION))
  {
    LOG_ERROR("GEOS Version mismatch. Runtime: " << geosVersion << " Build time: " << GEOS_VERSION);
  }

  reinit();
}

void Hoot::reinit()
{
  long max = _toBytes(ConfigOptions().getMaxMemoryUsage());
  if (max > 0l)
  {
    struct rlimit rl;
    getrlimit(RLIMIT_AS, &rl);
    rl.rlim_cur = max;
    setrlimit(RLIMIT_AS, &rl);
    getrlimit(RLIMIT_AS, &rl);
    LOG_DEBUG("Set max memory usage to: " << rl.rlim_cur << "bytes.");
  }

  Log::getInstance().init();
}

long Hoot::_toBytes(const QString& str) const
{
  QString s = str;
  long multiplier = 1;
  if (s.endsWith("KB"))
  {
    multiplier = 1000;
    s = s.remove(s.size() - 2, 2);
  }
  else if (s.endsWith("MB"))
  {
    multiplier = 1000 * 1000;
    s = s.remove(s.size() - 2, 2);
  }
  else if (s.endsWith("GB"))
  {
    multiplier = 1000 * 1000 * 1000;
    s = s.remove(s.size() - 2, 2);
  }

  bool ok;
  long result = s.toLong(&ok) * multiplier;

  if (!ok)
  {
    throw HootException("Unable to parse max memory usage: " + s);
  }

  return result;
}

}
