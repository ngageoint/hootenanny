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
package hoot.services.controllers.job;

import java.sql.Connection;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db2.ElementIdMappings;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QMaps;
import hoot.services.db2.QReviewItems;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.models.review.ReviewReferences;
import hoot.services.models.review.ReviewableItemsStatistics;
import hoot.services.readers.review.ReviewableItemsStatisticsCalculator;
import hoot.services.review.ReviewItemsUpdater;
import hoot.services.review.ReviewItemsPreparer;
import hoot.services.review.ReviewUtils;
import hoot.services.validators.review.ReviewInputParamsValidator;

import javax.ws.rs.Consumes;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;

import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;

import com.mysema.query.sql.SQLQuery;

/**
 * Non-WPS service endpoint for the conflated data review process
 *
 * @todo Unfortunately, not having these default values as attributes in the methods makes
  validation impossible, but having them as attributes renders their config values useless.
  Need to come up with a better way to handle default values.  Is there some way to populate the
  attribute values directly from a file?
 */
@Path("/review")
public class ReviewResource
{
  private static final Logger log = LoggerFactory.getLogger(ReviewResource.class);
  
  private static final QReviewItems reviewItems = QReviewItems.reviewItems;
  private static final QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;

  //These parameters are passed in by the unit tests only.  With better unit test coverage,
  //these params could probably go away.
  public static long testDelayMilliseconds = 0;
  public static boolean simulateFailure = false;
  //TODO: see #6270
  public static String reviewRecordWriter = "reviewPrepareDbWriter2";

  private ClassPathXmlApplicationContext appContext;
  @SuppressWarnings("unused")
  private PlatformTransactionManager transactionManager;

  public ReviewResource() throws Exception
  {
    log.debug("Reading application settings...");
    appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
    log.debug("Initializing transaction manager...");
    transactionManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);
  }

	/**
	 * <NAME>Conflated Data Review Service Prepare Items for Review</NAME>
	 * <DESCRIPTION>
	 * Prepares conflated data for review for the specified map ID.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * 	<mapId>
	 * 	string; required; ID string or unique name of the map associated with the reviewable conflated data
	 * 	</mapId>
	 *  <overwriteExistingData>
	 *  boolean; optional; if true, will overwrite any data for the map that has been previously prepared for review;
	 *  if false, will cause an empty string to be returned for the job ID if the map has data that has already
	 *  been prepared for review; in the case that a prepare job is currently running for the specified map,
	 *   this parameter will be ignored; defaults to false
	 *  </overwriteExistingData>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * a job ID for tracking the prepare job
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review?mapId=1&overwriteExistingData=true</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>
	 * </INPUT>
	 * <OUTPUT>38400000-8cf0-11bd-b23e-10b96e4ef00d</OUTPUT>
	 * </EXAMPLE>
  *
  *
  * @param mapId ID of the map to prepare the review data for
  * @param overwriteExistingData if true, will overwrite any existing review data for the given map
  * @return a job ID for tracking the prepare job
  * @throws Exception
  */
  @POST
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.TEXT_PLAIN)
  public String prepareItemsForReview(
    @QueryParam("mapId")
    String mapId,
    @DefaultValue("false")
    @QueryParam("overwriteExistingData")
    boolean overwriteExistingData)
    throws Exception
  {
    String jobId = null;
    Connection conn = DbUtils.createConnection();
    final String errorMessageStart = "preparing items for review";
    try
    {
      Map<String, Object> inputParams = new HashMap<String, Object>();
      inputParams.put("mapId", mapId);
      inputParams.put("overwriteExistingData", overwriteExistingData);
      ReviewInputParamsValidator inputParamsValidator = new ReviewInputParamsValidator(inputParams);
      mapId =
        (String)inputParamsValidator.validateAndParseInputParam("mapId", "", null, null, false, null);
      overwriteExistingData =
        (Boolean)inputParamsValidator.validateAndParseInputParam(
          "overwriteExistingData",
          false,
          null,
          null,
          true,
          Boolean.parseBoolean(
            HootProperties.getInstance().getProperty(
              "reviewPrepareOverwriteExistingDataDefault",
              HootProperties.getDefault("reviewPrepareOverwriteExistingDataDefault"))));

      log.debug("Initializing database connection...");

      jobId =
        (new ReviewItemsPreparer(
           conn, testDelayMilliseconds, simulateFailure, mapId, reviewRecordWriter))
         .prepare(overwriteExistingData);
    }
    catch (Exception e)
    {
      ReviewUtils.handleError(e, errorMessageStart, false);
    }
    finally
    {
      try
      {
        DbUtils.closeConnection(conn);
      }
      catch (Exception e)
      {
        ReviewUtils.handleError(e, errorMessageStart, false);
      }
    }

    return jobId;
  }

	/**
	 * <NAME>Conflated Data Review Service Retrieve Statistics for Reviewable Items</NAME>
	 * <DESCRIPTION>
	 * 	Retrieves statistics about the reviewable data for a given map.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * 	<mapId>
	 * 	string; required; ID string or unique name of the map associated with the reviewable conflated data
	 * 	</mapId>
	 *  <reviewScoreThresholdMinimum>
	 *   double; optional; items with a review score lower than this threshold will not be included in the returned statistics; defaults to 0.0
	 *  </reviewScoreThresholdMinimum>
	 *  <geospatialBounds>
	 *  string (WPS GET, Jersey); OWS BoundingBox (WPS POST); optional; OSM geospatial bounds
	 *  to search for reviewable and reviewed data within; the format is minLon,minLat,maxLon,maxLat;
	 *  optional; default bounds = world; only coordinate system EPSG:4326 will be supported initially
	 *  </geospatialBounds>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	a set of reviewable item statistics
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/statistics?mapId=1&reviewScoreThresholdMinimum=0.8&geospatialBounds=-77.09655761718749,38.89958342598271,-77.09106445312499,38.90385833966776</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 * {
	 *   "mapId": 1,
	 *   "numReviewableItems": 5,
	 *   "numReviewedItems": 2,
	 *   "numTotalItems": 10
	 * }
	 * </OUTPUT>
	 * </EXAMPLE>
   *
   *
   * @param mapId ID of the map to retrieve review statistics for
   * @param reviewScoreThresholdMinimum for the reviewable items used to calculate the review
   * statistics, the minimum review score the items should have
   * @param geospatialBounds for the reviewable items used to calculate the review
   * statistics, the geospatial bounding box the items should reside in
   * @return a set of reviewable item statistics
   * @throws Exception
   */
  @GET
  @Path("/statistics")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewableItemsStatistics getReviewableItemsStatistics(
    @QueryParam("mapId")
    String mapId,
    @DefaultValue("0.0")
    @QueryParam("reviewScoreThresholdMinimum")
    double reviewScoreThresholdMinimum,
    @DefaultValue("-180,-90,180,90")
    @QueryParam("geospatialBounds")
    String geospatialBounds)
    throws Exception
  {
  	log.debug("Initializing database connection...");
  	Connection conn = DbUtils.createConnection();
    final String errorMessageStart = "retrieving reviewable items statistics";
    ReviewableItemsStatistics stats = null;
    try
    {
      Map<String, Object> inputParams = new HashMap<String, Object>();
      inputParams.put("mapId", mapId);
      inputParams.put("reviewScoreThresholdMinimum", reviewScoreThresholdMinimum);
      inputParams.put("geospatialBounds", geospatialBounds);
      ReviewInputParamsValidator inputParamsValidator = new ReviewInputParamsValidator(inputParams);
      mapId =
        (String)inputParamsValidator.validateAndParseInputParam("mapId", "", null, null, false, null);
      reviewScoreThresholdMinimum =
        (Double)inputParamsValidator.validateAndParseInputParam(
          "reviewScoreThresholdMinimum",
          new Double(0.0),
          new Double(0.0),
          new Double(1.0),
          true,
          Double.parseDouble(
            HootProperties.getInstance().getProperty(
              "reviewGetReviewScoreThresholdMinimumDefault",
              HootProperties.getDefault("reviewGetReviewScoreThresholdMinimumDefault"))));
      final BoundingBox geospatialBoundsObj =
        (BoundingBox)inputParamsValidator.validateAndParseInputParam(
          "geospatialBounds",
          "",
          null,
          null,
          true,
          HootProperties.getInstance().getProperty(
            "reviewGetGeospatialBoundsDefault",
            HootProperties.getDefault("reviewGetGeospatialBoundsDefault")));

      stats =
        (new ReviewableItemsStatisticsCalculator(conn, mapId, true)).getStatistics(
          reviewScoreThresholdMinimum, geospatialBoundsObj);
      
      ReviewItemsUpdater marker = new ReviewItemsUpdater(conn, mapId);

    	long cnt = marker.getAvailableReviewCntQuery().count();
    	//nextItem.put("status", "noneavailable");
    	if(cnt == 0)
    	{
    		stats.setNumReviewableItems(0);
    	}
    }
    catch (Exception e)
    {
      //ReviewUtils.handleError(e, errorMessageStart, false);
    	try
    	{
	    	// Instead of throwing error we will just return empty stat
	    	QMaps maps = QMaps.maps;
	      final long mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapId, conn,
	      		maps, maps.id, maps.displayName);
	      
	    	stats = new ReviewableItemsStatistics();
	    	stats.setMapId(mapIdNum);
	    	stats.setNumReviewableItems(0);
	    	stats.setNumReviewedItems(0);
	    	stats.setNumTotalItems(0);
    	}
    	catch (Exception ee)
    	{
    		ReviewUtils.handleError(ee, errorMessageStart, false);
    	}
    }
    finally
    {
    	try
      {
        DbUtils.closeConnection(conn);
      }
      catch (Exception e)
      {
        ReviewUtils.handleError(e, errorMessageStart, false);
      }
    }

    return stats;
  }
  
  @SuppressWarnings("unchecked")
  @PUT
  @Path("/updatestatus")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public JSONObject updateStatus(
  	JSONObject updateReviewStatusRequest,
    @QueryParam("mapId")
    String mapId)
    throws Exception
  {
    Connection conn = DbUtils.createConnection();
    final String errorMessageStart = "marking items as reviewed";
    try
    {
    	String reviewId = updateReviewStatusRequest.get("reviewid").toString();
    	Object oAgainst = updateReviewStatusRequest.get("reviewagainstid");
    	String reviewAgainst = (oAgainst == null)? null : oAgainst.toString();

    	java.util.Date date= new java.util.Date();
    	Timestamp now = new Timestamp(date.getTime());
    	(new ReviewItemsUpdater(conn, mapId)).updateReviewLastAccessTime(reviewId, now, reviewAgainst);
    }
    catch (Exception e)
    {
      ReviewUtils.handleError(e, errorMessageStart, false);
    }
    finally
    {
    	try
      {
    		conn.setAutoCommit(true);
        DbUtils.closeConnection(conn);
      }
      catch (Exception e)
      {
        ReviewUtils.handleError(e, errorMessageStart, false);
      }
    }
    JSONObject updateReviewStatusResponse = new JSONObject();
    updateReviewStatusResponse.put("status", "ok");
    updateReviewStatusResponse.put("locktime", "" + ReviewItemsUpdater.LOCK_TIME);
    return updateReviewStatusResponse;
  }
  
  /*@PUT
  @Path("/resetstatus")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public JSONObject resetStatus(
  	JSONObject markItemsReviewedRequest,
    @QueryParam("mapId")
    String mapId)
    throws Exception
  {
    Connection conn = DbUtils.createConnection();
    final String errorMessageStart = "marking items as reviewed";
    
    try
    {
    	String reviewId = markItemsReviewedRequest.get("reviewid").toString();
    	
    	Object oAgainst = markItemsReviewedRequest.get("reviewagainstid");
    	String reviewAgainst = (oAgainst == null)? null : oAgainst.toString();

    	java.util.Date date= new java.util.Date();
    	Timestamp past = new Timestamp(date.getTime() - ReviewItemsUpdater.LOCK_TIME);
    	(new ReviewItemsUpdater(conn, mapId)).updateReviewLastAccessTime(reviewId, past, reviewAgainst);

    }
    catch (Exception e)
    {
      ReviewUtils.handleError(e, errorMessageStart, false);
    }
    finally
    {
    	try
      {
    		conn.setAutoCommit(true);
        DbUtils.closeConnection(conn);
      }
      catch (Exception e)
      {
        ReviewUtils.handleError(e, errorMessageStart, false);
      }
    }
    JSONObject markItemsReviewedResponse = new JSONObject();
    markItemsReviewedResponse.put("status", "ok");
    return markItemsReviewedResponse;
  }*/
  
  @SuppressWarnings("unchecked")
  @PUT
  @Path("/next")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public JSONObject getNextAvailableReviewAndLock(
    	JSONObject nextReviewItemRequest,
      @QueryParam("mapId")
      String mapId) throws Exception
  {
    Connection conn = DbUtils.createConnection();
    final String errorMessageStart = "marking items as reviewed";
    JSONObject nextReviewableResponse = new JSONObject();
    nextReviewableResponse.put("status", "none");
    try
    {
      int offset = -1;
      Object oReqOffset = nextReviewItemRequest.get("offset");
      
      if(oReqOffset != null)
      {
        offset = Integer.parseInt(oReqOffset.toString());
      }
      
      Object oDirection = nextReviewItemRequest.get("direction");
      
      boolean isForward = true;
      if(oDirection != null)
      {
        String direction = oDirection.toString();
        if(!direction.equals("forward"))
        {
          isForward = false;
        }
        
      }
    
      ReviewItemsUpdater marker = new ReviewItemsUpdater(conn, mapId);
      nextReviewableResponse = marker.getAvaiableReviewItem(offset, isForward);
      nextReviewableResponse.put("locktime", ReviewItemsUpdater.LOCK_TIME);
      long totalReviewableCnt = marker.getTotalReviewCntQuery().count();
      nextReviewableResponse.put("total", totalReviewableCnt);
      long reviewedCnt = marker.getReviewedReviewCnt();
      nextReviewableResponse.put("reviewedcnt", reviewedCnt);
      long lockedCnt = marker.getLockedReviewCntQuery().count();
      nextReviewableResponse.put("lockedcnt", lockedCnt);
    }
    catch (Exception e)
    {
      ReviewUtils.handleError(e, errorMessageStart, false);
    }
    finally
    {
      try
      {
        conn.setAutoCommit(true);
        DbUtils.closeConnection(conn);
      }
      catch (Exception e)
      {
        ReviewUtils.handleError(e, errorMessageStart, false);
      }
    }
    
    return nextReviewableResponse;
  }
  
  @SuppressWarnings("unchecked")
  @GET
  @Path("/lockcount")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public JSONObject getReviewLockCount(
    @QueryParam("mapId")
    String mapId)
    throws Exception
  {
    Connection conn = DbUtils.createConnection();
    final String errorMessageStart = "getting review lock count";
    JSONObject ret = new JSONObject();
    long lockcnt = 0;
    try
    {    	
    	ReviewItemsUpdater marker = new ReviewItemsUpdater(conn, mapId);
    	lockcnt = marker.getLockedReviewCntQuery().count();
    }
    catch (Exception e)
    {
      ReviewUtils.handleError(e, errorMessageStart, false);
    }
    finally
    {
    	try
      {
    		conn.setAutoCommit(true);
        DbUtils.closeConnection(conn);
      }
      catch (Exception e)
      {
        ReviewUtils.handleError(e, errorMessageStart, false);
      }
    }
    
    ret.put("count", lockcnt);
    return ret;
  }

  /**
   * Returns any review record references to the elements passed in including all references to any 
   * item the specified element still needs to be reviewed against, as well as any other item that 
   * needs to use the specified element to review against it.
   * 
   * @param mapId map owning the features whose ID's are passed in featureUniqueIds
   * @param elementId OSM ID of the element to be retrieved
   * @param elementType OSM type of the element to be retrieved
   * @return an array of review records
   * @throws Exception
   * @todo write unit test
   */
  @GET
  @Path("/refs")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewReferences getReviewReferences(
    @QueryParam("mapId")
    final long mapId,
    @QueryParam("elementId")
    final long elementId,
    @QueryParam("elementType")
    final String elementType)
    throws Exception
  {
  	Connection conn = DbUtils.createConnection();
  	ReviewReferences response = new ReviewReferences();
  	try
  	{
  		log.debug("mapId: " + mapId);
  		log.debug("elementId: " + elementId);
  		log.debug("elementType: " + elementType);
  		
  		final String elementUniqueId = 
  			new SQLQuery(conn, DbUtils.getConfiguration(mapId))
	        .from(elementIdMappings)
		      .where(
		    	  elementIdMappings.mapId.eq(mapId)
		    	  .and(elementIdMappings.osmElementId.eq(elementId))
		    	  .and(elementIdMappings.osmElementType.eq(Element.elementEnumForString(elementType))))
		      .singleResult(elementIdMappings.elementId);
  		log.debug("elementUniqueId: " + elementUniqueId);
  		
  		List<ReviewAgainstItem> reviewAgainstItems = new ArrayList<ReviewAgainstItem>();
  		//get all review against items that are referenced by the input elements; since
  		//the review table data breaks up one to many reviewable uuid to review against uuid
  		//relationships, we don't have to break up the review against uuid's list here
  		final List<String> reviewAgainstItemUniqueIds =
        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
  		    .from(reviewItems)
    		  .where(
    		  	reviewItems.reviewableItemId.eq(elementUniqueId)
    		  	  .and(reviewItems.mapId.eq(mapId)))
    		  .list(reviewItems.reviewAgainstItemId);
  		if (reviewAgainstItemUniqueIds.size() > 0)
  		{
  			final List<ElementIdMappings> reviewAgainstItemRecords =
	        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
	    		  .from(elementIdMappings)
	      		.where(
	      			elementIdMappings.elementId.in(reviewAgainstItemUniqueIds)
	      			  .and(elementIdMappings.mapId.eq(mapId)))
	      		.list(elementIdMappings);
	  		for (ElementIdMappings itemRecord : reviewAgainstItemRecords)
	  		{
	  			ReviewAgainstItem item = new ReviewAgainstItem();
	  			item.setId(itemRecord.getOsmElementId());
	  			item.setType(itemRecord.getOsmElementType().toString().toLowerCase());
	  			item.setUuid(itemRecord.getElementId());
	  			reviewAgainstItems.add(item);
	  		}
  		}
  		response.setReviewAgainstItems(reviewAgainstItems.toArray(new ReviewAgainstItem[]{}));
  		
  		List<ReviewAgainstItem> reviewableItems = new ArrayList<ReviewAgainstItem>();
      //add all items which reference the input features as a review against item
  		final List<String> reviewableItemUniqueIds =
        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
  		    .from(reviewItems)
    		  .where(
    		  	reviewItems.reviewAgainstItemId.eq(elementUniqueId)
    		  	  .and(reviewItems.mapId.eq(mapId)))
    		  .list(reviewItems.reviewableItemId);
  		if (reviewableItemUniqueIds.size() > 0)
  		{
  			final List<ElementIdMappings> reviewableItemRecords =
	        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
	    		  .from(elementIdMappings)
	      		.where(
	      			elementIdMappings.elementId.in(reviewableItemUniqueIds)
	      			  .and(elementIdMappings.mapId.eq(mapId)))
	      		.list(elementIdMappings);
	  		for (ElementIdMappings itemRecord : reviewableItemRecords)
	  		{
	  			//returning a review against item here, instead of a reviewable item, b/c the review against
	  			//item has all of the info that's needed to return the references, whereas ReviewableItem
	  			//has a lot of extra unneeded info...client won't know the different between the two anyway
	  			ReviewAgainstItem item = new ReviewAgainstItem();
	  			item.setId(itemRecord.getOsmElementId());
	  			item.setType(itemRecord.getOsmElementType().toString().toLowerCase());
	  			item.setUuid(itemRecord.getElementId());
	  			reviewableItems.add(item);
	  		}
  		}
  		response.setReviewableItems(reviewableItems.toArray(new ReviewAgainstItem[]{}));
  	}
  	catch (Exception e)
    {
      ReviewUtils.handleError(
      	e, 
      	"Unable to retrieve review references for input: " + "mapId: " + mapId + 
      	  ",elementId: " + elementId + ", elementType: " + elementType, 
      	false);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }
  	
  	log.debug("response : " + response.toString());
  	return response;
  }
}
