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
#include "BaseRandomForestManager.h"

//Qt Includes
#include <QFile>
#include <QStringList>
#include <QTextStream>

//Std Includes
#include <cmath>

#include <tgs/RandomForest/MissingDataHandler.h>

namespace Tgs
{
  const float BaseRandomForestManager::RF_XML_VERSION = 2.0;

  BaseRandomForestManager::BaseRandomForestManager() : _initialized(false), _trained(false)
  {
    try
    {
      _data = std::shared_ptr<DataFrame>(new DataFrame);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::addTrainingVector(
    const std::string& classLabel, const std::vector<double>& trainVec) const
  { 
    try
    {
      _data->addDataVector(classLabel, trainVec);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::classifyTestVector(
    const std::string& objId, const std::string& objClass,
    std::vector<double>& dataVector, std::map<std::string, double>& scores)
  {
    try
    {
      if (_initialized && !_rfList.empty())
      {
        if (_modelMethod == MULTICLASS)
        {
          _classifyMultiClassTestVector(objId, objClass, dataVector, scores);
        }
        else if (_modelMethod == BINARY)
        {
          _classifyBinaryTestVector(objId, objClass, dataVector, scores);
        }
        else if (_modelMethod == ROUND_ROBIN)
        {
          _classifyRoundRobinTestVector(objId, objClass, dataVector, scores);
        }
        else
        {
          throw Exception(__LINE__, "Invalid model method set");
        }
      }
      else
      {
        throw Exception(__LINE__,
          "Unable to classify vector against an uninitialized Random Forest Manager");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::classifyVector(const std::string& objId,
    std::vector<double>& dataVector, std::map<std::string, double>& scores)
  {
    try
    {
      if (_initialized && !_rfList.empty())
      {
        if (_modelMethod == MULTICLASS)
        {
          _classifyMultiClassVector(objId, dataVector, scores);
        }
      }
      else
      {
        throw Exception(__LINE__,
          "Unable to classify vector against an uninitialized Random Forest Manager");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::exportModel(const std::string& outputFilePath)
  {
    try
    {
      QFile modelFile(outputFilePath.c_str());

      if (!modelFile.open(QIODevice::WriteOnly | QIODevice::Text))
      {
        std::stringstream ss;
        ss << "Unable to open the file " << outputFilePath << " for writing the XML model.";
        throw Exception(__LINE__, ss.str());
      }

      QTextStream stream(&modelFile);
      stream << toXml().c_str();
      modelFile.close();
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::exportModel(std::ostream & fileStream)
  {
    try
    {
      fileStream << toXml();
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::exportTrainingData(std::fstream & fileStream) const
  {
    try
    {
      QDomDocument modelDoc("XML");

      QDomElement rootNode = modelDoc.createElement("TrainingData");

      _data->exportData(modelDoc, rootNode);

      std::string xmlData = modelDoc.toString().toLatin1().constData();
      fileStream << xmlData;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::generateRemappedReports(std::string& reportName,
    std::map<std::string, std::vector<std::string>>& classMap)
  {
    try
    {
      if (!_results.empty())
      {
        _generateRemappedResults(reportName, classMap);
        generateTopFactors(reportName);
      }
      else
      {
        throw Exception(__LINE__,
          "Unable to generate reports from an empty result set. Test on the model");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::generateReports(std::string& filename)
  {
    try
    {
      if (!_results.empty())
      {
        generateResults(filename);
        generateTopFactors(filename);
      }
      else
      {
        throw Exception(__LINE__,
          "Unable to generate reports from an empty result set. Test on the model");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::generateResults(const std::string& filename)
  {
    try
    {
      if (!_results.empty())
      {
        //Get the labels in the random forest training set
        std::set<std::string> classLabels = _data->getClassLabels();
        //classLabels.insert("zz00");
        std::set<std::string>::iterator setItr;
        std::set<std::string>::iterator setItr2;

        std::string rsltfile = filename + "_results.txt";
        std::string conffile = filename + "_confusion.txt";

        std::fstream rsltStream;
        std::fstream confStream;

        rsltStream.open(rsltfile.c_str(), std::fstream::out);
        confStream.open(conffile.c_str(), std::fstream::out);

        if (rsltStream.is_open() && confStream.is_open())
        {
          //Generate headers for confusion matrix and results files
          rsltStream << "ID";  //Will change soon with new header style

          for (setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
          {
            rsltStream << "\t" << *setItr;
            confStream << "\t" << *setItr;
          }

          rsltStream << std::endl;
          confStream << std::endl;

          //Initialize the confusion matrix with zeros
          std::map<std::string, std::map<std::string, int>> confusionCount;
          for (setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
          {
            for (setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
            {
              confusionCount[*setItr][*setItr2] = 0;
            }
          }

          //Fill the confusion matrix by getting the highest score from the
          //returned scores and incrementing the value in the cell indexed by
          //[true class][predicted class]

          std::map<std::string, double>::iterator scoreItr;
          for (unsigned int j = 0; j < _results.size(); j++)
          {
            std::string highestScoreName;
            double maxScore = -1.0;

            for (scoreItr = _results[j].begin(); scoreItr != _results[j].end(); ++scoreItr)
            {
              if (scoreItr->second > maxScore)
              {
                maxScore = scoreItr->second;
                highestScoreName = scoreItr->first;
              }
            }

            confusionCount[_testClasses[j]][highestScoreName] += 1;
          }

          // Output confusion matrix to file
          for (setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
          {
            confStream << * setItr;
            for (setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
            {
              confStream << "\t" << confusionCount[*setItr][*setItr2];
            }
            confStream << std::endl;
          }

          confStream.close();

          // Output results to file
          for (unsigned int j = 0; j < _results.size(); j++)
          {
            rsltStream << _testObjectIds[j] << "\t";

            std::map<std::string, double> curScore = _results[j];

            for (setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
            {
              rsltStream << curScore[*setItr] << "\t";
            }

            rsltStream << std::endl;
          }
          rsltStream.close();
        }
        else
        {
          throw Exception(__LINE__,
            "Unable to open results file or confusion matrix file for writing");
        }
      }
      else
      {
        throw Exception(__LINE__, "Unable to generate reports from empty result set");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::generateTopFactors(std::string& basefilename)
  {
    try
    {
      if (_initialized)
      {
        if (_trained)
        {
          std::fstream outStream;

          basefilename += "_top_factors.txt";

          outStream.open(basefilename.c_str(), std::fstream::out);

          if (outStream.is_open())
          {
            std::map<std::string, double> factMap;
            std::map<std::string, double>::iterator itr;

            for (unsigned int i = 0; i < _rfList.size(); i++)
            {
              std::map<std::string, double> tempFactMap;
              _rfList[i]->getFactorImportance(_data, tempFactMap);

              for (itr = tempFactMap.begin(); itr != tempFactMap.end(); ++itr)
              {
                factMap[itr->first] += tempFactMap[itr->first];
              }
            }


            for (itr = factMap.begin(); itr != factMap.end(); ++itr)
            {
              outStream << itr->first << " " << itr->second << std::endl;
            }

            outStream.close();
          }
        }
        else
        {
          throw Exception(__LINE__, "Unable to generate top factors from untrained model");
        }
      }
      else
      {
        throw Exception(__LINE__,
          "Unable to generate top factors from uninitialized Random Forest Manager");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::getFactorLabels(std::vector<std::string>& factors) const
  {
    try
    {
      factors = _data->getFactorLabels();
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::importModel(const std::string& sourceFile)
  {
    try
    {
      QDomDocument xmlDoc;

      QFile xmlFile(sourceFile.c_str());

      if (!xmlFile.open(QIODevice::ReadOnly))
      {
        xmlFile.close();
        std::stringstream ss;
        ss << "Error opening XML model file " <<  sourceFile << ".  Check the file path.";
        throw Exception(__LINE__, ss.str());
      }

      QString err;
      if (!xmlDoc.setContent(&xmlFile, &err))
      {
        std::stringstream ss;
        ss << "Error parsing XML model file "<< sourceFile << " - " << err.toLatin1().constData();
        throw Exception(__LINE__, ss.str());
      }

      QDomElement docElem = xmlDoc.documentElement();

      bool parsedVersion;
      float version = docElem.attribute("Version", "1.0").toFloat(&parsedVersion);

      if (!parsedVersion || version != RF_XML_VERSION)
      {
        std::stringstream ss;
        ss << "This version of Random Forest can not parse this version of the input XML file";
        throw Exception(__LINE__, ss.str());
      }

      //First read data sources
      QDomNodeList childList = docElem.childNodes();

      for (unsigned int i = 0; i < (unsigned int)childList.size(); i++)
      {
        if (childList.at(i).nodeType() == QDomNode::CommentNode)
        {
          continue;
        }

        if (childList.at(i).isElement())
        {
          QDomElement e = childList.at(i).toElement(); // try to convert the node to an element.

          QString tag = e.tagName().toUpper();

          bool parseOk = true;

          if (tag == "PARAMETERS")
          {
            QString value = e.text();
            QStringList tokens = value.split(" ");

            if (tokens.size() != 2)
            {
              parseOk = false;
            }
            else
            {
              _modelMethod = tokens[0].toInt(&parseOk);
              int numForests = tokens[1].toInt(&parseOk);

              _initForests(numForests);
            }
          }
          else if (tag == "RANDOMFORESTS")
          {
            QDomNodeList forestNodes = e.childNodes();

            _parseXmlForestNodes(forestNodes);

          }
          else if (tag == "DATAFRAME")
          {
            _data->clear();
            _data->import(e);
          }

          if (!parseOk)
          {
            std::stringstream ss;
            ss << "Error parsing tag " << tag.toLatin1().constData() << " with the value " <<
              e.text().toLatin1().constData();
            throw Exception(__LINE__, ss.str());
          }
        }
      }

      _initialized = true;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::init(
    unsigned int modelMethod, const std::vector<std::string> & factorLabels)
  {
    try
    {
      if (modelMethod >= MULTICLASS && modelMethod <= ROUND_ROBIN)
      {
        _modelMethod = modelMethod;
      }
      else
      {
        throw Exception(__LINE__, "Unsupported model method for training model");
      }

      if (!factorLabels.empty())
      {
        _data->setFactorLabels(factorLabels);
      }
      else
      {
        throw Exception(__LINE__, "Factors label vector of size 0");
      }

      _initialized = true;
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::replaceMissingData(double missingDataValue) const
  {
    try
    {
      MissingDataHandler::replaceMissingValuesFast(missingDataValue, _data);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::reset()
  {
    try
    {
      for (unsigned int i = 0; i < _rfList.size(); i++)
      {
        _rfList[i]->clear();
      }
      resetResults();
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::resetResults()
  {
    try
    {
      _results.clear();
      _testObjectIds.clear();
      _testClasses.clear();
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::setFactorLabels(const std::vector<std::string>& factorLabels) const
  {
    try
    {
      _data->setFactorLabels(factorLabels);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  std::string BaseRandomForestManager::toXml()
  {
    try
    {
      if (!_initialized)
      {
        throw Exception(__LINE__, "Model has not been initialized");
      }

      QDomDocument modelDoc("XML");

      QDomElement rootNode = modelDoc.createElement("RandomForestModel");
      rootNode.setAttribute("Version", 2.0);

      //Create Parameter node
      QDomElement paramNode = modelDoc.createElement("Parameters");
      std::stringstream paramStream;
      paramStream << _modelMethod << " " << _rfList.size();
      QDomText paramText = modelDoc.createTextNode(paramStream.str().c_str());
      paramNode.appendChild(paramText);
      rootNode.appendChild(paramNode);

      QDomElement forestsNode = modelDoc.createElement("RandomForests");
      //Append Forest(s)
      for (unsigned int i = 0; i < _rfList.size(); i++)
      {
        _rfList[i]->exportModel(modelDoc, forestsNode);
      }

      rootNode.appendChild(forestsNode);

      //Append Data Frame
      _data->exportData(modelDoc, rootNode);

      modelDoc.appendChild(rootNode);

      return modelDoc.toString().toLatin1().constData();
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::train(unsigned int numTrees, int numFactors,
    unsigned int nodeSize, double retrain, bool balanced)
  {
    try
    {
      if (numFactors < 0)
      {
        numFactors = (int)sqrt((float)_data->getNumFactors());
      }
      if (_initialized)
      {
        if (_modelMethod == MULTICLASS)
        {
          _trainMultiClass(numTrees, numFactors, nodeSize, retrain, balanced);
        }
        else if (_modelMethod == BINARY)
        {
          _trainBinary(numTrees, numFactors, nodeSize, retrain, balanced);
        }
        else if (_modelMethod == ROUND_ROBIN)
        {
          _trainRoundRobin(numTrees, numFactors, nodeSize, retrain, balanced);
        }
        else
        {
          throw Exception(__LINE__, "Unsupported method for training model");
        }
        _trained = true;
      }
      else
      {
        throw Exception(__LINE__, "Unable to train uninitialized Random Forest Manager");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::_classifyBinaryTestVector(const std::string& objId,
    const std::string& objClass, std::vector<double>& dataVector,
    std::map<std::string, double>& scores)
  {
    try
    {
      if (_initialized && !_rfList.empty())
      {
        if (_trained)
        {
          std::set<std::string> classSet = _data->getClassLabels();
          std::set<std::string>::iterator itr;

          for (itr = classSet.begin(); itr != classSet.end(); ++itr)
          {
            scores[*itr] = 0;
          }

          std::map<std::string, double>::iterator mItr;
          double sqrSum = 0;
          for (unsigned int i = 0; i < _rfList.size(); i++)
          {
            std::map<std::string, double> tempResult;
            _rfList[i]->classifyVector(dataVector, tempResult);

            for (mItr = tempResult.begin(); mItr != tempResult.end(); ++mItr)
            {
              if (mItr->first != "other")
              {
                scores[mItr->first] = mItr->second;
                sqrSum += pow(mItr->second, 2);
              }
            }
          }

          if (sqrSum > 0)
          {
            //Normalize Scores
            for (mItr = scores.begin(); mItr != scores.end(); ++mItr)
            {
              scores[mItr->first] /= sqrSum;
            }
          }
          else
          {
            throw Exception("Empty score returned from testing");
          }

          _testObjectIds.push_back(objId);
          _results.push_back(scores);
          _testClasses.push_back(objClass);
        }
        else
        {
          throw Exception("Unable to classify vector against an untrained forest");
        }
      }
      else
      {
        throw Exception("Random Forest Manager is not initialized");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::_classifyMultiClassTestVector(const std::string& objId,
    const std::string& objClass, std::vector<double>& dataVector,
    std::map<std::string, double>& scores)
  {
    try
    {
      if (_initialized && !_rfList.empty())
      {
        if (_rfList[0]->isTrained())
        {
          std::set<std::string> classSet = _data->getClassLabels();
          std::set<std::string>::iterator itr;

          for (itr = classSet.begin(); itr != classSet.end(); ++itr)
          {
            scores[*itr] = 0;
          }

          //scores["zz00"] = 0;

          _rfList[0]->classifyVector(dataVector, scores);
          _testObjectIds.push_back(objId);
          _results.push_back(scores);
          _testClasses.push_back(objClass);
        }
        else
        {
          throw Exception(__LINE__,
            "Unable to classify vector against an untrained forest");
        }
      }
      else
      {
        throw Exception(__LINE__, "Random Forest Manager is not initialized");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::_classifyRoundRobinTestVector(const std::string& objId,
    const std::string& objClass, std::vector<double>& dataVector,
    std::map<std::string, double>& scores)
  {
    try
    {
      if (_initialized && !_rfList.empty())
      {
        if (_trained)
        {
          std::set<std::string> classSet = _data->getClassLabels();
          std::set<std::string>::iterator itr;

          for (itr = classSet.begin(); itr != classSet.end(); ++itr)
          {
            scores[*itr] = 0;
          }

          std::map<std::string, double>::iterator mItr;

          for (unsigned int i = 0; i < _rfList.size(); i++)
          {
            std::map<std::string, double> tempResult;
            _rfList[i]->classifyVector(dataVector, tempResult);

            for (mItr = tempResult.begin(); mItr != tempResult.end(); ++mItr)
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
          throw Exception(__LINE__, "Unable to classify vector against an untrained forest");
        }
      }
      else
      {
        throw Exception(__LINE__, "Random Forest Manager is not initialized");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::_classifyMultiClassVector(const std::string& objId,
    std::vector<double>& dataVector, std::map<std::string, double>& scores)
  {
    try
    {
      if (_initialized && !_rfList.empty())
      {
        if (_rfList[0]->isTrained())
        {
          std::set<std::string> classSet = _data->getClassLabels();
          std::set<std::string>::iterator itr;

          for (itr = classSet.begin(); itr != classSet.end(); ++itr)
          {
            scores[*itr] = 0;
          }

          _rfList[0]->classifyVector(dataVector, scores);
          _testObjectIds.push_back(objId);
          _results.push_back(scores);
        }
        else
        {
          throw Exception(__LINE__, "Unable to classify vector against an untrained forest");
        }
      }
      else
      {
        throw Exception(__LINE__, "Random Forest Manager is not initialized");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }


  void BaseRandomForestManager::_generateRemappedResults(
    const std::string& filename, std::map<std::string, std::vector<std::string>>& classMap)
  {
    try
    {
      if (!_results.empty())
      {
        //Get the labels in the random forest training set
        std::set<std::string> classLabels = _data->getClassLabels();
        //classLabels.insert("zz00");
        std::set<std::string>::iterator setItr;
        std::set<std::string>::iterator setItr2;

        std::string rsltfile = filename + "_results.txt";
        std::string conffile = filename + "_confusion.txt";

        std::fstream rsltStream;
        std::fstream confStream;

        rsltStream.open(rsltfile.c_str(), std::fstream::out);
        confStream.open(conffile.c_str(), std::fstream::out);

        if (rsltStream.is_open() && confStream.is_open())
        {
          //Generate headers for confusion matrix and results files
          rsltStream << "Class\tID";  //Will change soon with new header style

          for (setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
          {
            rsltStream << "\t" << *setItr;
            confStream << "\t" << *setItr;
          }

          rsltStream << "\t NewClass" << std::endl;
          confStream << std::endl;

          //Initialize the confusion matrix with zeros
          std::map<std::string, std::map<std::string, int>> confusionCount;
          for (setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
          {
            for (setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
            {
              confusionCount[*setItr][*setItr2] = 0;
            }
          }

          //Fill the confusion matrix by getting the highest score from the
          //returned scores and incrementing the value in the cell indexed by
          //[true class][predicted class]

          std::map<std::string, double>::iterator scoreItr;
          for (unsigned int j = 0; j < _results.size(); j++)
          {
            std::string highestScoreName;
            double maxScore = -1.0;

            for (scoreItr = _results[j].begin(); scoreItr != _results[j].end(); ++scoreItr)
            {
              if (scoreItr->second > maxScore)
              {
                maxScore = scoreItr->second;
                highestScoreName = scoreItr->first;
              }
            }

            std::vector<std::string> mapLabels = classMap[_testClasses[j]];

            std::string matchClass;

            for (unsigned int i = 0; i < mapLabels.size(); i++)
            {
              matchClass = mapLabels[i];
              if (matchClass == highestScoreName)  //If any class name matched then that is the classification
              {
                break;
              }
            }

            confusionCount[matchClass][highestScoreName] += 1;
          }

          //Output confusion matrix to file
          for (setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
          {
            confStream << * setItr;
            for (setItr2 = classLabels.begin(); setItr2 != classLabels.end(); ++setItr2)
            {
              confStream << "\t" << confusionCount[*setItr][*setItr2];
            }
            confStream << std::endl;
          }

          confStream.close();

          //Output results to file
          for (unsigned int j = 0; j < _results.size(); j++)
          {
            rsltStream << _testClasses[j] << "\t" << _testObjectIds[j] << "\t";

            std::map<std::string, double> curScore = _results[j];

            std::string highestScoreName;
            double maxScore = -1.0;

            for (scoreItr = _results[j].begin(); scoreItr != _results[j].end(); ++scoreItr)
            {
              if (scoreItr->second > maxScore)
              {
                maxScore = scoreItr->second;
                highestScoreName = scoreItr->first;
              }
            }

            for (setItr = classLabels.begin(); setItr != classLabels.end(); ++setItr)
            {
              rsltStream << curScore[*setItr] << "\t";
            }

            rsltStream << highestScoreName << "\t" << std::endl;
          }
          rsltStream.close();
        }
        else
        {
          throw Exception(__LINE__,
            "Unable to open results file or confusion matrix file for writing");
        }
      }
      else
      {
        throw Exception(__LINE__, "Unable to generate reports from empty result set");
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::_trainBinary(unsigned int numTrees, unsigned int numFactors,
    unsigned int nodeSize, double retrain, bool balanced)
  {
    try
    {
      std::set<std::string> classNames = _data->getClassLabels();
      std::set<std::string>::iterator itr;
      //Create one forest per class label
      _rfList.resize(classNames.size());

      unsigned int itrCtr = 0;
      for (itr = classNames.begin(); itr != classNames.end(); ++itr)
      {
        std::string currentClass = *itr;

        _rfList[itrCtr]->trainBinary(_data, numTrees, numFactors, currentClass,
          nodeSize, retrain, balanced);
        itrCtr++;
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::_trainMultiClass(unsigned int numTrees,
    unsigned int numFactors, unsigned int nodeSize, double retrain, bool balanced)
  {
    try
    {
      _rfList.resize(1);
      _rfList[0]->trainMulticlass(_data, numTrees, numFactors, nodeSize, retrain, balanced);
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }

  void BaseRandomForestManager::_trainRoundRobin(unsigned int numTrees,
    unsigned int numFactors, unsigned int nodeSize, double retrain, bool balanced)
  {
    try
    {
      std::set<std::string> classNames = _data->getClassLabels();

      unsigned int numClasses = _data->getClassLabels().size();

      //Create one forest per class label
      _rfList.resize(numClasses * (numClasses - 1) / 2);

      unsigned int itrCtr = 0;
      for (std::set<std::string>::iterator itr = classNames.begin(); itr != classNames.end(); ++itr)
      {
        for ( std::set<std::string>::iterator itr2 = itr; itr2 != classNames.end(); ++itr2)
        {
          std::string posClass = *itr;
          std::string negClass = *itr2;

          if (posClass != negClass)
          {
            _rfList[itrCtr]->trainRoundRobin(_data, numTrees, numFactors, posClass, negClass,
              nodeSize, retrain, balanced);
            itrCtr++;
          }
        }
      }
    }
    catch(const Exception & e)
    {
      throw Exception(typeid(this).name(), __FUNCTION__, __LINE__, e);
    }
  }
}

