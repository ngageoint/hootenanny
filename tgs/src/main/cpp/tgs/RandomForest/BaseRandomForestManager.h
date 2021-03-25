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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef BASERANDOMFORESTMANAGER_H
#define BASERANDOMFORESTMANAGER_H

//Std Includes
#include <fstream>

#include <tgs/RandomForest/BaseRandomForest.h>
#include <tgs/RandomForest/DataFrame.h>

namespace Tgs
{
  class BaseRandomForestManager
  {
  public:
    /**
     * @brief BaseRandomForestManager constructor
     */
    BaseRandomForestManager();

    /**
     * @brief ~BaseRandomForestManager destructor
     */
    virtual ~BaseRandomForestManager() = default;

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
     * @brief exportModel exports the model an XML file
     * @param outputFile the target file path/name
     */
    void exportModel(std::string outputFilePath);

    /**
    *  Exports the random forest model and data to a file
    *
    * @param fileStream the file stream to export the model
    */
    void exportModel(std::ostream & fileStream);

    /**
    * Exports the currently loaded training data
    *
    * @param fileStream the file stream to export the forest
    * @param tabDepth set of tabs to indent the exported content
    */
    void exportTrainingData(std::fstream & fileStream);

    /**
    * This convenience method is used after the classification of labeled test vectors
    * to create reports such as the confusion matrix and the list of
    * classification results
    *
    * @param reportName a name to use as the base for the generated reports.  A prefix
    * describing the report (_confusion or _results) and .txt will be appended to the name
    * @param classMap a map of the original class to new classes
    */
    void generateRemappedReports(std::string reportName, std::map<std::string,
      std::vector<std::string>> & classMap);

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
    std::set<std::string> getClassLabels(){return _data->getClassLabels();}

    /**
    *  Gets the factor importance as generated by the random forest
    *
    *  @param factorImportance a container to hold the results as a list of pairs
    *  of factor name to importance value
    */
    void getFactorImportance(std::vector<std::pair<std::string, double>> & factorImportance);

    /**
    * Gets the list of factor labels
    *
    * @param factors a container to hold the factor labels
    */
    void getFactorLabels(std::vector<std::string> & factors);

    /**
     * @brief import import the random forest from a file
     * @param sourceFile the path to the source XML file
     */
    void importModel(std::string sourceFile);

    /**
    * Initializes the state of the InternalRandomForestManager
    *
    * @param modelMethod an id for how the model is being trained (1 = Multiclass,
    * 2 = Binary 1 to All, 3 = Round Robin class to class)
    * @param factorLabels the list of factor labels (order in parallel with data vector use)
    */
    void init(unsigned int modelMethod, std::vector<std::string> & factorLabels);

    /**
     * @brief replaceMissingData
     *
     * Use after all training data is added to replace missing data with each factors median value by class
     *
     * @param missingDataValue the missing data value
     */
    void replaceMissingData(double missingDataValue);

    /**
    *  Clears the forest and the results
    */
    void reset();

    /**
    * Clears the results
    */
    void resetResults();

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
     * @brief toXml converts the model to an XML formatted string
     * @return the XML representation of the model
     */
    std::string toXml();

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

    ///Training Types
    static const unsigned int MULTICLASS = 1;
    static const unsigned int BINARY = 2;
    static const unsigned int ROUND_ROBIN = 3;

    //Version of XML file
    static const float RF_XML_VERSION;

  protected:
    /**
    * Classifies a data vector against a set of binary generated random forests.
    *
    * The vector is classified against each tree in the forest and the final classification is the
    * result of majority vote for each tree.
    *
    * @param objId the global id of the object being classified
    * @param objClass the known object class label
    * @param dataVector a single data vector of size N where N is the number of factors
    * @param scores a map to hold the classification results as class name mapped to probability
    */
    virtual void _classifyBinaryTestVector(std::string objId, std::string objClass,
      std::vector<double> & dataVector, std::map<std::string, double> & scores);

    /**
    * Classifies a data vector against a single multi-class generated random forest.
    *
    * The vector is classified against each tree in the forest and the final classification is the
    * result of majority vote for each tree.
    *
    * @param objId the global id of the object being classified
    * @param objClass the known object class label
    * @param dataVector a single data vector of size N where N is the number of factors
    * @param scores a map to hold the classification results as class name mapped to probability
    */
    virtual void _classifyMultiClassTestVector(std::string objId, std::string objClass,
      std::vector<double> & dataVector, std::map<std::string, double> & scores);

    /**
    * Classifies a data vector against a set of pairwise generated random forest.
    *
    * The vector is classified against each tree in the forest and the final classification is the
    * result of majority vote for each tree.
    *
    * @param objId the global id of the object being classified
    * @param objClass the known object class label
    * @param dataVector a single data vector of size N where N is the number of factors
    * @param scores a map to hold the classification results as class name mapped to probability
    */
    virtual void _classifyRoundRobinTestVector(std::string objId, std::string objClass,
      std::vector<double> & dataVector, std::map<std::string, double> & scores);

    /**
    * Classifies a data vector against a single multi-class generated random forest.
    *
    * The vector is classified against each tree in the forest and the final classification is the
    * result of majority vote for each tree.
    *
    * @param objId the global id of the object being classified
    * @param dataVector a single data vector of size N where N is the number of factors
    * @param scores a map to hold the classification results as class name mapped to probability
    */
    virtual void _classifyMultiClassVector(std::string objId, std::vector<double> & dataVector,
      std::map<std::string, double> & scores);

    /**
    * Generates text files for the results of the classification as a list of probabilities by
    * class and the resulting confusion matrix
    *
    *  The base input file name will be appended with "_results.txt" and
    *  "_confusion.txt" for the output file names
    *
    * @param basefilename the name of the output file without file suffix
    * @param map of stuff
    */
    void _generateRemappedResults(std::string basefilename,
      std::map<std::string, std::vector<std::string>> & classMap);

    /**
     * @brief _initForests initializes the list of random forests
     * @param numForests the number of forests to create
     */
    virtual void _initForests(int numForests) = 0;

    /**
     * @brief _parseXmlForestNodes loads the XML form of a fores
     * @param forestsNode the list of <RandomForest> DOM nodes
     */
    virtual void _parseXmlForestNodes(QDomNodeList & forestNodes) = 0;

    /**
    *  Trains a set of binary 1 to all random forests on the training vectors previously added
    *
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the random forests should be balanced
    */
    virtual void _trainBinary(unsigned int numTrees, unsigned int numFactors, unsigned int nodeSize,
      double retrain, bool balanced);

    /**
    *  Trains a single multi-class random forest on the training vectors previously added
    *
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the random forests should be balanced
    */
    virtual void _trainMultiClass(unsigned int numTrees, unsigned int numFactors, unsigned int nodeSize,
      double retrain, bool balanced);

    /**
    *  Trains a set of pair wise class to class random forests on the training vectors previously added
    *
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the random forests should be balanced
    */
    virtual void _trainRoundRobin(unsigned int numTrees, unsigned int numFactors, unsigned int nodeSize,
      double retrain, bool balanced);

    std::shared_ptr<DataFrame> _data;

    ///These three containers are parallel indexed
    std::vector<std::string> _testObjectIds;  //The object ids of the data object being classified
    std::vector<std::string> _testClasses;  //The true class labels for the testing objects
    std::vector<std::map<std::string, double>> _results;  ///A container to hold classification results

    unsigned int _modelMethod;
    bool _initialized;
    bool _trained;

    std::vector<std::shared_ptr<BaseRandomForest>> _rfList;  ///The base forest
  };
}

#endif // BASERANDOMFORESTMANAGER_H
