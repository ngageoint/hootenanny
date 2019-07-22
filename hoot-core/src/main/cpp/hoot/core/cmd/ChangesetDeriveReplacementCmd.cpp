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
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/algorithms/alpha-shape/AlphaShapeGenerator.h>
#include <hoot/core/conflate/CookieCutter.h>
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/ops/UnconnectedWaySnapper.h>

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

    const QString input1 = args[0].trimmed();
    const QString input2 = args[1].trimmed();
    const QString boundsStr = args[2].trimmed();
    const geos::geom::Envelope bounds = GeometryUtils::envelopeFromConfigString(boundsStr);
    const QString critClassName = args[3].trimmed();
    std::shared_ptr<ConflatableElementCriterion> featureCrit =
      std::dynamic_pointer_cast<ConflatableElementCriterion>(
        std::shared_ptr<ElementCriterion>(
          Factory::getInstance().constructObject<ElementCriterion>(critClassName)));
    if (!featureCrit)
    {
      throw IllegalArgumentException("TODO");
    }
    const QString output = args[4].trimmed();
    // TODO: Is there any way to get rid of this param?
    QString osmApiDbUrl;
    if (output.endsWith(".osc.sql"))
    {
      if (args.size() != 6)
      {
        std::cout << getHelp() << std::endl << std::endl;
        throw IllegalArgumentException(
          QString("%1 with SQL output takes six parameters.").arg(getName()));
      }
      osmApiDbUrl = args[5].trimmed();
    }

    _setConfigOpts(lenientBounds, critClassName);

    // load each dataset separately and crop to the aoi

    conf().set(
      ConfigOptions::getConvertBoundingBoxKey(), boundsStr);

    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
      _convertRefKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
      _convertRefKeepOnlyInsideBounds);
    OsmMapPtr refMap(new OsmMap());
    IoUtils::loadMap(refMap, input1, true, Status::Unknown1);

    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
      _convertSecKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
      _convertSecKeepOnlyInsideBounds);
    OsmMapPtr secMap(new OsmMap());
    IoUtils::loadMap(secMap, input2, true, Status::Unknown2);

    // prune down to just the feature types specified by the filter

    RemoveElementsVisitor elementsRemover(true);
    elementsRemover.addCriterion(featureCrit);
    elementsRemover.setRecursive(true);
    refMap->visitRw(elementsRemover);
    secMap->visitRw(elementsRemover);

    // generate a cutter shape based on the cropped secondary map

    OsmMapPtr cutterShapeMap = AlphaShapeGenerator(1000.0, 0.0).generateMap(secMap);

    // cookie cut the shape of the cutter shape map out of the cropped ref map

    conf().set(
      ConfigOptions::getCropKeepEntireFeaturesCrossingBoundsKey(), _cropKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getCropKeepOnlyFeaturesInsideBoundsKey(), _cropKeepOnlyInsideBounds);
    CookieCutter(false, 0.0).cut(cutterShapeMap, refMap);
    OsmMapPtr cookieCutMap = refMap;

    // conflate the cookie cut ref map with the cropped sec map

    // TODO: this won't work
    //conf().set(ConfigOptions::getConflateUseDataSourceIds1Key(), "true");
    //conf().set(ConfigOptions::getConflateUseDataSourceIds2Key(), "false");
    //IoUtils::loadMap(map, input1, ConfigOptions().getConflateUseDataSourceIds1(), Status::Unknown1);
    // TODO: need to add node hashes
    cookieCutMap->append(secMap);
    OsmMapPtr conflateMap = cookieCutMap;
    // TODO: restrict conflate matchers to only those relevant based on the filter?
    UnifyingConflator().apply(conflateMap);

    // snap secondary features back to reference features if dealing with linear features and a
    // non-lenient bounds where ref features may have been cut along it

    if (!lenientBounds && _isLinearCrit(critClassName))
    {
      UnconnectedWaySnapper snapper;
      snapper.setConfiguration(conf());
      snapper.setSnapToWayStatus("Input1");
      snapper.setSnapWayStatus("Input2;Conflated");
      snapper.setWayNodeToSnapToCriterionClassName(critClassName);
      snapper.setWayToSnapCriterionClassName(critClassName);
      snapper.setWayToSnapToCriterionClassName(critClassName);
      snapper.apply(conflateMap);
    }

    // derive a changeset that replaces ref features with secondary features

    conf().set(
      ConfigOptions::getChangesetReferenceKeepEntireFeaturesCrossingBoundsKey(),
      _changesetRefKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getChangesetReferenceKeepOnlyFeaturesInsideBoundsKey(),
      _changesetRefKeepOnlyInsideBounds);
    conf().set(
      ConfigOptions::getChangesetSecondaryKeepEntireFeaturesCrossingBoundsKey(),
      _changesetSecKeepEntireCrossingBounds);
    conf().set(
      ConfigOptions::getChangesetSecondaryKeepOnlyFeaturesInsideBoundsKey(),
      _changesetSecKeepOnlyInsideBounds);
    conf().set(
      ConfigOptions::getChangesetAllowDeletingReferenceFeaturesOutsideBoundsKey(),
      _changesetAllowDeletingRefOutsideBounds);
    conf().set(
      ConfigOptions::getInBoundsCriterionStrictKey(),
      _inBoundsStrict);
    LOG_VART(printStats);
    // TODO: this won't work
    //ChangesetWriter(printStats, osmApiDbUrl).write(output, input1, input2);

    if (writeBoundsFile)
    {
      OsmMapWriterFactory::write(
        GeometryUtils::createMapFromBounds(bounds), ConfigOptions().getBoundsOutputFile());
    }

    return 0;
  }

private:

  bool _convertRefKeepEntireCrossingBounds;
  bool _convertRefKeepOnlyInsideBounds;
  bool _convertSecKeepEntireCrossingBounds;
  bool _convertSecKeepOnlyInsideBounds;
  bool _cropKeepEntireCrossingBounds;
  bool _cropKeepOnlyInsideBounds;
  bool _changesetRefKeepEntireCrossingBounds;
  bool _changesetSecKeepEntireCrossingBounds;
  bool _changesetRefKeepOnlyInsideBounds;
  bool _changesetSecKeepOnlyInsideBounds;
  bool _changesetAllowDeletingRefOutsideBounds;
  bool _inBoundsStrict;

  // hardcode these for now; only one of the following should return true for any input

  bool _isPointCrit(const QString& critClassName) const
  {
    return critClassName.contains("PoiCriterion");
  }

  bool _isLinearCrit(const QString& critClassName) const
  {
    return
      critClassName.contains("HighwayCriterion") ||
      critClassName.contains("LinearWaterwayCriterion") ||
      critClassName.contains("PowerLineCriterion") ||
      critClassName.contains("RailwayCriterion");
  }

  bool _isPolyCrit(const QString& critClassName) const
  {
    return
      critClassName.contains("AreaCriterion") ||
      critClassName.contains("BuildingCriterion");
  }

  void _setConfigOpts(const bool lenientBounds, const QString& critClassName)
  {
    // changeset and db opts should have been set when calling this command

    // global opts

    conf().set(ConfigOptions::getChangesetXmlWriterAddTimestampKey(), "false");
    conf().set(ConfigOptions::getReaderAddSourceDatetimeKey(), "false");
    conf().set(ConfigOptions::getWriterIncludeCircularErrorTagsKey(), "false");

    // dataset specific opts

    // only one of these should ever be true
    const bool replacingPoints = _isPointCrit(critClassName);
    const bool replacingLines = _isLinearCrit(critClassName);
    const bool replacingPolys = _isPolyCrit(critClassName);

    // TODO: Hopefully, these three sets of bounds options can be collapsed (??).

    _convertRefKeepOnlyInsideBounds = false;
    _cropKeepOnlyInsideBounds = false;
    _changesetRefKeepOnlyInsideBounds = false;

    if (replacingPoints)
    {
      if (lenientBounds)
      {
        LOG_WARN("--lenient-bounds option ignored with point datasets.");
      }

      _convertRefKeepEntireCrossingBounds = false;
      _convertSecKeepEntireCrossingBounds = false;
      _convertSecKeepOnlyInsideBounds = false;
      _cropKeepEntireCrossingBounds = false;
      _changesetRefKeepEntireCrossingBounds = false;
      _changesetSecKeepEntireCrossingBounds = false;
      _changesetSecKeepOnlyInsideBounds = true;
      _changesetAllowDeletingRefOutsideBounds = true;
      _inBoundsStrict = false;
    }
    else if (replacingLines)
    {
      if (lenientBounds)
      {
        _convertRefKeepEntireCrossingBounds = true;
        _convertSecKeepEntireCrossingBounds = true;
        _convertSecKeepOnlyInsideBounds = false;
        _cropKeepEntireCrossingBounds = false;
        _changesetRefKeepEntireCrossingBounds = true;
        _changesetSecKeepEntireCrossingBounds = true;
        _changesetSecKeepOnlyInsideBounds = false;
        _changesetAllowDeletingRefOutsideBounds = true;
        _inBoundsStrict = false;
      }
      else
      {
        _convertRefKeepEntireCrossingBounds = true;
        _convertSecKeepEntireCrossingBounds = false;
        _convertSecKeepOnlyInsideBounds = false;
        _cropKeepEntireCrossingBounds = false;
        _changesetRefKeepEntireCrossingBounds = true;
        _changesetSecKeepEntireCrossingBounds = true;
        _changesetSecKeepOnlyInsideBounds = false;
        _changesetAllowDeletingRefOutsideBounds = false;
        _inBoundsStrict = false;
      }
    }
    else if (replacingPolys)
    {
      if (lenientBounds)
      {
        _convertRefKeepEntireCrossingBounds = true;
        _convertSecKeepEntireCrossingBounds = true;
        _convertSecKeepOnlyInsideBounds = false;
        _cropKeepEntireCrossingBounds = true;
        _changesetRefKeepEntireCrossingBounds = true;
        _changesetSecKeepEntireCrossingBounds = true;
        _changesetSecKeepOnlyInsideBounds = false;
        _changesetAllowDeletingRefOutsideBounds = true;
        _inBoundsStrict = false;
      }
      else
      {
        _convertRefKeepEntireCrossingBounds = true;
        _convertSecKeepEntireCrossingBounds = false;
        _convertSecKeepOnlyInsideBounds = true;
        _cropKeepEntireCrossingBounds = true;
        _changesetRefKeepEntireCrossingBounds = true;
        _changesetSecKeepEntireCrossingBounds = false;
        _changesetSecKeepOnlyInsideBounds = true;
        _changesetAllowDeletingRefOutsideBounds = false;
        _inBoundsStrict = true;
      }
    }
    else
    {
      // shouldn't ever get here
      throw IllegalArgumentException("TODO");
    }
  }
};

HOOT_FACTORY_REGISTER(Command, ChangesetDeriveReplacementCmd)

}


