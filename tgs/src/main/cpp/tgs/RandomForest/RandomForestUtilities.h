#ifndef RANDOMFORESTUTILITIES_H
#define RANDOMFORESTUTILITIES_H

//Std Includes
#include <map>
#include <string>
#include <vector>

#include "RandomForestManager.h"
#include "MultithreadedRandomForestManager.h"

namespace Tgs
{
  struct RandomForestInputs
  {
    std::vector<std::string> featureLabels;  //The feature labels matching each element in _dataVectors
    std::vector<std::string> classLabels;  //The ordered class labels
    std::map<std::string, std::vector<std::vector<double> > > trainingVectors; //The data for each training vector
  };

  class RandomForestUtilities
  {
  public:

    /**
     * @brief createDataSets takes the input data and splits it by classes into output indices
     *
     * If you want one the first set with 30% and the rest in the second then splitPercentages would contain 0.3 and 1.0.
     * A 30%, 40%, 30% split would be 0.3, 0.7 and 1.0.
     *
     * @param rfInputs the struct of training data
     * @param splitPercentages the percentages values for splitting data (should be size of number of splits and final value should be 1.0)
     * @return sets of data vector indices by classes
     */
    static std::vector<std::map<std::string, std::pair<unsigned int, unsigned int> > > createDataSets(
      RandomForestInputs rfInputs, std::vector<double> splitPercentages);

    /**
     * @brief generateModel generates a random forest model
     * @param rfInputs the struct of training data
     * @param numTrees the number of trees in the model
     * @param trainingRangeMap the map of classNames to indices within the rfInput trainingVectors to use for training
     * @return the trained random forest model
     */
    static RandomForestManager generateModel(RandomForestInputs & rfInputs, unsigned int numTrees,
      std::map<std::string, std::pair<unsigned int, unsigned int> > & trainingRangeMap);

    /**
     * @brief generateModel generates a multithreaded random forest model
     * @param rfInputs the struct of training data
     * @param numTrees the number of trees in the model
     * @param trainingRangeMap the map of classNames to indices within the rfInput trainingVectors to use for training
     * @return the trained random forest model
     */
    static MultithreadedRandomForestManager generateMultithreadModel(RandomForestInputs & rfInputs,
      unsigned int numTrees,
      std::map<std::string, std::pair<unsigned int, unsigned int> > & trainingRangeMap);

    /**
     * @brief generateTrainingDataFromFile creates a random forest from a file containing labeled training data
     * @param filename the path to the target file
     * @return a set of training vectors
     */
    static RandomForestInputs generateTrainingDataFromFile(std::string filename);

  private:
    /**
     * @brief _generateTrainingDataFromAARFFile reads a WEKA AARF file
     * @param filename the name of the file
     * @return a set of training vectors
     */
    static RandomForestInputs _generateTrainingDataFromAARFFile(std::string filename);
  };
}


#endif // RANDOMFORESTUTILITIES_H
