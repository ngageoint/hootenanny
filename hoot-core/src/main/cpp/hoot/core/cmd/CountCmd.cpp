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
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/visitors/FeatureCountVisitor.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class CountCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::CountCmd"; }

  CountCmd() { }

  virtual QString getName() const { return "count"; }

  virtual QString getDescription() const
  { return "Counts the number of features matching a specified criterion"; }

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
    LOG_VARD(countFeaturesOnly);

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
    LOG_VARD(criterionClassName);
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
    double total = 0;
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

      boost::shared_ptr<ElementCriterion> crit;
      try
      {
        crit.reset(
          Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
      }
      catch (const boost::bad_any_cast&)
      {
        throw IllegalArgumentException("Invalid criterion: " + criterionClassName);
      }

      if (ConfigOptions().getElementCriterionNegate())
      {
        crit.reset(new NotCriterion(crit));
      }
      LOG_VART(crit.get());

      boost::shared_ptr<Configurable> critConfig;
      if (crit.get())
      {
        critConfig = boost::dynamic_pointer_cast<Configurable>(crit);
      }
      LOG_VART(critConfig.get());
      if (critConfig.get())
      {
        critConfig->setConfiguration(conf());
      }

      LOG_TRACE("Using criterion...");
      if (countFeaturesOnly)
      {
        total = FilteredVisitor::getStat(crit, featureCountVis, map);
      }
      else
      {
        total = FilteredVisitor::getStat(crit, elementCountVis, map);
      }
    }
    LOG_VART(total);

    std::cout << "Total: " << total << std::endl;
  }
};

HOOT_FACTORY_REGISTER(Command, CountCmd)

}

