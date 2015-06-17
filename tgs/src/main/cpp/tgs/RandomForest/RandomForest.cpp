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
 * @copyright Copyright (C) 2013, 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RandomForest.h"

//STD Includes
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <map>
#include <utility>

#include "../TgsException.h"

namespace Tgs
{
  RandomForest::RandomForest() : _numSplitFactors(0), _forestCreated(false) 
  {
    //Set the random seed
    //clock_t t1 = clock();
    //srand((unsigned int)t1);
  }

  RandomForest::~RandomForest()
  {
   
  }

  void RandomForest::classifyVector(std::vector<double> & dataVector,
    std::map<std::string, double> & scores) const
  {
    double itrVal = 1.0/(double)_forest.size();

    for(unsigned int i = 0; i < _forest.size(); i++)
    {
      std::string result;
      _forest[i].classifyDataVector(dataVector, result);
      scores[result] += itrVal;
    }
  }

  void RandomForest::clear()
  {
    _forest.clear();
  }

  void RandomForest::exportModel(std::ostream & fileStream, std::string tabDepth)
  {
    if(fileStream.good())
    {
      fileStream << tabDepth << "<RandomForest>" << std::endl;
      fileStream << tabDepth << "\t<NumTrees>\t" << _forest.size() << "\t</NumTrees>" << std::endl;
      fileStream << tabDepth << "\t<NumSplitFactors>\t" << _numSplitFactors << "\t</NumSplitFactors>" << std::endl;

      fileStream << tabDepth << "\t<FactorLabels>";
      for (size_t i = 0; i < _factorLabels.size(); ++i)
      {
        fileStream << "\t" << _factorLabels[i];
      }
      fileStream << "\t</FactorLabels>" << std::endl;

      for(unsigned int i = 0; i < _forest.size(); i++)
      {
        _forest[i].exportTree(fileStream, tabDepth + "\t");
      }
      fileStream << tabDepth << "</RandomForest>" << std::endl;
    }
    else
    {
      throw Exception("SaUrgent::RandomForest::export - File stream is not open for exporting");
    }
  }

  void RandomForest::findAverageError(DataFrame & data, double & average, double & stdDev)
  {
    if(isTrained())
    {
      double errorSum = 0;
      double errorSumSqr = 0;
      double variance;
  
      for(unsigned int i = 0; i < _forest.size(); i++)
      {
        double errRate = _forest[i].computeErrorRate(data);
        errorSum += errRate;
        errorSumSqr += errRate * errRate;
        
      }

      average = errorSum / ((double)_forest.size());
      variance = errorSumSqr / ((double)_forest.size()) - average * average;
      stdDev = sqrt(variance);      
    }
    else
    {
      throw Exception("SaUrgent::RandomForest::findAverageError - Forest has not been trained");
    }
  }

  void RandomForest::findProximity(DataFrame & data, std::vector<unsigned int> & proximity)
  {
    unsigned int dSize = data.getNumDataVectors();

    if(_forestCreated  && dSize > 0)
    {
      proximity.resize(dSize * dSize);
      std::fill(proximity.begin(), proximity.end(), 0);

      for(unsigned int i = 0; i < _forest.size(); i++)
      {
        _forest[i].findProximity(data, proximity);
      }
    }
    else
    {
      throw Exception("SaUrgent::RandomForest::findProximity - Forest has not been trained");
    }
    
  }

  void RandomForest::getFactorImportance(DataFrame & data, 
    std::map<std::string, double> & factorImportance)
  {
    std::map<unsigned int, double>::iterator itr;
    std::vector<std::string> factorLabels = data.getFactorLabels();

    //Init factor importance map with all factors
    for(unsigned int j = 0; j < factorLabels.size(); j++)
    {
      factorImportance[factorLabels[j]] = 0;
    }

    //Calc factor importance for each tree in forest
    //and aggregate the results
    for(unsigned int i = 0; i < _forest.size(); i++)
    {
      std::map<unsigned int, double> factPureMap;
      _forest[i].getFactorImportance(factPureMap);

      for(itr = factPureMap.begin(); itr != factPureMap.end(); ++itr)
      {
        factorImportance[factorLabels[itr->first]] += itr->second;
      }
    }
  }

  void RandomForest::import(std::istream & fileStream)
  {
    clear();
    _forestCreated = false;

    if(fileStream.good())
    {
      unsigned int numTrees = 0;
      std::string buffer;
      std::getline(fileStream, buffer);
      std::string headerString;
      std::stringstream ss(buffer);
      ss >> headerString;
      
      if(headerString == "<RandomForest>")
      {
        std::getline(fileStream, buffer);
        std::string firstStr;
        
        while(buffer.find("</RandomForest>") == std::string::npos)
        {
          std::stringstream ss0(buffer);
          ss0 >> firstStr;

          if(firstStr == "<NumTrees>")
          {
            ss0 >> numTrees; 
            //std::cout << "Num Trees " << numTrees << std::endl;
            _forest.reserve(numTrees);
          }
          else if(firstStr == "<NumSplitFactors>")
          {
            ss0 >> _numSplitFactors;
          }
          else if(firstStr == "<RandomTree>")
          {
            _forest.push_back(RandomTree());
            _forest.back().import(fileStream);
          }
          else if (firstStr == "<FactorLabels>")
          {
            _importFactorLabels(buffer);
          }
          else
          {
            stringstream err;
            err << "Tgs::RandomForest::import - illegal tag read from import file " << firstStr;
            throw Exception(err.str());
          }

          std::getline(fileStream, buffer);
        }
      }
      else
      {
        throw Exception("Error reading the <RandomForest> header.");
      }
       
      _forestCreated = true;
    }
    else
    {
      throw Exception("SaUrgent::RandomForest::import - File stream is not open for importing");
    }
  }

  void RandomForest::_importFactorLabels(string& factorLabelsLine)
  {
    std::stringstream ss(factorLabelsLine);
    string tmp;
    // read off the <FactorLabels> tag
    ss >> tmp;
    while (ss.peek() != EOF)
    {
      string fl;
      ss >> fl;
      _factorLabels.push_back(fl);
    }
    // remove the </FactorLabels> tag
    _factorLabels.resize(_factorLabels.size() - 1);
  }

  void RandomForest::trainBinary(DataFrame & data, unsigned int numTrees, unsigned int numFactors,
    std::string posClass, unsigned int nodeSize, double retrain, bool balanced)
  {
    data.validateData();

    _factorLabels = data.getFactorLabels();
    _forest.clear();
    _numSplitFactors = numFactors;

    if(!data.empty())
    {
      _forest.reserve(numTrees);
     
      data.setBinaryClassLabels(posClass); //Set all non pos example to negative

      for(unsigned int i = 0; i < numTrees; i++)
      {
        _forest.push_back(RandomTree());
        _forest.back().trainBinary(data, numFactors, posClass, nodeSize, true);
        std::cout << "Trained Tree # " << i + 1 << " / " << numTrees << "     \r";
        std::cout.flush();
      }
      std::cout << std::endl;

      

      //std::cout << "Mode Trained " << std::endl;
      if(retrain >= 0 && retrain < 1.0)
      {
        std::cout << "Retraining model on top " << (retrain * 100) << "% of factors" << std::endl;
        std::map<std::string, double> topFactors;
        std::map<std::string, double>::iterator mapItr;

        getFactorImportance(data, topFactors);

        std::vector<std::string> badFactors;

        unsigned int cutOffIdx = topFactors.size() - (unsigned int)((double)topFactors.size() * retrain);

        std::multimap<double, std::string> sortedFactors;
        std::multimap<double, std::string>::iterator mMapItr;

        //Create a map from lowest to highest of important mapped to factor type
        for(mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
        {
          sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));
        }

        unsigned int cutOffCtr = 0;

        for(mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
        {
          if(cutOffCtr <  cutOffIdx)
          {
            badFactors.push_back(mMapItr->second);
            cutOffCtr++;
          }
          else
          {
            break;
          }
        }

        for(unsigned int i = 0; i < badFactors.size(); i++)
        {
          data.deactivateFactor(badFactors[i]);
        }

        _forest.clear();

        _forest.reserve(numTrees);

        for(unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(RandomTree());
          _forest.back().trainBinary(data, (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx)), posClass, 1 , balanced);
        }
      }

      _forestCreated = true;
      data.restoreClassLabels();  //Restore the original class labels
    }
    else
    {
      throw Exception("SaUrgent::RandomForest::buildTest - Unable to operate on empty dataset");
    }
  }

  void RandomForest::trainMulticlass(DataFrame & data, unsigned int numTrees, unsigned int numFactors, 
    unsigned int nodeSize, double retrain, bool balanced)
  {
    data.validateData();

    _factorLabels = data.getFactorLabels();
    _forest.clear();
    numFactors = min<size_t>(data.getActiveFactorCount(), numFactors);
    _numSplitFactors = numFactors;
    cout << _numSplitFactors;

    if(!data.empty())
    {
      _forest.reserve(numTrees);

      for(unsigned int i = 0; i < numTrees; i++)
      {
        _forest.push_back(RandomTree());
        _forest.back().trainMulticlass(data, numFactors, nodeSize, true);
        std::cout << "Trained Tree # " << i + 1 << " / " << numTrees << "     \r";
        std::cout.flush();
      }
      std::cout << std::endl;

      //std::cout << "Mode Trained " << std::endl;
      if(retrain >= 0 && retrain < 1.0)
      {
        std::cout << "Retraining model on top " << (retrain * 100) << "% of factors" << std::endl;
        std::map<std::string, double> topFactors;
        std::map<std::string, double>::iterator mapItr;

        getFactorImportance(data, topFactors);

        std::vector<std::string> badFactors;

        unsigned int cutOffIdx = topFactors.size() - (unsigned int)((double)topFactors.size() * retrain);

        std::multimap<double, std::string> sortedFactors;
        std::multimap<double, std::string>::iterator mMapItr;

        //Create a map from lowest to highest of important mapped to factor type
        for(mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
        {
          sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));
        }

        unsigned int cutOffCtr = 0;

        for(mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
        {
          if(cutOffCtr <  cutOffIdx)
          {
            badFactors.push_back(mMapItr->second);
            cutOffCtr++;
          }
          else
          {
            break;
          }
        }

        for(unsigned int i = 0; i < badFactors.size(); i++)
        {
          data.deactivateFactor(badFactors[i]);
        }

        _forest.clear();

        _forest.reserve(numTrees);

        for(unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(RandomTree());
          _forest.back().trainMulticlass(data, (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx)), 1, balanced);
        }
      }

      _forestCreated = true;
    }
    else
    {
      throw Exception("SaUrgent::RandomForest::buildTest - Unable to operate on empty dataset");
    }
  }

  void RandomForest::trainRoundRobin(DataFrame & data, unsigned int numTrees, 
    unsigned int numFactors, std::string posClass, std::string negClass, unsigned int nodeSize,
    double retrain, bool balanced)
  {
    data.validateData();

    _factorLabels = data.getFactorLabels();
    _forest.clear();
    _numSplitFactors = numFactors;

    if(!data.empty())
    {
      _forest.reserve(numTrees);

      for(unsigned int i = 0; i < numTrees; i++)
      {
        _forest.push_back(RandomTree());
        _forest.back().trainRoundRobin(data, numFactors, posClass, negClass, nodeSize, true);
        std::cout << "Trained Tree # " << i + 1 << " / " << numTrees << "     \r";
        std::cout.flush();
      }
      std::cout << std::endl;

     //std::cout << "Mode Trained " << std::endl;
      if(retrain >= 0 && retrain < 1.0)
      {
        std::cout << "Retraining model on top " << (retrain * 100) << "% of factors" << std::endl;
        std::map<std::string, double> topFactors;
        std::map<std::string, double>::iterator mapItr;

        getFactorImportance(data, topFactors);

        std::vector<std::string> badFactors;

        unsigned int cutOffIdx = topFactors.size() - (unsigned int)((double)topFactors.size() * retrain);

        std::multimap<double, std::string> sortedFactors;
        std::multimap<double, std::string>::iterator mMapItr;

        //Create a map from lowest to highest of important mapped to factor type
        for(mapItr = topFactors.begin(); mapItr != topFactors.end(); ++mapItr)
        {
          sortedFactors.insert(std::pair<double, std::string>(mapItr->second, mapItr->first));
        }

        unsigned int cutOffCtr = 0;

        for(mMapItr = sortedFactors.begin(); mMapItr != sortedFactors.end(); ++mMapItr)
        {
          if(cutOffCtr <  cutOffIdx)
          {
            badFactors.push_back(mMapItr->second);
            cutOffCtr++;
          }
          else
          {
            break;
          }
        }

        for(unsigned int i = 0; i < badFactors.size(); i++)
        {
          data.deactivateFactor(badFactors[i]);
        }

        _forest.clear();

        _forest.reserve(numTrees);

        for(unsigned int i = 0; i < numTrees; i++)
        {
          _forest.push_back(RandomTree());
          _forest.back().trainMulticlass(data, (unsigned int)sqrt((double)(topFactors.size() - cutOffIdx)), 1, balanced);
        }
      }

      _forestCreated = true;
    }
    else
    {
      throw Exception("SaUrgent::RandomForest::buildTest - Unable to operate on empty dataset");
    }
  }
}  //End namespace



