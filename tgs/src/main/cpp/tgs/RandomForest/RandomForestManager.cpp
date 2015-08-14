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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RandomForestManager.h"
#include "InternalRandomForestManager.h"

//STD Includes
#include <exception>
#include <fstream>

namespace Tgs
{
  RandomForestManager::RandomForestManager()
  {
    _irfm = new InternalRandomForestManager();
  }

  RandomForestManager::~RandomForestManager()
  {
    delete _irfm;
  }

  void RandomForestManager::addTrainingVector(std::string classLabel, 
    const std::vector<double>& trainVec)
  {
    _irfm->addTrainingVector(classLabel, trainVec);
  }

  void RandomForestManager::classifyTestVector(std::string objId, std::string objClass, 
    std::vector<double> & dataVector, std::map<std::string, double> & scores)
  {
    _irfm->classifyTestVector(objId, objClass, dataVector, scores);
  }

  void RandomForestManager::classifyVector(std::string objId, std::vector<double> & dataVector, 
    std::map<std::string, double> & scores)
  {
    _irfm->classifyVector(objId, dataVector, scores);
  }

  void RandomForestManager::exportModel(std::ostream & fileStream, std::string tabDepth)
  {
    _irfm->exportModel(fileStream, tabDepth);
  }

  void RandomForestManager::exportTrainingData(std::fstream & fileStream, std::string tabDepth)
  {
    _irfm->exportTrainingData(fileStream, tabDepth);
  }

//   void RandomForestManager::findTopFactors(unsigned int numTrees, unsigned int numFactors, 
//     std::vector<std::string> & factorLabels)
//   {
//     _irfm->findTopFactors(numTrees, numFactors, factorLabels);
//   }

  void RandomForestManager::generateRemappedReports(std::string reportName, std::map<std::string,
    std::vector<std::string> > & classMap)
  {
    _irfm->generateRemappedReports(reportName, classMap);
  }

  void RandomForestManager::generateReports(std::string filename)
  {
    _irfm->generateReports(filename);
  }

  void RandomForestManager::generateResults(std::string filename)
  {
    _irfm->generateResults(filename);
  }

  std::set<std::string> RandomForestManager::getClassLabels()
  {
    return _irfm->getClassLabels();
  }

  void RandomForestManager::generateTopFactors(std::string basefilename)
  {
    _irfm->generateTopFactors(basefilename);
  }

  void RandomForestManager::getFactorLabels(std::vector<std::string> & factors)
  {
    _irfm->getFactorLabels(factors);
  }

  void RandomForestManager::import(std::istream & fileStream)
  {
    _irfm->import(fileStream);
  }

  void RandomForestManager::init(unsigned int modelMethod, std::vector<std::string> & factorLabels)
  {
    _irfm->init(modelMethod, factorLabels);
  }

  void RandomForestManager::reset()
  {
    _irfm->reset();
  }

  void RandomForestManager::resetResults()
  {
    _irfm->resetResults();
  }

  void RandomForestManager::setFactorLabels(std::vector<std::string> & factorLabels)
  {
    _irfm->setFactorLabels(factorLabels);
  }

  void RandomForestManager::train(unsigned int numTrees, int numFactors,
    unsigned int nodeSize, double retrain, bool balanced)
  {
    _irfm->train(numTrees, numFactors, nodeSize, retrain, balanced);
  }
}  //End namespace
