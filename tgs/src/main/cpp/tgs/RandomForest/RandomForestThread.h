#ifndef RANDOMFORESTTHREAD_H
#define RANDOMFORESTTHREAD_H

//Qt Includes
#include <QObject>
#include <QRunnable>
#include <QSharedPointer>

#include "RandomTree.h"

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
    RandomForestThread(boost::shared_ptr<DataFrame> data, boost::shared_ptr<RandomTree> tree,
      unsigned int numFactors, unsigned int nodeSize = 1, bool makeBalanced = false);

    /**
     * @brief ~RandomForestThread Destructor
     */
    ~RandomForestThread();


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
    boost::shared_ptr<RandomTree> _tree;
    boost::shared_ptr<DataFrame> _data;
    unsigned int _numFactors;
    unsigned int _nodeSize;
    bool _makeBalanced;
  };
}

#endif // RANDOMFORESTTHREAD_H
