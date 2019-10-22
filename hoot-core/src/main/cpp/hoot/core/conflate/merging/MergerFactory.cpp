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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MergerFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

int MergerFactory::logWarnCount = 0;

std::shared_ptr<MergerFactory> MergerFactory::_theInstance;

MergerFactory::MergerFactory()
{
}

MergerFactory::~MergerFactory()
{
  reset();
}

void MergerFactory::reset()
{
  _creators.clear();
}

void MergerFactory::createMergers(const OsmMapPtr& map, const MatchSet& matches,
  vector<MergerPtr>& result) const
{
  for (size_t i = 0; i < _creators.size(); i++)
  {
    PROGRESS_DEBUG("Creating merger " << i + 1 << " / " << _creators.size() << "...");

    OsmMapConsumer* omc = dynamic_cast<OsmMapConsumer*>(_creators[i].get());
    if (omc)
    {
      omc->setOsmMap(map.get());
    }
    if (_creators[i]->createMergers(matches, result))
    {
      return;
    }
    // we don't want the creators to hold onto a map pointer that will go out of scope
    if (omc)
    {
      omc->setOsmMap((OsmMap*)0);
    }
  }

  // In #2069, a ScriptMatch and a NetworkMatch are being grouped together, which ultimately causes
  // the exception below to be thrown.  For now, attempting to bypass and only log a warning.  This
  // also required additional error handling in ScriptMerger (see ScriptMerger::_applyMergePair).
  if (logWarnCount < Log::getWarnMessageLimit())
  {
    LOG_WARN("Unable to create merger for the provided set of matches: " << matches);
  }
  else if (logWarnCount == Log::getWarnMessageLimit())
  {
    LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
  }
  logWarnCount++;
  //throw HootException("Error creating a merger for the provided set of matches.");
}

vector<CreatorDescription> MergerFactory::getAllAvailableCreators() const
{
  vector<CreatorDescription> result;

  // get all match creators from the factory
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(MergerCreator::className());

  for (size_t i = 0; i < names.size(); i++)
  {
    // get all names known by this creator.
    std::shared_ptr<MergerCreator> mc(
      Factory::getInstance().constructObject<MergerCreator>(names[i]));

    vector<CreatorDescription> d = mc->getAllCreators();
    result.insert(result.end(), d.begin(), d.end());
  }

  return result;
}

MergerFactory& MergerFactory::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new MergerFactory());
  }

  if (_theInstance->_creators.size() == 0)
  {
    _theInstance->registerDefaultCreators();
  }

  return *_theInstance;
}

bool MergerFactory::isConflicting(const ConstOsmMapPtr& map, const ConstMatchPtr& m1,
                                  const ConstMatchPtr& m2) const
{
  LOG_VART(_creators.size());
  // if any creator considers a match conflicting then it is a conflict
  for (size_t i = 0; i < _creators.size(); i++)
  {
    if (_creators[i]->isConflicting(map, m1, m2))
    {
      LOG_TRACE("Conflicting matches: " << m1 << ", " << m2);
      return true;
    }
  }
  return false;
}

void MergerFactory::registerDefaultCreators()
{  
  const QStringList mergerCreators = ConfigOptions().getMergerCreators();
  LOG_VARD(mergerCreators);
  for (int i = 0; i < mergerCreators.size(); i++)
  {
    QString c = mergerCreators[i];
    QStringList args = c.split(",");
    QString className = args[0];
    if (className.length() > 0)
    {
      args.removeFirst();
      MergerCreatorPtr mc(Factory::getInstance().constructObject<MergerCreator>(className));
      registerCreator(mc);

      if (args.size() > 0)
      {
        mc->setArguments(args);
      }
    }
  }
}

}

