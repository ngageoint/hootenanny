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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __DATA_FRAME_H__
#define __DATA_FRAME_H__

//Qt Includes
#include <QDomDocument>
#include <QDomElement>

//Std Includes
#include <limits>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <tgs/HashMap.h>
#include <tgs/TgsExport.h>

namespace Tgs
{

  /**
  * The DataFrame is a vector of data vectors.  Each data vector contains 
  * the factor values for each piece of data as doubles.  
  *
  * The data frame also contains the methods required for finding information
  * gain for various splits of the data.  In the future this will probably
  * be put in a separate class. 
  */
  class TGS_EXPORT DataFrame
  {
  public:

    enum FactorType
    {
      Numerical,
      Nominal
    };

    enum NullTreatment
    {
      NullAsValue,
      NullAsMissingValue
    };

    /**
    * Constructor
    */
    DataFrame();

    DataFrame(const DataFrame& from);

    /**
    * Destructor
    */
    virtual ~DataFrame() = default;

    /**
    * Appends a data vector to the end of the dataframe
    *
    * @param label the class label for the training vector
    * @param dataItem the data vector to add
    * @param eventWeight the weight associated with the data vector
    */
    void addDataVector(std::string label, const std::vector<double>& dataItem, double eventWeight = 1.);

    /**
     * Similar to above, but the pointer to dataItem is assumed to be an array of a length equal to
     * getFactorTypes().size().
     */
    void addDataVector(std::string label, const double* dataItem, double eventWeight = 1.);

    /**
    *  Resets all the internal data structures associated with data frame
    */  
    void clear();

    /**
    *  Computes the Silverman's rule for a set of data vectors values for a single 
    *  factor
    *
    * @param fIdx the factor index
    * @param dataIndices the data vectors of interest
    * @param minVal returns the minimum factor value computed by the function
    * @param minVal returns the minimum factor value computed by the function
    * @param mean returns the mean factor value computed by the function
    * @param q1 returns the first quartile 
    * @param q3 returns the third quartile
    * @return the computed bandwidth
    */
    double computeBandwidthByFactor(unsigned int fIdx, std::vector<unsigned int> & dataIndices, 
      double & minVal, double & maxVal, double & mean, double & q1, double & q3);

    /**
    * Deactivates the factor by removing its index from the 
    * list of active factor indices
    *
    * @param factor the factor to deactivate
    */
    void deactivateFactor(std::string factor);

    /**
    *  @return true is DataFrame contains 0 data vectors
    */
    bool empty(){return _data.empty();}

    /**
     * @brief exportData exports the data to an XML document
     * @param modelDoc the main XML doc object
     * @param parentNode the node to add the data to
     */
    void exportData(QDomDocument & modelDoc, QDomElement & parentNode) const;

    /**
    *  Export the data frame as XML through to the file stream
    *
    * @param fileStream the output file stream
    * @param tabDepth set of tabs to indent the exported content
    */
    void exportData(std::ostream & fileStream) const;

    /**
     * @brief getActiveFactorCount
     * @return the number of active factors
     */
    size_t getActiveFactorCount() const { return _activeFactorIndices.size(); }

    /**
    *  Gets the class labels unique to the whole data set
    *
    * @return the class labels
    */
    const std::set<std::string>& getClassLabels() const { return _classSet; }

    /**
    *  Get the number of instances per class based on the subset of data
    *  vectors represented by the index value within the indices input parameter
    * 
    *  @param indices a list of data vector indices in the data set
    *  @param populations the output map of class names to number of instances
    */
    void getClassPopulations(const std::vector<unsigned int>& indices, 
      HashMap<std::string, int>& populations);

    /**
     *  Get the number of instances per class based on the subset of data
     *  vectors represented by the index value within the indices input parameter. This cannot
     *  be mapped back to the enumerated classes and is provided because it is slightly more
     *  efficient than the above function.
     * 
     *  @param indices a list of data vector indices in the data set
     *  @param populations the output of number of instances in each class represented
     */
    void getClassDistribution(const std::vector<unsigned int> & indices, 
      std::vector<int>& distribution) const;

    /**
    * Gets the value of a factor within a data vector
    * within the data frame
    *
    * @param vIdx the vector index
    * @param fIdx the factor index
    * @return the requested value
    * 
    */
    double getDataElement(unsigned int vIdx, int fIdx) const;  

    /**
    * Gets reference to a single data vector in the data frame
    *
    * @param vIdx the vector index
    * @return a reference to the requested vector
    */
    const std::vector<double> & getDataVector(unsigned int vIdx) const;

    /**
    * Returns the weight for the row
    * @param vIdx the vector index
    */
    double getWeight(unsigned int vIdx) const { return _weights[vIdx]; }

    /**
    * Gets the list of factor labels
    *
    * @param factors a container to hold the factor labels
    */
    const std::vector<std::string> getFactorLabels() const;

    const std::vector<int>& getFactorTypes() const { return _factorType; }

    /**
    * @return the number of data vectors in the data set
    */
    unsigned int getNumDataVectors() const {return _data.size();}

    /**
     * @return the number of factors in the data set
     */
    unsigned int getNumFactors() const { return _factorLabels.size(); }

    /**
    * Gets the index into the factor list based on its string
    * representation
    *
    * @param fLabel the factor label to find the index
    * @return the index into the list of factors 
    */
    unsigned int getIndexFromFactorLabel(const std::string &fLabel) const;

    /**
    *  Gets the factor label corresponding to the factor index
    *
    * @param fIdx the index into the list of factors
    * @return the factor label corresponding to the entry in the factor list
    */
    std::string getFactorLabelFromIndex(int fIdx) const;

    /**
    *  Gets the majority class label corresponding to the data set
    *
    * @param dataSet the data vector set
    * @return the class label corresponding to the majority of data vectors
    */
    std::string getMajorityTrainingLabel(std::vector<unsigned int> & dataSet);

    /**
     * Returns the NullTreatment for a given factor
     */
    int getNullTreatment(int fIdx);

    /**
    *  Gets the class label corresponding to the data vector index
    *
    * @param dIdx the data vector index into the dataset
    * @return the class label corresponding to the data vector
    */
    std::string getTrainingLabel(unsigned int dIdx) const;

    /**
     * @brief getTrainingLabelList
     * @return the list of class labels corresponding to the training data vectors
     */
    std::vector<std::string> getTrainingLabelList(){return _trainingLabels;}

    /**
     * @brief hasFactorTypes
     * @return true is factor types have been set
     */
    bool hasFactorTypes(){return !_factorType.empty();}

    /**
     * @brief hasNullTreatments
     * @return true if null treatment values have been set
     */
    bool hasNullTreatments(){return !_nullTreatment.empty();}

    /**
    *  Import the data frame from the file stream
    *
    * @param fileStream the input file stream
    */
    void import(std::istream & fileStream);

    /**
    *  Import the data frame
    *
    * @param e a QDomElement containing the contents of tag <DataFrame> from an XML file
    */
    void import(QDomElement & e);

    /**
    *  Checks to see if the data vectors belonging to the set of indices
    * are all of the same class
    */
    bool isDataSetPure(std::vector<unsigned int> & indices);

    /**
     * Returns true if the specified column is nominal.
     */
    bool isNominal(int fIdx) const;

    /**
     * Returns true if v is a "null"
     */
    static bool isNull(double v);

    /**
    *  Creates balanced bootstrap and out of bag sets with replacement
    * based on paper:
    *
    * C. Chen, et.al. Using Random Forest to Learn Imbalanced Data
    *
    * @param bootstrap the output container to hold indices to data vectors for the bootstrap set
    * @param oob  the output container to hold indices to data vectors for the out of bag set
    */
    void makeBalancedBoostrapAndOobSets(std::vector<unsigned int> & bootstrap, 
      std::vector<unsigned int> & oob);

    /**
    *  Creates balanced binary bootstrap and out of bag sets with replacement
    * based on paper:
    *
    * C. Chen, et.al. Using Random Forest to Learn Imbalanced Data
    *
    * 
    * @param className1 the name of the class to represent the positive class
    * @param className2 the name of the class to represent the negative class
    * @param bootstrap the output container to hold indices to data vectors for the bootstrap set
    * @param oob  the output container to hold indices to data vectors for the out of bag set
    */
    void makeBalancedRoundRobinBootstrapAndOobSets(std::string className1, std::string className2, 
      std::vector<unsigned int> & bootstrap, std::vector<unsigned int> & oob);

    /**
    *  Creates bootstrap and out of bag sets with replacement
    *
    * @param bootstrap the output container to hold indices to data vectors for the bootstrap set
    * @param oob  the output container to hold indices to data vectors for the out of bag set
    */
    void makeBoostrapAndOobSets(std::vector<unsigned int> & bootstrap, 
      std::vector<unsigned int> & oob);

    /**
     * Returns what DataFrame considers a null value (NaN in double land).
     */
    static double null()
    {
      return std::numeric_limits<double>::quiet_NaN();
    }

    /**
     * Assignment operator, copies all data. Potentially very expensive.
     */
    DataFrame& operator=(const DataFrame& from);

    /**
     * @brief operator [] provides access to the stored training data vectors
     * @param vIdx the index to the vector of interest
     * @return a reference to the data vector
     */
    std::vector<double> & operator[](unsigned int vIdx);

    /**
    *  Remaps all the classes to different labels.  The original class
    * labels can be restored using restoreClassLabels;
    *
    * @param labelMap a map of the original class name to a new class name
    */
    void remapClassLabels(std::map<std::string, std::string> & labelMap);
    
    /**
    * Partitions the dataframe into two parts based on the splitting idx
    *
    *
    * NOT IMPLEMENTED
    * @param splitIdx the data vector to split on
    * @param leftSplit the data vectors indices on the left side of the split
    * @param rightSplit the data vectors indices on the right side of the split
    */
//     void partition(unsigned int splitIdx, std::vector<unsigned int> & leftSplit, 
//       std::vector<unsigned int> & rightSplit);

    /**
    * Restores the list of class labels from the backup (use after creating binary classes)
    */
    void restoreClassLabels();

    /**
    * Random picks indices of m factors without replacement
    *
    * @param fIndices a list of the resulting factor indices that
    * has already been resized to the number of requested factors
    */
    void selectRandomFactors(unsigned int numFactors, std::vector<unsigned int> & fIndices);

    /**
    * Adds all factors to the active factor list.
    */
    void setAllFactorsActive();

    /**
    * Goes through the training label list and renames all classes not matching posClass
    * to "other"
    * 
    * Use restoreClassLabels to restore to original scheme
    *
    * @param posClass the positive class label
    */
    void setBinaryClassLabels(std::string posClass);

    /**
     * Sets the value of a single data element.
     */
    void setDataElement(int vIdx, int fIdx, double v) { _data[vIdx][fIdx] = v; }
    
    /**
    *  Copies the factor names into an internal vector (not strictly needed inside the data set)
    */
    void setFactorLabels(const std::vector<std::string>& factors);

    /**
     * Sets the factor type to one of Nominal or Numeric
     */
    void setFactorTypes(const std::vector<int>& types);

    /**
     * Sets one factor type to one of Nominal or Numeric
     */
    void setFactorType(int fIdx, int factorType);

    /**
     * Set the null treatment to one of NullAsMissingValue or NullAsValue. The exact way that a
     * null will be treated will depend on the algorithm that operates on the data.
     */
    void setNullTreatment(int fIdx, int nullTreatment);

    /**
    * Sorts a vector of indices to data vectors by the selected factor
    * value.  
    *
    * @param indices the input/output containing the data vector indices
    * @param fIdx the factor index on the data vector
    */
    void sortIndicesOnFactorValue(std::vector<unsigned int> & indices, unsigned int fIdx) const;

    /**
    * Checks to ensure that there is a value for each factor.  If all values are the
    * same then the factor is deactivated.
    */
    void validateData();

  private:
     
    /**
    *  Imports a data vectpr
    *
    * @param e a QDomElement containing the contents of child node within tag <DataVectors> from an XML file
    */
    void _importDataVector(QDomElement & e);

    /**
    * Sorts a vector of indices to data vectors by the selected factor
    * value.  
    *
    * @param indices the input/output containing the data vector indices
    * @param fIdx the factor index on the data vector
    */
    void _qSortIndicesOnFactorValue(std::vector<unsigned int> & indices, unsigned int posP, 
      unsigned int posR, unsigned int fIdx) const;

    /**
    * This is the quicksort partition procedure as shown in:
    * Introduction to Algorithms.  Cormen, et. al. 1990.
    *
    * @param indices the vector of indices to data vectors
    * @param posP the beginning partition position
    * @param posR the ending partition position
    * @param fIdx the index of the factor to use as the basis for sorting the indices
    * @return the new split position 
    * 
    */
    unsigned int _qSortPartition(std::vector<unsigned int> & indices, unsigned int posP, 
      unsigned int posR, unsigned int fIdx) const;

    std::vector<std::string> _trainingLabels;  ///The class labels corresponding to the training data vectors
    std::vector<std::string> _trainingLabelsBak;  ///The class labels backup corresponding to the training data vectors
    std::set<std::string> _classSet; ///The set of unique class labels for all the training data
    std::vector<std::string> _factorLabels;  ///The factor labels corresponding to the data vectors
    std::vector<unsigned int> _activeFactorIndices;  ///The indices corresponding to the currently active factors
    std::vector<std::vector<double>> _data; ///The set of data vectors
    std::vector<int> _factorType; /// Numeric or Nominal
    std::map<std::string, int> _trainingLabelEnum;
    std::vector<std::map<std::string, double>> _medianMaps;  ///For each factor a map of each class to its median value

    /**
     * true if nulls should be interpreted as values, false if nulls should be interpreted as 
     * missing values. See http://en.wikipedia.org/wiki/Missing_value for a discussion.
     */ 
    std::vector<int> _nullTreatment;

    std::vector<double> _weights; ///weight for each row

    int _trainingEnumCnt;

    unsigned int _randSeedCtr;  ///This is used to add a constant to the received clock to ensure a new seed
                                      
  };

  TGS_EXPORT std::ostream& operator<<(std::ostream& s, const DataFrame& df);

} //End namespace

#endif


