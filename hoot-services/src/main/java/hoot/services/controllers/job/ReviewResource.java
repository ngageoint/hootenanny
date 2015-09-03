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
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.ArrayList;
import java.util.Arrays;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db2.QMaps;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.models.review.MarkItemsReviewedRequest;
import hoot.services.models.review.MarkItemsReviewedResponse;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableItemsResponse;
import hoot.services.models.review.ReviewableItemsStatistics;
import hoot.services.readers.review.ReviewableItemRetriever;
import hoot.services.readers.review.ReviewableItemsStatisticsCalculator;
import hoot.services.review.DisplayBoundsCalculator;
import hoot.services.review.ReviewItemsMarker;
import hoot.services.review.ReviewItemsPreparer;
import hoot.services.review.ReviewUtils;
import hoot.services.validators.review.ReviewInputParamsValidator;
import hoot.services.writers.review.ReviewableItemsResponseWriter;

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
//import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.support.DefaultTransactionDefinition;

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

  //These parameters are passed in by the unit tests only.  With better unit test coverage,
  //these params could probably go away.
  public static long testDelayMilliseconds = 0;
  public static boolean simulateFailure = false;
  //TODO: see #6270
  public static String reviewRecordWriter = "reviewPrepareDbWriter2";

  private ClassPathXmlApplicationContext appContext;
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

      log.debug("Initializing database connection...");


      stats =
        (new ReviewableItemsStatisticsCalculator(conn, mapId, true)).getStatistics(
          reviewScoreThresholdMinimum, geospatialBoundsObj);
      
      ReviewItemsMarker marker = new ReviewItemsMarker(conn, mapId);

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

	/**
	 * <NAME>Conflated Data Review Service Retrieve Items to Review</NAME>
	 * <DESCRIPTION>
	 * 	Once the conflated data has been prepared for review, clients may call this to get one or more items to present to the user for reviewing.
	 *  In many cases, clients might only request a single item for review with each call (default setting), but the option to retrieve more than one at
	 *  a time is being made available, in case it ends up being necessary. The response returns a set of reviewable items containing
	 *  an OSM element ID and type, as well as a suggested geospatial bounds for each item it is to be reviewed against
	 *  (smallest bounds that encompasses the reviewable item and the item it is reviewed against, plus some small buffer).
	 *  The response returns OSM data in XML format inside the JSON response, since the iD editor already knows how to parse OSM XML for display.
	 *  The IDs of the response items correspond to the OSM IDs of the elements returned by a Hootenanny map query request against the services database.
	 *  it is not intended that element unique IDs be shown to end users, however, the client will be responsible for updating the unique ID contained in
	 *  the OSM review tags for elements split into new elements during the review process, since this operation is not feasible to perform in the server.
	 *  IMPORTANT: If an reviewable item has its associated OSM element deleted from the services database,
	 *  that item will not be returned in this query, despite the fact it was never reviewed. For now, it is the responsibility of
	 *  clients to not delete reviewable elements until they have been reviewed.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * 	<mapId>
	 * 	string; required; ID string or unique name of the map associated with the reviewable conflated data
	 * 	</mapId>
	 *  <numItems>
	 *  integer; optional; number of review items to retrieve; optional; defaults to 1; max = 5000 (configurable)
	 *  </numItems>
	 *  <highestReviewScoreFirst>
	 *   boolean; optional; when specified as true, from the available review items returns the ones with the highest score first;
	 *    optional; default = true
	 *  </highestReviewScoreFirst>
	 *  <reviewScoreThresholdMinimum>
	 *  double; optional; items with a review score lower than this threshold will not be returned; defaults to 0.5
	 *  </reviewScoreThresholdMinimum>
	 *  <geospatialBounds>
	 *   string (WPS GET, Jersey); OWS BoundingBox (WPS POST); optional; OSM geospatial bounds
	 *   to search for reviewable data within; the format is minLon,minLat,maxLon,maxLat; optional; default bounds =
	 *   world; only coordinate system EPSG:4326 will be supported initially
	 *  </geospatialBounds>
	 *  <displayBoundsZoomAdjust>
	 *   double; optional; a constant amount in degrees by which the display bounds
	 *    returned for each review item pair will be adjusted; negative values will zoom the display bounds out by a constant
	 *    amount for all review item pairs; positive values will zoom it in; valid range: -1.0 to 1.0; defaults to -0.0015
	 *  </displayBoundsZoomAdjust>
	 *  <boundsDisplayMethod>
	 *  string; optional; Specifies the method by which the display bounds for each review item pair is calculated.
	 *  Valid values are: reviewableItemOnly, reviewAgainstItemOnly, and     reviewableAndReviewAgainstItemCombined; defaults
	 *  to reviewableAndReviewAgainstItemCombined; This can be useful for debugging purposes.
	 *  </boundsDisplayMethod>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	a set of reviewable items
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review?mapId=1&numItems=2&highestReviewScoreFirst=true&
	 * reviewScoreThresholdMinimum=0.8&geospatialBounds=-77.09655761718749,38.89958342598271,-77.09106445312499,
	 * 38.90385833966776&displayBoundsZoomAdjust=-0.0015&boundsDisplayMethod=reviewableAndReviewAgainstItemCombined</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 * {
	 *   "mapId": 1,
	 *   "numItemsRequested": 2,
	 *   "numItemsReturned": 2,
	 *   "reviewScoreThresholdMinimum": 0.8,
	 *   "highestReviewScoreFirst": "true",
	 *   "geospatialBounds": "-77.09655761718749,38.89958342598271,-77.09106445312499,38.90385833966776",
	 *   "coordSys": "EPSG:4326",
	 *   "reviewableItems":
	 *   [
	 *     {
	 *       "id": 2402,
	 *       "type": "way",
	 *       "displayBounds": "-77.09655761718749,38.89958342598271,-77.09106445312499,38.90385833966776",
	 *       "itemToReviewAgainst":
	 *       {
	 *         "id": 2403,
	 *         "type": "way"
	 *       }
	 *     },
	 *     {
	 *       "id": 2405,
	 *       "type": "way",
	 *       "displayBounds": "-77.09655761718749,38.89958342598271,-77.09106445312499,38.90385833966776",
	 *       "itemToReviewAgainst":
	 *       {
	 *         "id": 2406,
	 *         "type": "way"
	 *       }
	 *     }
	 *   ]
	 * }
	 * </OUTPUT>
	 * </EXAMPLE>
   *
   * Retrieves reviewable items for the given map
   *
   * @param mapId ID of the map for which reviewable items should be retrieved
   * @param numItems the number of reviewable items that should be returned
   * @param highestReviewScoreFirst if true; items will be returned sorted by descending review
   * score
   * @param reviewScoreThresholdMinimum for the reviewable items returned, the minimum review
   * score the items should have
   * @param geospatialBounds for the reviewable items returned, the geospatial bounding box the
   * items should reside in
   * @return a set of reviewable items
   * @throws Exception
   */
  @GET
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewableItemsResponse getReviewableItems(
    @QueryParam("mapId")
    String mapId,
    @DefaultValue("1")
    @QueryParam("numItems")
    int numItems,
    @DefaultValue("true")
    @QueryParam("highestReviewScoreFirst")
    boolean highestReviewScoreFirst,
    @DefaultValue("0.5")
    @QueryParam("reviewScoreThresholdMinimum")
    double reviewScoreThresholdMinimum,
    @DefaultValue("-180,-90,180,90")
    @QueryParam("geospatialBounds")
    String geospatialBounds,
    @DefaultValue("-0.0015")
    @QueryParam("displayBoundsZoomAdjust")
    double displayBoundsZoomAdjust,
    @DefaultValue("reviewableAndReviewAgainstItemCombined")
    @QueryParam("boundsDisplayMethod")
    String boundsDisplayMethod)
    throws Exception
  {
  	Connection conn = DbUtils.createConnection();
    final String errorMessageStart = "retrieving reviewable items";
    ReviewableItemsResponse reviewableItemsResponse = null;
    try
    {
      Map<String, Object> inputParams = new HashMap<String, Object>();
      inputParams.put("mapId", mapId);
      inputParams.put("numItems", numItems);
      inputParams.put("highestReviewScoreFirst", highestReviewScoreFirst);
      inputParams.put("reviewScoreThresholdMinimum", reviewScoreThresholdMinimum);
      inputParams.put("geospatialBounds", geospatialBounds);
      inputParams.put("displayBoundsZoomAdjust", displayBoundsZoomAdjust);
      inputParams.put("boundsDisplayMethod", boundsDisplayMethod);
      ReviewInputParamsValidator inputParamsValidator = new ReviewInputParamsValidator(inputParams);
      mapId =
        (String)inputParamsValidator.validateAndParseInputParam("mapId", "", null, null, false, null);
      numItems =
        (Integer)inputParamsValidator.validateAndParseInputParam(
          "numItems",
          new Integer(0),
          new Integer(1),
          Integer.parseInt(
            HootProperties.getInstance().getProperty(
              "reviewGetMaxReviewSize", HootProperties.getDefault("reviewGetMaxReviewSize"))),
          true,
          Integer.parseInt(
            HootProperties.getInstance().getProperty(
              "reviewGetNumItemsDefault", HootProperties.getDefault("reviewGetNumItemsDefault"))));
      highestReviewScoreFirst =
        (Boolean)inputParamsValidator.validateAndParseInputParam(
          "highestReviewScoreFirst",
          false,
          null,
          null,
          true,
          Boolean.parseBoolean(
            HootProperties.getInstance().getProperty(
              "reviewGetHighestReviewScoreFirstDefault",
              HootProperties.getDefault("reviewGetHighestReviewScoreFirstDefault"))));
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
      displayBoundsZoomAdjust =
        (Double)inputParamsValidator.validateAndParseInputParam(
          "displayBoundsZoomAdjust",
          new Double(0.0),
          //arbitrarily limiting it to 1 degree
          new Double(-1.0),
          new Double(1.0),
          true,
          Double.parseDouble(
            HootProperties.getInstance().getProperty(
              "reviewDisplayBoundsZoomAdjustDefault",
              HootProperties.getDefault("reviewDisplayBoundsZoomAdjustDefault"))));
      //TODO: this isn't validating the validity of the actual enum value yet; need better error
      //checking here
      boundsDisplayMethod =
        (String)inputParamsValidator.validateAndParseInputParam(
          "boundsDisplayMethod",
          "",
          null,
          null,
          true,
          HootProperties.getInstance().getProperty(
            "reviewDisplayBoundsMethod",
            HootProperties.getDefault("reviewDisplayBoundsMethod")));
      final DisplayBoundsCalculator.DisplayMethod boundsDisplayMethodEnumVal =
        DisplayBoundsCalculator.DisplayMethod.valueOf(boundsDisplayMethod);

      log.debug("Initializing database connection...");

      //query for review items
      ReviewableItemRetriever itemRetriever = new ReviewableItemRetriever(conn, mapId);
      final List<ReviewableItem> reviewItems =
        itemRetriever.getReviewItems(
          numItems, highestReviewScoreFirst, reviewScoreThresholdMinimum, geospatialBoundsObj,
          displayBoundsZoomAdjust, boundsDisplayMethodEnumVal);
      assert(reviewItems.size() <= numItems);

      //write the items out to the response
      //besides string output, deegree only supports XML and binary types, so not returning as
      //application/json
      reviewableItemsResponse =
        (new ReviewableItemsResponseWriter()).writeResponse(
          reviewItems, itemRetriever.getMapId(), numItems, highestReviewScoreFirst,
          reviewScoreThresholdMinimum,  geospatialBoundsObj);
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

    return reviewableItemsResponse;
  }

	/**
	 * <NAME>Conflated Data Review Service Mark Items as Reviewed</NAME>
	 * <DESCRIPTION>
	 * 	After editing reviewable items, this method is called to mark the items as having been reviewed.
	 * The inputs to the service method are either a JSON structure - which describes the status of the review for
	 * each reviewed item - or when setting a boolean true to mark all items as reviewed the structure is not required.
	 * Also optionally for convenience sake, a OSM XML changeset may be included in the request to upload a
	 * changeset in the same call which marks data as reviewed. If a changeset is uploaded, the service
	 * automatically creates and closes the changeset that stores the uploaded data. The response from the server contains
	 * the outcome of the changeset upload and the changeset ID created (if a changeset was uploaded) as well as the number
	 * of submitted items that were actually marked as reviewed. Clients can compare this number to the number of
	 *  items sent for review, and if they are not equal, further troubleshooting should occur.
	 *  For each item in the reviewed items JSON, the service will:
	 *  (1) mark the reviewable item as reviewed, so that it will not be returned for review again;
	 *  (2) append the UUID of the item the reviewed item was reviewed against to its "uuid" OSM tag;
	 *   (3) remove the UUID of the item reviewed against from the "hoot:review:uuid" OSM tag of the reviewed item; and,
	 *   (4) remove all OSM review tags from the reviewed item, if it no longer contains items to be reviewed against.
	 *   The caller is responsible for doing the following things, as the service will not automatically do them:
	 *   (1) Adding the "changeset" XML attribute to the elements in the XML changeset being uploaded, as is required by
	 *    the standard OSM changeset upload service. The changeset ID attribute value may either be blank or populated with a number,
	 *     however, the changeset ID will be overwritten with the ID of the changeset created by the service method execution.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * 	<mapId>
	 * 	string; required; ID string or unique name of the map associated with the reviewable conflated data
	 * 	</mapId>
	 *  <markAll>
	 *  boolean; optional; defaults to false; indicates whether all items belonging to the map should be marked as reviewed
	 *  </markAll>
	 *  <markItemsReviewedRequest>
	 *   JSON object; sent in request body payload; required; object is made up of:
	 *   reviewedItems - JSON object; required if markAll is set to false; optional otherwise; lists the items which should be marked as reviewed
	 *   reviewedItemsChangeset - XML string; optional; OSM changeset XML
	 *  </markItemsReviewedRequest>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	A number string to show how many items were marked as reviewed as a result of the service call.
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review?mapId=1&markAll=false</URL>
	 * 	<REQUEST_TYPE>PUT</REQUEST_TYPE>
	 * 	<INPUT>
	 * {
	 *   "reviewedItems":
	 *   [
	 *     {
	 *       "id": 2402,
	 *       "type": "way",
	 *       "reviewedAgainstId": 2403,
	 *       "reviewedAgainstType": "way",
	 *     },
	 *     {
	 *       "id": 2404,
	 *       "type": "way",
	 *       "reviewedAgainstId": 2405,
	 *       "reviewedAgainstType": "way",
	 *     },
	 *   ]
	 * }
	 *	</INPUT>
	 * <OUTPUT>
	 * 2
	 * </OUTPUT>
	 * </EXAMPLE>
   *
   * Marks a set of reviewable items as reviewed and updates the tags of their corresponding OSM
   * elements
   *
   * @param reviewItemsChangeset an OSM changeset to be uploaded into the services database
   * @param mapId ID of the map for which items are being marked as reviewed
   * @return the number of items marked as reviewed
   * @throws Exception
   */
  @PUT
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public MarkItemsReviewedResponse markItemsReviewed(
    MarkItemsReviewedRequest markItemsReviewedRequest,
    @QueryParam("mapId")
    String mapId,
    @DefaultValue("false")
    @QueryParam("markAll")
    boolean markAll)
    throws Exception
  {
    Connection conn = DbUtils.createConnection();
    final String errorMessageStart = "marking items as reviewed";
    MarkItemsReviewedResponse markItemsReviewedResponse = null;
    try
    {
      log.debug("markItemsReviewedRequest: " + markItemsReviewedRequest.toString());

      Map<String, Object> inputParams = new HashMap<String, Object>();
      inputParams.put("mapId", mapId);
      inputParams.put("markAll", markAll);
      ReviewInputParamsValidator inputParamsValidator = new ReviewInputParamsValidator(inputParams);
      mapId =
        (String)inputParamsValidator.validateAndParseInputParam("mapId", "", null, null, false, null);
      markAll =
        (Boolean)inputParamsValidator.validateAndParseInputParam(
          "markAll", false, null, null, true, false);
      if (!markAll &&
          (markItemsReviewedRequest.getReviewedItems() == null ||
           markItemsReviewedRequest.getReviewedItems().getReviewedItems() == null ||
           markItemsReviewedRequest.getReviewedItems().getReviewedItems().length == 0))
      {
        throw new Exception(
          "Invalid input parameter: markAll set to false and " +
          "markItemsReviewedRequest.reviewedItems empty.");
      }

      log.debug("Initializing database connection...");

      log.debug("Intializing transaction...");
      TransactionStatus transactionStatus =
        transactionManager.getTransaction(
          new DefaultTransactionDefinition(TransactionDefinition.PROPAGATION_REQUIRED));
      conn.setAutoCommit(false);

      try
      {
        markItemsReviewedResponse =
          (new ReviewItemsMarker(conn, mapId)).markItemsReviewed(markItemsReviewedRequest, markAll);
      }
      catch (Exception e)
      {
        log.error(
          "Rolling back database transaction for ReviewResource::markItemsAsReviewed" + 
          e.getMessage());
        transactionManager.rollback(transactionStatus);
        conn.rollback();
        throw e;
      }

      log.debug("Committing ReviewResource::markItemsAsReviewed. database transaction...");
      transactionManager.commit(transactionStatus);
      conn.commit();
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

    //TODO: MarkItemsReviewedResponse toString() not working
//    if (markItemsReviewedResponse != null)
//    {
//      log.debug("Returning mark items reviewed response: " +
//        StringUtils.abbreviate(markItemsReviewedResponse.toString(), 100) + " ...");
//    }

    return markItemsReviewedResponse;
  }
  
  

  
  @PUT
  @Path("/updatestatus")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public JSONObject updateStatus(
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
    	Timestamp now = new Timestamp(date.getTime());
    	(new ReviewItemsMarker(conn, mapId)).updateReviewLastAccessTime(reviewId, now, reviewAgainst);

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
    markItemsReviewedResponse.put("locktime", "" + ReviewItemsMarker.LOCK_TIME);
    return markItemsReviewedResponse;
  }
  
  
  @PUT
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
    	Timestamp past = new Timestamp(date.getTime() - ReviewItemsMarker.LOCK_TIME);
    	(new ReviewItemsMarker(conn, mapId)).updateReviewLastAccessTime(reviewId, past, reviewAgainst);

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
  }
  
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
    
     
      ReviewItemsMarker marker = new ReviewItemsMarker(conn, mapId);
      nextReviewableResponse = marker.getAvaiableReviewItem(offset, isForward);
      nextReviewableResponse.put("locktime", ReviewItemsMarker.LOCK_TIME);
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
  
  @GET
  @Path("/lockcount")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public JSONObject getReviewLockCount(
    @QueryParam("mapId")
    String mapId
  )
    throws Exception
  {

    Connection conn = DbUtils.createConnection();
    final String errorMessageStart = "getting review lock count";
    JSONObject ret = new JSONObject();
    long lockcnt = 0;
    try
    {    	
    	ReviewItemsMarker marker = new ReviewItemsMarker(conn, mapId);
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

}
