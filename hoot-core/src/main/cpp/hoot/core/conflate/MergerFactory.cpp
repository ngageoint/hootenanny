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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MergerFactory.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMapConsumer.h>
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/polygon/BuildingMergerCreator.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

shared_ptr<MergerFactory> MergerFactory::_theInstance;

MergerFactory::MergerFactory()
{
}

MergerFactory::~MergerFactory()
{
  clear();
}

void MergerFactory::clear()
{
  // delete all creators.
  for (size_t i = 0; i < _creators.size(); ++i)
  {
    delete _creators[i];
  }
  _creators.clear();
}

void MergerFactory::createMergers(const OsmMapPtr& map, const MatchSet& matches,
  vector<Merger*>& result) const
{
  for (size_t i = 0; i < _creators.size(); i++)
  {
    OsmMapConsumer* omc = dynamic_cast<OsmMapConsumer*>(_creators[i]);

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

  LOG_WARN("Error finding Mergers for these matches: " << matches);
  LOG_WARN("Creators: " << _creators);
  throw HootException("Error creating a merger for the provided set of matches.");
}

vector<MergerCreator::Description> MergerFactory::getAllAvailableCreators() const
{
  vector<MergerCreator::Description> result;

  // get all match creators from the factory
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(MergerCreator::className());

  for (size_t i = 0; i < names.size(); i++)
  {
    // get all names known by this creator.
    auto_ptr<MergerCreator> mc(Factory::getInstance().constructObject<MergerCreator>(names[i]));

    vector<MergerCreator::Description> d = mc->getAllCreators();
    result.insert(result.end(), d.begin(), d.end());
  }

  return result;
}

MergerFactory &MergerFactory::getInstance()
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

bool MergerFactory::isConflicting(const ConstOsmMapPtr& map, const Match* m1, const Match* m2)
  const
{
  // if any creator considers a match conflicting then it is a conflict
  for (size_t i = 0; i < _creators.size(); i++)
  {
    if (_creators[i]->isConflicting(map, m1, m2))
    {
      return true;
    }
  }
  return false;
}

void MergerFactory::registerDefaultCreators()
{
  QStringList creators = ConfigOptions().getMergerCreators().split(";");
  LOG_DEBUG("MergeFactory creators: " << creators);

  for (int i = 0; i < creators.size(); i++)
  {
    QString c = creators[i];
    QStringList args = c.split(",");
    QString className = args[0];
    args.removeFirst();
    MergerCreator* mc = Factory::getInstance().constructObject<MergerCreator>(
      className);
    registerCreator(mc);

    if (args.size() > 0)
    {
      mc->setArguments(args);
    }
  }
}

}

