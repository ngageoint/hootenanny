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
package hoot.services.controllers.job;

import java.sql.Connection;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import hoot.services.HootProperties;
import hoot.services.controllers.osm.MapResource;
import hoot.services.db.DbUtils;
import hoot.services.db.postgres.PostgresUtils;
import hoot.services.db2.QMaps;
import hoot.services.db2.QReviewBookmarks;
import hoot.services.db2.ReviewBookmarks;
import hoot.services.geo.BoundingBox;
import hoot.services.models.review.ReviewBookmarkDelResponse;
import hoot.services.models.osm.ElementInfo;
import hoot.services.models.review.AllReviewableItems;
import hoot.services.models.review.ReviewRef;
import hoot.services.models.review.ReviewRefsRequest;
import hoot.services.models.review.ReviewResolverRequest;
import hoot.services.models.review.ReviewResolverResponse;
import hoot.services.models.review.ReviewRefsResponse;
import hoot.services.models.review.ReviewRefsResponses;
import hoot.services.models.review.ReviewBookmarkDelRequest;
import hoot.services.models.review.ReviewBookmarkSaveRequest;
import hoot.services.models.review.ReviewBookmarksGetResponse;
import hoot.services.models.review.ReviewBookmarksSaveResponse;
import hoot.services.models.review.ReviewBookmarksStatResponse;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableStatistics;
import hoot.services.readers.review.ReviewReferencesRetriever;
import hoot.services.readers.review.ReviewBookmarkRetriever;
import hoot.services.readers.review.ReviewableReader;
import hoot.services.review.ReviewUtils;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.writers.review.ReviewResolver;
import hoot.services.writers.review.ReviewBookmarksRemover;
import hoot.services.writers.review.ReviewBookmarksSaver;

import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.support.DefaultTransactionDefinition;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.OrderSpecifier;

/**
 * Service endpoint for the conflated data review process
 */
@Path("/review")
public class ReviewResource
{
  private static final Logger log = LoggerFactory.getLogger(ReviewResource.class);
  
  private QMaps maps = QMaps.maps;

  private ClassPathXmlApplicationContext appContext;
  private PlatformTransactionManager transactionManager;
  
	private static long MAX_RESULT_SIZE = 60000;
	static
	{
		try
		{
			String maxQuerySize = HootProperties.getProperty("maxQueryNodes"); 
			MAX_RESULT_SIZE = Long.parseLong(maxQuerySize);
		}
		catch(Exception ex)
		{
			log.error(ex.getMessage());
		}
	}

  public ReviewResource() throws Exception
  {
    appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
    transactionManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);
  }

  /**
   * <NAME>Resolve All Reviews</NAME>
	 * <DESCRIPTION>
	 * Resolves all reviews for a given map
   * 
   * Have to use a request object here, rather than a single map ID query param, since d3 can't
   * send plain text in a PUT statement.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <request>
	 *  a JSON request containing the map ID for the reviews to be resolved
	 * </request>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	a JSON response with the changeset ID used to resolve the reviews
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/resolveall?TODO</URL>
	 *  <REQUEST_TYPE>PUT</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 *  <OUTPUT>
	 *   TODO
	 *  </OUTPUT>
	 * </EXAMPLE>
   */
  @PUT
  @Path("/resolveall")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewResolverResponse resolveAllReviews(final ReviewResolverRequest request) 
    throws Exception
  {
  	log.debug("Setting all items reviewed for map with ID: " + request.getMapId() + "...");
  	
  	Connection conn = DbUtils.createConnection();
    TransactionStatus transactionStatus = 
      transactionManager.getTransaction(
        new DefaultTransactionDefinition(TransactionDefinition.PROPAGATION_REQUIRED));
    conn.setAutoCommit(false);
    
    final long mapIdNum = MapResource.validateMap(request.getMapId(), conn);
    assert(mapIdNum != -1);
    
  	long userId = -1;
  	try
  	{
  		log.debug(
	      "Retrieving user ID associated with map having ID: " + String.valueOf(request.getMapId()) + 
	      " ...");
	  	userId = 
	  		new SQLQuery(conn, DbUtils.getConfiguration())
	  	    .from(maps)
	  	    .where(maps.id.eq(mapIdNum))
	  	    .singleResult(maps.userId);
	    log.debug("Retrieved user ID: " + userId);
  	}
  	catch (Exception e)
    {
      ResourceErrorHandler.handleError(
        "Error locating user associated with map having ID: " + request.getMapId() +  " (" + 
          e.getMessage() + ")", 
        Status.BAD_REQUEST,
        log);
    }
  	assert(userId != -1);
    
  	long changesetId = -1;
  	try
  	{
  	  changesetId = (new ReviewResolver(conn)).setAllReviewsResolved(mapIdNum, userId);
  		
  		log.debug("Committing set all items reviewed transaction...");
      transactionManager.commit(transactionStatus);
      conn.commit();	
  	}
  	catch (Exception e)
    {
      transactionManager.rollback(transactionStatus);
      conn.rollback();
      ReviewUtils.handleError(
      	e, 
      	"Error setting all records to reviewed for map ID: " + request.getMapId(), 
      	false);
    }
    finally
    {
    	conn.setAutoCommit(true);
      DbUtils.closeConnection(conn);
    }
  	
  	log.debug(
  		"Set all items reviewed for map with ID: " + request.getMapId() + " using changesetId: " + 
  	  changesetId);
  	return new ReviewResolverResponse(changesetId);
  }
  
  /**
   * <NAME>Get Review References</NAME>
	 * <DESCRIPTION>
	 * Returns any review references to the elements associated with the ID's passed in
	 * 
	 * Technically, this should be a GET request, but since the size of the input could potentially
   * be large, making it a POST request to get past any size limit restrictions on GET requests.
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <request>
	 *  JSON request containing a collection of elements for which review references are to be 
	 *  retrieved
	 * </request>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	an array of review references in JSON; one set of references for each query element passed in;
   *  The returned ReviewRef object extends the ElementInfo object to add the associated review 
   *  relation id.
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/refs?TODO</URL>
	 *  <REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 *  <OUTPUT>
	 *   TODO
	 *  </OUTPUT>
	 * </EXAMPLE>
   */
  @POST
  @Path("/refs")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewRefsResponses getReviewReferences(
  	final ReviewRefsRequest request) 
  	throws Exception
  {
  	log.debug("Returning review references...");
  	
  	ReviewRefsResponses response = new ReviewRefsResponses();
  	Connection conn = DbUtils.createConnection();
  	try
  	{
  		ReviewReferencesRetriever refsRetriever = new ReviewReferencesRetriever(conn);
  		List<ReviewRefsResponse> responseRefsList = new ArrayList<ReviewRefsResponse>();
  		for (ElementInfo elementInfo : request.getQueryElements())
  		{
  			ReviewRefsResponse responseRefs = new ReviewRefsResponse();
  			// Now we are returning self since in one to many queried element can be involved in
				// many different relations and since we do not know the element's parent relation (or even if there is one)
				// we are forced return all including self. (Client need to handle self)
  			List<ReviewRef> references = refsRetriever.getAllReferences(elementInfo);
	  		log.debug(
	  			"Returning " + references.size() + " review references for requesting element: " + 
	  			elementInfo.toString());
	  		responseRefs.setReviewRefs(references.toArray(new ReviewRef[]{}));
	  		responseRefs.setQueryElementInfo(elementInfo);
	  		responseRefsList.add(responseRefs);
  		}
  		response.setReviewRefsResponses(responseRefsList.toArray(new ReviewRefsResponse[]{}));
  	}
    finally
    {
      DbUtils.closeConnection(conn);
    }
  	
  	log.debug("response : " + StringUtils.abbreviate(response.toString(), 1000));
  	return response;
  }
  
	/**
	 * <NAME>Review Service Get Random Reviewable</NAME>
	 * <DESCRIPTION>
	 * To retrieve the random reviewable item. If a reviewable is not available
	 * then return 0 result count
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <mapid>
	 *  Target map id
	 * </mapid>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	JSON in ReviewableItem format
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/random?mapid=15</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 * {"mapId":15,"relationId":-1,"sortOrder":-1,"resultCount":0}
	 * </OUTPUT>
	 * </EXAMPLE>
	 * @param mapId
	 * @return
	 */
	@GET
	@Path("/random")
	@Produces(MediaType.APPLICATION_JSON)
	public ReviewableItem getRandomReviewable(@QueryParam("mapid") String mapId)
	{

		ReviewableItem ret = new ReviewableItem(-1, -1,-1);
		try(Connection conn = DbUtils.createConnection())
		{
			long nMapId = Long.parseLong(mapId);
			ReviewableReader reader = new ReviewableReader(conn);
			ret = reader.getRandomReviewableItem(nMapId);
		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
	        "Error getting random reviewable item: " + mapId +  " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
		}
		return ret;
	}
	
	//
	
	
	
	/**
	 * <NAME>Review Service Get Next Reviewable</NAME>
	 * <DESCRIPTION>
	 * To retrieve the next reviewable item based on offset sequence id. If next reviewable is not available
	 * then try to get random reviewable item. 
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <mapid>
	 *  Target map id
	 * </mapid>
	 * <offsetseqid>
	 * 	Current Offset sequence id which gets incremented to  offsetseqid+1.
	 * </offsetseqid>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	JSON in ReviewableItem format
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/next?mapid=15&offsetseqid=2</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 * {"mapId":15,"relationId":-1,"sortOrder":3,"resultCount":0}
	 * </OUTPUT>
	 * </EXAMPLE>
	 * @param mapId
	 * @param offsetSeqId
	 * @return
	 */
	@GET
	@Path("/next")
	@Produces(MediaType.APPLICATION_JSON)
	public ReviewableItem getNextReviewable(@QueryParam("mapid") String mapId,
			@QueryParam("offsetseqid") String offsetSeqId,
			@QueryParam("direction") String direction)
	{

		ReviewableItem ret = new ReviewableItem(-1, -1,-1);
		try(Connection conn = DbUtils.createConnection())
		{
			
			long nMapId = Long.parseLong(mapId);
			long nOffsetSeqId = Long.parseLong(offsetSeqId);
			
			// if nextSquence is - or out of index value we will get random
			long nextSequence = nOffsetSeqId + 1;
			if(direction != null && direction.equalsIgnoreCase("backward"))
			{
				nextSequence = nOffsetSeqId - 1;
			}
				
			ReviewableReader reader = new ReviewableReader(conn);
			ret = reader.getReviewableItem(nMapId, nextSequence);
			// get random if we can not find immediate next sequence item
			if(ret.getResultCount() < 1)
			{
				ret = getRandomReviewable(mapId);
			}
		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
	        "Error getting next reviewable item: " + mapId +  " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
		}
		return ret;
	}
	
	
	/**
	 * <NAME>Review Service Get Reviewable</NAME>
	 * <DESCRIPTION>
	 * To retrieve the reviewable item based on offset sequence id. If reviewable is not available
	 * then return 0 result count
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <mapid>
	 *  Target map id
	 * </mapid>
	 * <offsetseqid>
	 * 	Offset sequence id.
	 * </offsetseqid>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	JSON in ReviewableItem format
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/next?mapid=15&offsetseqid=2</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 * {"mapId":15,"relationId":-1,"sortOrder":3,"resultCount":0}
	 * </OUTPUT>
	 * </EXAMPLE>
	 * @param mapId
	 * @param offsetSeqId
	 * @return
	 */
	@GET
	@Path("/reviewable")
	@Produces(MediaType.APPLICATION_JSON)
	public ReviewableItem getReviewable(@QueryParam("mapid") String mapId,
			@QueryParam("offsetseqid") String offsetSeqId)
	{

		ReviewableItem ret = new ReviewableItem(-1, -1,-1);
		try(Connection conn = DbUtils.createConnection())
		{
			long nMapId = Long.parseLong(mapId);
			long nOffsetSeqId = Long.parseLong(offsetSeqId);
			ReviewableReader reader = new ReviewableReader(conn);
			ret = reader.getReviewableItem(nMapId, nOffsetSeqId);
		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
	        "Error getting reviewable item: " + mapId +  " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
		}
		return ret;
	}
	
	//
	
	/**
	 * <NAME>Review Service Get Reviewable Statistics</NAME>
	 * <DESCRIPTION>
	 * To retrieve the reviewable statistics for a map
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <mapid>
	 *  Target map id
	 * </mapid>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	JSON in ReviewableStatistics format
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/statistics?mapid=15</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 * {"totalCount":5,"unreviewedCount":0}
	 * </OUTPUT>
	 * </EXAMPLE>
	 * @param mapId
	 * @return
	 */
	@GET
	@Path("/statistics")
	@Produces(MediaType.APPLICATION_JSON)
	public ReviewableStatistics getReviewableSstatistics(@QueryParam("mapId") String mapId)
	{

		ReviewableStatistics ret = new ReviewableStatistics(-1, -1);
		try(Connection conn = DbUtils.createConnection())
		{
			long nMapId = Long.parseLong(mapId);
			ReviewableReader reader = new ReviewableReader(conn);
			ret = reader.getReviewablesStatistics(nMapId);
		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
	        "Error getting reviewables statistics: " + mapId +  " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
		}
		return ret;
	}
	
	
	/**
	 * <NAME>Review Service Get geojson for all reviewable items</NAME>
	 * <DESCRIPTION>
	 * To retrieve GeoJson of all reviewable items within bouding box
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <mapid>
	 *  Target map id
	 * </mapid>
	 * <minlon>
	 *  Minimum longitude
	 * </minlon>
	 * <minlat>
	 *  Minimum latitude
	 * </minlat>
	 * <maxlon>
	 *  Maximum longitude
	 * </maxlon>
	 * <maxlat>
	 *  Maximum latitude
	 * </maxlat>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	GeoJson containing reviewable bounding box and state
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/allreviewables?mapid=53&minlon=-180&minlat=-90&maxlon=180&maxlat=90</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
	 * GeoJson
	 * </OUTPUT>
	 * </EXAMPLE>
	 * @param mapId
	 * @param minLon
	 * @param minLat
	 * @param maxLon
	 * @param maxLat
	 * @return
	 */
	@GET
	@Path("/allreviewables")
	@Produces(MediaType.APPLICATION_JSON)
	public JSONObject getReviewable(@QueryParam("mapid") String mapId,
			@QueryParam("minlon") String minLon,
			@QueryParam("minlat") String minLat,
			@QueryParam("maxlon") String maxLon,
			@QueryParam("maxlat") String maxLat
			)
	{

		JSONObject ret = new JSONObject();
		ret.put("type", "FeatureCollection");
		ret.put("features", new JSONArray());
		
		try(Connection conn = DbUtils.createConnection())
		{
			long nMapId = Long.parseLong(mapId);
			double minlon = Double.parseDouble(minLon);
			double minlat = Double.parseDouble(minLat);
			double maxlon = Double.parseDouble(maxLon);
			double maxlat = Double.parseDouble(maxLat);
			ReviewableReader reader = new ReviewableReader(conn);
			AllReviewableItems result = reader.getAllReviewableItems(nMapId, new BoundingBox(minlon, minlat, maxlon, maxlat));
			ret = new JSONObject();
			if(result.getOverFlow() == true)
			{
				ret.put("warning", "The result size is greater than maximum limit of:" + MAX_RESULT_SIZE
						+ ". Returning truncated data.");
			}
			ret.put("total", result.getReviewableItems().size());
			ret.put("geojson", result.toGeoJson());
		}
		catch (Exception ex)
		{
			ResourceErrorHandler.handleError(
	        "Error getting reviewable item: " + mapId +  " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
		}
		return ret;
	}
	
	
	
	/**
	 * <NAME>Review bookmark save</NAME>
	 * <DESCRIPTION>
	 * To create or update review bookmark
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <request>
	 *  ReviewBookmarkSaveRequest class
	 * </request>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	json containing created/updated bookmark id
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/bookmarks/save</URL>
	 * 	<REQUEST_TYPE>POST</REQUEST_TYPE>
	 * 	<INPUT>
	 * {
	 *  "mapId":1,
	 *  "relationId":3,
	 *  "detail": {"k1":"v1","l3":"v3"},
	 *  "userId":-1
	 *  }
	 *	</INPUT>
	 * <OUTPUT>
   * {
   *     "bookmarkid": 1
   * }
	 * </OUTPUT>
	 * </EXAMPLE>
   * @param request
   * @return
   * @throws Exception
   */
  @POST
  @Path("/bookmarks/save")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewBookmarksSaveResponse createReviewBookmark(
  	final ReviewBookmarkSaveRequest request) 
  	throws Exception
  {
  	
  	ReviewBookmarksSaveResponse response = new ReviewBookmarksSaveResponse();
  	
  	try(Connection conn = DbUtils.createConnection())
  	{
  		JSONObject oDetail = request.getDetail();
  		Object oNotes = oDetail.get("bookmarknotes");
  		if(oNotes != null) 
  		{
  			
  			JSONArray aNotes = (JSONArray)oNotes;
  			
  			for(int i=0; i<aNotes.size(); i++)
  			{
  				JSONObject note = (JSONObject)aNotes.get(i);
  				if(!note.containsKey("id"))
  				{
  					String sNewId = UUID.randomUUID().toString();
  					sNewId = sNewId.replace('-', '0');
  					note.put("id", sNewId);
  					Calendar calendar = Calendar.getInstance();
  					long now = calendar.getTimeInMillis();
  					note.put("createdAt", now);
  					note.put("modifiedAt", now);
  				}
  				
  				if(!note.containsKey("modifiedAt"))
  				{
  					Calendar calendar = Calendar.getInstance();
  					long now = calendar.getTimeInMillis();
  					note.put("modifiedAt", now);
  				}
  			}
  		}
  		ReviewBookmarksSaver saver = new ReviewBookmarksSaver(conn);
  		long nSaved = saver.save(request);
  		response.setSavedCount(nSaved);
  	}
  	catch(Exception ex)
  	{
  		ResourceErrorHandler.handleError(
	        "Error saving review bookmark: " + " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
  	}
   
  	return response;
  }
  
  
	/**
	 * <NAME>Review bookmark retrieve</NAME>
	 * <DESCRIPTION>
	 * To retrieve review bookmark
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <mapId>
	 *  map Id
	 * </mapId>
	 * <relationId>
	 *  relation id
	 * </relationId>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	json containing list of review bookmarks
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/bookmarks/get?mapId=1&relationId=2</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
   * {
   *     "reviewBookmarks":
   *     [
   *         {
   *             "createdAt": 1453229299354,
   *             "createdBy": -1,
   *             "detail":
   *             {
   *                 "type": "hstore",
   *                 "value": ""k1"=>"v1", "l3"=>"v3""
   *             },
   *             "id": 2,
   *             "lastModifiedAt": null,
   *             "lastModifiedBy": null,
   *             "mapId": 1,
   *             "relationId": 2
   *         }
   *     ]
   * }
	 * </OUTPUT>
	 * </EXAMPLE>
   * @param mapid
   * @param relid
   * @return
   * @throws Exception
   */
  @GET
  @Path("/bookmarks/get")
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewBookmarksGetResponse getReviewBookmark(@QueryParam("bookmarkId") String bookmarkid, 
  		@QueryParam("mapId") String mapid,
  		@QueryParam("relationId") String relid) throws Exception
  {
  	ReviewBookmarksGetResponse response = new  ReviewBookmarksGetResponse();
  	
  	try(Connection conn = DbUtils.createConnection())
  	{  		
  		ReviewBookmarkRetriever retriever = new ReviewBookmarkRetriever(conn);
  		List<ReviewBookmarks>res = null;
  		if(bookmarkid != null)
  		{
  			long bookmarkId = Long.parseLong(bookmarkid);
  			res = retriever.retrieve(bookmarkId);
  		}
  		else
  		{
  			long mapId = Long.parseLong(mapid);
    		long relationId = Long.parseLong(relid);
  			res = retriever.retrieve(mapId, relationId);
  		}
  		
  		
  		
  		for(ReviewBookmarks mk : res)
  		{
  			Object oDetail = mk.getDetail();
  			Map<String, String> hstoreMap = PostgresUtils.postgresObjToHStore((org.postgresql.util.PGobject)oDetail);
  			JSONObject oBmkDetail = new JSONObject();
  			_appendHstoreElement(hstoreMap.get("bookmarkdetail"), oBmkDetail, "bookmarkdetail");

  			String bmkNotes = hstoreMap.get("bookmarknotes");
  			if(bmkNotes != null && bmkNotes.length() > 0)
  			{
  				bmkNotes = bmkNotes.replace("\\\"", "\"");
  				bmkNotes = bmkNotes.replace("\\\\", "\\");
	  			JSONParser parser = new JSONParser();
	  			JSONArray oParsed = (JSONArray)parser.parse(bmkNotes);
	  			
	  			oBmkDetail.put("bookmarknotes", oParsed);	  			
  			}
  			
  			_appendHstoreElement(hstoreMap.get("bookmarkreviewitem"), oBmkDetail, "bookmarkreviewitem");
  			
  			if(oBmkDetail != null)
  			{
  				mk.setDetail(oBmkDetail);
  			}
  			
  		}
  		
  		
  		
  		response.setReviewBookmarks(res);
  	}
  	catch(Exception ex)
  	{
  		ResourceErrorHandler.handleError(
	        "Error getting review bookmark: " + " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
  	}
  	return response;
  }
  
  /**
   * Helper function to handle JSON string conversion to Hstore friendly format
   * 
   * @param rawElem
   * @param oBmkDetail
   * @param elemName
   * @throws Exception
   */
  protected void _appendHstoreElement(final String rawElem, final JSONObject oBmkDetail, final String elemName) throws Exception
  {
  	String bmkElem = rawElem;
		if(bmkElem != null && bmkElem.length() > 0)
		{
			bmkElem = bmkElem.replace("\\\"", "\"");
			bmkElem = bmkElem.replace("\\\\", "\\");
			JSONParser parser = new JSONParser();
			JSONObject oParsed = (JSONObject)parser.parse(bmkElem);
					
			oBmkDetail.put(elemName, oParsed);
			
		}
  }
  
	/**
	 * <NAME>Review bookmark retrieve all</NAME>
	 * <DESCRIPTION>
	 * To retrieve all review bookmark
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <orderBy>
	 *  order by column [createdAt | createdBy |  id | lastModifiedAt | lastModifiedBy | mapId | relationId]
	 * </orderBy>
	 * <asc>
	 *  is ascending [true | false]
	 * </asc>
	 * <limit>
	 *  Limit count for paging . 
	 * </limit>
	 * <offset>
	 *  offset index for paging . 
	 * </offset>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	json containing list of review bookmarks
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/bookmarks/getall?orderBy=createdAt&asc=false&limit=2&offset=1</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
   * {
   *     "reviewBookmarks":
   *     [
   *         {
   *             "createdAt": 1453229299354,
   *             "createdBy": -1,
   *             "detail":
   *             {
   *                 "type": "hstore",
   *                 "value": ""k1"=>"v1", "l3"=>"v3""
   *             },
   *             "id": 2,
   *             "lastModifiedAt": null,
   *             "lastModifiedBy": null,
   *             "mapId": 1,
   *             "relationId": 2
   *         }
   *     ]
   * }
	 * </OUTPUT>
	 * </EXAMPLE>
   * @param orderByCol
   * @param asc
   * @param limitSize
   * @param offset
   * @return
   * @throws Exception
   */
  @GET
  @Path("/bookmarks/getall")
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewBookmarksGetResponse getAllReviewBookmark(@QueryParam("orderBy") String orderByCol,
  		@QueryParam("asc") String asc, @QueryParam("limit") String limitSize,
  		 @QueryParam("offset") String offset, @QueryParam("filterby") String filterBy
  		 , @QueryParam("filterbyval") String filterByVal) throws Exception
  {
  	ReviewBookmarksGetResponse response = new  ReviewBookmarksGetResponse();
  	
  	try(Connection conn = DbUtils.createConnection())
  	{

  		boolean isAsc = true;
  		if(asc != null)
  		{
  			isAsc = (asc.equalsIgnoreCase("true"));
  		}
  		
  		long limit = -1;
  		
  		if(limitSize != null)
  		{
  			limit = Long.parseLong(limitSize);
  		}
  		
  		long offsetCnt = -1;
  		if(offset != null)
  		{
  			offsetCnt = Long.parseLong(offset);
  		}
  		
  		String filterByCol = null;
  		Long filterVal = null;
  		
  		if(filterBy != null && filterBy.length()>0 && 
  				filterByVal != null && filterByVal.length() > 0)
  		{
  			if(filterBy.equalsIgnoreCase("createdBy"))
  			{
  				filterByCol = "createdBy";
  				filterVal = Long.parseLong(filterByVal);
  			} 
  			else if(filterBy.equalsIgnoreCase("mapId"))
  			{
  				filterByCol = "mapId";
  				filterVal = Long.parseLong(filterByVal);
  			}
  		}
  		
  		ReviewBookmarkRetriever retriever = new ReviewBookmarkRetriever(conn);
  		List<ReviewBookmarks>res = retriever.retrieveAll(orderByCol, isAsc, limit, offsetCnt, filterByCol, filterVal);
  		
  		for(ReviewBookmarks mk : res)
  		{
  			Object oDetail = mk.getDetail();
  			Map<String, String> hstoreMap = PostgresUtils.postgresObjToHStore((org.postgresql.util.PGobject)oDetail);
  			
  			String bmkDetail = hstoreMap.get("bookmarkdetail");
  			if(bmkDetail != null && bmkDetail.length() > 0)
  			{
  				bmkDetail = bmkDetail.replace("\\\"", "\"");
  				bmkDetail = bmkDetail.replace("\\\\", "\\");
	  			JSONParser parser = new JSONParser();
	  			JSONObject oParsed = (JSONObject)parser.parse(bmkDetail);
	  			
	  			
	  			JSONObject oBmkDetail = new JSONObject();
	  			oBmkDetail.put("bookmarkdetail", oParsed);
	  			
	  			mk.setDetail(oBmkDetail);
  			}
  			
  		}
  		response.setReviewBookmarks(res);
  	}
  	catch(Exception ex)
  	{
  		ResourceErrorHandler.handleError(
	        "Error getting review bookmark: " + " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
  	}
  	return response;
  }
  
	/**
	 * <NAME>Review bookmark stat</NAME>
	 * <DESCRIPTION>
	 * To retrieve review bookmarks stat
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	json stat info
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/bookmarks/stat</URL>
	 * 	<REQUEST_TYPE>GET</REQUEST_TYPE>
	 * 	<INPUT>
	 *	</INPUT>
	 * <OUTPUT>
   * 
   * {
   *     "totalCount": 2
   * }
	 * </OUTPUT>
	 * </EXAMPLE>
   * @return
   * @throws Exception
   */
  @GET
  @Path("/bookmarks/stat")
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewBookmarksStatResponse getAllReviewBookmarkStat() throws Exception
  {
  	ReviewBookmarksStatResponse response = new ReviewBookmarksStatResponse();
  	try(Connection conn = DbUtils.createConnection())
  	{
  		ReviewBookmarkRetriever retriever = new ReviewBookmarkRetriever(conn);
  		long nCnt = retriever.getbookmarksCount();
  		response.setTotalCount(nCnt);
  	}
  	catch(Exception ex)
  	{
  		ResourceErrorHandler.handleError(
	        "Error getting review bookmark counts: " + " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
  	}
  	return response;
  }
  
	/**
	 * <NAME>Review bookmark delete</NAME>
	 * <DESCRIPTION>
	 * To delete review bookmark
	 * </DESCRIPTION>
	 * <PARAMETERS>
	 * <ReviewBookmarkDelRequest>
	 *  Delete request
	 * </ReviewBookmarkDelRequest>
	 * </PARAMETERS>
	 * <OUTPUT>
	 * 	json containing total numbers of deleted
	 * </OUTPUT>
	 * <EXAMPLE>
	 * 	<URL>http://localhost:8080/hoot-services/job/review/bookmarks/delete</URL>
	 * 	<REQUEST_TYPE>DELETE</REQUEST_TYPE>
	 * 	<INPUT>
	 *  {
	 * "mapId":397,
	 * "relationId":3
	 * }
	 *	</INPUT>
	 * <OUTPUT>
   * 
   * {
   *     "deleteCount": 1
   * }
	 * </OUTPUT>
	 * </EXAMPLE>
   * @param request
   * @return
   * @throws Exception
   */
  @DELETE
  @Path("/bookmarks/delete")
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewBookmarkDelResponse delReviewBookmark(@QueryParam("bookmarkId") final String bmkId) throws Exception
  {
  	ReviewBookmarkDelRequest request = new ReviewBookmarkDelRequest(Long.parseLong(bmkId));
  	ReviewBookmarkDelResponse response = new  ReviewBookmarkDelResponse();
  	
  	try(Connection conn = DbUtils.createConnection())
  	{
  		ReviewBookmarksRemover remover = new ReviewBookmarksRemover(conn);
  		long nDel = remover.remove(request);
  		response.setDeleteCount(nDel);
  	}
  	catch(Exception ex)
  	{
  		ResourceErrorHandler.handleError(
	        "Error deleting review bookmark: " + " (" + 
	          ex.getMessage() + ")", 
	        Status.BAD_REQUEST,
	        log);
  	}
  	return response;
  }
  

}
