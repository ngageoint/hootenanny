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
#include "RandomForestUtilities.h"

//Qt Includes
#include <QFileInfo>
#include <QString>

//Std Includes
#include <fstream>
#include <iostream>

#include <tgs/TgsException.h>
#include <tgs/RandomForest/RandomForestManager.h>

namespace Tgs
{
  std::vector<std::map<std::string, std::pair<unsigned int, unsigned int>>> RandomForestUtilities::createDataSets(
    RandomForestInputs rfInputs, std::vector<double> splitPercentages)
  {
    try
    {
      std::vector<std::map<std::string, std::pair<unsigned int, unsigned int>>> dataSets(splitPercentages.size());

      std::map<std::string, std::vector<std::vector<double>>>::iterator trainItr;

      for (trainItr = rfInputs.trainingVectors.begin(); trainItr != rfInputs.trainingVectors.end(); ++trainItr)
      {
        std::string className = trainItr->first;

        double startPercent = 0;

        for (unsigned int i = 0; i < splitPercentages.size(); i++)
        {

          double endPercent = splitPercentages[i];

          if (i == splitPercentages.size() - 1 && endPercent != 1.0)
          {
            throw Exception(__LINE__, "The final splitPercentages entry should be 1.0");
          }

          dataSets[i][className].first = rfInputs.trainingVectors[className].size() * startPercent;
          dataSets[i][className].second =
            rfInputs.trainingVectors[className].size() * endPercent;

          startPercent = endPercent;
        }
      }

      return dataSets;
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception("RandomForestUtilities", __FUNCTION__, __LINE__, e);
    }
  }

  RandomForestManager RandomForestUtilities::generateModel(RandomForestInputs & rfInputs,
    unsigned int numTrees,
    std::map<std::string, std::pair<unsigned int, unsigned int>> & trainingRangeMap)
  {
    try
    {
      Tgs::RandomForestManager initialModel;

      initialModel.init(BaseRandomForestManager::MULTICLASS, rfInputs.featureLabels);

      std::map<std::string, std::vector<std::vector<double>>>::iterator mapItr;

      for (mapItr = rfInputs.trainingVectors.begin(); mapItr != rfInputs.trainingVectors.end(); ++mapItr)
      {
        std::string className = mapItr->first;

        unsigned int initialTrainingIndex = trainingRangeMap[className].first;
        unsigned int initialTrainingSize = trainingRangeMap[className].second;

        for (unsigned int i = 0; i < initialTrainingSize; i++)
        {
          initialModel.addTrainingVector(className, mapItr->second[initialTrainingIndex + i]);
        }
      }

      initialModel.train(numTrees);

      return initialModel;
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception("RandomForestUtilities", __FUNCTION__, __LINE__, e);
    }
  }

  MultithreadedRandomForestManager RandomForestUtilities::generateMultithreadModel(
    RandomForestInputs & rfInputs, unsigned int numTrees,
    std::map<std::string, std::pair<unsigned int, unsigned int>> & trainingRangeMap)
  {
    try
    {
      std::cerr << "DEBUG GEN MULTI MODEL" << std::endl;

      MultithreadedRandomForestManager initialModel;

      initialModel.init(BaseRandomForestManager::MULTICLASS, rfInputs.featureLabels);

      std::map<std::string, std::vector<std::vector<double>>>::iterator mapItr;

      for (mapItr = rfInputs.trainingVectors.begin(); mapItr != rfInputs.trainingVectors.end(); ++mapItr)
      {
        std::string className = mapItr->first;

        unsigned int initialTrainingIndex = trainingRangeMap[className].first;
        unsigned int initialTrainingSize = trainingRangeMap[className].second;

        for (unsigned int i = 0; i < initialTrainingSize; i++)
        {
          initialModel.addTrainingVector(className, mapItr->second[initialTrainingIndex + i]);
        }
      }

      initialModel.train(numTrees);

      return initialModel;
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception("RandomForestUtilities", __FUNCTION__, __LINE__, e);
    }
  }

  RandomForestInputs RandomForestUtilities::generateTrainingDataFromFile(
    const std::string& filename)
  {
    try
    {
      QFileInfo fi(filename.c_str());
      QString suffix = fi.suffix().toUpper();

      if (suffix == "ARFF")
      {
        return _generateTrainingDataFromAARFFile(filename);

      }
      else
      {
        std::stringstream ss;
        ss << "The file suffix " << suffix.toLatin1().constData() << " is not supported.";
        throw Tgs::Exception(__LINE__, ss.str());
      }
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception("RandomForestUtilities", __FUNCTION__, __LINE__, e);
    }
  }

  RandomForestInputs RandomForestUtilities::_generateTrainingDataFromAARFFile(
    const std::string& filename)
  {
    try
    {
      RandomForestInputs rfInputs;

      std::fstream arrfStream(filename.c_str(), std::fstream::in);

      if (!arrfStream.is_open())
      {
        std::stringstream ss;
        ss << "Unable to open file " << filename;
        throw Tgs::Exception(__LINE__, ss.str());
      }

      bool readingData = false;
      int dataLine = 0;

      std::string buffer;
      while (!arrfStream.eof())
      {
        std::getline(arrfStream, buffer);

        if (buffer.empty())
        {
          dataLine++;
          continue;
        }

        std::stringstream bufferStr(buffer);
        std::vector<std::string> tokenList;
        std::string token;

        if (readingData)  //Read training data
        {
          while (std::getline(bufferStr, token, ','))
          {
            tokenList.push_back(token);
          }

          if ((tokenList.size() - 1) != rfInputs.featureLabels.size())
          {
            throw Tgs::Exception(__LINE__, "The number features read in the attribute section does"
              " not match the number of values in the data section");
          }

          std::vector<double> data(tokenList.size() - 1);

          bool parseOk;

          for (unsigned int i = 0; i < tokenList.size(); i++)
          {
            if (i != tokenList.size() - 1) //Read numeric data
            {

              double value = QString(tokenList[i].c_str()).toDouble(&parseOk);

              if (parseOk)
              {
                data[i] = value;
              }
              else
              {
                std::cerr << "SKIPPING: " << buffer << std::endl;
                break;
  //              std::stringstream ss;
  //              ss << "Parsing of data failed for vector at Line: " << dataLine << " with data vector: " <<
  //                std::endl << buffer;
  //              throw Tgs::Exception(__LINE__, ss.str());
              }
            }
            else //Read class name
            {
              rfInputs.trainingVectors[tokenList[i]].push_back(data);
            }
          }
        }
        else
        {
          while (std::getline(bufferStr, token, ' '))
          {
            tokenList.push_back(token);
          }

          if (tokenList.empty())
          {
            continue;
          }
          else if (tokenList.size() == 1)
          {
            if (tokenList[0] == "@DATA")
            {
              readingData = true;
            }
          }
          else
          {
            if (tokenList[0] == "@ATTRIBUTE")
            {
              if (tokenList[1] != "class")
              {
                if (tokenList.size() == 3) //Format @ATTRIBUTE FeatureName Type
                {
                  rfInputs.featureLabels.push_back(tokenList[1]);
                }
                else
                {
                  throw Tgs::Exception(__LINE__, "Invalid reading for @ATTRIBUTE");
                }
              }
              else
              {
                if (tokenList.size() == 3) //Format @ATTRIBUTE class {class1,class2,classN}
                {
                  char chars[] = "{}";
                  std::string classes = tokenList[2];
                  for (unsigned int i = 0; i < strlen(chars); ++i)
                  {
                    classes.erase (std::remove(classes.begin(), classes.end(), chars[i]), classes.end());
                  }

                  std::stringstream classStr(classes);
                  std::string classToken;

                  while (std::getline(classStr, classToken, ','))
                  {
                    rfInputs.classLabels.push_back(classToken);
                  }
                }
                else
                {
                  throw Tgs::Exception(__LINE__, "Invalid reading for @ATTRIBUTE class");
                }
              }
            }
          }
        }

         dataLine++;
      }

      return rfInputs;
    }
    catch(const Tgs::Exception & e)
    {
      throw Tgs::Exception("RandomForestUtilities", __FUNCTION__, __LINE__, e);
    }
  }
}

