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

      std::vector<std::string> readerNames =
        Factory::getInstance().getObjectNamesByBase(OsmMapReader::className());
      QSet<QString> formats;
      for (size_t i = 0; i < readerNames.size(); i++)
      {
        boost::shared_ptr<OsmMapReader> c(
          Factory::getInstance().constructObject<OsmMapReader>(readerNames[i]));
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
      formatsList.sort();

      for (int i = 0; i < formatsList.size(); i++)
      {
        std::cout << formatsList.at(i) << std::endl;
      }
      std::cout << std::endl;
    }

    if (args.size() == 0 || args.contains("--output"))
    {
      std::cout << "Output formats:" << std::endl << std::endl;

      std::vector<std::string> writerNames =
        Factory::getInstance().getObjectNamesByBase(OsmMapWriter::className());
      QSet<QString> formats;
      for (size_t i = 0; i < writerNames.size(); i++)
      {
        boost::shared_ptr<OsmMapWriter> c(
          Factory::getInstance().constructObject<OsmMapWriter>(writerNames[i]));
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
      formats += OgrUtilities::getInstance().getSupportedFormats(false);
      QStringList formatsList = formats.toList();
      //not any better way to add these right now
      formatsList.append(".osc");
      formatsList.append(".osc.sql");
      formatsList.sort();

      for (int i = 0; i < formatsList.size(); i++)
      {
        std::cout << formatsList.at(i) << std::endl;
      }
      std::cout << std::endl;
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, FormatsCmd)

}

