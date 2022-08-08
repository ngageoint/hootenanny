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
#ifndef ELEMENT_TRANSLATOR_THREAD_H
#define ELEMENT_TRANSLATOR_THREAD_H

// Hoot
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/io/ElementCache.h>

// Qt
#include <QQueue>
#include <QThread>

// std
#include <mutex>

namespace hoot
{

class ElementTranslatorThread : public QThread
{
  Q_OBJECT

public:

  ElementTranslatorThread(std::mutex& initMutex, std::mutex& translationMutex);

  void run() override;  

  void setTranslation(const QString& translation) { _translation = translation; }
  void setElementQueue(QQueue<ElementPtr>* queue) { _pElementQueue = queue; }
  void setTransFeaturesQueue(QQueue<std::pair<std::shared_ptr<geos::geom::Geometry>, TranslatedFeatureVector>>* queue)
  { _pTransFeaturesQueue = queue; }
  void setFinishedTranslating(bool* finished) { _pFinishedTranslating = finished; }
  void setElementCache(const ElementCachePtr& cache) { _pElementCache = cache; }
  void setConversionOps(const QList<ElementVisitorPtr>& ops) { _conversionOps = ops; }

private:

  QString _translation;
  QQueue<ElementPtr>* _pElementQueue;
  std::mutex& _initMutex;
  std::mutex& _transFeaturesQueueMutex;
  QQueue<std::pair<std::shared_ptr<geos::geom::Geometry>, TranslatedFeatureVector>>* _pTransFeaturesQueue;
  bool* _pFinishedTranslating;
  ElementCachePtr _pElementCache;
  QList<ElementVisitorPtr> _conversionOps;
};

}

#endif // ELEMENT_TRANSLATOR_THREAD_H
