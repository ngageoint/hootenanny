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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "OgrWriterThread.h"

#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/js/v8Engine.h>

// std
#include <vector>

namespace hoot
{

void OgrWriterThread::run()
{
  // Messing with these parameters did not improve performance:
  // http://trac.osgeo.org/gdal/wiki/ConfigOptions - e.g. using CPLSetConfigOption to set
  // "GDAL_CACHEMAX" to "512" or using CPLSetConfigOption to set "FGDB_BULK_LOAD" to "YES"

  //  Create an isolate for our thread
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate * threadIsolate = v8::Isolate::New(params);
  threadIsolate->Enter();
  v8::Locker v8Lock(threadIsolate);

  bool done = false;
  std::pair<std::shared_ptr<geos::geom::Geometry>,
    std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>> feature;

  // Setup writer
  std::shared_ptr<OgrWriter> ogrWriter;
  { // Mutex Scope
    QMutexLocker lock(_pInitMutex);
    ogrWriter.reset(new OgrWriter());
    ogrWriter->setSchemaTranslationScript(_translation);
    ogrWriter->open(_output);
  }

  while (!done)
  {
    bool doSleep = false;

    // Get element
    { // Mutex Scope
      QMutexLocker lock(_pTransFeaturesQMutex);
      if (_pTransFeaturesQ->isEmpty())
      {
        doSleep = true;
        if (*_pFinishedTranslating)
        {
          done = true;
        }
      }
      else
      {
        feature = _pTransFeaturesQ->dequeue();
      }
    }

    // Write element or sleep
    if (doSleep)
    {
      msleep(100);
    }
    else
    {
      ogrWriter->writeTranslatedFeature(feature.first, feature.second);
    }
  }
  ogrWriter->close();
}

}
