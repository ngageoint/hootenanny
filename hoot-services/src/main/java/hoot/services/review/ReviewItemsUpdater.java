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
package hoot.services.review;

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;

import javax.xml.transform.TransformerException;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db.DbUtils.RecordBatchType;
import hoot.services.db2.ElementIdMappings;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QMaps;
import hoot.services.db2.QReviewItems;
import hoot.services.db2.ReviewItems;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Element;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.models.review.ReviewableItem;
import hoot.services.validators.review.ReviewMapValidator;

import org.apache.commons.lang3.StringUtils;
import org.apache.xpath.XPathAPI;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.DOMException;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.mysema.query.types.expr.BooleanExpression;

/**
 * Marks reviewable items as reviewed;  This is a wrapper around the process of auto
 * creating/closing a changeset and marking items as reviewed.
 */
public class ReviewItemsUpdater
{
  private static final Logger log = LoggerFactory.getLogger(ReviewItemsUpdater.class);
  
  private static final QReviewItems reviewItems = QReviewItems.reviewItems;
  private static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;

  private long mapId;
  private long userId;
  private Connection conn;
  // 5 min
  public static long LOCK_TIME = 300000;
  protected int maxRecordBatchSize;
  
  //for tests only
  protected ReviewItemsUpdater()
  {	
  }

  public ReviewItemsUpdater(final Connection conn, final String mapId) throws Exception
  {
    this.conn = conn;
    //Check to see if the map exists in the maps table. (404); input mapId may be a map ID or a
    //map name; this will throw if it doesn't find the map
    log.debug("Checking maps table for map with ID: " + mapId + " ...");
    this.mapId = _verifyMap(mapId);
    //now make sure the user associated with the map exists;
    try
    {
    	//reviewLockTime
    	LOCK_TIME = Long.parseLong(HootProperties.getProperty("reviewLockTime").toString());
      assert(this.mapId != -1);
      log.debug(
        "Retrieving user ID associated with map having ID: " + String.valueOf(this.mapId) + " ...");

      userId =
      		_getUserIdFromMapId();

      log.debug("Retrieved user ID: " + userId);
    }
    catch (Exception e)
    {
      throw new Exception("Error locating user associated with map with ID: " + this.mapId);
    }
    maxRecordBatchSize = 
  		Integer.parseInt(HootProperties.getInstance()
  		   .getProperty("maxRecordBatchSize", HootProperties.getDefault("maxRecordBatchSize")));
  }
  
  private String[] reviewAgainstUuidsFromChangesetElement(final org.w3c.dom.Node elementXml) 
  	throws DOMException, TransformerException
  {
  	String[] reviewAgainstUuids = null;
  	final String reviewAgainstUuidStr = 
    	XPathAPI.selectSingleNode(elementXml, "//tag[@k = 'hoot:review:uuid']").getNodeValue();
  	if (StringUtils.trimToNull(reviewAgainstUuidStr) != null)
  	{
  		if (reviewAgainstUuidStr.contains(";"))
  		{
  			reviewAgainstUuids = reviewAgainstUuidStr.split(";");
  		}
  		else
  		{
  			reviewAgainstUuids = new String[1];
  			reviewAgainstUuids[0] = reviewAgainstUuidStr;
  		}
  	}
  	return reviewAgainstUuids;
  }
  
  private int updateCreatedReviewItems(final Document changesetDoc) throws Exception
  {
  	log.debug("updateCreatedReviewItems");
  	
  	int numReviewItemsUpdated = 0;
  	
    //check create changeset for any newly created reviewable items
  	final NodeList createdReviewItems = 
  	  XPathAPI.selectNodeList(
  	    changesetDoc, "//osmChange/create/node|way|relation/tag[@k='hoot:review:needs and @v='yes']");
  	List<ElementIdMappings> elementIdMappingRecordsToInsert = new ArrayList<ElementIdMappings>();
  	List<ReviewItems> reviewItemRecordsToInsert = new ArrayList<ReviewItems>();
  	for (int i = 0; i < createdReviewItems.getLength(); i++)
  	{
  		//add the associated review data; not checking to see if the element already exists in the
  		//review data, b/c it shouldn't
  		final org.w3c.dom.Node elementXml = createdReviewItems.item(i);
  		final String uuid = 
  			XPathAPI.selectSingleNode(elementXml, "//tag[@k = 'uuid']").getNodeValue();
  		elementIdMappingRecordsToInsert.add(
  			ReviewUtils.createElementIdMappingRecord(
  				uuid, 
  				Long.parseLong(elementXml.getAttributes().getNamedItem("id").getNodeValue()), 
  				Element.elementTypeFromString(elementXml.getNodeName()), 
  				mapId));
  		final String[] reviewAgainstUuids = reviewAgainstUuidsFromChangesetElement(elementXml);
  		if (reviewAgainstUuids != null)
  		{
    		for (int j = 0; j < reviewAgainstUuids.length; j++)
    		{
    			//assuming that an element id mapping already exists for each of the review against items
    			reviewItemRecordsToInsert.add(
    	  		ReviewUtils.createReviewItemRecord(
    	  			uuid, 
    	  		  //TODO: the way to retrieve the correct score would be to trace back the elements 
    	  			//that made up this (presumably) merged element...although you could argue that the 
    	  			//score is inaccurate after a merge; this is possibly complicated, so holding off on 
    	  			//doing it for now
    	  			1.0,
    	  			reviewAgainstUuids[j], 
    	  			mapId));
    		}
  		}
  		else
  		{
  			//adding the item here as reviewed if it has no review against id's left for bookkeeping 
  			//purposes
  			ReviewItems reviewItemRecord = 
  				ReviewUtils.createReviewItemRecord(
	  			  uuid, 
	  			  1.0, //TODO: see explanation above
	  			  null, 
	  			  mapId);
  			reviewItemRecord.setReviewStatus(DbUtils.review_status_enum.reviewed);
  			reviewItemRecordsToInsert.add(reviewItemRecord);
  		}
  	}
  	DbUtils.batchRecords(
    	mapId, elementIdMappingRecordsToInsert, elementIdMappings, null, RecordBatchType.INSERT, conn, 
    	maxRecordBatchSize);
  	DbUtils.batchRecords(
    	mapId, reviewItemRecordsToInsert, reviewItems, null, RecordBatchType.INSERT, conn, 
    	maxRecordBatchSize);
  	numReviewItemsUpdated += reviewItemRecordsToInsert.size();
  	
  	return numReviewItemsUpdated;
  }
  
  private int updateModifiedReviewItems(final Document changesetDoc) throws Exception
  {
    log.debug("updateModifiedReviewItems");
  	
  	int numReviewItemsUpdated = 0;
    
  	//map modified elements by unique id
    final NodeList modifiedItems = 
  	  XPathAPI.selectNodeList(changesetDoc, "//osmChange/modify/node|way|relation]");
    Map<String, org.w3c.dom.Node> uuidsToXml = new HashMap<String, org.w3c.dom.Node>();
  	for (int i = 0; i < modifiedItems.getLength(); i++)
  	{
  		final org.w3c.dom.Node elementXml = modifiedItems.item(i);
  		final String uuid = 
    		XPathAPI.selectSingleNode(elementXml, "//tag[@k = 'uuid']").getNodeValue();
  		uuidsToXml.put(uuid, elementXml);
  	}
  	
  	//determine all modified items that actually have a review record
  	final List<String> modifiedUniqueIds =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    		.from(elementIdMappings)
        .where(
      	  elementIdMappings.elementId.in(uuidsToXml.keySet())
      		  .and(elementIdMappings.mapId.eq(mapId)))
      	.list(elementIdMappings.elementId);
  	
  	//for each id, create an update record with the updated hoot:review:uuid tag contents; only 
  	//the hoot:review:uuid tag should have changed client side, so we only update that one
  	List<ReviewItems> reviewItemRecordsToUpdate = new ArrayList<ReviewItems>();
  	for (String uuid : modifiedUniqueIds)
  	{
  		final org.w3c.dom.Node elementXml = uuidsToXml.get(uuid);
    	final String[] reviewAgainstUuids = reviewAgainstUuidsFromChangesetElement(elementXml);
  		if (reviewAgainstUuids != null)
  		{
  			for (int j = 0; j < reviewAgainstUuids.length; j++)
  			{
  				reviewItemRecordsToUpdate.add(
  		  		ReviewUtils.createReviewItemRecord(
  		  			uuid, 
  		  			1.0, //TODO: see comment in updateCreatedReviewItems
  		  			reviewAgainstUuids[j], 
  		  			mapId));
  			}
  		}
  		else
  		{
  			//record has nothing left to review against it, so set it to reviewed; the client is
  			//expected to have dropped all the review tags from the feature
  			ReviewItems reviewItemRecord = 
  				ReviewUtils.createReviewItemRecord(
    			  uuid, 
    			  1.0, //TODO: see comment in updateCreatedReviewItems
    			  null, 
    			  mapId);
  			reviewItemRecord.setReviewStatus(DbUtils.review_status_enum.reviewed);
  			reviewItemRecordsToUpdate.add(reviewItemRecord);
  		}
  	}
		List<List<BooleanExpression>> predicatelist = new ArrayList<List<BooleanExpression>>();
  	for (int i = 0; i < reviewItemRecordsToUpdate.size(); i++)
  	{
  		List<BooleanExpression> predicates = new ArrayList<BooleanExpression>();
  		predicates.add(reviewItems.reviewId.eq(reviewItemRecordsToUpdate.get(i).getReviewId()));
  		predicatelist.add(predicates);
  	}
  	
    DbUtils.batchRecords(
    	mapId, reviewItemRecordsToUpdate, reviewItems, predicatelist, RecordBatchType.UPDATE, conn, 
    	maxRecordBatchSize);
    numReviewItemsUpdated += reviewItemRecordsToUpdate.size();
    	
  	return numReviewItemsUpdated;
  }
  
  private int updateDeletedReviewItems(final Document changesetDoc) throws TransformerException
  {
  	log.debug("updateDeletedReviewItems");
  	
  	int numReviewItemsUpdated = 0;
    
    //check delete changeset for any deleted items that have a review record entry and delete the 
  	//records from the review data
  	final NodeList deletedItems = 
  	  XPathAPI.selectNodeList(changesetDoc, "//osmChange/delete/node|way|relation/tag/@uuid");
  	List<String> deletedItemUuids = new ArrayList<String>();
  	for (int i = 0; i < deletedItems.getLength(); i++)
  	{
  		deletedItemUuids.add(deletedItems.item(i).getNodeValue());
  	}
  	log.debug("deletedItemUuids: " + deletedItemUuids.toString());
  	final String[] existingReviewItemUuids = 
  		new SQLQuery(conn, DbUtils.getConfiguration(mapId))
	      .from(reviewItems)
		    .where(reviewItems.reviewableItemId.in(deletedItemUuids.toArray(new String[]{})))
		    .list(reviewItems.reviewableItemId)
		    .toArray(new String[]{});
  	numReviewItemsUpdated += 
  		new SQLDeleteClause(conn, DbUtils.getConfiguration(mapId), reviewItems)
  	    .where(reviewItems.reviewableItemId.in(existingReviewItemUuids))
			  .execute();
  	numReviewItemsUpdated += 
  		new SQLDeleteClause(
  			conn, DbUtils.getConfiguration(mapId), elementIdMappings)
	      .where(elementIdMappings.elementId.in(existingReviewItemUuids))
		    .execute();
  	
  	return numReviewItemsUpdated;
  }
  
  /**
   * Synchronizes review table data based on OSM changeset input
   * 
   * @param changesetDoc OSM changeset
   * @return the number of review records updated
   * @throws Exception 
   */
  public int updateReviewItems(final Document changesetDoc) throws Exception
  {
  	int numReviewItemsUpdated = 0;
    log.debug("Updating review items for changeset...");
  	numReviewItemsUpdated += updateCreatedReviewItems(changesetDoc);
  	numReviewItemsUpdated += updateModifiedReviewItems(changesetDoc);
  	numReviewItemsUpdated += updateDeletedReviewItems(changesetDoc);
  	log.debug(
      String.valueOf(numReviewItemsUpdated) + " review records were updated as a " +
      "result of the changeset save.");
  	return numReviewItemsUpdated;
  }
  
  protected final long _verifyMap(final String mapId) throws Exception
  {
  	return (new ReviewMapValidator(conn)).verifyMapPrepared(mapId);
  }
  
  protected final long _getUserIdFromMapId() throws Exception
  {
  	QMaps maps = QMaps.maps;

    return new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(maps)
    .where(maps.id.eq(this.mapId))
    .singleResult(maps.userId);
  }
  
  public void setUserId(final long userid) throws Exception
  {
  	this.userId = userid;
  }
  
  public void setMapId(final String mapId) throws Exception
  {
  	this.mapId = _verifyMap(mapId);
  }
  
  public void setMapId(final long mapId) throws Exception
  {
  	this.mapId = mapId;
  }

  protected final SQLUpdateClause _getLastAccessUpdateClause(final String reviewItemId, 
    final Timestamp newLastAccessTime, final String reviewAgainst) throws Exception
  {
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLUpdateClause q = new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), rm)
    .set(rm.lastAccessed, newLastAccessTime)
    .where(rm.mapId.eq(mapId).and(rm.reviewableItemId.eq(reviewItemId)));
    
    if(reviewAgainst != null)
    {
       q = new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), rm)
        .set(rm.lastAccessed, newLastAccessTime)
        .where(rm.mapId.eq(mapId).and(rm.reviewableItemId.eq(reviewItemId))
            .and(rm.reviewAgainstItemId.eq(reviewAgainst)));
    }
    
    return q;
  }
  // Update Review LastAccess column
  public void updateReviewLastAccessTime(final String reviewItemId, final Timestamp newLastAccessTime,
      final String reviewAgainst) throws Exception
  {
    _getLastAccessUpdateClause(reviewItemId, newLastAccessTime, reviewAgainst)
      .execute(); 
  }
  
 
  protected final SQLQuery _getAvailableReviewQuery(final Timestamp compareTime, final long offsetId, 
      final boolean isAsc) throws Exception
  {   
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLQuery q =  new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    .from(rm)
    .where(rm.mapId.eq(mapId).and(rm.reviewStatus.eq(DbUtils.review_status_enum.unreviewed)
        .and(rm.lastAccessed.lt(compareTime).or(rm.lastAccessed.isNull()))).or(rm.reviewId.eq(offsetId)));
    
    SQLQuery ret = null;
    if(! isAsc)
    {
      ret = q.orderBy(rm.reviewId.desc());
    }
    else
    {
      ret = q.orderBy(rm.reviewId.asc());
    }
    
    return ret;
  }
  
  
  
  // returns total available review count
  public SQLQuery getAvailableReviewCntQuery() throws Exception
  {
    java.util.Date date= new java.util.Date();
    
    long waittime = date.getTime() - LOCK_TIME;
    Timestamp compareTime = new Timestamp(waittime);
    
    SQLQuery q = _getAvailableReviewQuery(compareTime, -1, true);
    
    return q;
  }
  
  
  public SQLQuery getLockedReviewCntQuery() throws Exception
  {
    
    java.util.Date date= new java.util.Date();
    
    long waittime = date.getTime() - LOCK_TIME;
    Timestamp compareTime = new Timestamp(waittime);
    
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    .from(rm)
    .where(rm.mapId.eq(mapId).and(rm.reviewStatus.eq(DbUtils.review_status_enum.unreviewed)
        .and(rm.lastAccessed.goe(compareTime))));
    return q;
  }
  
  
  
  public SQLQuery getTotalReviewCntQuery() throws Exception
  {   
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    .from(rm)
    .where(rm.mapId.eq(mapId));
    return q;
  }
  
  public long getReviewedReviewCnt() throws Exception
  {   
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    .from(rm)
    .where(rm.mapId.eq(mapId).and(rm.reviewStatus.eq(DbUtils.review_status_enum.reviewed)));
    return q.count();
  }

  
  protected final SQLQuery _getReviewAgainstForReviewable(final String uuid) throws Exception
  {
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                  .from(rm)
                  .where(rm.mapId.eq(mapId).and(rm.reviewableItemId.eq(uuid)
                      .and(rm.reviewStatus.eq(DbUtils.review_status_enum.unreviewed))))
                  .orderBy(rm.reviewId.asc());
    
    return q;
  }
  
  protected final SQLQuery _getElementMappingForReviewable(final String uuid) throws Exception
  {
    QElementIdMappings em = QElementIdMappings.elementIdMappings;
    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(em)
                .where(em.mapId.eq(mapId).and(em.elementId.eq(uuid)));
    
    return q;
  }
  protected final SQLQuery _getAvailableReviewWithOffsetQuery(final Timestamp compareTime
      , final long offsetId, final boolean isForward) throws Exception
  {
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(rm)
                .where(rm.mapId.eq(mapId).and(rm.reviewStatus.eq(DbUtils.review_status_enum.unreviewed)
                    .and(rm.lastAccessed.lt(compareTime).or(rm.lastAccessed.isNull()))
                    .and(rm.reviewId.goe(offsetId))))
                .orderBy(rm.reviewId.asc());
    
    if(isForward ==  false)
    {
      q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
      .from(rm)
      .where(rm.mapId.eq(mapId).and(rm.reviewStatus.eq(DbUtils.review_status_enum.unreviewed)
          .and(rm.lastAccessed.lt(compareTime).or(rm.lastAccessed.isNull()))
          .and(rm.reviewId.loe(offsetId))))
      .orderBy(rm.reviewId.desc());
    }
    
    return q;
  }
  
  
  protected final SQLUpdateClause _updateLastAccessWithSubSelect(final Timestamp now, 
      final long reviewId) throws Exception
  {
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLUpdateClause q = new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), rm)
    .set(rm.lastAccessed, now)
    .where(rm.mapId.eq(mapId).and(rm.reviewId.eq(reviewId)));
    
    return q;
  }  
  
  protected SQLQuery _getRelationBboxQuery(final long id) throws Exception
  {
  	 QCurrentNodes cn = QCurrentNodes.currentNodes;
     QCurrentWayNodes cwn = QCurrentWayNodes.currentWayNodes;
     QCurrentRelationMembers crm = QCurrentRelationMembers.currentRelationMembers;
     
     SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
     .from(cn)
     .where(
          cn.id.in(new SQLSubQuery().from(cwn).where(
              cwn.wayId.in(new SQLSubQuery().from(crm).where(crm.relationId.eq(id)
                  .and(crm.memberType.eq(DbUtils.nwr_enum.way))).list(crm.memberId) ))
              .list(cwn.nodeId))
          .or(cn.id.in(new SQLSubQuery().from(crm).where(crm.relationId.eq(id)
              .and(crm.memberType.eq(DbUtils.nwr_enum.node))).list(crm.memberId)))
         );
     return q;
  }

  private BoundingBox _getRelationBbox(final long id) throws Exception
  {

    QCurrentNodes cn = QCurrentNodes.currentNodes;
    
    SQLQuery q = _getRelationBboxQuery(id);
    List<Tuple> bounds = q.list(cn.latitude.max(), cn.latitude.min(), 
        cn.longitude.max(), cn.longitude.min());
    
    Double maxLat = bounds.get(0).get(0, Double.class);
    Double minLat = bounds.get(0).get(1, Double.class);
    Double maxLon = bounds.get(0).get(2, Double.class);
    Double minLon = bounds.get(0).get(3, Double.class);
    
    BoundingBox bbox = new BoundingBox(minLon, minLat, maxLon, maxLat);
    return bbox;
  }
  
  protected SQLQuery _getWayBboxQuery(final long id) throws Exception
  {
  	QCurrentNodes cn = QCurrentNodes.currentNodes;
    QCurrentWayNodes cwn = QCurrentWayNodes.currentWayNodes;

    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    .from(cn)
    .where(
        cn.id.in(
            new SQLSubQuery().from(cwn).where(cwn.wayId.eq(id)).list(cwn.nodeId)
            )
        );
    return q;
  }
  private BoundingBox _getWayBbox(final long id) throws Exception
  {

    QCurrentNodes cn = QCurrentNodes.currentNodes;

    
    SQLQuery q = _getWayBboxQuery(id);
    List<Tuple> bounds = q.list(cn.latitude.max(), cn.latitude.min(), 
        cn.longitude.max(), cn.longitude.min());

    Double maxLat = bounds.get(0).get(0, Double.class);
    Double minLat = bounds.get(0).get(1, Double.class);
    Double maxLon = bounds.get(0).get(2, Double.class);
    Double minLon = bounds.get(0).get(3, Double.class);
    
    BoundingBox bbox = new BoundingBox(minLon, minLat, maxLon, maxLat);
    return bbox;
  }
  
  protected SQLQuery _getNodeCoordQuery(final long id) throws Exception
  {
  	QCurrentNodes cn = QCurrentNodes.currentNodes;

    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    .from(cn)
    .where(
        cn.id.eq(id)
        );
    
    return q;
  }
  private BoundingBox _getNodeCoord(final long id) throws Exception
  {

    QCurrentNodes cn = QCurrentNodes.currentNodes;

    
    SQLQuery q = _getNodeCoordQuery(id);
    List<Tuple> bounds = q.list(cn.latitude, cn.longitude);
    
    Double lat = bounds.get(0).get(0, Double.class);
    Double lon = bounds.get(0).get(1, Double.class);

    BoundingBox bbox = new BoundingBox(lon, lat, lon, lat);
    return bbox;
  }
  
  public JSONObject getAvaiableReviewItem(final long offsetReviewId, final boolean isForward) throws Exception
  {
  	String strStatus = "failed";
    
    QReviewItems rm = QReviewItems.reviewItems;
    java.util.Date date= new java.util.Date();
    final long curTimeMili = date.getTime();
    Timestamp now = new Timestamp(curTimeMili);
    Timestamp past = new Timestamp(curTimeMili - ReviewItemsUpdater.LOCK_TIME);
    
    long waittime = curTimeMili - LOCK_TIME;
    Timestamp compareTime = new Timestamp(waittime);
    
    List<Tuple> firstReviewables = _getAvailableReviewQuery(compareTime, offsetReviewId, true).limit(1)
        .list(rm.reviewId, rm.reviewableItemId, rm.reviewAgainstItemId, rm.reviewStatus);
    
    List<Tuple> lastReviewables = _getAvailableReviewQuery(compareTime, offsetReviewId, false).limit(1)
        .list(rm.reviewId, rm.reviewableItemId, rm.reviewAgainstItemId, rm.reviewStatus);
    
    Tuple nextAvailableReviewItem = null;
    
    if(firstReviewables.size() > 0 && lastReviewables.size() > 0)
    {
      final Tuple firstReviewableTuple = firstReviewables.get(0);
      final Tuple lastReviewableTuple = lastReviewables.get(0);
      final long firstReviewId = firstReviewableTuple.get(rm.reviewId);
      final long lastReviewId = lastReviewableTuple.get(rm.reviewId);
      
      
      if(offsetReviewId > -1)
      {
        // Only one left
        if(firstReviewId == lastReviewId)
        {
        	DbUtils.review_status_enum rstat = (DbUtils.review_status_enum)firstReviewableTuple.get(rm.reviewStatus);
        	if(rstat == DbUtils.review_status_enum.unreviewed)
        	{
        		nextAvailableReviewItem = firstReviewableTuple;
        	}
        	else
        	{
        		strStatus = "noneavailable";
        	}
        }
        else
        {
          if(isForward)
          {
          	nextAvailableReviewItem = _getNextItem(lastReviewId, offsetReviewId, 
            		firstReviewableTuple, compareTime);
          }
          else // is backward
          {
          	nextAvailableReviewItem = _getPreviousItem( firstReviewId, offsetReviewId, 
            		lastReviewableTuple, compareTime);
          }
        }
      }
      else // it is first request so we just return first
      {
        nextAvailableReviewItem = firstReviewableTuple;
      }
    }
    else // Nothing left
    {
      nextAvailableReviewItem = null;
    }
    
    
    
    
    return _createNextReviewableResponse(strStatus, nextAvailableReviewItem,
    		offsetReviewId, past, now);
    
  }

  protected Tuple _getPreviousItem(final long firstReviewId, final long offsetReviewId, 
  		final Tuple lastReviewableTuple, final Timestamp compareTime) throws Exception
  {
  	QReviewItems rm = QReviewItems.reviewItems;
  	Tuple prevAvailableReviewItem = null;
    // first in backward so get the last available
    if(firstReviewId == offsetReviewId)
    {
    	prevAvailableReviewItem = lastReviewableTuple;
    }
    else // normal backward search
    {
      // get previous item
      List<Tuple> reviewables = _getAvailableReviewWithOffsetQuery(compareTime
          ,offsetReviewId, false).limit(2).list(rm.reviewId, rm.reviewableItemId, rm.reviewAgainstItemId);
      
      if(reviewables.size() > 0)
      {
        Tuple firstPotential = reviewables.get(0);
        if(firstPotential.get(rm.reviewId) != offsetReviewId)
        {
        	prevAvailableReviewItem = firstPotential;
        }
        else
        {
          // This should not happen...
        	prevAvailableReviewItem = null;
        }
      }
    }
    return prevAvailableReviewItem;
  }
  protected Tuple _getNextItem(final long lastReviewId, final long offsetReviewId, 
  		final Tuple firstReviewableTuple, final Timestamp compareTime) throws Exception
  {
  	QReviewItems rm = QReviewItems.reviewItems;
  	Tuple nextAvailableReviewItem = null;
    // See if current offset is the last if so then get the first
    if(lastReviewId == offsetReviewId)
    {
      nextAvailableReviewItem = firstReviewableTuple;
    }
    else
    {
      // get next item
      List<Tuple> reviewables = _getAvailableReviewWithOffsetQuery(compareTime
          ,offsetReviewId, true).limit(2).list(rm.reviewId, rm.reviewableItemId, rm.reviewAgainstItemId);
      
      if(reviewables.size() > 0)
      {
        Tuple firstPotential = reviewables.get(0);
        if(firstPotential.get(rm.reviewId) == offsetReviewId)
        {
          // We have next
          if(reviewables.size() > 1)
          {
            nextAvailableReviewItem = reviewables.get(1);
          }
          else // we do not have next so send not found
          {
            nextAvailableReviewItem = null;
          }
        }
        else
        {
          // Something happened to offset review we will just return whatever left
          // This should not happen...
          nextAvailableReviewItem = firstPotential;
        }
      }
    }
    return nextAvailableReviewItem;
  }
 
  protected JSONObject _createNextReviewableResponse(final String status, final Tuple nextAvailableReviewItem,
  		final long offsetReviewId, final Timestamp past, final Timestamp now) throws Exception
  {
  	QReviewItems rm = QReviewItems.reviewItems;
  	JSONObject nextItem = new JSONObject();
    nextItem.put("status", status);
    if(nextAvailableReviewItem != null)
    {   
      final long nextReviewId = nextAvailableReviewItem.get(rm.reviewId);
      final String reviewItemUUID = nextAvailableReviewItem.get(rm.reviewableItemId);
      final String reviewAgainstUUID = nextAvailableReviewItem.get(rm.reviewAgainstItemId);
      
      boolean doLock = false;
      
      
      if(offsetReviewId > -1)
      {
        // free previous lock
        
        long freedRowsCnt = _updateLastAccessWithSubSelect(past, offsetReviewId)
        .execute(); 
        
        if(freedRowsCnt > 0)
        {
          doLock = true;
        }
      }
      else
      {
        doLock = true;
      }
      
      if(doLock)
      {
        // lock the item if still available
        long rowsEffected =  _updateLastAccessWithSubSelect(now, nextReviewId)
        .execute(); 
        
        if(rowsEffected > 0)
        {
          QElementIdMappings em = QElementIdMappings.elementIdMappings;
          if(reviewItemUUID != null)
          {
            List<Tuple> reviewElemMappings = _getElementMappingForReviewable(reviewItemUUID)
                .list(em.elementId, em.osmElementId, em.osmElementType);
            List<Tuple> reviewAgainstElemMappings = _getElementMappingForReviewable(reviewAgainstUUID)
                .list(em.elementId, em.osmElementId, em.osmElementType);
            if(reviewElemMappings.size() > 0)
            {
            	// Create reviewableItem object
            	ReviewableItem nextReviewablItem = _createReviewItem(reviewElemMappings, reviewAgainstElemMappings, 
              		nextReviewId, reviewItemUUID);
              nextItem.put("status", "success");
              nextItem.put("reviewItem", nextReviewablItem);
            }
          }
        }
      }
      else
      {
        nextItem.put("status", "noneavailable");
      }
    }
    return nextItem;
  }
  protected ReviewableItem _createReviewItem(final List<Tuple> reviewElemMappings, 
  		final List<Tuple> reviewAgainstElemMappings, 
  		final long nextReviewId, final String reviewItemUUID) throws Exception
  {

  	QElementIdMappings em = QElementIdMappings.elementIdMappings;
  	QReviewItems rm = QReviewItems.reviewItems;
  	
  	ReviewableItem ri = new ReviewableItem();
    
    Tuple mapping = reviewElemMappings.get(0);

    ri.setReviewId(nextReviewId);
    ri.setUuid(mapping.get(em.elementId));
    
    final long osmId = mapping.get(em.osmElementId);
    ri.setId(osmId);
    
    final DbUtils.nwr_enum osmType = (DbUtils.nwr_enum)mapping.get(em.osmElementType);
    ri.setType(osmType.toString());
    
    if(osmType == DbUtils.nwr_enum.node)
    {
      BoundingBox bbox = this._getNodeCoord(osmId);
      ri.setDisplayBounds(bbox.toServicesString());
    }
    else if(osmType == DbUtils.nwr_enum.way)
    {
      BoundingBox bbox = this._getWayBbox(osmId);
      ri.setDisplayBounds(bbox.toServicesString());
    }
    else if(osmType == DbUtils.nwr_enum.relation)
    {
      BoundingBox bbox = this._getRelationBbox(osmId);
      ri.setDisplayBounds(bbox.toServicesString());
    }
    
    
    /// against
    Tuple againstMapping = reviewAgainstElemMappings.get(0);
    
    ReviewAgainstItem rai = new ReviewAgainstItem();
                  
    String againstUUID = againstMapping.get(em.elementId);
    rai.setUuid(againstUUID);
    ri.setAgainstList(againstUUID);
    
    final long osmAgId = againstMapping.get(em.osmElementId);
    rai.setId(osmAgId);
    
    final DbUtils.nwr_enum osmAgType = (DbUtils.nwr_enum)againstMapping.get(em.osmElementType);
    rai.setType(osmAgType.toString());
    
    
    if(osmAgType == DbUtils.nwr_enum.node)
    {
      BoundingBox bbox = this._getNodeCoord(osmAgId);
      rai.setDisplayBounds(bbox.toServicesString());
    }
    else if(osmAgType == DbUtils.nwr_enum.way)
    {
      BoundingBox bbox = this._getWayBbox(osmAgId);
      rai.setDisplayBounds(bbox.toServicesString());
    }
    else if(osmAgType == DbUtils.nwr_enum.relation)
    {
      BoundingBox bbox = this._getRelationBbox(osmAgId);
      rai.setDisplayBounds(bbox.toServicesString());
    }
    
    ri.setItemToReviewAgainst(rai);
    
    List<String> agList = new ArrayList<String>();
    // get the unreviewed  of list of against list
    List<Tuple> availableAgainstList = _getReviewAgainstForReviewable(reviewItemUUID)
              .list(rm.reviewId, rm.reviewableItemId, rm.reviewAgainstItemId); 
    for(Tuple availAgainst :  availableAgainstList)
    {
    
      agList.add(availAgainst.get(rm.reviewAgainstItemId));
    }
    String availAgList = StringUtils.join(agList,";");
    ri.setAgainstList(availAgList);


    return ri;
  }

}
