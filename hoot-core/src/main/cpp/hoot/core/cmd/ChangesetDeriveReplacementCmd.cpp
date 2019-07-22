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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ChangesetWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>

namespace hoot
{

/**
 * TODO
 */
class ChangesetDeriveReplacementCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::ChangesetDeriveReplacementCmd"; }

  ChangesetDeriveReplacementCmd() {}

  virtual QString getName() const { return "changeset-derive-replacement"; }

  virtual QString getDescription() const { return "TODO"; }

  virtual int runSimple(QStringList args)
  {
    // process optional params

    bool lenientBounds = false;
    if (args.contains("--lenient-bounds"))
    {
      lenientBounds = true;
      args.removeAll("--lenient-bounds");
    }
    bool printStats = false;
    if (args.contains("--stats"))
    {
      printStats = true;
      args.removeAll("--stats");
    }
    bool writeBoundsFile = false;
    if (args.contains("--write-bounds"))
    {
      writeBoundsFile = true;
      args.removeAll("--write-bounds");
    }

    // process non-optional params

    if (args.size() < 5 || args.size() > 6)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes five or six parameters.").arg(getName()));
    }

    const QString input1 = args[0];
    const QString input2 = args[1];
    const geos::geom::Envelope bounds = GeometryUtils::envelopeFromConfigString(args[2]);
    std::shared_ptr<ConflatableElementCriterion> featureCrit =
      std::dynamic_pointer_cast<ConflatableElementCriterion>(
        std::shared_ptr<ElementCriterion>(
          Factory::getInstance().constructObject<ElementCriterion>(args[3].trimmed())));
    if (!featureCrit)
    {
      throw IllegalArgumentException("TODO");
    }
    const QString output = args[4];
    // TODO: Is there any way to get rid of this param?
    QString osmApiDbUrl;
    if (output.endsWith(".osc.sql"))
    {
      if (args.size() != 6)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw HootException(QString("%1 with SQL output takes five parameters.").arg(getName()));
      }
      osmApiDbUrl = args[5];
    }

    /*
     * general opts
     *
     * changeset.xml.writer.add.timestamp=false
     * reader.add.source.datetime=false
     * writer.include.circular.error.tags=false
     *
     * db opts
     *
     * changeset.user.id=1
     *
     * conflate opts
     *
     * conflate.use.data.source.ids.1=true
     * conflate.use.data.source.ids.2=false (make sure this can work for the first three tests)
     *
     * snap opts (road strict only)
     *
     * way.joiner=hoot::ReplacementSnappedWayJoiner
     * convert.ops=hoot::UnconnectedHighwaySnapper;hoot::WayJoinerOp
     * snap.unconnected.ways.snap.to.way.status=Input1
     * snap.unconnected.ways.snap.way.status=Input2;Conflated
     * snap.unconnected.ways.existing.way.node.tolerance=45.0
     * snap.unconnected.ways.snap.tolerance=45.0
     *
     * BUILDING
     *
     * crop and prune
     *
     * convert.bounding.box.keep.entire.features.crossing.bounds=true (both)
     *
     * cookie cut opts
     *
     * crop.keep.entire.features.crossing.bounds=true
     *
     * changeset opts
     *
     * changeset.reference.keep.entire.features.crossing.bounds=true
     * changeset.secondary.keep.entire.features.crossing.bounds=true
     * changeset.reference.keep.only.features.inside.bounds=false
     * changeset.secondary.keep.only.features.inside.bounds=false
     *
     * BUILDING STRICT
     *
     * crop and prune
     *
     * convert.bounding.box.keep.entire.features.crossing.bounds=true (ref)
     * convert.bounding.box.keep.entire.features.crossing.bounds=false (sec)
     *
     * cookie cut opts
     *
     * crop.keep.entire.features.crossing.bounds=true
     *
     * changeset opts
     *
     * changeset.reference.keep.entire.features.crossing.bounds=true
     * changeset.secondary.keep.entire.features.crossing.bounds=false
     * changeset.reference.keep.only.features.inside.bounds=false
     * changeset.secondary.keep.only.features.inside.bounds=true
     * changeset.allow.deleting.reference.features.outside.bounds=false
     * in.bounds.criterion.strict=true
     *
     * ROAD
     *
     * crop and prune
     *
     * convert.bounding.box.keep.entire.features.crossing.bounds=true (both)
     *
     * cookie cut opts
     *
     * crop.keep.entire.features.crossing.bounds=false
     *
     * changeset opts
     *
     * changeset.reference.keep.entire.features.crossing.bounds=true
     * changeset.secondary.keep.entire.features.crossing.bounds=true
     * changeset.reference.keep.only.features.inside.bounds=false
     * changeset.secondary.keep.only.features.inside.bounds=false
     *
     * ROAD STRICT
     *
     * crop and prune
     *
     * convert.bounding.box.keep.entire.features.crossing.bounds=true (ref)
     * convert.bounding.box.keep.entire.features.crossing.bounds=false (sec)
     *
     * cookie cut opts
     *
     * crop.keep.entire.features.crossing.bounds=false
     *
     * changeset opts
     *
     * changeset.reference.keep.entire.features.crossing.bounds=true
     * changeset.secondary.keep.entire.features.crossing.bounds=true
     * changeset.reference.keep.only.features.inside.bounds=false
     * changeset.secondary.keep.only.features.inside.bounds=false
     * changeset.allow.deleting.reference.features.outside.bounds=false
     * in.bounds.criterion.strict=false
     */

    // crop



    // prune



    // cookie cut



    // conflate



    // snap (if necessary)



    // derive changeset

    LOG_VART(lenientBounds);
    ChangesetWriter(printStats, osmApiDbUrl).write(output, input1, input2);



    if (writeBoundsFile)
    {
      OsmMapWriterFactory::write(
        GeometryUtils::createMapFromBounds(bounds), ConfigOptions().getBoundsOutputFile());
    }

    return 0;
  }

private:

  // hardcode these for now

  bool isPointCrit(const QString& critClassName) const
  {
    return critClassName.contains("PoiCriterion");
  }

  bool isLinearCrit(const QString& critClassName) const
  {
    return
      critClassName.contains("HighwayCriterion") ||
      critClassName.contains("LinearWaterwayCriterion") ||
      critClassName.contains("PowerLineCriterion") ||
      critClassName.contains("RailwayCriterion");
  }

  bool isPolyCrit(const QString& critClassName) const
  {
    return
      critClassName.contains("AreaCriterion") ||
      critClassName.contains("BuildingCriterion");
  }
};

HOOT_FACTORY_REGISTER(Command, ChangesetDeriveReplacementCmd)

}


