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
#ifndef ELEMENT_TRANSLATOR_THREAD_H
#define ELEMENT_TRANSLATOR_THREAD_H

// Hoot
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/io/ElementCache.h>

// Qt
#include <QQueue>
#include <QThread>
#include <QMutex>

namespace hoot
{

class ElementTranslatorThread : public QThread
{
  Q_OBJECT

public:

  void run() override;  

  void setTranslation(const QString& translation) { _translation = translation; }
  void setElementQueue(QQueue<ElementPtr>* queue) { _pElementQ = queue; }
  void setTransFeaturesQueueMutex(QMutex* mutex) { _pTransFeaturesQMutex = mutex; }
  void setInitMutex(QMutex* mutex) { _pInitMutex = mutex; }
  void setTransFeaturesQueue(
    QQueue<std::pair<std::shared_ptr<geos::geom::Geometry>,
      std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>>>* queue)
  { _pTransFeaturesQ = queue; }
  void setFinishedTranslating(bool* finished) { _pFinishedTranslating = finished; }
  void setElementCache(const ElementCachePtr& cache) { _pElementCache = cache; }

private:

  QString _translation;
  QQueue<ElementPtr>* _pElementQ;
  QMutex* _pTransFeaturesQMutex;
  QMutex* _pInitMutex;
  QQueue<std::pair<std::shared_ptr<geos::geom::Geometry>,
         std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>>>* _pTransFeaturesQ;
  bool* _pFinishedTranslating;
  ElementCachePtr _pElementCache;
};

}

#endif // ELEMENT_TRANSLATOR_THREAD_H
