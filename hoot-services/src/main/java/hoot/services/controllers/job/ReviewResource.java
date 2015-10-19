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
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import hoot.services.controllers.osm.MapResource;
import hoot.services.db.DbUtils;
import hoot.services.db2.QMaps;
import hoot.services.models.review.MapReviewResolverRequest;
import hoot.services.models.review.MapReviewResolverResponse;
import hoot.services.models.review.ReviewInfo;
import hoot.services.models.review.ReviewReferences;
import hoot.services.models.review.ReviewReferencesCollection;
import hoot.services.readers.review.ReviewReferencesRetriever;
import hoot.services.review.ReviewUtils;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.validators.job.InputParamsValidator;
import hoot.services.writers.review.MapReviewResolver;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.support.DefaultTransactionDefinition;

import com.mysema.query.sql.SQLQuery;

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

  public ReviewResource() throws Exception
  {
    log.debug("Reading application settings...");
    appContext = new ClassPathXmlApplicationContext(new String[] { "db/spring-database.xml" });
    log.debug("Initializing transaction manager...");
    transactionManager = appContext.getBean("transactionManager", PlatformTransactionManager.class);
  }

  /**
   * Resolves all reviews for a given map
   * 
   * Have to use a request object here, rather than a single map ID query param, since d3 can't
   * send plain text in a PUT statement.
   * 
   * @param request a request containing the map ID for the reviews to be resolved
   * @return a response with the changeset ID used to resolve the reviews
   * @throws Exception
   */
  @PUT
  @Path("/resolveall")
  @Consumes(MediaType.APPLICATION_JSON)
  @Produces(MediaType.APPLICATION_JSON)
  public MapReviewResolverResponse resolveAllReviews(final MapReviewResolverRequest request) 
    throws Exception
  {
  	log.debug("Setting all items reviewed for map with ID: " + request.getMapId() + "...");
  	
  	Connection conn = DbUtils.createConnection();
    TransactionStatus transactionStatus = 
      transactionManager.getTransaction(
        new DefaultTransactionDefinition(TransactionDefinition.PROPAGATION_REQUIRED));
    conn.setAutoCommit(false);
    
    final long mapIdNum = MapResource.validateMap(request.getMapId(), conn);
    
  	long userId = -1;
  	try
  	{
  		log.debug(
	      "Retrieving user ID associated with map having ID: " + String.valueOf(mapIdNum) + " ...");
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
  	  changesetId = (new MapReviewResolver(conn)).setAllReviewsResolved(mapIdNum, userId);
  		
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
  	return new MapReviewResolverResponse(changesetId);
  }
  
  /**
   * Returns any review references to the elements associated with the ID's passed in
   * 
   * @param mapId contains an array of unique ID's and their associated map ID; all unique ID's
   * should be associated with features owned by the single map ID
   * @param elementUniqueIds a collection of element unique ID's associated with the specified map
   * @return an array of review references; one set of references for each unique ID passed in
   * @throws Exception
   */
  @GET
  @Path("/refs")
  @Consumes(MediaType.TEXT_PLAIN)
  @Produces(MediaType.APPLICATION_JSON)
  public ReviewReferencesCollection getReviewReferences(
  	@QueryParam("mapId")
    String mapId,
    @QueryParam("elementUniqueIds")
    String elementUniqueIds) 
  	throws Exception
  {
  	log.debug("Returning review references...");
  	
  	Connection conn = DbUtils.createConnection();
  	ReviewReferencesCollection response = new ReviewReferencesCollection();
  	
  	Map<String, Object> inputParams = new HashMap<String, Object>();
    inputParams.put("mapId", mapId);
    inputParams.put("elementUniqueIds", elementUniqueIds);
    
  	try
  	{
  		InputParamsValidator inputParamsValidator = new InputParamsValidator(inputParams);
      mapId =
        (String)inputParamsValidator.validateAndParseInputParam(
        	"mapId", "", null, null, false, null);
      final long mapIdNum = MapResource.validateMap(mapId, conn);
      elementUniqueIds =
        (String)inputParamsValidator.validateAndParseInputParam(
        	"elementUniqueIds", "", null, null, false, null);
  		String[] elementUniqueIdsArr;
  		if (elementUniqueIds.contains(";"))
  		{
  			elementUniqueIdsArr = elementUniqueIds.split(";");
  		}
  		else
  		{
  			elementUniqueIdsArr = new String[] { elementUniqueIds };
  		}
  		
  		List<ReviewReferences> responseRefsList = new ArrayList<ReviewReferences>();
  		for (String elementUniqueId : elementUniqueIdsArr)
  		{
  			ReviewReferences responseRefs = new ReviewReferences();
  			List<ReviewInfo> references = 
	  			(new ReviewReferencesRetriever(conn, mapIdNum)).getReferences(elementUniqueId);
	  		log.debug(
	  			"Returning " + references.size() + " review against items for unique ID: " + 
	  		  elementUniqueId);
	  		responseRefs.setReviewInfoItems(references.toArray(new ReviewInfo[]{}));
	  		responseRefs.setRequestElementUniqueId(elementUniqueId);
	  		responseRefsList.add(responseRefs);
  		}
  		response.setReviewReferences(responseRefsList.toArray(new ReviewReferences[]{}));
  	}
  	catch (Exception e)
    {
      ReviewUtils.handleError(
      	e, 
      	"Unable to retrieve review references for map ID: " + mapId + " and element unique ID's: " + 
      	  elementUniqueIds, 
      	false);
    }
    finally
    {
      DbUtils.closeConnection(conn);
    }
  	
  	log.debug("response : " + StringUtils.abbreviate(response.toString(), 1000));
  	return response;
  }
}