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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __RANDOM_FOREST_MANAGER_H__
#define __RANDOM_FOREST_MANAGER_H__

//STD Includes
#include <fstream>
#include <map>
#include <set>
#include <utility>
#include <vector>

//Urgent Includes
#include "../TgsExport.h"

namespace Tgs
{
  class InternalRandomForestManager;
  /**
  * This class is to handle tasks external to the actual random
  * forest such as generate result lists and confusion matrices 
  */
  class TGS_EXPORT RandomForestManager
  {
  public:
    /**
    *  Constructor
    */
    RandomForestManager();

    /**
    *  Destructor
    */
    ~RandomForestManager();

    /**
    *  Adds a training vector to the random forest training set   
    */
    void addTrainingVector(std::string classLabel, const std::vector<double>& trainVec);

    /**
    * Classifies a test data vector against a generated random forest. This method is used
    * when evaluating the 
    *
    * The vector is classified against each tree in the forest and the final classification is the
    * result of majority vote for each tree.
    *
    * @param objId the global id of the object being classified
    * @param objClass the class label for the testing object
    * @param dataVector a single data vector of size N where N is the number of factors
    * @param scores a map to hold the classification results as class name mapped to probability
    */
    void classifyTestVector(std::string objId, std::string objClass, std::vector<double> & dataVector, 
      std::map<std::string, double> & scores);

    /**
    * Classifies a data vector against a generated random forest.  
    *
    * The vector is classified against each tree in the forest and the final classification is the
    * result of majority vote for each tree.
    *
    * @param objId the global id of the object being classified
    * @param dataVector a single data vector of size N where N is the number of factors
    * @param scores a map to hold the classification results as class name mapped to probability
    */
    void classifyVector(std::string objId, std::vector<double> & dataVector, 
      std::map<std::string, double> & scores);

    /**
    *  Exports the random forest to a file
    *
    * @param fileStream the file stream to export the forest
    * @param tabDepth set of tabs to indent the exported content
    */
    void exportModel(std::ostream & fileStream, std::string tabDepth = "");

    /**
    * Exports the currently loaded training data
    *
    * @param fileStream the file stream to export the forest
    * @param tabDepth set of tabs to indent the exported content
    */
    void exportTrainingData(std::fstream & fileStream, std::string tabDepth = "");

    /**
    *  Trains the random forest, sorts the factors by info gain, removes the bottom factor
    * and repeats.     
    */
    //void findTopFactors(unsigned int numTrees, unsigned int numFactors, std::vector<std::string> & factorLabels);

    /**
    * This convenience method is used after the classification of labeled test vectors
    * to create reports such as the confusion matrix and the list of
    * classification results
    *
    * @param reportName a name to use as the base for the generated reports.  A prefix 
    * describing the report (_confusion or _results) and .txt will be appended to the name
    * @param classMap a map of the original class to new classes
    */
    void generateRemappedReports(std::string reportName, 
      std::map<std::string,std::vector<std::string> > & classMap);

    /**
    * Generates a set of text files for the results of the classification including:
    * 1) The result of each object classification as a list of probabilities by class
    * 2) The confusion matrix for the set of result vectors
    * 3) The factor importance values
    *
    * Typical generated file name will be filename + "_confusion.txt"
    *
    * Essentially this function calls the two output generating
    * functions in one step.
    * 
    * @param basefilename the name of the file to write the top factors
    *  without file suffix
    */
    void generateReports(std::string basefilename);

    /**
    * Generates text files for the results of the classification as a list of probabilities by 
    * class and the resulting confusion matrix
    *
    *  The base input file name will be appended with "_results.txt" and
    *  "_confusion.txt" for the output file names
    *
    * @param basefilename the name of the output file without file suffix
    */
    void generateResults(std::string basefilename);

    /**
    *  Generates a text file output from the generated top factors 
    *
    *  The base input file name will be appended with "_top_factors.txt"
    *
    *  @param basefilename the name of the file to write the top factors
    *  without file suffix
    */
    void generateTopFactors(std::string basefilename);

    /**
    *  Gets the class labels unique to the training data set mapped to a 
    * unique index value
    *
    * @return the class labels
    */
    std::set<std::string> getClassLabels();

    /**
    *  Gets the factor importance as generated by the random forest
    *
    *  @param factorImportance a container to hold the results as a list of pairs
    *  of factor name to importance value
    */
    void getFactorImportance(std::vector<std::pair<std::string, double> > & factorImportance);
    
    /**
    * Gets the list of factor labels
    *
    * @param factors a container to hold the factor labels
    */
    void getFactorLabels(std::vector<std::string> & factors);

    /**
    *  Imports the random forest to a file
    *
    * @param fileStream the stream to import the forest
    */
    void import(std::istream & fileStream);

    /**
    * Initializes the state of the InternalRandomForestManager
    *
    * @param modelMethod an id for how the model is being trained (1 = Multiclass, 
    * 2 = Binary 1 to All, 3 = Round Robin class to class)
    * @param factorLabels the list of factor labels (order in parallel with data vector use)
    */
    void init(unsigned int modelMethod, std::vector<std::string> & factorLabels);

    /**
    *  Sets the factor labels to the forest.  This is necessary for
    *  performing top factor analysis.  This need to be ordered
    *  in the same as the data vectors (i.e. index in data vector 
    *  corresponds to index in factorLabels)
    *
    * @param factorLabels the list of factor labels 
    */
    void setFactorLabels(std::vector<std::string> & factorLabels);

    /**
    *  Clears the forest and the results
    */
    void reset();

    /**
    * Clears the results
    */
    void resetResults();

    /**
    *  Trains the random forest on the training vectors previously added
    *
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    *   If the default (-1) is used, then it will use sqrt(number of factors).
    * @param nodeSize the minimum number of data vectors in a set to split a node 
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the random forest should use balanced data
    */
    void train(unsigned int numTrees, int numFactors = -1, unsigned int nodeSize = 1, 
      double retrain = 1.0, bool balanced = false);

  private:
    InternalRandomForestManager * _irfm;
    
  };
}
#endif



