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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/ExternalMergeElementSorter.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/elements/InMemoryElementSorter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class SortCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::SortCmd"; }

  SortCmd() = default;

  QString getName() const override { return "sort"; }
  QString getDescription() const override { return "Sorts a map to the OSM standard"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    if (args.size() != 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    const QString input = args[0];
    const QString output = args[1];

    LOG_STATUS(
      "Sorting maps ..." << FileUtils::toLogFormat(input, 25) << " and writing output to ..." <<
      FileUtils::toLogFormat(output, 25) << "...");

    if (_inputIsSorted(input))
    {
      LOG_WARN("Input is already sorted.");
      return 0;
    }

    if (OsmMapReaderFactory::hasElementInputStream(input) &&
        ConfigOptions().getElementSorterElementBufferSize() != -1)
    {
      _sortExternally(input, output);
    }
    else
    {
      _sortInMemory(input, output);
    }

    LOG_STATUS("Map sorted in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }

private:

  bool _inputIsSorted(const QString& input) const
  {
    return OsmPbfReader().isSupported(input) && OsmPbfReader().isSorted(input);
  }

  void _sortInMemory(const QString& input, const QString& output) const
  {
    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, input, true, Status::Unknown1);
    InMemoryElementSorterPtr(new InMemoryElementSorter(map));
    IoUtils::saveMap(map, output);
  }

  void _sortExternally(const QString& input, const QString& output) const
  {
    std::shared_ptr<PartialOsmMapReader> reader =
      std::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::createReader(input));
    reader->setUseDataSourceIds(true);
    reader->open(input);
    reader->initializePartial();

    std::shared_ptr<ExternalMergeElementSorter> sorted(new ExternalMergeElementSorter());
    sorted->sort(std::dynamic_pointer_cast<ElementInputStream>(reader));

    reader->finalizePartial();
    reader->close();

    std::shared_ptr<PartialOsmMapWriter> writer =
      std::dynamic_pointer_cast<PartialOsmMapWriter>(
        OsmMapWriterFactory::createWriter(output));
    writer->open(output);
    writer->initializePartial();
    while (sorted->hasMoreElements())
    {
      writer->writePartial(sorted->readNextElement());
    }
    writer->finalizePartial();
    writer->close();
  }
};

HOOT_FACTORY_REGISTER(Command, SortCmd)

}

