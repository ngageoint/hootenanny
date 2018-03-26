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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class InlineOperatorsCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::InlineOperatorsCmd"; }

  InlineOperatorsCmd() { }

  virtual QString getName() const { return "inline-operators"; }

  virtual QString getDescription() const
  { return "Prints available inline operators on map data"; }

  //TODO: clean this up with templated functions?

  static bool operationCompare(const std::string& n1, const std::string& n2)
  {
    boost::shared_ptr<OsmMapOperation> c1(
      Factory::getInstance().constructObject<OsmMapOperation>(n1));
    boost::shared_ptr<OsmMapOperation> c2(
      Factory::getInstance().constructObject<OsmMapOperation>(n2));

    return c1->getName() < c2->getName();
  }

  static bool criterionCompare(const std::string& n1, const std::string& n2)
  {
    boost::shared_ptr<ElementCriterion> c1(
      Factory::getInstance().constructObject<ElementCriterion>(n1));
    boost::shared_ptr<ElementCriterion> c2(
      Factory::getInstance().constructObject<ElementCriterion>(n2));

    return c1->getName() < c2->getName();
  }

  static bool visitorCompare1(const std::string& n1, const std::string& n2)
  {
    boost::shared_ptr<ElementVisitor> c1(
      Factory::getInstance().constructObject<ElementVisitor>(n1));
    boost::shared_ptr<ElementVisitor> c2(
      Factory::getInstance().constructObject<ElementVisitor>(n2));
    return c1->getName() < c2->getName();
  }

  static bool visitorCompare2(const std::string& n1, const std::string& n2)
  {
    LOG_VARD(n1);
    LOG_VARD(n2);
    boost::shared_ptr<ConstElementVisitor> c1(
      Factory::getInstance().constructObject<ConstElementVisitor>(n1));
    boost::shared_ptr<ConstElementVisitor> c2(
      Factory::getInstance().constructObject<ConstElementVisitor>(n2));
    LOG_VARD(c1.get());
    LOG_VARD(c2.get());
    LOG_VARD(c1->getName());
    LOG_VARD(c2->getName());
    return c1->getName() < c2->getName();
  }

  virtual int runSimple(QStringList args)
  {
    if (args.size() > 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes zero to three parameters.").arg(getName()));
    }

    QStringList parsedArgs;
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args.at(i);
      if (parsedArgs.contains(arg) ||
          (arg != "--operations" && arg != "--visitors" && arg != "--criteria"))
      {
        throw IllegalArgumentException("Invalid parameter: " + arg + " passed to " + getName());
      }
      parsedArgs.append(arg);
    }

    if (parsedArgs.size() == 0)
    {
      parsedArgs.append("--operations");
      parsedArgs.append("--visitors");
      parsedArgs.append("--criteria");
    }

    DisableLog dl;

    //the size of the longest operator name plus a small buffer
    const int indent = 42;
    for (int i = 0; i < parsedArgs.size(); i++)
    {
      const QString arg = parsedArgs.at(i);
      if (arg == "--operations")
      {
        std::vector<std::string> cmds =
          Factory::getInstance().getObjectNamesByBase(OsmMapOperation::className());
        std::sort(cmds.begin(), cmds.end(), operationCompare);
        for (size_t i = 0; i < cmds.size(); i++)
        {
          boost::shared_ptr<OsmMapOperation> c(
            Factory::getInstance().constructObject<OsmMapOperation>(cmds[i]));
          if (!c->getName().isEmpty() && !c->getDescription().isEmpty())
          {
            LOG_VARD(c->getName());
            const int spaceSize = indent - c->getName().size();
            const QString line =
              "  " + c->getName() + QString(spaceSize, ' ') + "operation         " +
              c->getDescription();
            std::cout << line << std::endl;
          }
        }
        std::cout << std::endl;
      }
      else if (arg == "--visitors")
      {
        std::vector<std::string> cmds1 =
          Factory::getInstance().getObjectNamesByBase(ElementVisitor::className());
        std::sort(cmds1.begin(), cmds1.end(), visitorCompare1);
        for (size_t i = 0; i < cmds1.size(); i++)
        {
          boost::shared_ptr<ElementVisitor> c(
            Factory::getInstance().constructObject<ElementVisitor>(cmds1[i]));
          if (!c->getName().isEmpty() && !c->getDescription().isEmpty())
          {
            LOG_VARD(c->getName());
            const int spaceSize = indent - c->getName().size();
            const QString line =
              "  " + c->getName() + QString(spaceSize, ' ') + "visitor           " +
              c->getDescription();
            std::cout << line << std::endl;
          }
        }
        std::cout << std::endl;

        std::vector<std::string> cmds2 =
          Factory::getInstance().getObjectNamesByBase(ConstElementVisitor::className());
        LOG_VARD(cmds2);
        std::sort(cmds2.begin(), cmds2.end(), visitorCompare2);
        LOG_VARD(cmds2);
        for (size_t i = 0; i < cmds2.size(); i++)
        {
          LOG_VARD(cmds2[i]);
          boost::shared_ptr<ConstElementVisitor> c(
            Factory::getInstance().constructObject<ConstElementVisitor>(cmds2[i]));
          if (!c->getName().isEmpty() && !c->getDescription().isEmpty())
          {
            LOG_VARD(c->getName());
            const int spaceSize = indent - c->getName().size();
            const QString line =
              "  " + c->getName() + QString(spaceSize, ' ') + "visitor (const)   " +
              c->getDescription();
            std::cout << line << std::endl;
          }
        }
        std::cout << std::endl;
      }
      else if (arg == "--criteria")
      {
        std::vector<std::string> cmds =
          Factory::getInstance().getObjectNamesByBase(ElementCriterion::className());
        std::sort(cmds.begin(), cmds.end(), criterionCompare);
        for (size_t i = 0; i < cmds.size(); i++)
        {
          boost::shared_ptr<ElementCriterion> c(
            Factory::getInstance().constructObject<ElementCriterion>(cmds[i]));
          if (!c->getName().isEmpty() && !c->getDescription().isEmpty())
          {
            LOG_VARD(c->getName());
            const int spaceSize = indent - c->getName().size();
            const QString line =
              "  " + c->getName() + QString(spaceSize, ' ') + "criterion         " +
              c->getDescription();
            std::cout << line << std::endl;
          }
        }
        std::cout << std::endl;
      }
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, InlineOperatorsCmd)

}

