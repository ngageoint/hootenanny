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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "FormatsDisplayer.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OgrUtilities.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapWriter.h>

// Qt
#include <QTextStream>

namespace hoot
{

template<typename IoClass>
QString getFormats(const std::string& className, const QStringList extraFormats = QStringList())
{
  std::vector<std::string> readerNames =
    Factory::getInstance().getObjectNamesByBase(className);
  QSet<QString> formats;
  for (size_t i = 0; i < readerNames.size(); i++)
  {
    std::shared_ptr<IoClass> c(
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

  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  for (int i = 0; i < formatsList.size(); i++)
    ts << formatsList.at(i) << endl;
  ts << endl;

  return ts.readAll();
}

QString FormatsDisplayer::display(const bool displayInputs, const bool displayOutputs)
{
  DisableLog dl;

  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  if (displayInputs)
  {
    ts << "Input formats:" << endl << endl;
    ts << getFormats<OsmMapReader>(OsmMapReader::className()) << endl;
  }

  if (displayOutputs)
  {
    ts << "Output formats:" << endl << endl;
    QStringList formatsList;
    //These are supported by the changeset writers, who aren't map readers/writers.  Possibly,
    //a lightweight interface could be used on all the readers writers instead of modifying
    //OsmMapReader/OsmMapwriter with the supportedFormats method to make this better.
    formatsList.append(".osc");
    formatsList.append(".osc.sql");
    ts << getFormats<OsmMapWriter>(OsmMapWriter::className(), formatsList) << endl;
  }
  return ts.readAll();
}

}
