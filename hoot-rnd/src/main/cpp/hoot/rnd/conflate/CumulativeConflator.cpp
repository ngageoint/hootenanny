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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "CumulativeConflator.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/SetTagVisitor.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/rnd/visitors/KeepReviewsVisitor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/conflate/UnifyingConflator.h>

namespace hoot
{

void CumulativeConflator::conflate(const QStringList inputs, const QString output)
{
  assert(inputs.size() >= 3);

  //for NoInformationElementRemover
  if (ConfigOptions().getReviewTagsTreatAsMetadata())
  {
    throw HootException(
      "Multi-conflation must be run with " + ConfigOptions::getReviewTagsTreatAsMetadataKey() +
      "=false");
  }

  //for TagMergerFactory
  if (ConfigOptions().getTagMergerDefault() != "hoot::ProvenanceAwareOverwriteTagMerger")
  {
    throw HootException(
      "Multi-conflation must be run with " + ConfigOptions::getTagMergerDefaultKey() +
      "=hoot::ProvenanceAwareOverwriteTagMerger");
  }

  OsmMapPtr cumulativeMap(new OsmMap());
  LOG_VARD(inputs.size());
  for (int i = 0; i < inputs.size(); i++)
  {
    OsmMapPtr reviewCache;
    if (i == 0)
    {
      OsmMapReaderFactory::read(
        cumulativeMap, inputs[i], ConfigOptions().getConflateUseDataSourceIds(), Status::Unknown1);

      //keep a source tag history on the data for provenance; append to any existing source values
      //(this shouldn't be added to any review relations)
      LOG_DEBUG("Setting source tags for map " << QString::number(i + 1) << "...");
      SetTagVisitor sourceTagVisitor(MetadataTags::HootSource(), QString::number(i + 1));
      cumulativeMap->visitRw(sourceTagVisitor);
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

      //I'm not yet sure all the projecting going on here is the right way to go about this, but
      //the maps must be in the same projection for the appending to work.

      OsmMapPtr unknown2Map(new OsmMap());
      OsmMapReaderFactory::read(
        unknown2Map, inputs[i], ConfigOptions().getConflateUseDataSourceIds(), Status::Unknown2);
      MapProjector::projectToWgs84(unknown2Map);

      //Same as above, but do this before combining the cumulative map with the unknown2 map to
      //prevent incorrect tags from being added to the cumulative map.
      LOG_DEBUG("Setting source tags for map " << QString::number(i + 1) << "...");
      SetTagVisitor sourceTagVisitor(MetadataTags::HootSource(), QString::number(i + 1)/*, true*/);
      unknown2Map->visitRw(sourceTagVisitor);

      //now combine the two maps before conflation
      MapProjector::projectToWgs84(cumulativeMap);
      MapProjector::projectToWgs84(unknown2Map);
      cumulativeMap->append(unknown2Map);

      //load in cached reviews from previous conflations - I believe this is necessary, b/c the
      //UnifyingConflator is ignoring any incoming reviews by design (need to verify this).  It
      //could be argued that modifying it to optionally retain the reviews is a better design
      //than the caching going on here...
      if (reviewCache.get() && reviewCache->getElementCount() > 0)
      {
        LOG_DEBUG("Adding previous reviews...");
        const RelationMap& reviews = reviewCache->getRelations();
        for (RelationMap::const_iterator it = reviews.begin(); it != reviews.end(); ++it)
        {
          RelationPtr review = it->second;
          review->setId(cumulativeMap->createNextRelationId());
          cumulativeMap->addRelation(review);
        }
        LOG_DEBUG("Added " << reviews.size() << " cached reviews.");
      }

      NamedOp(ConfigOptions().getConflatePreOps()).apply(cumulativeMap);
      UnifyingConflator().apply(cumulativeMap);
      //going to apply this at the end of all conflation jobs, but maybe we'll find out later that
      //it needs to be done here instead (?)
      //NamedOp(ConfigOptions().getConflatePostOps()).apply(cumulativeMap);

      if (i < inputs.size() - 1)
      {
        //Up until just before the last conflate job, set the status tag back to 1 so that the
        //accumulated data will conflate with the next dataset.
        //TODO: there is a bug here that will affect river conflation in that somehow hoot:status=3
        //tags are being left in at some point which causes the SearchRadiusCalculator to skip the
        //features.
        LOG_DEBUG("Setting status tags for map " << QString::number(i + 1) << "...");
        SetTagVisitor statusTagVisitor(
          MetadataTags::HootStatus(), QString("%1").arg(Status(Status::Unknown1).getEnum()));
        cumulativeMap->visitRw(statusTagVisitor);
      }

      //copy the map and save the reviews
      LOG_DEBUG("Caching reviews...");
      reviewCache.reset(new OsmMap(cumulativeMap->getProjection()));
      KeepReviewsVisitor vis;
      reviewCache->visitRw(vis);
      LOG_DEBUG("Cached " << reviewCache->getElementCount() << " reviews.");
    }
  }

  NamedOp(ConfigOptions().getConflatePostOps()).apply(cumulativeMap);

  MapProjector::projectToWgs84(cumulativeMap);
  OsmMapWriterFactory::write(cumulativeMap, output);
}

}
