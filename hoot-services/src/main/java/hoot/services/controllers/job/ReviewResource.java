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

import hoot.services.db.DbUtils;
import hoot.services.db2.QMaps;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.models.review.MapReviewResolverRequest;
import hoot.services.models.review.MapReviewResolverResponse;
import hoot.services.review.ReviewUtils;
import hoot.services.utils.ResourceErrorHandler;
import hoot.services.writers.review.MapReviewResolver;

import javax.ws.rs.Consumes;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response.Status;

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
    
    long mapIdNum = -1;
  	try
  	{
  	  //input mapId may be a map ID or a map name
      mapIdNum = 
      	ModelDaoUtils.getRecordIdForInputString(
      		request.getMapId(), conn, maps, maps.id, maps.displayName);
      assert(mapIdNum != -1);
  	}
  	catch (Exception e)
    {
      if (e.getMessage().startsWith("Multiple records exist"))
      {
        ResourceErrorHandler.handleError(
          e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"), 
          Status.NOT_FOUND,
          log);
      }
      else if (e.getMessage().startsWith("No record exists"))
      {
        ResourceErrorHandler.handleError(
          e.getMessage().replaceAll("records", "maps").replaceAll("record", "map"), 
          Status.NOT_FOUND,
          log);
      }
      ResourceErrorHandler.handleError(
        "Error requesting map with ID: " + request.getMapId() + " (" + e.getMessage() + ")", 
        Status.BAD_REQUEST,
        log);
    }
    
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
}