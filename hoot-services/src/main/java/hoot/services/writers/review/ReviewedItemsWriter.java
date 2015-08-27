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

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.commons.lang3.StringUtils;
import org.apache.commons.lang3.reflect.MethodUtils;
import org.postgresql.util.PGobject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.expr.BooleanExpression;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.RecordBatchType;
import hoot.services.db.postgres.PostgresUtils;
import hoot.services.db2.QReviewItems;
import hoot.services.db2.ReviewItems;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.review.ReviewedItem;
import hoot.services.models.review.ReviewedItems;
import hoot.services.review.ReviewUtils;

/**
 * Writer for reviewed items
 */
public class ReviewedItemsWriter
{
  private static final Logger log = LoggerFactory.getLogger(ReviewedItemsWriter.class);

  protected static final QReviewItems reviewItems = QReviewItems.reviewItems;

  private Connection conn;
  private long mapId;
  private long changesetId;

  private List<ReviewItems> reviewRecordsToUpdate = new ArrayList<ReviewItems>();
  private Map<ElementType, List<Object>> osmRecordsToUpdate =
    new HashMap<ElementType, List<Object>>();

  private int maxRecordBatchSize = -1;
  public ReviewedItemsWriter(Connection conn, final long mapId, final long changesetId)
    throws Exception
  {
    this.conn = conn;
    this.mapId = mapId;
    this.changesetId = changesetId;

    maxRecordBatchSize =
        Integer.parseInt(
          HootProperties.getInstance()
            .getProperty(
              "maxRecordBatchSize", HootProperties.getDefault("maxRecordBatchSize")));
  }

  /**
   * Updates the review tables in the services database with the reviewed item details associated
   * with the specified map layer
   *
   *
   * @param reviewedItems the reviewed items
   * @returns the number of items marked as reviewed
   * @throws Exception
   */
  public int writeReviewedItems(final ReviewedItems reviewedItems) throws Exception
  {
    if (reviewedItems != null && reviewedItems.getReviewedItems() != null)
    {
      for (ReviewedItem reviewedItem : reviewedItems.getReviewedItems())
      {
        final String reviewedItemUniqueId =
          ReviewUtils.getUniqueIdForElement(
            mapId,
            reviewedItem.getId(),
            Element.elementTypeFromString(reviewedItem.getType().toLowerCase()),
            conn);
        String reviewedAgainstItemUniqueId = null;
        if (reviewedItem.getId() != reviewedItem.getReviewedAgainstId())
        {
          reviewedAgainstItemUniqueId =
            ReviewUtils.getUniqueIdForElement(
              mapId,
              reviewedItem.getReviewedAgainstId(),
              Element.elementTypeFromString(reviewedItem.getReviewedAgainstType().toLowerCase()),
              conn);
        }
        else
        {
          reviewedAgainstItemUniqueId = reviewedItemUniqueId;
        }
        log.debug(
          "Reviewed item unique ID: " + reviewedItemUniqueId + ", " +
          "Reviewed against item unique ID: " + reviewedAgainstItemUniqueId);

        //mark the pair as reviewed in review_items
        try
        {
          addUpdatedReviewRecord(reviewedItemUniqueId, reviewedAgainstItemUniqueId);
        }
        catch (Exception e)
        {
          throw new Exception(
            "Error preparing reviewed item record for save during review save: " + e.getMessage());
        }
        //now update the review and uuid tags on the corresponding osm elements
        try
        {
        	//now trying to do this client side instead...
          /*addUpdatedOsmRecord(
            reviewedItem.getId(),
            Element.elementTypeFromString(reviewedItem.getType().toLowerCase()),
            reviewedAgainstItemUniqueId,
            false);*/
          //TODO: what does "duplicted review pairs" mean?  Disabling this for now, until I figure
          //it out 
          //this second call will handle any duplicated review pairs
          /*addUpdatedOsmRecord(
            reviewedItem.getReviewedAgainstId(),
            Element.elementTypeFromString(reviewedItem.getReviewedAgainstType().toLowerCase()),
            reviewedItemUniqueId,
            true);*/
        }
        catch (Exception e)
        {
          throw new Exception(
            "Error preparing reviewed OSM record for save during review save: " + e.getMessage());
        }
      }

      if (reviewRecordsToUpdate.size() > 0)
      {
      	List<List<BooleanExpression>> predicatelist = new ArrayList<List<BooleanExpression>>();

      	for (int i=0; i<reviewRecordsToUpdate.size(); i++)
      	{
      		List<BooleanExpression> predicates = new ArrayList<BooleanExpression>();
      		predicates.add(reviewItems.reviewId.eq(reviewRecordsToUpdate.get(i).getReviewId()));
      		predicatelist.add(predicates);
      	}

        DbUtils.batchRecords(mapId, reviewRecordsToUpdate, reviewItems, predicatelist, RecordBatchType.UPDATE, conn, maxRecordBatchSize);
        log.debug(reviewRecordsToUpdate.size() + " review records updated.");
      }
      else
      {
        log.warn("No review items updated as a result of mark items as reviewed process.");
      }

      int numOsmRecordsUpdated = 0;
      if (osmRecordsToUpdate.size() > 0)
      {
        for (ElementType elementType : ElementType.values())
        {
          if (!elementType.equals(ElementType.Changeset) &&
              osmRecordsToUpdate.get(elementType) != null)
          {
          	//List<List<BooleanExpression>> predicatelist = new ArrayList<List<BooleanExpression>>();
          	//com.mysema.query.sql.RelationalPathBase<?> t = null;
          	List<?> elems = osmRecordsToUpdate.get(elementType);
          	//TODO: make this generic again as before the jooq --> querydsl conversion
          	if(elementType == ElementType.Node)
          	{
          		DbUtils.batchRecordsDirectNodes(mapId, elems,
          				RecordBatchType.UPDATE, conn, maxRecordBatchSize);
          	}
          	else if(elementType == ElementType.Way)
          	{
          		DbUtils.batchRecordsDirectWays(mapId, elems,
          				RecordBatchType.UPDATE, conn, maxRecordBatchSize);
          	}
          	else if(elementType == ElementType.Relation)
          	{
          		DbUtils.batchRecordsDirectRelations(mapId, elems,
          				RecordBatchType.UPDATE, conn, maxRecordBatchSize);
          	}

          	//DbUtils.batchRecords(elems, t, predicatelist, RecordBatchType.UPDATE, conn, maxRecordBatchSize);

            numOsmRecordsUpdated += osmRecordsToUpdate.get(elementType).size();
          	/*
            DbUtils.batchRecords(
              osmRecordsToUpdate.get(elementType), RecordBatchType.UPDATE, conn);
            numOsmRecordsUpdated += osmRecordsToUpdate.get(elementType).size();*/
          }
        }
        log.debug(numOsmRecordsUpdated + " OSM records updated.");
      }
      else
      {
        log.warn("No OSM features updated as a result of mark items as reviewed process.");
      }
    }

    return reviewRecordsToUpdate.size();
  }

  private void addUpdatedReviewRecord(final String reviewedItemUniqueId,
    final String reviewedAgainstItemUniqueId)
  {
  	ReviewItems reviewedItemPojo =
  			new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItems)
  		.where(
  				reviewItems.reviewableItemId.eq(reviewedItemUniqueId)
  				.and(reviewItems.mapId.eq(mapId))
  				.and(reviewItems.reviewAgainstItemId.eq(reviewedAgainstItemUniqueId))
  				.and(reviewItems.reviewStatus.eq(DbUtils.review_status_enum.unreviewed))
  				)
  		.singleResult(reviewItems);

    if (reviewedItemPojo != null)
    {
      reviewedItemPojo.setReviewStatus(DbUtils.review_status_enum.reviewed);
      reviewedItemPojo.setChangesetId(changesetId);
      Timestamp st = new Timestamp(0);
      reviewedItemPojo.setLastAccessed(st);
      reviewRecordsToUpdate.add(reviewedItemPojo);
    }
  }

  @SuppressWarnings("unused")
  private void addUpdatedOsmRecord(final long reviewedItemOsmId,
    final ElementType reviewedItemOsmType, final String reviewedAgainstItemUniqueId,
    final boolean isDuplicate) throws Exception
  {
    Set<Long> elementIds = new HashSet<Long>();
    elementIds.add(reviewedItemOsmId);
    //final Element prototype = ElementFactory.getInstance().create(reviewedItemOsmType, conn);
    //there should just either be one record returned here, or none at all, if the client has
    //already deleted the feature
    List<?> reviewedElementRecords =
      Element.getElementRecords(mapId, reviewedItemOsmType, elementIds, conn);
    if (reviewedElementRecords != null && reviewedElementRecords.size() != 0 &&
        reviewedElementRecords.get(0) != null)
    {
      Object reviewedElementRecord = reviewedElementRecords.get(0);
      boolean tagChanged = false;
      Map<String, String> tags =
        PostgresUtils.postgresObjToHStore(
          (PGobject)MethodUtils.invokeMethod(reviewedElementRecord, "getTags", new Object[]{}));

      //let the osm changeset save from the client handle this instead
      /*if (tags.containsKey("uuid"))
      {
        String uuid = tags.get("uuid");
        if (!isDuplicate && !uuid.contains(reviewedAgainstItemUniqueId))
        {
          //update the reviewed item's osm element's uuid tag; append
          uuid += ";" + reviewedAgainstItemUniqueId;
          tags.put("uuid", uuid);
          tagChanged = true;
        }
      }
      else
      {
        log.warn(
          "uuid tag removed from reviewed element with ID: " + reviewedItemOsmId +
          " and type: " + reviewedItemOsmType);
      }*/

      if (tags.containsKey("uuid"))
      {
        String reviewAgainstUuids = tags.get("hoot:review:uuid");
        if (StringUtils.trimToNull(reviewAgainstUuids) == null)
        {
          reviewAgainstUuids = "";
        }
        if (reviewAgainstUuids.contains(reviewedAgainstItemUniqueId))
        {
          //update the reviewed item's osm element's hoot:review:uuid tag; remove
          String reviewedAgainstItemUniqueIdReplaceStr =
            "\\{" + reviewedAgainstItemUniqueId.replaceAll("\\{", "").replaceAll("\\}", "") + "\\}";
          reviewAgainstUuids =
            reviewAgainstUuids
              .replaceAll(reviewedAgainstItemUniqueIdReplaceStr + ";", "")
              .replaceAll(reviewedAgainstItemUniqueIdReplaceStr, "");
          if (!isDuplicate && StringUtils.trimToNull(reviewAgainstUuids) != null &&
              StringUtils.trimToNull(reviewAgainstUuids) != ";")
          {
            tags.put("hoot:review:uuid", reviewAgainstUuids);
            log.debug("Updated hoot:review:uuid tag to: " + reviewAgainstUuids);
          }
          else
          {
            //nothing else to review against, so remove all review tags on the reviewed item
            List<String> reviewTagKeys = new ArrayList<String>();
            for (Map.Entry<String, String> tagEntry : tags.entrySet())
            {
              if (tagEntry.getKey().contains("hoot:review"))
              {
                reviewTagKeys.add(tagEntry.getKey());
              }
            }
            for (String reviewTagKey : reviewTagKeys)
            {
              tags.remove(reviewTagKey);
              log.debug("Removed review tag: " + reviewTagKey);
            }
          }
          tagChanged = true;
        }
      }
      else
      {
        log.warn(
          "hoot:review:uuid tag removed from reviewed element with ID: " + reviewedItemOsmId +
          " and type: " + reviewedItemOsmType);
      }

      if (tagChanged)
      {
        Element reviewedElement =
          ElementFactory.getInstance().create(mapId, reviewedItemOsmType, reviewedItemOsmId, conn);
        reviewedElement.setVersion(reviewedElement.getVersion() + 1);
        reviewedElement.setTags(tags);
        if (osmRecordsToUpdate.get(reviewedItemOsmType) == null)
        {
          osmRecordsToUpdate.put(reviewedItemOsmType, new ArrayList<Object>());
        }
        osmRecordsToUpdate.get(reviewedItemOsmType).add(reviewedElement.getRecord());
        log.debug(
          "Added OSM record with ID: " + reviewedItemOsmId + " and type: " +
          reviewedItemOsmType + " to collection for updating.");
//          log.debug("updated tags: ");
//          for (Map.Entry<String, String> tagEntry : reviewedElement.getTags().entrySet())
//          {
//            if(tagEntry.getKey().contains("hoot:review"))
//            {
//              log.debug("key: " + tagEntry.getKey() + " value: " + tagEntry.getValue());
//            }
//          }
      }
    }
    else
    {
      log.warn(
        "No OSM feature exists for reviewed item with ID: " + reviewedItemOsmId + " type: " +
        reviewedItemOsmType + " and review against ID: " + reviewedAgainstItemUniqueId);
    }
  }
}
