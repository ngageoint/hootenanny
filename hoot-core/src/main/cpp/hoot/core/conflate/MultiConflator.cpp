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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "MultiConflator.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/SetTagVisitor.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/visitors/KeepReviewsVisitor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/conflate/UnifyingConflator.h>

namespace hoot
{

MultiConflator::MultiConflator()
{
}

void MultiConflator::conflate(const QStringList args)
{
  //TODO: make this work with stats
  if (args.contains("--stats"))
  {
    throw HootException("Multi-conflation does not work with the --stats option.");
  }

  if (ConfigOptions().getReviewTagsTreatAsMetadata())
  {
    throw HootException(
      "Multi-conflation must be run with " + ConfigOptions::getReviewTagsTreatAsMetadataKey() +
      "=false");
  }

  const QStringList inputs = args[0].split(";");
  const QString output = args[1];

  OsmMapPtr map(new OsmMap());
  OsmMapPtr reviewCache;
  LOG_VARD(inputs.size());
  for (int i = 0; i < inputs.size(); i++)
  {
    if (i == 0)
    {
      OsmMapReaderFactory::read(
        map, inputs[i], ConfigOptions().getConflateUseDataSourceIds(), Status::Unknown1);

      LOG_DEBUG("Setting source tags...");
      SetTagVisitor sourceTagVisitor(MetadataTags::HootSource(), QString::number(i + 1));
      map->visitRw(sourceTagVisitor);
    }
    else
    {
      if (i == 1)
      {
        LOG_INFO("Conflating " << inputs[i - 1] << " with " << inputs[i] << "...");
      }
      else
      {
        LOG_INFO("Conflating cumulative map with " << inputs[i] << "...");
      }

      OsmMapReaderFactory::read(
        map, inputs[i], ConfigOptions().getConflateUseDataSourceIds(), Status::Unknown2);

      if (i == 1 && Log::getInstance().isDebugEnabled())
      {
        LOG_DEBUG("Writing debug map.");
        OsmMapPtr debug(new OsmMap(map));
        MapProjector::projectToWgs84(debug);
        OsmMapWriterFactory::write(debug, ConfigOptions().getDebugMapFilename());
      }

      //keep a source tag history on the data for provenance; append to any existing source values
      //(this shouldn't be added to any review relations)
      LOG_DEBUG("Setting source tags...");
      SetTagVisitor sourceTagVisitor(MetadataTags::HootSource(), QString::number(i + 1), true);
      map->visitRw(sourceTagVisitor);

      //load in cached reviews from previous conflations
      if (reviewCache.get() && reviewCache->getElementCount() > 0)
      {
        LOG_DEBUG("Adding previous reviews...");
        const RelationMap& reviews = reviewCache->getRelationMap();
        for (RelationMap::const_iterator it = reviews.begin(); it != reviews.end(); ++it)
        {
          RelationPtr review = it->second;
          review->setId(map->createNextRelationId());
          map->addRelation(review);
        }
        LOG_DEBUG("Added " << reviews.size() << " cached reviews.");
      }

      NamedOp(ConfigOptions().getConflatePreOps()).apply(map);

      UnifyingConflator().apply(map);

      //NamedOp(ConfigOptions().getConflatePostOps()).apply(map);

      if (i < inputs.size() - 1)
      {
        //Up until just before the last conflate job, set the status tag back to 1 so that the
        //accumulated data will conflate with the next dataset.
        //TODO: this shouldn't be modifying review relations
        SetTagVisitor statusTagVisitor(
          "hoot:status", QString("%1").arg(Status(Status::Unknown1).getEnum()));
        map->visitRw(statusTagVisitor);

//          if (Log::getInstance().isDebugEnabled())
//          {
//            LOG_DEBUG("Writing debug map.");
//            OsmMapPtr debug(new OsmMap(map));
//            MapProjector::projectToWgs84(debug);
//            OsmMapWriterFactory::write(debug, ConfigOptions().getDebugMapFilename());
//          }
      }

      //copy the map and save the reviews
      LOG_DEBUG("Caching reviews...");
      reviewCache.reset(new OsmMap(map->getProjection()));
      KeepReviewsVisitor vis;
      reviewCache->visitRw(vis);
      LOG_DEBUG("Cached " << reviewCache->getElementCount() << " reviews.");
    }
  }

  NamedOp(ConfigOptions().getConflatePostOps()).apply(map);

  MapProjector::projectToWgs84(map);
  OsmMapWriterFactory::write(map, output);
}

}
