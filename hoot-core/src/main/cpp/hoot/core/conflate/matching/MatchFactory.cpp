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
#include "MatchFactory.h"

// hoot
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/OptionsValidator.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/TagAdvancedCriterion.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

//Qt
#include <QString>

using namespace geos::geom;
using namespace std;

namespace hoot
{

MatchFactory::MatchFactory() :
_negateCritFilter(false)
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

  return nullptr;
}

void MatchFactory::createMatches(const ConstOsmMapPtr& map, std::vector<ConstMatchPtr>& matches,
  const std::shared_ptr<Geometry>& bounds, std::shared_ptr<const MatchThreshold> threshold) const
{
  for (size_t i = 0; i < _creators.size(); ++i)
  { 
    std::shared_ptr<MatchCreator> matchCreator = _creators[i];
    LOG_STATUS(
      "Running matcher: " << i + 1 << " / " << _creators.size() << ": " <<
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

void MatchFactory::_checkMatchCreatorBoundable(
  const std::shared_ptr<MatchCreator>& matchCreator, const std::shared_ptr<Geometry>& bounds) const
{
  if (bounds.get())
  {
    std::shared_ptr<Boundable> boundable = std::dynamic_pointer_cast<Boundable>(matchCreator);
    if (boundable == nullptr)
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

  // Get all match creators from the factory.
  vector<QString> names = Factory::getInstance().getObjectNamesByBase(MatchCreator::className());
  for (size_t i = 0; i < names.size(); i++)
  {
    // Get all names known by this creator.
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
    mc->setFilter(_createFilter());

    registerCreator(mc);

    if (!args.empty())
    {
      mc->setArguments(args);
    }
  }
}

ElementCriterionPtr MatchFactory::_createFilter()
{
  ElementCriterionPtr filter;

  // Offering the option here to use both a tag criterion and another criterion of any type. Perhaps
  // these can be combined into one.

  ElementCriterionPtr tagFilter;
  if (!_tagFilterJson.trimmed().isEmpty())
  {
    // We're specifically checking for an option to feed this tag criterion. Additional combined
    // criteria can be added to this match creator if needed.
    tagFilter.reset(new TagAdvancedCriterion(_tagFilterJson));
  }

  ElementCriterionPtr critFilter;
  if (!_critFilterClassName.trimmed().isEmpty())
  {
    ElementCriterionPtr elementCrit(
      Factory::getInstance().constructObject<ElementCriterion>(_critFilterClassName));
    if (_negateCritFilter)
    {
      critFilter.reset(new NotCriterion(elementCrit));
    }
    else
    {
      critFilter = elementCrit;
    }
  }

  if (tagFilter && critFilter)
  {
    filter.reset(new ChainCriterion(tagFilter, critFilter));
  }
  else if (tagFilter)
  {
    filter = tagFilter;
  }
  else if (critFilter)
  {
    filter = critFilter;
  }

  return filter;
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
  ConfigOptions configOpts(s);
  _tagFilterJson = configOpts.getConflateTagFilter();
  _critFilterClassName = configOpts.getConflateElementCriterion();
  _negateCritFilter = configOpts.getConflateElementCriterionNegate();
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
  if (instance._creators.empty())
    instance._setMatchCreators(ConfigOptions().getMatchCreators());
  return instance;
}

void MatchFactory::reset()
{
  _creators.clear();
  _tagFilterJson = "";
  _critFilterClassName = "";
  _negateCritFilter = false;
}

}
