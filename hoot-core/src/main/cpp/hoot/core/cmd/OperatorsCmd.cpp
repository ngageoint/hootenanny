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
      return
        QString::fromStdString(name1).replace("hoot::", "") <
        QString::fromStdString(name2).replace("hoot::", "");
    }
  };

  template<typename Operator>
  void printOperators(const std::string& operatorClassName, const QString operatorType)
  {
    //the size of the longest operator/type names plus a 3 space buffer
    const int maxNameSize = 45;
    const int maxTypeSize = 18;

    std::vector<std::string> cmds =
      Factory::getInstance().getObjectNamesByBase(operatorClassName);
    OperatorNameComparator<Operator> operatorNameComparator;
    std::sort(cmds.begin(), cmds.end(), operatorNameComparator);
    for (size_t i = 0; i < cmds.size(); i++)
    {
      LOG_VARD(cmds[i]);
      boost::shared_ptr<Operator> c(
        Factory::getInstance().constructObject<Operator>(cmds[i]));
      if (!c->getDescription().isEmpty())
      {
        const QString name = QString::fromStdString(cmds[i]).replace("hoot::", "");
        const int indentAfterName = maxNameSize - name.size();
        const int indentAfterType = maxTypeSize - operatorType.size();
        const QString line =
          "  " + name + QString(indentAfterName, ' ') + operatorType +
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

    std::cout << "Operators (prepend 'hoot::'):" << std::endl << std::endl;

    DisableLog dl;
    for (int i = 0; i < parsedArgs.size(); i++)
    {
      const QString arg = parsedArgs.at(i);
      if (arg == "--criteria")
      {
        printOperators<ElementCriterion>(ElementCriterion::className(), "criterion");
      }
      else if (arg == "--operations")
      {
        printOperators<OsmMapOperation>(OsmMapOperation::className(), "operation");
      }
      else if (arg == "--visitors")
      {
        printOperators<ElementVisitor>(ElementVisitor::className(), "visitor");
        printOperators<ConstElementVisitor>(ConstElementVisitor::className(), "visitor (const)");
      }
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, OperatorsCmd)

}

