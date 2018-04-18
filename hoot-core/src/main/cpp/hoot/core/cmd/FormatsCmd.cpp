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
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OgrUtilities.h>

// Qt
#include <QSet>

namespace hoot
{

class FormatsCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::FormatsCmd"; }

  FormatsCmd() { }

  virtual QString getName() const { return "formats"; }

  virtual QString getDescription() const
  { return "Prints supported data formats"; }

  template<typename IoClass>
  void printFormats(const std::string& className, const QStringList extraFormats = QStringList())
  {
    std::vector<std::string> readerNames =
      Factory::getInstance().getObjectNamesByBase(className);
    QSet<QString> formats;
    for (size_t i = 0; i < readerNames.size(); i++)
    {
      boost::shared_ptr<IoClass> c(
        Factory::getInstance().constructObject<IoClass>(readerNames[i]));
      const QString supportedFormats = c->supportedFormats();
      if (!supportedFormats.isEmpty())
      {
        QStringList supportedFormatsList = supportedFormats.split(";");
        for (int j = 0; j < supportedFormatsList.size(); j++)
        {
          formats.insert(supportedFormatsList.at(j));
        }
      }
    }
    formats += OgrUtilities::getInstance().getSupportedFormats(true);
    QStringList formatsList = formats.toList();
    formatsList.append(extraFormats);
    formatsList.sort();

    for (int i = 0; i < formatsList.size(); i++)
    {
      std::cout << formatsList.at(i) << std::endl;
    }
    std::cout << std::endl;
  }

  virtual int runSimple(QStringList args)
  {
    if (args.size() > 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes between zero and two parameters.").arg(getName()));
    }

    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args.at(i);
      if (arg != "--input" && arg != "--output")
      {
        throw IllegalArgumentException("Invalid parameter: " + arg + " passed to " + getName());
      }
    }

    DisableLog dl;

    if (args.size() == 0 || args.contains("--input"))
    {
      std::cout << "Input formats:" << std::endl << std::endl;
      printFormats<OsmMapReader>(OsmMapReader::className());
    }

    if (args.size() == 0 || args.contains("--output"))
    {
      std::cout << "Output formats:" << std::endl << std::endl;
      QStringList formatsList;
      //These are supported by the changeset writers, who aren't map readers/writers.  Possibly,
      //a lightweight interface could be used on all the readers writers instead of modifying
      //OsmMapReader/OsmMapwriter with the supportedFormats method to make this better.
      formatsList.append(".osc");
      formatsList.append(".osc.sql");
      printFormats<OsmMapWriter>(OsmMapWriter::className(), formatsList);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, FormatsCmd)

}

