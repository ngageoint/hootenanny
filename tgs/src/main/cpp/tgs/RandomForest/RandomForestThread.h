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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef RANDOMFORESTTHREAD_H
#define RANDOMFORESTTHREAD_H

//Qt Includes
#include <QObject>
#include <QRunnable>
#include <QSharedPointer>

#include <tgs/RandomForest/RandomTree.h>

namespace Tgs
{
  /**
   * @brief The RandomForestThread class
   */
  class RandomForestThread : public QObject, public QRunnable
  {
    Q_OBJECT
  public:
    /**
     * @brief RandomForestThread Constructor
     */
    RandomForestThread(const std::shared_ptr<DataFrame>& data, const std::shared_ptr<RandomTree>& tree,
      unsigned int numFactors, unsigned int nodeSize = 1, bool makeBalanced = false);

    /**
     * @brief ~RandomForestThread Destructor
     */
    ~RandomForestThread() = default;


  signals:
    /**
     * @brief exceptionThrown a signal to emit when an exception is thrown
     * @param what the exception message
     */
    void exceptionThrown(QString what);
  protected:
    /**
     * @brief starts the thread
     */
    void run();

  private:
    std::shared_ptr<RandomTree> _tree;
    std::shared_ptr<DataFrame> _data;
    unsigned int _numFactors;
    unsigned int _nodeSize;
    bool _makeBalanced;
  };
}

#endif // RANDOMFORESTTHREAD_H
