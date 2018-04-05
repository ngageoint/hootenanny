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
#include <hoot/core/conflate/extractors/FeatureExtractor.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

class FeatureExtractorsCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::FeatureExtractorsCmd"; }

  FeatureExtractorsCmd() { }

  virtual QString getName() const { return "feature-extractors"; }

  virtual QString getDescription() const
  { return "Prints available feature extractors"; }

  static bool extractorCompare(const std::string& n1, const std::string& n2)
  {
    const QString cleanedName1 = QString::fromStdString(n1).replace("hoot::", "");
    const QString cleanedName2 = QString::fromStdString(n2).replace("hoot::", "");
    return cleanedName1 < cleanedName2;
  }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 0)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes zero parameters.").arg(getName()));
    }

    std::cout << "Feature extractors (prepend 'hoot::'):" << std::endl << std::endl;

    DisableLog dl;

    //the size of the longest extractor name plus a small buffer
    const int indent = 40;
    std::vector<std::string> extractorNames =
      Factory::getInstance().getObjectNamesByBase(FeatureExtractor::className());
    std::sort(extractorNames.begin(), extractorNames.end(), extractorCompare);
    for (size_t i = 0; i < extractorNames.size(); i++)
    {
      boost::shared_ptr<FeatureExtractor> c(
        Factory::getInstance().constructObject<FeatureExtractor>(extractorNames[i]));
      const QString cleanedName = QString::fromStdString(extractorNames[i]).replace("hoot::", "");
      if (!cleanedName.isEmpty() && !c->getDescription().isEmpty())
      {
        LOG_VARD(cleanedName);
        const int spaceSize = indent - cleanedName.size();
        const QString line = "  " + cleanedName + QString(spaceSize, ' ') + c->getDescription();
        std::cout << line << std::endl;
      }
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, FeatureExtractorsCmd)

}

