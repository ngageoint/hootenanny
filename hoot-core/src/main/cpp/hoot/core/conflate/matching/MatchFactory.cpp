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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MatchFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/criterion/TagAdvancedCriterion.h>
#include <hoot/core/conflate/matching/OptionsValidator.h>

//Qt
#include <QString>

using namespace geos::geom;
using namespace std;

namespace hoot
{

MatchFactory::MatchFactory()
{
  setConfiguration(conf());
}

MatchPtr MatchFactory::createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) const
{
  LOG_VART(eid1);
  LOG_VART(eid2);
  LOG_VART(_creators.size());

  for (size_t i = 0; i < _creators.size(); ++i)
  {
    const QString name = _creators[i]->getName();
    LOG_VART(name);
    MatchPtr m = _creators[i]->createMatch(map, eid1, eid2);
    if (m)
    {
      return m;
    }
  }

  return 0;
}

void MatchFactory::createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
  const Envelope& bounds, std::shared_ptr<const MatchThreshold> threshold) const
{
  for (size_t i = 0; i < _creators.size(); ++i)
  { 
    std::shared_ptr<MatchCreator> matchCreator = _creators[i];
    LOG_STATUS(
      "Launching matcher: " << i + 1 << " / " << _creators.size() << ": " <<
      matchCreator->getName() << "...");
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

void MatchFactory::_checkMatchCreatorBoundable(const std::shared_ptr<MatchCreator>& matchCreator,
                                               const Envelope& bounds) const
{
  if (bounds.isNull() == false)
  {
    std::shared_ptr<Boundable> boundable = std::dynamic_pointer_cast<Boundable>(matchCreator);
    if (boundable == 0)
    {
      throw HootException("One or more match creators is not boundable and cannot be used with "
                          "boundable match operations.");
    }
    boundable->setBounds(bounds);
  }
}

vector<CreatorDescription> MatchFactory::getAllAvailableCreators() const
{
  vector<CreatorDescription> result;

  // get all match creators from the factory
  vector<std::string> names =
    Factory::getInstance().getObjectNamesByBase(MatchCreator::className());

  for (size_t i = 0; i < names.size(); i++)
  {
    // get all names known by this creator.
    std::shared_ptr<MatchCreator> mc(
      Factory::getInstance().constructObject<MatchCreator>(names[i]));

    vector<CreatorDescription> d = mc->getAllCreators();
    result.insert(result.end(), d.begin(), d.end());
  }

  return result;
}

void MatchFactory::registerCreator(const QString& c)
{
  QStringList args = c.split(",");
  QString className = args[0];
  LOG_VART(className);
  if (className.length() > 0)
  {
    args.removeFirst();
    std::shared_ptr<MatchCreator> mc(
      Factory::getInstance().constructObject<MatchCreator>(className));

    if (!_tagFilter.trimmed().isEmpty())
    {
      // We're specifically checking for an option to feed this tag criterion.  Additional combined
      // criteria can be added to this match creator if needed.
      std::shared_ptr<TagAdvancedCriterion> filter(new TagAdvancedCriterion(_tagFilter));
      mc->setCriterion(filter);
    }

    registerCreator(mc);

    if (args.size() > 0)
    {
      mc->setArguments(args);
    }
  }
}

void MatchFactory::_setMatchCreators(QStringList matchCreatorsList)
{
  LOG_DEBUG("MatchFactory creators: " << matchCreatorsList);

  //  Setting the match creators replaces the previous creators
  _creators.clear();

  for (int i = 0; i < matchCreatorsList.size(); i++)
  {
    registerCreator(matchCreatorsList[i]);
  }
}

void MatchFactory::setConfiguration(const Settings& s)
{
  _tagFilter = ConfigOptions(s).getConflateTagFilter();
}

MatchFactory& MatchFactory::getInstance()
{
  static MatchFactory instance;
  if (ConfigOptions().getAutocorrectOptions())
  {
    /* TODO: remove this hack after UI issues are fixed; see OptionsValidator
     *
     * UPDATE: 4/3/20 - Initially thought the above issues should all have been fixed in v2 version
     * of the UI but apparently not. So, keep this in for now.
     * */
    OptionsValidator::fixMisc();

    // Think we may either want to keep this around or throw exceptions with the checks instead.
    OptionsValidator::fixGenericMatcherOrdering();
  }

  // keep this outside of _tempFixDefaults, since it needs to be checked from the command line as
  // well
  OptionsValidator::validateMatchers();

  //only get the match creators that are specified in the config
  if (instance._creators.size() == 0)
    instance._setMatchCreators(ConfigOptions().getMatchCreators());
  return instance;
}

void MatchFactory::reset()
{
  _creators.clear();
  _tagFilter = "";
}

QString MatchFactory::getCreatorsStr() const
{
  QString str;
  for (size_t i = 0; i < _creators.size(); ++i)
  {
    const QString name = _creators[i]->getName();
    str += name + ";";
  }
  str.chop(1);
  return str;
}


}
