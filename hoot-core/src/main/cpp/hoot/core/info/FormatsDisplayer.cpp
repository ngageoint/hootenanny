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
#include <hoot/core/util/Boundable.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/PartialOsmMapWriter.h>

// Qt
#include <QTextStream>

namespace hoot
{

QString FormatsDisplayer::display(
  const bool displayInputs, const bool displayInputsSupportingStreaming,
  const bool displayInputsSupportingBounds, const bool displayOutputs,
  const bool displayOutputsSupportingStreaming, const bool displayOgrOnly)
{
  DisableLog dl;

  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");

  if (displayInputs)
  {
    ts << "Input formats:" << endl << endl;
    ts <<
      _getFormatsString<OsmMapReader>(
        OsmMapReader::className(), QStringList(), displayOgrOnly, true)
       << endl;
  }

  if (displayInputsSupportingStreaming)
  {
    ts << "Input formats supporting streaming:" << endl << endl;
    ts << _getInputFormatsSupportingStreamingString(displayOgrOnly) << endl;
  }

  if (displayInputsSupportingBounds)
  {
    ts << "Input formats supporting bounded reading:" << endl << endl;
    ts << _getFormatsSupportingBoundsString(displayOgrOnly) << endl;
  }

  if (displayOutputs)
  {
    ts << "Output formats:" << endl << endl;
    QStringList formatsList;
    // These are supported by the changeset writers, who aren't map readers/writers.  Possibly,
    // a lightweight interface could be used on all the readers writers instead of modifying
    // OsmMapReader/OsmMapwriter with the supportedFormats method to make this better.
    formatsList.append(".osc");
    formatsList.append(".osc.sql");
    ts <<
      _getFormatsString<OsmMapWriter>(
        OsmMapWriter::className(), formatsList, displayOgrOnly, false)
       << endl;
  }

  if (displayOutputsSupportingStreaming)
  {
    ts << "Output formats supporting streaming:" << endl << endl;
    ts << _getOutputFormatsSupportingStreamingString(displayOgrOnly) << endl;
  }

  return ts.readAll();
}

template<typename IoClass>
QString FormatsDisplayer::_getFormatsString(
  const std::string& className, const QStringList extraFormats, const bool ogrOnly,
  const bool ogrReadOnly)
{
  return _getPrintableString(_getFormats<IoClass>(className, extraFormats, ogrOnly, ogrReadOnly));
}

template<typename IoClass>
QStringList FormatsDisplayer::_getFormats(
  const std::string& className, const QStringList extraFormats, const bool ogrOnly,
  const bool ogrReadOnly)
{
  QSet<QString> formats;

  if (!ogrOnly)
  {
    std::vector<QString> classNames =
      Factory::getInstance().getObjectNamesByBase(className);
    for (size_t i = 0; i < classNames.size(); i++)
    {
      std::shared_ptr<IoClass> ioClass(
        Factory::getInstance().constructObject<IoClass>(classNames[i]));
      const QString supportedFormats = ioClass->supportedFormats();
      if (!supportedFormats.isEmpty())
      {
        QStringList supportedFormatsList = supportedFormats.split(";");
        for (int j = 0; j < supportedFormatsList.size(); j++)
        {
          formats.insert(supportedFormatsList.at(j));
        }
      }
    }
  }

  formats += OgrUtilities::getInstance().getSupportedFormats(ogrReadOnly);

  QStringList formatsList = formats.toList();
  formatsList.append(extraFormats);
  formatsList.sort();
  return formatsList;
}

QString FormatsDisplayer::_getPrintableString(const QStringList& items)
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  for (int i = 0; i < items.size(); i++)
    ts << items.at(i) << endl;
  ts << endl;
  return ts.readAll();
}

QString FormatsDisplayer::_getFormatsSupportingBoundsString(const bool ogrOnly)
{
  const QStringList formats =
    _getFormats<OsmMapReader>(OsmMapReader::className(), QStringList(), ogrOnly, true);
  LOG_VART(formats);
  QStringList boundableFormats;
  for (int i = 0; i < formats.size(); i++)
  {
    QString format = formats[i];
    QString formatTemp = format;
    if (formatTemp.startsWith(MetadataTags::HootApiDbScheme() + "://"))
    {
      formatTemp += "myhost:5432/mydb/mylayer";
    }
    else if (formatTemp.startsWith(MetadataTags::OsmApiDbScheme() + "://"))
    {
      formatTemp += "myhost:5432/osmapi_test";
    }
    const QString supportedReaderName = OsmMapReaderFactory::getReaderName(formatTemp);
    LOG_VART(supportedReaderName);
    if (!supportedReaderName.trimmed().isEmpty())
    {
      std::shared_ptr<OsmMapReader> reader(
        Factory::getInstance().constructObject<OsmMapReader>(supportedReaderName));
      LOG_VART(reader.get());
      std::shared_ptr<Boundable> boundable = std::dynamic_pointer_cast<Boundable>(reader);
      LOG_VART(boundable.get());
      if (boundable)
      {
        boundableFormats.append(format);
      }
    }
  }
  return _getPrintableString(boundableFormats);
}

// TODO: consolidate these two streaming supported methods

QString FormatsDisplayer::_getInputFormatsSupportingStreamingString(const bool ogrOnly)
{
  const QStringList formats =
    _getFormats<OsmMapReader>(OsmMapReader::className(), QStringList(), ogrOnly, true);
  LOG_VART(formats);
  QStringList streamableFormats;
  for (int i = 0; i < formats.size(); i++)
  {
    QString format = formats[i];
    QString formatTemp = format;
    if (formatTemp.startsWith(MetadataTags::HootApiDbScheme() + "://"))
    {
      formatTemp += "myhost:5432/mydb/mylayer";
    }
    else if (formatTemp.startsWith(MetadataTags::OsmApiDbScheme() + "://"))
    {
      formatTemp += "myhost:5432/osmapi_test";
    }
    const QString supportedReaderName = OsmMapReaderFactory::getReaderName(formatTemp);
    LOG_VART(supportedReaderName);
    if (!supportedReaderName.trimmed().isEmpty())
    {
      std::shared_ptr<OsmMapReader> reader(
        Factory::getInstance().constructObject<OsmMapReader>(supportedReaderName));
      LOG_VART(reader.get());
      std::shared_ptr<PartialOsmMapReader> partial =
        std::dynamic_pointer_cast<PartialOsmMapReader>(reader);
      LOG_VART(partial.get());
      if (partial)
      {
        streamableFormats.append(format);
      }
    }
  }
  return _getPrintableString(streamableFormats);
}

QString FormatsDisplayer::_getOutputFormatsSupportingStreamingString(const bool ogrOnly)
{
  const QStringList formats =
    _getFormats<OsmMapWriter>(OsmMapWriter::className(), QStringList(), ogrOnly, false);
  LOG_VART(formats);
  QStringList streamableFormats;
  for (int i = 0; i < formats.size(); i++)
  {
    QString format = formats[i];
    QString formatTemp = format;
    if (formatTemp.startsWith(MetadataTags::HootApiDbScheme() + "://"))
    {
      formatTemp += "myhost:5432/mydb/mylayer";
    }
    else if (formatTemp.startsWith(MetadataTags::OsmApiDbScheme() + "://"))
    {
      formatTemp += "myhost:5432/osmapi_test";
    }
    const QString supportedWriterName = OsmMapWriterFactory::getWriterName(formatTemp);
    LOG_VART(supportedWriterName);
    if (!supportedWriterName.trimmed().isEmpty())
    {
      std::shared_ptr<OsmMapWriter> writer(
        Factory::getInstance().constructObject<OsmMapWriter>(supportedWriterName));
      LOG_VART(writer.get());
      std::shared_ptr<PartialOsmMapWriter> partial =
        std::dynamic_pointer_cast<PartialOsmMapWriter>(writer);
      LOG_VART(partial.get());
      if (partial)
      {
        streamableFormats.append(format);
      }
    }
  }
  return _getPrintableString(streamableFormats);
}

}
