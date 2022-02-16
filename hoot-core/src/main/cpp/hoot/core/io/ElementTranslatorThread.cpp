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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "ElementTranslatorThread.h"

// hoot
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/util/ConfigOptions.h>

#include <hoot/js/v8Engine.h>

namespace hoot
{

ElementTranslatorThread::ElementTranslatorThread(std::mutex& initMutex, std::mutex& translationMutex)
  : _pElementQueue(nullptr),
    _initMutex(initMutex),
    _transFeaturesQueueMutex(translationMutex),
    _pTransFeaturesQueue(nullptr),
    _pFinishedTranslating(nullptr)
{
}

void ElementTranslatorThread::run()
{
  //  Create an isolate for the thread
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate * threadIsolate = v8::Isolate::New(params);
  threadIsolate->Enter();
  v8::Locker v8Lock(threadIsolate);

  // Setup writer used for translation
  std::shared_ptr<OgrWriter> ogrWriter;
  { // Mutex Scope
    // We use this init mutex as a bandaid. Hoot uses a lot of problematic Singletons that cause
    // issues when you try to multithread stuff.
    std::lock_guard<std::mutex> lock(_initMutex);
    ogrWriter = std::make_shared<OgrWriter>();
    ogrWriter->setSchemaTranslationScript(_translation);
    ogrWriter->initTranslator();
    ogrWriter->setCache(_pElementCache);
  }

  ElementPtr pNewElement;
  ElementProviderPtr cacheProvider(_pElementCache);
  while (!_pElementQueue->empty())
  {
    pNewElement = _pElementQueue->dequeue();

    // Run any convert ops present.
    for (const auto& op : _conversionOps)
      op->visit(pNewElement);

    std::shared_ptr<geos::geom::Geometry> g;
    std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature> tf;
    ogrWriter->translateToFeatures(cacheProvider, pNewElement, g, tf);  

    { // Mutex Scope
      std::lock_guard<std::mutex> lock(_transFeaturesQueueMutex);
      _pTransFeaturesQueue->enqueue(
        std::pair<std::shared_ptr<geos::geom::Geometry>, TranslatedFeatureVector>(g, tf));
    }
  }

  { // Mutex Scope
    std::lock_guard<std::mutex> lock(_transFeaturesQueueMutex);
    *_pFinishedTranslating = true;
  }
}

}
