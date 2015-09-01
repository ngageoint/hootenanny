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
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Changeset;
import hoot.services.models.review.MarkItemsReviewedRequest;
import hoot.services.models.review.MarkItemsReviewedResponse;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.models.review.ReviewableItem;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.validators.osm.ChangesetUploadXmlValidator;
import hoot.services.validators.review.ReviewMapValidator;
import hoot.services.writers.osm.ChangesetDbWriter;
import hoot.services.writers.review.ReviewedItemsWriter;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import com.mysema.query.Tuple;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.SQLSubQuery;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.sql.dml.SQLUpdateClause;

/**
 * Marks reviewable items as reviewed;  This is a wrapper around the process of auto
 * creating/closing a changeset and marking items as reviewed.
 */
public class ReviewItemsMarker
{
  private static final Logger log = LoggerFactory.getLogger(ReviewItemsMarker.class);

  private long mapId;
  private long userId;
  private Connection conn;
  // 5 min
  public static long LOCK_TIME = 300000;

  public ReviewItemsMarker(final Connection conn, final String mapId) throws Exception
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
  }
  
  
  protected final long _verifyMap(final String mapId) throws Exception
  {
  	return (new ReviewMapValidator(conn)).verifyMapPrepared(mapId);
  }
  
  protected final long _getUserIdFromMapId() throws Exception
  {
  	QMaps maps = QMaps.maps;
    //SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

    return
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(maps)
    .where(maps.id.eq(this.mapId))
    .singleResult(maps.userId);
  }
  
  public ReviewItemsMarker()
  {
  	
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

  /**
   * Uploads an OSM changeset with items marked as reviewed to the services database and then parses
   * the contents of the changeset, marking items as reviewed based on custom hoot review tags it
   * contains
   *
   * @param markItemsReviewedRequest a request to mark items as reviewed which contains an
   * object describing the items to be reviewed, as well as an optional OSM xml changeset; the two
   * sets of data are not cross validated with each other in any way
   * @param markAll an option to mark all data for the map layer as reviewed; when true, the
   * reviewed items object is no needed and ignored if populated
   * @return an mark items as reviewed response
   * @throws Exception
   */
  public MarkItemsReviewedResponse markItemsReviewed(
    final MarkItemsReviewedRequest markItemsReviewedRequest, final boolean markAll)
    throws Exception
  {
    MarkItemsReviewedResponse markItemsReviewedResponse = new MarkItemsReviewedResponse();
    markItemsReviewedResponse.setMapId(mapId);

    Document changesetUploadResponse = null;
    long changesetId = -1;
    if (StringUtils.trimToNull(markItemsReviewedRequest.getReviewedItemsChangeset()) != null)
    {
      boolean changesetHasElements = false;
      final Document changesetDiffDoc =
        XmlDocumentBuilder.parse(markItemsReviewedRequest.getReviewedItemsChangeset());
      changesetHasElements = ChangesetUploadXmlValidator.changesetHasElements(changesetDiffDoc);
      if (changesetHasElements)
      {
        changesetId =
          Changeset.createChangeset(
            Changeset.getChangesetCreateDoc("marking items reviewed for map ID: " + mapId),
            mapId,
            userId,
            conn);
        //TODO: There really needs to also be a check in here that makes sure every element tag
        //already has a changeset attribute (or iterate through the element tag DOM attributes.
        //For now, just assuming that iD has already added the changeset attributes.  Whether the
        //value is empty or not doesn't matter, since it will be overwritten here.
        markItemsReviewedRequest.setReviewedItemsChangeset(
          markItemsReviewedRequest.getReviewedItemsChangeset()
            .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\"")
            .replaceAll("changeset=\"-*\\d+\"", "changeset=\"" + changesetId + "\""));
        //log.debug(markItemsReviewedRequest.getReviewedItemsChangeset());
        
        //the changeset upload process will catch any elements in the changeset xml which are out
        //of sync with the element versions in the OSM element tables, by design
        changesetUploadResponse =
          (new ChangesetDbWriter(conn)).write(mapId,
            changesetId, markItemsReviewedRequest.getReviewedItemsChangeset());
        Changeset.closeChangeset(mapId, changesetId, conn);

        markItemsReviewedResponse.setChangesetUploadResponse(
          XmlDocumentBuilder.toString(changesetUploadResponse));
      }
    }

    //mark all items as reviewed in review_items; record the changeset ID and other review
    //details
    if (markAll)
    {
      markItemsReviewedRequest.setReviewedItems(
        ReviewUtils.getReviewedItemsCollectionForAllRecords(mapId, conn));
    }
    final int numItemsMarkedReviewed =
      (new ReviewedItemsWriter(conn, mapId, changesetId)).writeReviewedItems(
        markItemsReviewedRequest.getReviewedItems());

    //this will still be = -1 at this point if nothing was in the changeset input parameter or it
    //was invalid
    markItemsReviewedResponse.setChangesetId(changesetId);
    //this will null at this point if nothing was in the changeset input parameter or it was invalid
    markItemsReviewedResponse.setNumItemsMarkedReviewed(numItemsMarkedReviewed);
    return markItemsReviewedResponse;
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
  public long getAvailableReviewCnt() throws Exception
  {
    long nRet = 0;
    

    java.util.Date date= new java.util.Date();
    
    long waittime = date.getTime() - LOCK_TIME;
    Timestamp compareTime = new Timestamp(waittime);
    
    SQLQuery q = _getAvailableReviewQuery(compareTime, -1, true);
    
    nRet = q.count();
    
    return nRet;
  }
  
  
  public long getLockedReviewCnt() throws Exception
  {
    
    java.util.Date date= new java.util.Date();
    
    long waittime = date.getTime() - LOCK_TIME;
    Timestamp compareTime = new Timestamp(waittime);
    
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    .from(rm)
    .where(rm.mapId.eq(mapId).and(rm.reviewStatus.eq(DbUtils.review_status_enum.unreviewed)
        .and(rm.lastAccessed.goe(compareTime))));
    return q.count();
  }
  
  
  
  public long getTotalReviewCnt() throws Exception
  {   
    QReviewItems rm = QReviewItems.reviewItems;
    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    .from(rm)
    .where(rm.mapId.eq(mapId));
    return q.count();
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
      .orderBy(rm.reviewId.asc());
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
  

  protected BoundingBox _getRelationBbox(final long id) throws Exception
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
    List<Tuple> bounds = q.list(cn.latitude.max(), cn.latitude.min(), 
        cn.longitude.max(), cn.longitude.min());
    
    JSONObject ret = new JSONObject();
    Double maxLat = bounds.get(0).get(0, Double.class);
    Double minLat = bounds.get(0).get(1, Double.class);
    Double maxLon = bounds.get(0).get(2, Double.class);
    Double minLon = bounds.get(0).get(3, Double.class);
    
    BoundingBox bbox = new BoundingBox(minLon, minLat, maxLon, maxLat);
    return bbox;
  }
  
  protected BoundingBox _getWayBbox(final long id) throws Exception
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
    List<Tuple> bounds = q.list(cn.latitude.max(), cn.latitude.min(), 
        cn.longitude.max(), cn.longitude.min());
    
    JSONObject ret = new JSONObject();
    Double maxLat = bounds.get(0).get(0, Double.class);
    Double minLat = bounds.get(0).get(1, Double.class);
    Double maxLon = bounds.get(0).get(2, Double.class);
    Double minLon = bounds.get(0).get(3, Double.class);
    
    BoundingBox bbox = new BoundingBox(minLon, minLat, maxLon, maxLat);
    return bbox;
  }
  
  protected BoundingBox _getNodeCoord(final long id) throws Exception
  {

    QCurrentNodes cn = QCurrentNodes.currentNodes;
    QCurrentWayNodes cwn = QCurrentWayNodes.currentWayNodes;

    
    SQLQuery q = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
    .from(cn)
    .where(
        cn.id.eq(id)
        );
    List<Tuple> bounds = q.list(cn.latitude, cn.longitude);
    
    JSONObject ret = new JSONObject();
    Double lat = bounds.get(0).get(0, Double.class);
    Double lon = bounds.get(0).get(1, Double.class);

    BoundingBox bbox = new BoundingBox(lon, lat, lon, lat);
    return bbox;
  }
  
  public JSONObject getAvaiableReviewItem(final long offsetReviewId, final boolean isForward) throws Exception
  {
    JSONObject nextItem = new JSONObject();
    QReviewItems rm = QReviewItems.reviewItems;
    java.util.Date date= new java.util.Date();
    Timestamp now = new Timestamp(date.getTime());
    Timestamp past = new Timestamp(date.getTime() - ReviewItemsMarker.LOCK_TIME);
    
    long waittime = date.getTime() - LOCK_TIME;
    Timestamp compareTime = new Timestamp(waittime);
    
    ReviewableItem ri = new ReviewableItem();
    
    List<Tuple> firstReviewables = _getAvailableReviewQuery(compareTime, offsetReviewId, true).limit(1)
        .list(rm.reviewId, rm.reviewableItemId, rm.reviewAgainstItemId);
    
    List<Tuple> lastReviewables = _getAvailableReviewQuery(compareTime, offsetReviewId, false).limit(1)
        .list(rm.reviewId, rm.reviewableItemId, rm.reviewAgainstItemId);
    
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
          nextAvailableReviewItem = firstReviewableTuple;
        }
        else
        {
          if(isForward)
          {
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
          }
          else // is backward
          {
            // first in backward so get the last available
            if(firstReviewId == offsetReviewId)
            {
              nextAvailableReviewItem = lastReviewableTuple;
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
                  nextAvailableReviewItem = firstPotential;
                }
                else
                {
                  // This should not happen...
                  nextAvailableReviewItem = null;
                }
              }
            }
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
    
    
    
    nextItem.put("status", "failed");
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
              
              
              
              Tuple mapping = reviewElemMappings.get(0);
              //nextItem.put("reviewid", nextReviewId);
              ri.setReviewId(nextReviewId);
              //nextItem.put("elementid", mapping.get(em.elementId));
              ri.setUuid(mapping.get(em.elementId));
              
              final long osmId = mapping.get(em.osmElementId);
              //nextItem.put("osmelementid", osmId);
              ri.setId(osmId);
              
              final DbUtils.nwr_enum osmType = (DbUtils.nwr_enum)mapping.get(em.osmElementType);
              //nextItem.put("osmelementtype", osmType);
              ri.setType(osmType.toString());
              
              if(osmType == DbUtils.nwr_enum.node)
              {
                BoundingBox bbox = this._getNodeCoord(osmId);
                ri.setDisplayBounds(bbox.toServicesString());
                //nextItem.put("geom", bbox.toServicesString());
              }
              else if(osmType == DbUtils.nwr_enum.way)
              {
                BoundingBox bbox = this._getWayBbox(osmId);
                //nextItem.put("geom", bbox);
                ri.setDisplayBounds(bbox.toServicesString());
              }
              else if(osmType == DbUtils.nwr_enum.relation)
              {
                BoundingBox bbox = this._getRelationBbox(osmId);
                //nextItem.put("geom", bbox);
                ri.setDisplayBounds(bbox.toServicesString());
              }
              
              
              /// against
              Tuple againstMapping = reviewAgainstElemMappings.get(0);
              
              ReviewAgainstItem rai = new ReviewAgainstItem();
                            
              //nextItem.put("againstelementid", againstMapping.get(em.elementId));
              String againstUUID = againstMapping.get(em.elementId);
              rai.setUuid(againstUUID);
              ri.setAgainstList(againstUUID);
              
              final long osmAgId = againstMapping.get(em.osmElementId);
              //nextItem.put("againstosmelementid", osmAgId);
              rai.setId(osmAgId);
              
              final DbUtils.nwr_enum osmAgType = (DbUtils.nwr_enum)againstMapping.get(em.osmElementType);
              //nextItem.put("againstosmelementtype", againstMapping.get(em.osmElementType));
              rai.setType(osmAgType.toString());
              
              
              if(osmAgType == DbUtils.nwr_enum.node)
              {
                BoundingBox bbox = this._getNodeCoord(osmAgId);
                //nextItem.put("againstgeom", coord);
                rai.setDisplayBounds(bbox.toServicesString());
              }
              else if(osmAgType == DbUtils.nwr_enum.way)
              {
                BoundingBox bbox = this._getWayBbox(osmAgId);
                //nextItem.put("againstgeom", bbox);
                rai.setDisplayBounds(bbox.toServicesString());
              }
              else if(osmAgType == DbUtils.nwr_enum.relation)
              {
                BoundingBox bbox = this._getRelationBbox(osmAgId);
                //nextItem.put("againstgeom", bbox);
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

              nextItem.put("status", "success");
              nextItem.put("reviewItem", ri);
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

 

}
