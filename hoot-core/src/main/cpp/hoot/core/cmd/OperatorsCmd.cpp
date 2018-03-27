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

class OperatorsCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::OperatorsCmd"; }

  OperatorsCmd() { }

  virtual QString getName() const { return "operators"; }

  virtual QString getDescription() const
  { return "Prints available inline operators on map data"; }

  template<typename Operator>
  class OperatorNameComparator
  {
  public:

    OperatorNameComparator() {}

    bool operator()(const std::string& name1, const std::string& name2)
    {
      boost::shared_ptr<Operator> op1(
        Factory::getInstance().constructObject<Operator>(name1));
      boost::shared_ptr<Operator> op2(
        Factory::getInstance().constructObject<Operator>(name2));
      return op1->getName() < op2->getName();
    }
  };

  template<typename Operator>
  void printOperators(const std::string& operatorClassName, const QString operatorType,
                      const int indentAfterType)
  {
    const int indent = 42;
    std::vector<std::string> cmds =
      Factory::getInstance().getObjectNamesByBase(operatorClassName);
    OperatorNameComparator<OsmMapOperation> operatorNameComparator;
    std::sort(cmds.begin(), cmds.end(), operatorNameComparator);
    for (size_t i = 0; i < cmds.size(); i++)
    {
      boost::shared_ptr<Operator> c(
        Factory::getInstance().constructObject<Operator>(cmds[i]));
      if (!c->getName().isEmpty() && !c->getDescription().isEmpty())
      {
        LOG_VARD(c->getName());
        const int indentAfterName = indent - c->getName().size();
        const QString line =
          "  " + c->getName() + QString(indentAfterName, ' ') + operatorType +
          QString(indentAfterType, ' ') + c->getDescription();
        std::cout << line << std::endl;
      }
    }
    std::cout << std::endl;
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
      parsedArgs.append("--criteria");
      parsedArgs.append("--operations");
      parsedArgs.append("--visitors");
    }

    DisableLog dl;

    //the size of the longest operator name plus a small buffer
    const int indent = 42;
    for (int i = 0; i < parsedArgs.size(); i++)
    {
      const QString arg = parsedArgs.at(i);
      if (arg == "--operations")
      {
//        std::vector<std::string> cmds =
//          Factory::getInstance().getObjectNamesByBase(OsmMapOperation::className());
//        OperatorNameComparator<OsmMapOperation> onc;
//        std::sort(cmds.begin(), cmds.end(), onc);
//        for (size_t i = 0; i < cmds.size(); i++)
//        {
//          boost::shared_ptr<OsmMapOperation> c(
//            Factory::getInstance().constructObject<OsmMapOperation>(cmds[i]));
//          if (!c->getName().isEmpty() && !c->getDescription().isEmpty())
//          {
//            LOG_VARD(c->getName());
//            const int spaceSize = indent - c->getName().size();
//            const QString line =
//              "  " + c->getName() + QString(spaceSize, ' ') + "operation         " +
//              c->getDescription();
//            std::cout << line << std::endl;
//          }
//        }
//        std::cout << std::endl;

        printOperators<OsmMapOperation>(OsmMapOperation::className(), "operation", 10);
      }
      else if (arg == "--visitors")
      {
        std::vector<std::string> cmds1 =
          Factory::getInstance().getObjectNamesByBase(ElementVisitor::className());
        OperatorNameComparator<ElementVisitor> onc;
        std::sort(cmds1.begin(), cmds1.end(), onc);
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
        OperatorNameComparator<ConstElementVisitor> onc2;
        std::sort(cmds2.begin(), cmds2.end(), onc2);
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
        OperatorNameComparator<ElementCriterion> onc;
        std::sort(cmds.begin(), cmds.end(), onc);
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

HOOT_FACTORY_REGISTER(Command, OperatorsCmd)

}

