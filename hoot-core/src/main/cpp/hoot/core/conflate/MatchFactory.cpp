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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/MatchThreshold.h>

//Qt
#include <QString>

namespace hoot
{

boost::shared_ptr<MatchFactory> MatchFactory::_theInstance;

MatchFactory::~MatchFactory()
{
}

MatchFactory::MatchFactory()
{
}

Match* MatchFactory::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) const
{
  for (size_t i = 0; i < _creators.size(); ++i)
  {
    Match* m = _creators[i]->createMatch(map, eid1, eid2);

    if (m)
    {
      return m;
    }
  }

  return 0;
}

void MatchFactory::createMatches(const ConstOsmMapPtr& map, vector<const Match*>& matches,
  const Envelope& bounds, boost::shared_ptr<const MatchThreshold> threshold) const
{
  for (size_t i = 0; i < _creators.size(); ++i)
  {
    boost::shared_ptr<MatchCreator> matchCreator = _creators[i];
    _checkMatchCreatorBoundable(matchCreator, bounds);
    if (threshold.get())
    {
      matchCreator->createMatches(map, matches, threshold);
    }
    else
    {
      matchCreator->createMatches(map, matches, matchCreator->getMatchThreshold());
    }
  }
}

void MatchFactory::_checkMatchCreatorBoundable(boost::shared_ptr<MatchCreator> matchCreator,
                                               const Envelope& bounds) const
{
  if (bounds.isNull() == false)
  {
    boost::shared_ptr<Boundable> boundable = dynamic_pointer_cast<Boundable>(matchCreator);
    if (boundable == 0)
    {
      throw HootException("One or more match creators is not boundable and cannot be used with "
                          "boundable match operations.");
    }
    boundable->setBounds(bounds);
  }
}

vector<MatchCreator::Description> MatchFactory::getAllAvailableCreators() const
{
  vector<MatchCreator::Description> result;

  // get all match creators from the factory
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(MatchCreator::className());

  for (size_t i = 0; i < names.size(); i++)
  {
    // get all names known by this creator.
    auto_ptr<MatchCreator> mc(Factory::getInstance().constructObject<MatchCreator>(names[i]));

    vector<MatchCreator::Description> d = mc->getAllCreators();
    result.insert(result.end(), d.begin(), d.end());
  }

  return result;
}

void MatchFactory::_setMatchCreators(QStringList matchCreatorsList)
{
  LOG_DEBUG("MatchFactory creators: " << matchCreatorsList);

  for (int i = 0; i < matchCreatorsList.size(); i++)
  {
    QString c = matchCreatorsList[i];
    QStringList args = c.split(",");
    QString className = args[0];
    LOG_VART(className);
    if (className.length() > 0)
    {
      args.removeFirst();
      boost::shared_ptr<MatchCreator> mc(Factory::getInstance().constructObject<MatchCreator>(className));
      _theInstance->registerCreator(mc);

      if (args.size() > 0)
      {
        mc->setArguments(args);
      }
    }
  }
}

MatchFactory& MatchFactory::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new MatchFactory());
  }

  if (_theInstance->_creators.size() == 0)
  {
    //only get the match creators that are specified in the config
    _setMatchCreators(ConfigOptions(conf()).getMatchCreators().split(";"));
  }
  return *_theInstance;
}

}
