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
package hoot.services.readers.review;

import java.sql.Connection;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Set;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.google.common.collect.ArrayListMultimap;
import com.google.common.collect.ListMultimap;
import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.OrderSpecifier;
import com.mysema.query.types.QTuple;
import com.mysema.query.types.expr.BooleanExpression;

import hoot.services.db.DbUtils;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QReviewItems;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.Map;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.models.review.ReviewableItem;
import hoot.services.review.DisplayBoundsCalculator;
import hoot.services.review.ReviewScoreComparator;
import hoot.services.validators.review.ReviewMapValidator;
import static com.mysema.query.group.GroupBy.*;

/**
 * Retrieves reviewable conflated data items from the services database
 *
 * @todo This class has been the biggest victim of the last minute May '14 demo review changes.
 * This class is virtually incomprensible at this point and needs to be completely rewritten.
 */
public class ReviewableItemRetriever
{
  private static final Logger log = LoggerFactory.getLogger(ReviewableItemRetriever.class);

  protected static final QReviewItems reviewItemsTbl = QReviewItems.reviewItems;

  protected static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;

  private long mapId;
  public long getMapId() { return mapId; }

  private Connection conn;

  private BoundingBox reviewAgainstItemBounds;

  //hack to prevent dupe items from being returned
  private ListMultimap<String, String> reviewableItemIdToReviewAgainstItemIds;

  public ReviewableItemRetriever(final Connection conn, final String mapId) throws Exception
  {
    this.conn = conn;
    //Check to see if the map exists in the maps table. (404); input mapId may be a map ID or a
    //map name; this will throw if it doesn't find the map
    this.mapId = (new ReviewMapValidator(conn)).verifyMapPrepared(mapId);
  }

  private boolean reviewPairAlreadyParsed(final String reviewableItemId,
    final String reviewAgainstItemId)
  {
    if (reviewAgainstItemId != null)
    {
      return
        (reviewableItemIdToReviewAgainstItemIds.containsKey(reviewableItemId) &&
            reviewableItemIdToReviewAgainstItemIds.get(reviewableItemId).contains(
              reviewAgainstItemId));
    }
    //TODO: can this situation still even occur?
    else
    {
      return reviewableItemIdToReviewAgainstItemIds.containsKey(reviewableItemId);
    }
  }

  /**
   * Executes a query against the services database for the review items that meet the input
   * parameter specifications
   *
   * This method assumes input parameters have already been validated.
   *
   * @param numItems the number of review items to retrieve; min = 1; max = the lesser of the total
   * number of items returned by the corresponding query or the value specified in the services
   * config for maxRecordBatchSize
   * @param highestReviewScoreFirst if true, sorts results with the highest review scores returned
   * first
   * @param reviewScoreThresholdMinimum lower review score an item may have and be returned in the
   * results; 0 to 1
   * @param geospatialBounds determines the geospatial bounds a result must reside within to be
   * returned in the results; if null then geospatialBounds = world
   * @param displayBoundsZoomAdjust automatically adjusts the display bounds by the specified
   * amount for every review item pair; negative values zoom out, positive values zoom in
   * @param boundsDisplayMethod determines how the display bounds is constructed
   * @return a collection of conflated data items for review
   * @throws Exception
   * @todo make some more generic internal helpre method (s) which allow for generically filtering
   * by column and sorting on columns generically
   */
  public List<ReviewableItem> getReviewItems(final int numItems,
    final boolean highestReviewScoreFirst, final double reviewScoreThresholdMinimum,
    final BoundingBox geospatialBounds, final double displayBoundsZoomAdjust,
    final DisplayBoundsCalculator.DisplayMethod boundsDisplayMethod) throws Exception
  {
    log.debug("Retrieving items for review with parameters:");
    log.debug("numItems: " + numItems);
    log.debug("highestReviewScoreFirst: " + highestReviewScoreFirst);
    log.debug("reviewScoreThresholdMinimum: " + reviewScoreThresholdMinimum);
    if (geospatialBounds != null)
    {
      log.debug("geospatialBounds: " + geospatialBounds.toServicesString());
    }
    log.debug("displayBoundsZoomAdjust: " + displayBoundsZoomAdjust);
    log.debug("boundsDisplayMethod: " + boundsDisplayMethod.toString());

    List<ReviewableItem> reviewItems = new ArrayList<ReviewableItem>();

    List<OrderSpecifier<?>> orderByFields = new ArrayList<OrderSpecifier<?>>();
    if (highestReviewScoreFirst)
    {
      orderByFields.add(reviewItemsTbl.reviewScore.desc());
    }
    //Since limit is being used, we always want results ordered to get predictable results,
    //even if score ordering wasn't specified.
    orderByFields.add(reviewItemsTbl.reviewId.asc());

    BooleanExpression whereCondition = reviewItemsTbl.reviewStatus.eq(DbUtils.review_status_enum.unreviewed);
    if (reviewScoreThresholdMinimum > 0)
    {
      whereCondition =
        whereCondition.and(
        		reviewItemsTbl.reviewScore.goe(reviewScoreThresholdMinimum));
    }

    //TODO: run all this within a read-only transaction; verify that no other writes are seen
    //during this transaction

    java.util.Map<ElementType, Set<Long>> elementIdsWithinBoundsByType = null;
    if (geospatialBounds != null && !geospatialBounds.equals(BoundingBox.worldBounds()))
    {
      //retrieve the IDs of all elements by type from the map that fall within the requested
      //geospatial bounds
      elementIdsWithinBoundsByType =
        (new Map(mapId, conn)).queryForElementIds(geospatialBounds);
    }

    /*
     * I don't like this, but doing a query of numItems size for each element type so that we make
       sure to get see all the available items and get the sorting correct.  Queries which join in
       element records have to be done separately for each element type, since element IDs aren't
       unique across element types for a given map.  At the end of the querying I'll sort by score
       and truncate down the items being returned down to the requested number of items.

       One possible away to avoid making the extra queries would be to perform some up front query
       counts and determine the minimum set of elements that needs to be queried to meet the
       requested count.
     */
    reviewableItemIdToReviewAgainstItemIds = ArrayListMultimap.create();
    for (ElementType elementType : ElementType.values())
    {
      if (!elementType.equals(ElementType.Changeset))
      {
        final Element prototype = ElementFactory.getInstance().create(mapId, elementType, conn);

        BooleanExpression elementJoinCondition =
        		elementIdMappings.osmElementId.eq(prototype.getElementIdField());
        if (elementIdsWithinBoundsByType != null && elementIdsWithinBoundsByType.get(elementType).size() > 0)
        {
          assert(geospatialBounds != null && !geospatialBounds.equals(BoundingBox.worldBounds()));
          elementJoinCondition =
            elementJoinCondition.and(
              prototype.getElementIdField().in(elementIdsWithinBoundsByType.get(elementType)));
        }

        //get unreviewed records for the specified map, of the specified element type, joined with
        //element info, filter on requested filter params, order by requested ordering params,
        //limit to requested number of records, grouped by unique id

        OrderSpecifier<?>[] fields = new OrderSpecifier<?>[orderByFields.size()];
        for(int ii=0; ii<orderByFields.size(); ii++)
        {
        	fields[ii] = orderByFields.get(ii);
        }
        SQLQuery query0 = new SQLQuery(conn, DbUtils.getConfiguration(mapId));
        final java.util.Map<String, List<Tuple>>
        unreviewedJoinedRecordsForElementTypeGroupedByUniqueId =
        		query0.from(reviewItemsTbl)
        	.join(elementIdMappings)
        	.on(
        			reviewItemsTbl.reviewableItemId.eq(elementIdMappings.elementId)
        			.and(reviewItemsTbl.mapId.eq(mapId))
        			.and(elementIdMappings.mapId.eq(mapId))
        			.and(
        					elementIdMappings.osmElementType.eq(Element.elementEnumForElementType(elementType))
        					)
        			)
        	.join(prototype.getElementTable())
        	.on(elementJoinCondition)
        	.where(whereCondition)
        	.orderBy(fields)
        	.limit(numItems)
        	.transform(groupBy(reviewItemsTbl.reviewableItemId).as(list(new QTuple(prototype.getElementTable(), reviewItemsTbl
        			,elementIdMappings
        			))));


        //get the items to be reviewed against; This needs to be done for all element types since
        //there can be cross-matching between reviewable element types and what they are reviewed
        //against.
        //TODO: This system has been designed so that any OSM element type can be reviewed against
        //any other type, but the reality is that, so far, ways and relations are the only cross
        //element type reviews.  So possibly, some more restrictions need to be put in place here.
        java.util.Map<ElementType, java.util.Map<String, List<Tuple>>>
          reviewAgainstRecordsGroupedByReviewableItemIdByElementType =
            new HashMap<ElementType, java.util.Map<String, List<Tuple>>>();
        for (ElementType elementType2 : ElementType.values())
        {
          if (!elementType2.equals(ElementType.Changeset))
          {
            final Element prototype2 = ElementFactory.getInstance().create(mapId, elementType2, conn);
            //using the unique element IDs obtained previously, get all review against item records
            //for the specified map, of the specified element type, joined with element info,
            //grouped by element id

            java.util.Map<String, List<Tuple>> reviewAgainstRecordsGroupedByReviewableItemId = new HashMap<String, List<Tuple>>();
            if(unreviewedJoinedRecordsForElementTypeGroupedByUniqueId.keySet().size() > 0)
            {
	            reviewAgainstRecordsGroupedByReviewableItemId =
	            		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItemsTbl)
	          	.join(elementIdMappings)
	          	.on(
	          			reviewItemsTbl.reviewAgainstItemId.eq(elementIdMappings.elementId)
	          			.and(reviewItemsTbl.mapId.eq(mapId))
	          			.and(elementIdMappings.mapId.eq(mapId))
	          			.and(
	          					elementIdMappings.osmElementType.eq(Element.elementEnumForElementType(elementType2))
	          					)
	          			)
	          	.join(prototype2.getElementTable())
	          	.on(elementIdMappings.osmElementId.eq(prototype2.getElementIdField())
	          			)
	          	.where(
	          			reviewItemsTbl.reviewableItemId.in(
	          					 unreviewedJoinedRecordsForElementTypeGroupedByUniqueId.keySet()
	          					)
	          			)
	          	.orderBy(fields)
	          	.transform(groupBy(reviewItemsTbl.reviewableItemId).as(list(new QTuple(prototype2.getElementTable(), reviewItemsTbl
	          			,elementIdMappings
	          			))));
            }


            reviewAgainstRecordsGroupedByReviewableItemIdByElementType.put(
              elementType2, reviewAgainstRecordsGroupedByReviewableItemId);
          }
        }

        //convert the records to ReviewItems
        for (java.util.Map.Entry<String, List<Tuple>> joinedRecordEntry :
             unreviewedJoinedRecordsForElementTypeGroupedByUniqueId.entrySet())
        {
          reviewItems.addAll(
            recordsToReviewItems(
              elementType, joinedRecordEntry.getValue(),
              reviewAgainstRecordsGroupedByReviewableItemIdByElementType,
              displayBoundsZoomAdjust, boundsDisplayMethod));
        }
      }
    }

    //It's possible by this point that we have more review items than were requested since we
    //performed the queries to get the elements separate for each element type.  So, reduce the
    //set down to the correct amount.  If sorting was requested, perform the sorting on the list
    //and then filter.
    if (highestReviewScoreFirst)
    {
      Collections.sort(reviewItems, new ReviewScoreComparator());
    }
    if (reviewItems.size() > numItems)
    {
      reviewItems = reviewItems.subList(0, numItems);
    }
//    log.debug("Returning sorted review items:");
//    for (ReviewableItem reviewItem : reviewItems)
//    {
//      log.debug(reviewItem.toString());
//    }

    //even if no items were found by the query, we still return an empty collection
    return reviewItems;
  }

  /*
   * We pass in elements to review against of all types here, because an item being reviewed can
   * be reviewed against elements of multiple element types (e.g. a way reviewed against a
   * relation).
   */
  private List<ReviewableItem> recordsToReviewItems(final ElementType reviewableElementType,
    final List<Tuple> reviewableJoinedRecords,
    final java.util.Map<ElementType, java.util.Map<String, List<Tuple>>>
      reviewAgainstRecordsGroupedByReviewableItemIdByElementType,
    final double displayBoundsZoomAdjust,
    final DisplayBoundsCalculator.DisplayMethod boundsDisplayMethod) throws Exception
  {
    List<ReviewableItem> reviewableItems = new ArrayList<ReviewableItem>();
    try
    {
      assert(reviewableJoinedRecords != null);
      assert(reviewableJoinedRecords.size() > 0);
      for (Tuple reviewableJoinedRecord : reviewableJoinedRecords)
      {
        assert(reviewableJoinedRecord != null);
        final Element reviewableElement =
          ElementFactory.getInstance().create(reviewableElementType, reviewableJoinedRecord, conn, mapId);
        assert(reviewableElement != null);
        reviewAgainstItemBounds = null;

        final List<ReviewAgainstItem> reviewAgainstItems =
          parseReviewAgainstItemDataFromRecords(
          		reviewableJoinedRecord.get(reviewItemsTbl).getReviewableItemId(),
            reviewAgainstRecordsGroupedByReviewableItemIdByElementType);
        for (ReviewAgainstItem reviewAgainstItem : reviewAgainstItems)
        {
          final String reviewableItemUuid = reviewableElement.getTags().get("uuid");
          if (!reviewPairAlreadyParsed(reviewableItemUuid, reviewAgainstItem.getUuid()))
          {
            ReviewableItem reviewableItem = new ReviewableItem();
            reviewableItem.setId(
              /*reviewableJoinedRecord.getValue(Tables.ELEMENT_ID_MAPPINGS.OSM_ELEMENT_ID)*/
            		reviewableJoinedRecord.get(elementIdMappings).getOsmElementId()
            		);
            reviewableItem.setUuid(reviewableItemUuid);
            reviewableItem.setType(reviewableElementType.toString().toLowerCase());
            reviewableItem.setReviewScore(
            		reviewableJoinedRecord.get(reviewItemsTbl).getReviewScore());

            reviewableItem.setItemToReviewAgainst(reviewAgainstItem);

            //make the display extent returned the smaller of the two extents of the review and
            //review against elements
            assert(reviewableElement.getBounds() != null);
            assert(reviewAgainstItemBounds != null);
            final BoundingBox displayBounds =
              (new DisplayBoundsCalculator(displayBoundsZoomAdjust, boundsDisplayMethod))
                .calculateDisplayBounds(reviewableElement.getBounds(), reviewAgainstItemBounds);
            reviewableItem.setDisplayBounds(displayBounds.toServicesString());
            log.debug("Adding reviewableItem: " + reviewableItem.toString());
            reviewableItems.add(reviewableItem);

            reviewableItemIdToReviewAgainstItemIds.put(
              reviewableItemUuid, reviewAgainstItem.getUuid());
          }
        }
      }
    }
    catch (Exception e)
    {
      throw new Exception("Error recordsToReviewItem: " + e.getMessage());
    }
    return reviewableItems;
  }

  /*
   * TODO: logic from before the review re-factoring is left in this method; it allows for
   * multiple review against items for each reviewable item when there will only be one, which
   * makes it very hard to follow
   */
  private List<ReviewAgainstItem> parseReviewAgainstItemDataFromRecords(
    final String reviewableItemId,
    final java.util.Map<ElementType, java.util.Map<String, List<Tuple>>> reviewAgainstRecordsGroupedByReviewableItemIdByElementType) throws Exception
  {
    List<ReviewAgainstItem> reviewAgainstItems = new ArrayList<ReviewAgainstItem>();
    try
    {
      for (java.util.Map.Entry<ElementType, java.util.Map<String, List<Tuple>>> reviewAgainstJoinedRecordsEntry :
           reviewAgainstRecordsGroupedByReviewableItemIdByElementType.entrySet())
       {
         final ElementType reviewAgainstElementType =  reviewAgainstJoinedRecordsEntry.getKey();
         final java.util.Map<String, List<Tuple>> reviewAgainstJoinedRecordsById =
           reviewAgainstJoinedRecordsEntry.getValue();
         for (java.util.Map.Entry<String, List<Tuple>> reviewAgainstJoinedRecordByIdEntry :
              reviewAgainstJoinedRecordsById.entrySet())
         {
           final String uniqueId = reviewAgainstJoinedRecordByIdEntry.getKey();
           if (uniqueId.equals(reviewableItemId))
           {
             final List<Tuple> reviewAgainstJoinedRecords =
               reviewAgainstJoinedRecordByIdEntry.getValue();
             for (Tuple reviewAgainstJoinedRecord : reviewAgainstJoinedRecords)
             {
               final Element reviewAgainstElement =
                 ElementFactory.getInstance().create(
                   reviewAgainstElementType, reviewAgainstJoinedRecord, conn, mapId);
               ReviewAgainstItem reviewAgainstItem = new ReviewAgainstItem();
               if (reviewAgainstItemBounds == null)
               {
                 reviewAgainstItemBounds = reviewAgainstElement.getBounds();
               }
               else
               {
                 reviewAgainstItemBounds.add(reviewAgainstElement.getBounds());
               }
               reviewAgainstItem.setId(reviewAgainstElement.getId());
               reviewAgainstItem.setUuid(reviewAgainstElement.getTags().get("uuid"));
               reviewAgainstItem.setType(reviewAgainstElementType.toString().toLowerCase());
               reviewAgainstItems.add(reviewAgainstItem);
             }
           }
         }
       }
    }
    catch (Exception e)
    {
      throw new Exception("Error parseReviewAgainstItemsDataFromRecords: " + e.getMessage());
    }
    return reviewAgainstItems;
  }
}
