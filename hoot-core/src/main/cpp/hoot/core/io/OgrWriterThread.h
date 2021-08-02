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
#ifndef OGR_WRITER_THREAD_H
#define OGR_WRITER_THREAD_H

// Hoot
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/io/ElementCache.h>

// Qt
#include <QQueue>
#include <QThread>
#include <QMutex>

namespace hoot
{

class OgrWriterThread : public QThread
{
  Q_OBJECT

public:

  void run() override;

  void setTranslation(const QString& translation) { _translation = translation; }
  void setOutput(const QString& output) { _output = output; }
  void setTransFeaturesQueueMutex(QMutex* mutex) { _pTransFeaturesQMutex = mutex; }
  void setInitMutex(QMutex* mutex) { _pInitMutex = mutex; }
  void setTransFeaturesQueue(
    QQueue<std::pair<std::shared_ptr<geos::geom::Geometry>,
      std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>>>* queue)
  { _pTransFeaturesQ = queue; }
  void setFinishedTranslating(bool* finished) { _pFinishedTranslating = finished; }

private:

  QString _translation;
  QString _output;
  QMutex* _pTransFeaturesQMutex;
  QMutex* _pInitMutex;
  QQueue<std::pair<std::shared_ptr<geos::geom::Geometry>,
         std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>>>* _pTransFeaturesQ;
  bool* _pFinishedTranslating;
};

}

#endif // OGR_WRITER_THREAD_H
