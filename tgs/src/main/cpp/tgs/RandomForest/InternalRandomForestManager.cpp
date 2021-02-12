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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */

#include "InternalRandomForestManager.h"

//STD Includes
#include <cmath>
#include <fstream>
#include <sstream>

//Urgent Includes
#include "../TgsException.h"

namespace Tgs
{
  InternalRandomForestManager::InternalRandomForestManager() : _initialized(false), _trained(false)
  {
   

  }

  InternalRandomForestManager::~InternalRandomForestManager()
  {

  }

  void InternalRandomForestManager::addTrainingVector(std::string classLabel, 
    const std::vector<double>& trainVec)
  {
    _data.addDataVector(classLabel, trainVec);
  }

  void InternalRandomForestManager::_classifyBinaryTestVector(std::string objId, 
    std::string objClass, std::vector<double> & dataVector, std::map<std::string, double> & scores)
  {
    if(_initialized && !_rfList.empty())
    {
      if(_trained)
      {
        std::set<std::string> classSet = _data.getClassLabels();
        std::set<std::string>::iterator itr;

        for(itr = classSet.begin(); itr != classSet.end(); ++itr)
        {
          scores[*itr] = 0;
        }

        std::map<std::string, double>::iterator mItr;
        double sqrSum = 0;
        for(unsigned int i = 0; i < _rfList.size(); i++)
        {
          std::map<std::string, double> tempResult;
          _rfList[i].classifyVector(dataVector, tempResult);

          for(mItr = tempResult.begin(); mItr != tempResult.end(); ++mItr)
          {
            if(mItr->first != "other")
            {
              scores[mItr->first] = mItr->second;
              sqrSum += pow(mItr->second, 2);
            }
          }
        }

        if(sqrSum > 0)
        {  
          //Normalize Scores
          for(mItr = scores.begin(); mItr != scores.end(); ++mItr)
          {
            scores[mItr->first] /= sqrSum;
          }
        }
        else
        {
          throw Exception("SaUrgent::InternalRandomForestManager::_classifyBinaryTestVector - Empty score"
            " returned from testing");
        }

        _testObjectIds.push_back(objId);
        _results.push_back(scores);
        _testClasses.push_back(objClass);
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::_classifyBinaryTestVector - Unable to classify"
          " vector against an untrained forest");
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::_classifyBinaryTestVector - Random Forest Manager is"
        " not initialized");
    }
  }

  void InternalRandomForestManager::_classifyMultiClassTestVector(std::string objId, 
    std::string objClass, std::vector<double> & dataVector, std::map<std::string, double> & scores)
  {
    if(_initialized && !_rfList.empty())
    {
      if(_rfList[0].isTrained())
      {
        std::set<std::string> classSet = _data.getClassLabels();
        std::set<std::string>::iterator itr;

        for(itr = classSet.begin(); itr != classSet.end(); ++itr)
        {
          scores[*itr] = 0;
        }

        //scores["zz00"] = 0;

        _rfList[0].classifyVector(dataVector, scores);
        _testObjectIds.push_back(objId);
        _results.push_back(scores);
        _testClasses.push_back(objClass);
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::_classifyMultiClassTestVector - Unable to classify"
          " vector against an untrained forest");
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::_classifyMultiClassTestVector - Random Forest Manager is"
        " not initialized");
    }
  }

  void InternalRandomForestManager::_classifyRoundRobinTestVector(std::string objId, 
    std::string objClass, std::vector<double> & dataVector, std::map<std::string, double> & scores)
  {
    if(_initialized && !_rfList.empty())
    {
      if(_trained)
      {
        std::set<std::string> classSet = _data.getClassLabels();
        std::set<std::string>::iterator itr;

        for(itr = classSet.begin(); itr != classSet.end(); ++itr)
        {
          scores[*itr] = 0;
        }

        std::map<std::string, double>::iterator mItr;
        
        for(unsigned int i = 0; i < _rfList.size(); i++)
        {
          std::map<std::string, double> tempResult;
          _rfList[i].classifyVector(dataVector, tempResult);

          for(mItr = tempResult.begin(); mItr != tempResult.end(); ++mItr)
          {
            scores[mItr->first] += mItr->second / (double)_rfList.size();
          }
        }

        _testObjectIds.push_back(objId);
        _results.push_back(scores);
        _testClasses.push_back(objClass);
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::_classifyBinaryTestVector - Unable to classify"
          " vector against an untrained forest");
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::_classifyBinaryTestVector - Random Forest Manager is"
        " not initialized");
    }
  }

  void InternalRandomForestManager::_classifyMultiClassVector(std::string objId,
    std::vector<double> & dataVector, std::map<std::string, double> & scores)
  {
    if(_initialized && !_rfList.empty())
    {
      if(_rfList[0].isTrained())
      {
        std::set<std::string> classSet = _data.getClassLabels();
        std::set<std::string>::iterator itr;

        for(itr = classSet.begin(); itr != classSet.end(); ++itr)
        {
          scores[*itr] = 0;
        }

        _rfList[0].classifyVector(dataVector, scores);
        _testObjectIds.push_back(objId);
        _results.push_back(scores);
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::_classifyMultiClassVector - Unable to classify"
          " vector against an untrained forest");
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::classifyMultiClassVector - Random Forest Manager is"
        " not initialized");
    }
  }

  void InternalRandomForestManager::classifyTestVector(std::string objId, std::string objClass, 
    std::vector<double> & dataVector, std::map<std::string, double> & scores)
  {
    if(_initialized && !_rfList.empty())
    {
      if(_modelMethod == MULTICLASS)
      {
        _classifyMultiClassTestVector(objId, objClass, dataVector, scores);
      }
      else if(_modelMethod == BINARY)
      {
        _classifyBinaryTestVector(objId, objClass, dataVector, scores);
      }
      else if(_modelMethod == ROUND_ROBIN)
      {
        _classifyRoundRobinTestVector(objId, objClass, dataVector, scores);
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::classifyTestVector - Invalid"
          " model method set");
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::classifyTestVector - Unable to classify"
        " vector against an uninitialized Random Forest Manager");
    }
  }

  void InternalRandomForestManager::classifyVector(std::string objId, std::vector<double> & dataVector, 
    std::map<std::string, double> & scores)
  {
    if(_initialized && !_rfList.empty())
    {
      if(_modelMethod == MULTICLASS)
      {
        _classifyMultiClassVector(objId, dataVector, scores);
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::classifyVector - Unable to classify"
        " vector against an uninitialized Random Forest Manager");
    }
    
  }

  void InternalRandomForestManager::exportModel(std::ostream & fileStream, std::string tabDepth)
  {
    if(fileStream.good() && _initialized && _trained)
    {
      fileStream << tabDepth << "<Parameters>\t" << _modelMethod << "\t" << _rfList.size() << "\t</Parameters>" << std::endl;
      
      for(unsigned int i = 0; i < _rfList.size(); i++)
      {
        _rfList[i].exportModel(fileStream, tabDepth);
      }

      _data.exportData(fileStream, tabDepth);
    }
    else if (!fileStream.good())
    {
      throw Exception("SaUrgent::InternalRandomForestManager::exportModel - Unable to export"
        " model because file stream is not open for writing");
    }
    else if(!_initialized)
    {
      throw Exception("SaUrgent::InternalRandomForestManager::exportModel - Unable to export"
        " uninitialized model");
    }
    else if(!_trained)
    {
      throw Exception("SaUrgent::InternalRandomForestManager::exportModel - Unable to export"
        " untrained model");
    }
    
  }

  void InternalRandomForestManager::exportTrainingData(std::fstream & fileStream, std::string tabDepth)
  {
    _data.exportData(fileStream, tabDepth);
  }

//  void InternalRandomForestManager::findTopFactors(unsigned int numTrees, unsigned int numFactors,
//    std::vector<std::string> & factorLabels)
//  {
//     _rf.setFactorLabels(factorLabels);
// 
//     for(unsigned int i = 0; i < factorLabels.size() - numFactors; i++)
//     {
//       _rf.train(numTrees, numFactors);
//       
//       double average;
//       double stdDev;
//       _rf.findAverageError(average, stdDev);
//       //std::cout << i << ".) Error " << average << " " << stdDev << std::endl;
//       std::map<std::string, double> factorImport;
//       std::map<std::string, double>::iterator itr;
//       _rf.getFactorImportance(factorImport);
// 
//       //Find minimum factor
//       double minVal = -1.0;
//       std::string minFactor;
//       
//       for(itr = factorImport.begin(); itr != factorImport.end(); ++itr)
//       {
//         if(minVal < itr->second)
//         {
//           minVal = itr->second;
//           minFactor = itr->first;
//         }
//       }
// 
//       _rf.deactivateFactor(minFactor);
//       //std::cout << "Remove " << minFactor << std::endl;
// 
// 
// 
//     }
//     
//  }

  void InternalRandomForestManager::generateRemappedReports(std::string reportName, 
    std::map<std::string, std::vector<std::string> > & classMap)
  {
    if(!_results.empty())
    {
      _generateRemappedResults(reportName, classMap);
      generateTopFactors(reportName);
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::generateReports - Unable to generate"
        " reports from an empty result set. Test on the model");
    }
  }

  void InternalRandomForestManager::generateReports(std::string filename)
  {
    if(!_results.empty())
    {
      generateResults(filename);
      generateTopFactors(filename);
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::generateReports - Unable to generate"
        " reports from an empty result set. Test on the model");
    }
  }

  void InternalRandomForestManager::_generateRemappedResults(std::string filename, 
    std::map<std::string, std::vector<std::string> > & classMap)
  {
    if(!_results.empty())
    {
      //Get the labels in the random forest training set
      std::set<std::string> classLabels = _data.getClassLabels();
      //classLabels.insert("zz00");
      std::set<std::string>::iterator setItr;
      std::set<std::string>::iterator setItr2;

      std::string rsltfile = filename + "_results.txt";
      std::string conffile = filename + "_confusion.txt";

      std::fstream rsltStream;
      std::fstream confStream;

      rsltStream.open(rsltfile.c_str(), std::fstream::out);
      confStream.open(conffile.c_str(), std::fstream::out);

      if(rsltStream.is_open() && confStream.is_open())
      {
        //Generate headers for confusion matrix and results files
        rsltStream << "Class\tID";  //Will change soon with new header style

        for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
        {
          rsltStream << "\t" << *setItr;
          confStream << "\t" << *setItr;
        }

        rsltStream << "\t NewClass" << std::endl;
        confStream << std::endl;

        //Initialize the confusion matrix with zeros
        std::map< std::string, std::map< std::string, int> > confusionCount;
        for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
        {
          for(setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
          {
            confusionCount[*setItr][*setItr2] = 0;
          }
        }

        //Fill the confusion matrix by getting the highest score from the 
        //returned scores and incrementing the value in the cell indexed by 
        //[true class][predicted class]

        std::map<std::string, double>::iterator scoreItr;
        for(unsigned int j = 0; j < _results.size(); j++)
        {
          std::string highestScoreName;
          double maxScore = -1.0;

          for(scoreItr = _results[j].begin(); scoreItr != _results[j].end(); ++scoreItr)
          {
            if(scoreItr->second > maxScore)
            {
              maxScore = scoreItr->second;
              highestScoreName = scoreItr->first;
            }
          }

          std::vector<std::string> mapLabels = classMap[_testClasses[j]];
          
          std::string matchClass;

          for(unsigned int i = 0; i < mapLabels.size(); i++)
          {
            matchClass = mapLabels[i];
            if(matchClass == highestScoreName)  //If any class name matched then that is the classification
            {
              break;
            }
          }

          confusionCount[matchClass][highestScoreName] += 1;
        }

        //Output confusion matrix to file
        for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
        {
          confStream << * setItr;
          for(setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
          {
            confStream << "\t" << confusionCount[*setItr][*setItr2];
          }
          confStream << std::endl;
        }

        confStream.close();

        //Output results to file
        for(unsigned int j = 0; j < _results.size(); j++)
        {
          rsltStream << _testClasses[j] << "\t" << _testObjectIds[j] << "\t"; 

          std::map<std::string, double> curScore = _results[j];

          std::string highestScoreName;
          double maxScore = -1.0;

          for(scoreItr = _results[j].begin(); scoreItr != _results[j].end(); ++scoreItr)
          {
            if(scoreItr->second > maxScore)
            {
              maxScore = scoreItr->second;
              highestScoreName = scoreItr->first;
            }
          }

          for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
          {
            rsltStream << curScore[*setItr] << "\t";
          }

          rsltStream << highestScoreName << "\t" << std::endl;
        }
        rsltStream.close();
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::generateResults - Unable to open"
          " results file or confusion matrix file for writing");
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::generateResults - Unable to generate"
        " reports from empty result set");
    }
  }

  void InternalRandomForestManager::generateResults(std::string filename)
  {
    if(!_results.empty())
    {
      //Get the labels in the random forest training set
      std::set<std::string> classLabels = _data.getClassLabels();
      //classLabels.insert("zz00");
      std::set<std::string>::iterator setItr;
      std::set<std::string>::iterator setItr2;

      std::string rsltfile = filename + "_results.txt";
      std::string conffile = filename + "_confusion.txt";

      std::fstream rsltStream;
      std::fstream confStream;

      rsltStream.open(rsltfile.c_str(), std::fstream::out);
      confStream.open(conffile.c_str(), std::fstream::out);

      if(rsltStream.is_open() && confStream.is_open())
      {
        //Generate headers for confusion matrix and results files
        rsltStream << "ID";  //Will change soon with new header style

        for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
        {
          rsltStream << "\t" << *setItr;
          confStream << "\t" << *setItr;
        }

        rsltStream << std::endl;
        confStream << std::endl;

        //Initialize the confusion matrix with zeros
        std::map< std::string, std::map< std::string, int> > confusionCount;
        for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
        {
          for(setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
          {
            confusionCount[*setItr][*setItr2] = 0;
          }
        }

        //Fill the confusion matrix by getting the highest score from the 
        //returned scores and incrementing the value in the cell indexed by 
        //[true class][predicted class]

        std::map<std::string, double>::iterator scoreItr;
        for(unsigned int j = 0; j < _results.size(); j++)
        {
          std::string highestScoreName;
          double maxScore = -1.0;

          for(scoreItr = _results[j].begin(); scoreItr != _results[j].end(); ++scoreItr)
          {
            if(scoreItr->second > maxScore)
            {
              maxScore = scoreItr->second;
              highestScoreName = scoreItr->first;
            }
          }

          confusionCount[_testClasses[j]][highestScoreName] += 1;
        }

        //Output confusion matrix to file
        for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
        {
          confStream << * setItr;
          for(setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
          {
            confStream << "\t" << confusionCount[*setItr][*setItr2];
          }
          confStream << std::endl;
        }

        confStream.close();

        //Output results to file
        for(unsigned int j = 0; j < _results.size(); j++)
        {
          rsltStream << _testObjectIds[j] << "\t"; 

          std::map<std::string, double> curScore = _results[j];

          for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
          {
            rsltStream << curScore[*setItr] << "\t";
          }

          rsltStream << std::endl;
        }
        rsltStream.close();
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::generateResults - Unable to open"
          " results file or confusion matrix file for writing");
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::generateResults - Unable to generate"
        " reports from empty result set");
    }
  }

  void InternalRandomForestManager::generateUrgentReports(std::string filename)
  {
    if(!_results.empty())
    {
      generateUrgentResults(filename);
      generateTopFactors(filename);
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::generateReports - Unable to generate"
        " reports from an empty result set. Test on the model");
    }
  }

  void InternalRandomForestManager::generateUrgentResults(std::string filename)
  {
    if(!_results.empty())
    {
      //Get the labels in the random forest training set
      std::set<std::string> classLabels = _data.getClassLabels();
      std::set<std::string>::iterator setItr;
      std::set<std::string>::iterator setItr2;

      std::string rsltfile = filename + "_results.txt";
      std::string conffile = filename + "_confusion.txt";

      std::fstream rsltStream;
      std::fstream confStream;

      rsltStream.open(rsltfile.c_str(), std::fstream::out);
      confStream.open(conffile.c_str(), std::fstream::out);

      if(rsltStream.is_open() && confStream.is_open())
      {
        //Generate headers for confusion matrix and results files
        rsltStream << "GlobalID";  //Will change soon with new header style

        for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
        {
          rsltStream << "\t" << *setItr;
          confStream << "\t" << *setItr;
        }

        rsltStream << std::endl;
        confStream << std::endl;

        //Initialize the confusion matrix with zeros
        std::map< std::string, std::map< std::string, int> > confusionCount;
        for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
        {
          for(setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
          {
            confusionCount[*setItr][*setItr2] = 0;
          }
        }

        //Fill the confusion matrix by getting the highest score from the 
        //returned scores and incrementing the value in the cell indexed by 
        //[true class][predicted class]

        std::map<std::string, double>::iterator scoreItr;
        for(unsigned int j = 0; j < _results.size(); j++)
        {
          std::string highestScoreName;
          double maxScore = -1.0;

          for(scoreItr = _results[j].begin(); scoreItr != _results[j].end(); ++scoreItr)
          {
            if(scoreItr->second > maxScore)
            {
              maxScore = scoreItr->second;
              highestScoreName = scoreItr->first;
            }
          }

          confusionCount[_testClasses[j]][highestScoreName] += 1;
        }

        //Output confusion matrix to file
        for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
        {
          confStream << * setItr;
          for(setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
          {
            confStream << "\t" << confusionCount[*setItr][*setItr2];
          }
          confStream << std::endl;
        }

        confStream.close();

        //Output results to file
        for(unsigned int j = 0; j < _results.size(); j++)
        {
          for(int k = -40; k <= 40; k+=10)
          {
            rsltStream << _testClasses[j] << "/" << _testObjectIds[j] << "/r" << k << ".png\t"; 

            std::map<std::string, double> curScore = _results[j];

            for(setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
            {
              rsltStream << curScore[*setItr] << "\t";
            }

            rsltStream << std::endl;
          }
        }
        rsltStream.close();
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::generateResults - Unable to open"
          " results file or confusion matrix file for writing");
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::generateResults - Unable to generate"
        " reports from empty result set");
    }
  }

  void InternalRandomForestManager::generateTopFactors(std::string basefilename)
  {
    if(_initialized)
    {
      if(_trained)
      {
        std::fstream outStream;

        basefilename += "_top_factors.txt";

        outStream.open(basefilename.c_str(), std::fstream::out);

        if(outStream.is_open())
        {
          std::map<std::string, double> factMap;
          std::map<std::string, double>::iterator itr;

          for(unsigned int i = 0; i < _rfList.size(); i++)
          {
            std::map<std::string, double> tempFactMap;
            _rfList[i].getFactorImportance(_data, tempFactMap);

            for(itr = tempFactMap.begin(); itr != tempFactMap.end(); ++itr)
            {
              factMap[itr->first] += tempFactMap[itr->first];
            }
          }
          

          for(itr = factMap.begin(); itr != factMap.end(); ++itr)
          {
            outStream << itr->first << " " << itr->second << std::endl;
          }

          outStream.close();
        }
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::generateTopFactors - Unable to generate"
          " top factors from untrained model");
      }
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::generateTopFactors - Unable to generate"
        " top factors from uninitialized Random Forest Manager");
    }
  }

  void InternalRandomForestManager::getFactorLabels(std::vector<std::string> & factors)
  {
    factors = _data.getFactorLabels();
  }

  void InternalRandomForestManager::import(std::istream & fileStream)
  {
    if(fileStream.good())
    {
      std::string buffer;
      std::string temp;
      unsigned int numForests;

      std::getline(fileStream, buffer);
      
      std::stringstream ss(buffer);

      ss >> temp >> _modelMethod >> numForests; 
      
      _rfList.resize(numForests);
      for(unsigned int i = 0; i < _rfList.size(); i++)
      {
        _rfList[i].import(fileStream);
      }

      _data.import(fileStream);
    }
    else if (!fileStream.good())
    {
      throw Exception("SaUrgent::InternalRandomForestManager::importModel - Unable to import"
        " model because file stream is not open for reading");
    }
    
   _initialized = true;
   _trained = true;

  }

  void InternalRandomForestManager::init(unsigned int modelMethod, 
    std::vector<std::string> & factorLabels)
  {
    if(modelMethod >= MULTICLASS && modelMethod <= ROUND_ROBIN)
    {
      _modelMethod = modelMethod;
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::init - Unsupported model method"
        " for training model");
    }

    if(!factorLabels.empty())
    {
      _data.setFactorLabels(factorLabels);
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::init - Factors label vector"
        " of size 0");
    }

    _initialized = true;
  }

  void InternalRandomForestManager::reset()
  {
    for(unsigned int i = 0; i < _rfList.size(); i++)
    {
      _rfList[i].clear();
    }
    resetResults();
  }

  void InternalRandomForestManager::resetResults()
  {
    _results.clear();
    _testObjectIds.clear();
    _testClasses.clear();
  }

  void InternalRandomForestManager::setFactorLabels(std::vector<std::string> & factorLabels)
  {
    _data.setFactorLabels(factorLabels);
  }

  void InternalRandomForestManager::train(unsigned int numTrees, int numFactors, 
    unsigned int nodeSize, double retrain, bool balanced)
  {
    if (numFactors < 0)
    {
      numFactors = (int)sqrt((float)_data.getNumFactors());
    }
    if(_initialized)
    {
      if(_modelMethod == MULTICLASS)
      {
        _trainMultiClass(numTrees, numFactors, nodeSize, retrain, balanced);
      }
      else if(_modelMethod == BINARY)
      {
        _trainBinary(numTrees, numFactors, nodeSize, retrain, balanced);
      }
      else if(_modelMethod == ROUND_ROBIN)
      {
        _trainRoundRobin(numTrees, numFactors, nodeSize, retrain, balanced);
      }
      else
      {
        throw Exception("SaUrgent::InternalRandomForestManager::train - Unsupported method"
          " for training model");
      } 
      _trained = true;
    }
    else
    {
      throw Exception("SaUrgent::InternalRandomForestManager::train - Unable to train"
        " uninitialized Random Forest Manager");
    }
    
  }

  void InternalRandomForestManager::_trainBinary(unsigned int numTrees, unsigned int numFactors,
    unsigned int nodeSize, double retrain, bool balanced)
  {
    std::set<std::string> classNames = _data.getClassLabels();
    std::set<std::string>::iterator itr;
    //Create one forest per class label
    _rfList.resize(classNames.size());

    unsigned int itrCtr = 0;
    for(itr = classNames.begin(); itr != classNames.end(); ++itr)
    {
      std::string currentClass = *itr;

      _rfList[itrCtr].trainBinary(_data, numTrees, numFactors, currentClass, 
        nodeSize, retrain, balanced);
      itrCtr++;
    }
  }

  void InternalRandomForestManager::_trainMultiClass(unsigned int numTrees, unsigned int numFactors, 
    unsigned int nodeSize, double retrain, bool balanced)
  {
    _rfList.resize(1);
    _rfList[0].trainMulticlass(_data, numTrees, numFactors, nodeSize, retrain, balanced);
  }

  void InternalRandomForestManager::_trainRoundRobin(unsigned int numTrees, unsigned int numFactors,
    unsigned int nodeSize, double retrain, bool balanced)
  {
    //Warning killer
    numTrees = numTrees;
    numFactors = numFactors;
    nodeSize = nodeSize;
    retrain = retrain;
    balanced = balanced;

    std::set<std::string> classNames = _data.getClassLabels();
    std::set<std::string>::iterator itr;
    std::set<std::string>::iterator itr2;

    unsigned int numClasses = _data.getClassLabels().size();
    
    //Create one forest per class label
    _rfList.resize(numClasses * (numClasses - 1) / 2);

    unsigned int itrCtr = 0;
    for(itr = classNames.begin(); itr != classNames.end(); ++itr)
    {
      for(itr2 = itr; itr2 != classNames.end(); ++itr2)
      {
        std::string posClass = *itr;
        std::string negClass = *itr2;

        if(posClass != negClass)
        {
          _rfList[itrCtr].trainRoundRobin(_data, numTrees, numFactors, posClass, negClass, nodeSize, retrain, balanced);
          itrCtr++;
        }
      }
    }
  }
}  //End namespace
