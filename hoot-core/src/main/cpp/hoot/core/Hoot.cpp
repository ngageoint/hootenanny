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
#include "Hoot.h"

// GDAL
#include <gdal.h>
#include <ogrsf_frmts.h>

// GEOS
#include <geos/geom/Geometry.h>
#include <geos/version.h>

// hoot
#include <hoot/core/proto/FileFormat.pb.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/SignalCatcher.h>

// Qt
#include <QLibrary>
#include <QNetworkSession>

// System
#include <sys/resource.h>

// Tgs
#include <tgs/TgsConfig.h>
#ifdef TGS_HAVE_LIBSTXXL
# include <tgs/BigContainers/Stxxl.h>
#endif

using namespace std;

namespace hoot
{

Hoot::Hoot()
{
  _init();
}

Hoot& Hoot::getInstance()
{
  /** Hootenanny singletons follow the Meyers' Singleton pattern seen below */
  static Hoot instance;
  return instance;
}

void Hoot::_init() const
{
  // Lower this log level temporarily *only* for debugging init issues. Some hoot services
  // functionality that parses log output is sensitive to extra logged statements and will fail
  // when this is lowered.
  Log::getInstance().setLevel(Log::Status);

  LOG_DEBUG("Hoot instance init...");

# ifdef TGS_HAVE_LIBSTXXL
  // initialize the environment variable for loading STXXL configuration. If the environment
  // variable has already been set then don't overwrite it (that is the 0 at the end).
  QString stxxlConf = ConfPath::getHootHome() + "/conf/core/stxxl.conf";
  Tgs::Stxxl::getInstance().setConfig(stxxlConf);
# endif

  SignalCatcher::getInstance().registerDefaultHandlers();

  // All streams will default to UTF-8. This makes supporting other scripts much easier.
  setlocale(LC_ALL, "en_US.UTF-8");

  // Make sure our GDAL versions are consistent.
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

# ifdef HOOT_HAVE_NODEJS
  // sometimes HootJs is loaded by node.js before we get to init.
  if (Factory::getInstance().hasClass(QString("HootJsLoaded")) == false)
    loadLibrary("HootJs");
# endif

# ifdef HOOT_HAVE_JOSM
  loadLibrary("HootJosm");
# endif

  Log::getInstance().setLevel(Log::Status);
  // Registering these metatypes here removes warning messages in threads that use
  // QNetworkAccessManager whose initialization routine isn't thread safe.
  qRegisterMetaType<QPair<QByteArray, QByteArray>>();
  qRegisterMetaType<QList<QPair<QByteArray,QByteArray>>>();
  qRegisterMetaType<QSharedPointer<QNetworkSession>>();
}

void Hoot::loadLibrary(const QString& name) const
{
  // This library sticks around in ram even after the object is destroyed.
  QLibrary lib(name);
  LOG_DEBUG("Loading library " << name);
  if (lib.load() == false)
  {
    // if the file doesn't exist, then we aren't too concerned.
    if (lib.errorString().contains("No such file or directory"))
    {
      LOG_WARN(lib.errorString());  // no biggie.
    }
    else
      throw HootException("Error loading libary: " + lib.errorString());
  }
}

void Hoot::reinit() const
{
  LOG_TRACE("Hoot instance reinit...");

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
    throw HootException("Unable to parse max memory usage: " + s);

  return result;
}

}
