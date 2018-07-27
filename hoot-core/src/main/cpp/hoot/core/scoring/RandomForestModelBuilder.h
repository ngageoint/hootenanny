
#ifndef RANDOMFORESTMODELBUILDER_H
#define RANDOMFORESTMODELBUILDER_H

// Qt
#include <QStringList>

namespace hoot
{

/**
 *
 */
class RandomForestModelBuilder
{
public:

  /**
   *
   *
   * @param trainingData
   * @param output
   * @param exportArffOnly
   */
  static void build(const QStringList trainingData, QString output,
                    const bool exportArffOnly = false);
};

}

#endif // RANDOMFORESTMODELBUILDER_H
