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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/SingleStatistic.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/visitors/FeatureCountVisitor.h>

namespace hoot
{

class CountFeaturesCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::CountFeaturesCmd"; }

  CountFeaturesCmd() { }

  virtual QString getName() const { return "count-features"; }

  virtual QString getDescription() const
  { return "Counts the number of features matching an optionally specified criterion"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 1 || args.size() > 3)
    {
      LOG_VAR(args);
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes one to three parameters.").arg(getName()));
    }

    bool countFeaturesOnly = true;
    if (args.contains("--all-elements"))
    {
      countFeaturesOnly = false;
      args.removeAt(args.indexOf("--all-elements"));
      LOG_VART(args);
    }

    LOG_VARD(args[0]);
    if (args.size() > 1)
    {
      LOG_VARD(args[1]);
    }

    const QStringList inputs = args[0].split(";");
    LOG_VART(inputs.size());
    OsmMapPtr map(new OsmMap());
    _readInputs(inputs, map);

    QString criterionClassName = "";
    if (args.size() > 1)
    {
      criterionClassName = args[1];
    }
    _applyOperator(criterionClassName, map, countFeaturesOnly);

    return 0;
  }

private:

  void _readInputs(const QStringList inputs, OsmMapPtr map)
  {
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs.at(i);
      boost::shared_ptr<OsmMapReader> reader =
        OsmMapReaderFactory::getInstance().createReader(input, true, Status::Unknown1);
      reader->open(input);
      reader->read(map);
    }
  }

  void _applyOperator(const QString criterionClassName, OsmMapPtr map, const bool countFeaturesOnly)
  {
    int total = 0;
    boost::shared_ptr<ElementCountVisitor> elementCountVis(new ElementCountVisitor());
    boost::shared_ptr<FeatureCountVisitor> featureCountVis(new FeatureCountVisitor());
    if (criterionClassName.trimmed().isEmpty())
    {
      //count all the elements/features

      if (countFeaturesOnly)
      {
        map->visitRo(*featureCountVis);
        total = (int)featureCountVis->getStat();
      }
      else
      {
        map->visitRo(*elementCountVis);
        total = (int)elementCountVis->getStat();
      }
    }
    else
    {
      //filter the elements/features

      boost::shared_ptr<ConstElementVisitor> visFilter;
      try
      {
        visFilter.reset(
          Factory::getInstance().constructObject<ConstElementVisitor>(criterionClassName));
      }
      catch (const boost::bad_any_cast&)
      {
      }
      LOG_VART(visFilter.get());
      boost::shared_ptr<SingleStatistic> singleStat;
      if (visFilter.get())
      {
        singleStat = boost::dynamic_pointer_cast<SingleStatistic>(visFilter);
      }
      LOG_VART(singleStat.get());
      boost::shared_ptr<ElementCriterion> crit;
      try
      {
        crit.reset(
          Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
        if (ConfigOptions().getElementCriterionNegate())
        {
          crit.reset(new NotCriterion(crit));
        }
      }
      catch (const boost::bad_any_cast&)
      {
      }
      LOG_VART(crit.get());

      if (visFilter.get() && singleStat.get())
      {
        map->visitRo(*visFilter);
        total = (int)singleStat->getStat();
      }
      else if (crit.get())
      {
        if (countFeaturesOnly)
        {
          total = (int)FilteredVisitor::getStat(crit, featureCountVis, map);
        }
        else
        {
          total = (int)FilteredVisitor::getStat(crit, elementCountVis, map);
        }
      }
      else
      {
        throw IllegalArgumentException("Invalid filter: " + criterionClassName);
      }
    }
    LOG_VART(total);

    std::cout << "Total: " << total << std::endl;
  }
};

HOOT_FACTORY_REGISTER(Command, CountFeaturesCmd)

}

