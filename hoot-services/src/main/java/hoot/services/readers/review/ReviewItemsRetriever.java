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
import java.sql.Timestamp;
import java.util.List;
import java.util.ArrayList;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelationMembers;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QMaps;
import hoot.services.db2.QReviewItems;
import hoot.services.exceptions.writer.review.ReviewItemsWriterException;
import hoot.services.geo.BoundingBox;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.models.review.ReviewableItem;
import hoot.services.validators.review.ReviewMapValidator;
import hoot.services.writers.review.ReviewItemsRetrieverWriter;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;

/**
 * Responsible for serving reviewable items to the client
 */
public class ReviewItemsRetriever
{
	private static final Logger log = LoggerFactory.getLogger(ReviewItemsRetriever.class);
	
	private long mapId;
  private long userId;
  private Connection conn;
  // 5 min
  public static long LOCK_TIME = 300000;
  
  private ReviewItemsRetrieverWriter _writer;
  
  //for tests only
  ReviewItemsRetriever()
  {	
  }

  public ReviewItemsRetriever(final Connection conn, final String mapId) throws Exception
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

      userId = _getUserIdFromMapId();

      log.debug("Retrieved user ID: " + userId);
    }
    catch (Exception e)
    {
      throw new Exception("Error locating user associated with map with ID: " + this.mapId);
    }
    
    _writer = new ReviewItemsRetrieverWriter(this.conn, this.mapId);
  }
  
  private final long _verifyMap(final String mapId) throws Exception
  {
  	return (new ReviewMapValidator(conn)).verifyMapPrepared(mapId);
  }
  
  private final long _getUserIdFromMapId() throws Exception
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

  // Update Review LastAccess column
  public void updateReviewLastAccessTime(final String reviewItemId, final Timestamp newLastAccessTime,
      final String reviewAgainst) throws ReviewItemsWriterException, Exception
  {
  	_writer.updateReviewLastAccessTime(reviewItemId, newLastAccessTime, reviewAgainst);
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
  
  
  private final long _getAllReviewAgainstCount(final String uuid) throws Exception
  {
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                  .from(rm)
                  .where(rm.mapId.eq(mapId).and(rm.reviewableItemId.eq(uuid)))
                  .orderBy(rm.reviewId.asc());
    
    return q.count();
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
  
  public JSONObject getAvaiableReviewItem(final long offsetReviewId, final boolean isForward) 
  		throws ReviewItemsWriterException, Exception
  {
  	String strStatus = "failed";
    
    QReviewItems rm = QReviewItems.reviewItems;
    java.util.Date date= new java.util.Date();
    final long curTimeMili = date.getTime();
    Timestamp now = new Timestamp(curTimeMili);
    Timestamp past = new Timestamp(curTimeMili - ReviewItemsRetriever.LOCK_TIME);
    
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
        	DbUtils.review_status_enum rstat = 
        		(DbUtils.review_status_enum)firstReviewableTuple.get(rm.reviewStatus);
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

  private Tuple _getPreviousItem(final long firstReviewId, final long offsetReviewId, 
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
  private Tuple _getNextItem(final long lastReviewId, final long offsetReviewId, 
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
    	SQLQuery q = _getAvailableReviewWithOffsetQuery(compareTime
          ,offsetReviewId, true);
    	
    	try
    	{
	      // get next item
	      List<Tuple> reviewables = q.limit(2).list(rm.reviewId, rm.reviewableItemId, rm.reviewAgainstItemId);
	      
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
    	catch(Exception ex) 
    	{
    		log.error("Failed to get next:(" + q.toString() + ") REASON: " + ex.getMessage());
    		throw ex;
    	}
    }
    return nextAvailableReviewItem;
  }
 
  private JSONObject _createNextReviewableResponse(final String status, final Tuple nextAvailableReviewItem,
  		final long offsetReviewId, final Timestamp past, final Timestamp now) throws ReviewItemsWriterException, Exception
  {
  	QReviewItems rm = QReviewItems.reviewItems;
  	JSONObject nextItem = new JSONObject();
    nextItem.put("status", status);
    if(nextAvailableReviewItem != null)
    {   
      final long nextReviewId = nextAvailableReviewItem.get(rm.reviewId);
      final String reviewItemUUID = nextAvailableReviewItem.get(rm.reviewableItemId);
      final String reviewAgainstUUID = nextAvailableReviewItem.get(rm.reviewAgainstItemId);
      
      if(offsetReviewId > -1)
      {
	      long freedRowsCnt = _writer.updateReviewLastAccessTimeWithReviewId(past, offsetReviewId); 
	          
	      if(freedRowsCnt == 0)
	      {
	        log.warn("Failed to unlock:" + offsetReviewId + " May be it was part of POI merge and deleted?");
	      }
	      
      }

      try
      {
	      long rowsEffected =  _writer.updateReviewLastAccessTimeWithReviewId(now, nextReviewId);
	      
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
	          	ReviewableItem nextReviewableItem = 
	          		_createReviewItem(
	          			reviewElemMappings, reviewAgainstElemMappings, nextReviewId, reviewItemUUID);
	            nextItem.put("status", "success");
	            nextItem.put("reviewItem", nextReviewableItem);
	          }
	        }
	      }
      }
      catch (Exception ex)
      {
      	log.error(
      		"createNextReviewableResponse failed for nextReviewId: " + nextReviewId + 
      		", reviewItemUUID:" + reviewItemUUID + ", reviewAgainstUUID: " + reviewAgainstUUID +
      		" Error: " + ex.getMessage());
      	throw ex;
      }
    }
    return nextItem;
  }
  private ReviewableItem _createReviewItem(final List<Tuple> reviewElemMappings, 
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

    long allAgainstCnt = _getAllReviewAgainstCount(reviewItemUUID);
    ri.setAllReviewAgainstCnt(allAgainstCnt);
    
    return ri;
  }

}
