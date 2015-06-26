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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.writers.review;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.reflect.MethodUtils;
import org.postgresql.util.PGobject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.collect.ArrayListMultimap;
import com.mysema.query.sql.SQLQuery;

import hoot.services.db.DbUtils;
import hoot.services.db.postgres.PostgresUtils;
import hoot.services.db2.ElementIdMappings;
import hoot.services.db2.ReviewItems;
import hoot.services.models.osm.Element.ElementType;

/**
 * Writes review data to the services database
 * 
 * This is an attempt to resolve the handling of fuzzy matches (see #6269), as well as make paired 
 * review items come from the same data sources consistently (see #6320).
 * 
 * @todo If this code is permanently usable, then the unit tests need to be rewritten for it.  See
 * #6270.
 */
public class ReviewPrepareDbWriter2 extends ReviewPrepareDbWriter
{
  private static final Logger log = LoggerFactory.getLogger(ReviewPrepareDbWriter2.class);

  public ReviewPrepareDbWriter2() throws Exception
  {
    super();
  }

  /*
   * logging records with invalid uuid's and skipping; if errors should be thrown, then the
   * unit tests will have to reworked
   */
  @Override
  protected boolean parseElementUniqueIdTags(final long mapId) throws Exception
  {
    final String logMsgStart =
      "Parsing element unique ID tags for map with ID: " + mapId + ".  Step 2 of 4.";
    log.info(logMsgStart);

    uniqueIdsParsed = 0;
    idMappingRecordWritten = false;
    List<ElementIdMappings> elementIdMappingRecordsToInsert = new ArrayList<ElementIdMappings>();
    //create this outside of the batch read loop, since we need to maintain a list of unique
    //ID's parsed over the entire map's set of reviewable records
    Set<String> elementIds = new HashSet<String>();

    for (ElementType elementType : ElementType.values())
    {
      if (!elementType.equals(ElementType.Changeset))
      {
        int numElementsReturned = Integer.MAX_VALUE;
        int elementIndex = 0;
        while (numElementsReturned > 0)
        {
          //get all reviewable elements
          final Map<Long, Object> reviewableElementRecords =
            getParseableElementRecords(mapId, elementType, maxRecordSelectSize, elementIndex);
          numElementsReturned = reviewableElementRecords.size();
          elementIndex += numElementsReturned;
          for (Map.Entry<Long, Object> reviewableElementRecordEntry :
               reviewableElementRecords.entrySet())
          {
            final long osmElementId = reviewableElementRecordEntry.getKey();
            final Object reviewableElementRecord = reviewableElementRecordEntry.getValue();
            final Map<String, String> tags =
              PostgresUtils.postgresObjToHStore(
              		(PGobject)MethodUtils.invokeMethod(reviewableElementRecord, "getTags", new Object[]{}));
            final String uniqueElementIdStr = StringUtils.trimToNull(tags.get("uuid"));
            if (uniqueElementIdStr == null)
            {
              log.warn(
                "Invalid UUID: " + uniqueElementIdStr + " for map with ID: " + mapId +
                ".  Skipping adding unique ID record...");
            }
            else
            {
              //In the case of the fuzzy match conflict example, this ID may be made up of multiple
              //parts.  Treat each ID part separately.  
            	// TODO: add test for this case
              String[] uniqueElementIds = null;
              if (uniqueElementIdStr.contains(";"))
              {
                log.debug("Multiple part UUID...");
                uniqueElementIds = uniqueElementIdStr.split(";");
              }
              else
              {
                uniqueElementIds = new String[1];
                uniqueElementIds[0] = uniqueElementIdStr;
              }
                
              for (String uniqueElementId : uniqueElementIds)
              {
              	//TODO: make this a batch query somehow
                if(
                  new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(elementIdMappings)
                	  .where(elementIdMappings.mapId.eq(mapId)
                  		.and(elementIdMappings.elementId.eq(uniqueElementId))).count() > 0)
                {
                  log.warn(
                    "UUID: " + uniqueElementId + " for map with ID: " + mapId + " already exists.  " +
                    "Skipping adding unique ID record...");
                }
                else
                {
                  if (elementIds.add(uniqueElementId))  //don't add duplicates
                  {
                    log.debug("Adding UUID: " + uniqueElementId);
                    elementIdMappingRecordsToInsert.add(
                      createElementIdMappingRecord(uniqueElementId, osmElementId, elementType, mapId));
                    flushIdMappingRecords(
                      elementIdMappingRecordsToInsert, maxRecordBatchSize, logMsgStart);
                  }
                  else
                  {
                    log.warn(
                      "Duplicate element ID: " + uniqueElementId.toString() + " for map with ID: " +
                      mapId + ".  Skipping adding unique ID record...");
                  }
                }
              }
            }
          }
        }
      }
    }

    //final flush
    flushIdMappingRecords(elementIdMappingRecordsToInsert, 0, logMsgStart);

    log.debug("Wrote " + elementIds.size() + " ID mappings.");

    return idMappingRecordWritten;
  }

  /*
   * logging records with invalid tag values and skipping; if errors should be thrown, then the
   * unit tests will have to reworked
   *
   * @todo What's a good way to write job completion percentage to the db from here?  This runs in
   * a transaction, so it won't show up for external job pollers until all the parsing is done.
   */
  @Override
  protected boolean parseElementReviewTags(final long mapId) throws Exception
  {
    final String logMsgStart =
      "Parsing element review tags for map with ID: " + mapId + ".  Step 3 of 4.";
    log.info(logMsgStart);
    //SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());
    //parse review tags for all nodes, ways, and relations from the OSM element tables for the given
    //map
    reviewRecordsParsed = 0;
    reviewRecordWritten = false;
    List<ReviewItems> reviewRecordsToInsert = new ArrayList<ReviewItems>();
    //create this outside of the batch read loop, since we need to maintain a list of unique
    //ID's parsed over the entire map's set of reviewable records
    reviewableItemIdToReviewAgainstItemIds = ArrayListMultimap.create();
    reviewableItemIdToReviewAgainstItemIds.putAll(previouslyReviewedItemIdToReviewAgainstItemIds);

    int numReviewItemsAdded = 0;
    for (ElementType elementType : ElementType.values())
    {
      if (!elementType.equals(ElementType.Changeset))
      {
        int numElementsReturned = Integer.MAX_VALUE;
        int elementIndex = 0;
        while (numElementsReturned > 0)
        {
          //get a batch of reviewable elements
          final Map<Long, Object> reviewableElementRecords =
            getReviewableElementRecords(mapId, elementType, maxRecordSelectSize, elementIndex);
          numElementsReturned = reviewableElementRecords.size();
          elementIndex += numElementsReturned;
          for (Map.Entry<Long, Object> reviewableElementRecordEntry : reviewableElementRecords.entrySet())
          {
            final Object reviewableElementRecord = reviewableElementRecordEntry.getValue();
            final Map<String, String> tags =
              PostgresUtils.postgresObjToHStore(
              	(PGobject)MethodUtils.invokeMethod(reviewableElementRecord, "getTags", new Object[]{}));
            final String reviewableItemIdStr = StringUtils.trimToNull(tags.get("uuid"));
            if (StringUtils.isEmpty(reviewableItemIdStr))
            {
              log.warn(
                "Invalid UUID: " + reviewableItemIdStr + " for map with ID: " + mapId +
                " Skipping adding review record...");
            }
            else
            {
            	//In the case of the fuzzy match conflict example, this ID may be made up of multiple
              //parts.  Treat each ID part separately.  TODO: add test for this case
              String[] reviewableItemIds = null;
              if (reviewableItemIdStr.contains(";"))
              {
                log.debug("Multiple part reviewable item ID...");
                reviewableItemIds = reviewableItemIdStr.split(";");
              }
              else
              {
            	  reviewableItemIds = new String[1];
            	  reviewableItemIds[0] = reviewableItemIdStr;
              }
              
              for (String reviewableItemId : reviewableItemIds)
              {
            	  //some items won't have a review score tag; For now, the way this is being handled
                //is that items missing a tag get a review score = 1.0; items with an empty string
                //or invalid string for a review tag get a review score of -1.0, which invalidates
                //the review pair.  The case could be argued that invalid/empty score strings should
                //also result in a review score = 1.0.
                double reviewScore = -1.0;
                if (tags.containsKey("hoot:review:score"))
                {
                  try
                  {
                    reviewScore = Double.parseDouble(tags.get("hoot:review:score"));
                  }
                  catch (NumberFormatException e)
                  {
                  }
                }
                else
                {
                  reviewScore = 1.0;
                }
                
                final String source = StringUtils.trimToNull(tags.get("hoot:review:source"));
              	assert(source != null);
                
                //paired item review
                if (tags.containsKey("hoot:review:uuid") && 
                		StringUtils.trimToNull(tags.get("hoot:review:uuid")) != null)
                {
                	final String itemsToReviewAgainstStr = 
                    StringUtils.trimToNull(tags.get("hoot:review:uuid"));
                	assert(itemsToReviewAgainstStr != null);
                	String[] reviewAgainstItemIds = null;
                  //We are parsing pairwise comparisons and don't want duplicates, so ignore one
                  //to many reviewable item to review against item relationships.  They are always
                  //represented with a duplicated one to one relationship in the data.
                  if (!itemsToReviewAgainstStr.contains(";"))
                  {
                    reviewAgainstItemIds = new String[] { itemsToReviewAgainstStr };
                  }
                  else
                  {
                    reviewAgainstItemIds = itemsToReviewAgainstStr.split(";");
                  }

                  for (int i = 0; i < reviewAgainstItemIds.length; i++)
                  {
                    final String reviewAgainstItemId = reviewAgainstItemIds[i];
                    //TODO: I believe this check is not correct, but I know of no other way to handle
                    //this for now...
                    //TODO: make this a batch query somehow
                    if (new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                          .from(elementIdMappings)
                          .where(elementIdMappings.mapId.eq(mapId)
                      		  .and(elementIdMappings.elementId.eq(reviewAgainstItemId)))
                      		.count() == 0)
                    {
                      log.warn(
                        "No element ID mapping exists for review against item with ID: " +
                        reviewAgainstItemId + " for map with ID: " + mapId + ".  Skipping adding " +
                        "review record...");
                    }
                    else
                    {
                      if (!reviewPairAlreadyParsed(reviewableItemId, reviewAgainstItemId))
                      {
                      	if (source.equals("2"))
                      	{
                        	//For paired reviews, we want the reviewableItem to always come from 
                      		//source 1 and the reviewAgainstItems to always come from source 2.  
                      		//So, re-ordering here is necessary for some records.
                      		log.debug(
                            "Adding review item with reviewable item ID: " + reviewAgainstItemId + 
                            ", review against item ID: " + reviewableItemId + ", and source: " + 
                            source);
                          reviewRecordsToInsert.add(
                            createReviewItemRecord(
                            	reviewAgainstItemId, reviewScore, reviewableItemId, mapId));
                          reviewableItemIdToReviewAgainstItemIds.put(
                          	reviewAgainstItemId, reviewableItemId);
                      	}
                      	else
                      	{
                      		log.debug(
                            "Adding review item with reviewable item ID: " +  reviewableItemId + 
                            ", review against item ID: " + reviewAgainstItemId + ", and source: " + 
                            source);
                          reviewRecordsToInsert.add(
                            createReviewItemRecord(
                            	reviewableItemId, reviewScore, reviewAgainstItemId, mapId));
                          reviewableItemIdToReviewAgainstItemIds.put(
                          	reviewableItemId, reviewAgainstItemId);
                      	}
                        
                        flushReviewRecords(reviewRecordsToInsert, maxRecordBatchSize, logMsgStart);
                        numReviewItemsAdded++;
                      }
                    }
                  }
                }
                //single item review (non-pair)
                else if (!tags.containsKey("hoot:review:uuid") ||
                         StringUtils.trimToNull(tags.get("hoot:review:uuid")) == null)
                {
                	//The one case where the reviewableItem can be from source = 2 is for a single 
                	//item review, hence the source check done for paired reviews is not done here. 
                	
                	if (!reviewPairAlreadyParsed(reviewableItemId, reviewableItemId))
                  {
                    log.debug(
                      "Adding review item with reviewable item ID: " +  reviewableItemId + " and " +
                      "review against item ID: " + reviewableItemId);
                    reviewRecordsToInsert.add(
                      createReviewItemRecord(
                        reviewableItemId, reviewScore, reviewableItemId, mapId));
                    reviewableItemIdToReviewAgainstItemIds.put(reviewableItemId, reviewableItemId);
                    flushReviewRecords(reviewRecordsToInsert, maxRecordBatchSize, logMsgStart);
                    numReviewItemsAdded++;
                  }
                }
              }
            }
          }
        }
      }
    }

    //final flush
    flushReviewRecords(reviewRecordsToInsert, 0, logMsgStart);

    log.debug("Wrote " + numReviewItemsAdded + " review items.");

    if (simulateFailure)
    {
      throw new Exception("Simulated test review data parse failure.");
    }

    return reviewRecordWritten;
  }
}
