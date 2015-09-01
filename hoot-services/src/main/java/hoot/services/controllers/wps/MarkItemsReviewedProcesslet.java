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
package hoot.services.controllers.wps;

import java.sql.Connection;

import hoot.services.db.DbUtils;
import hoot.services.models.review.MarkItemsReviewedRequest;
import hoot.services.models.review.MarkItemsReviewedResponse;
import hoot.services.models.review.ReviewedItems;
import hoot.services.review.ReviewItemsMarker;
import hoot.services.review.ReviewUtils;
import hoot.services.validators.wps.ReviewInputParamsValidator;

import org.apache.commons.lang3.StringUtils;
import org.codehaus.jackson.map.ObjectMapper;
import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.output.LiteralOutput;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.transaction.PlatformTransactionManager;
import org.springframework.transaction.TransactionDefinition;
import org.springframework.transaction.TransactionStatus;
import org.springframework.transaction.support.DefaultTransactionDefinition;

/**
 * Class implementing a WPS operation to mark conflated data as having been reviewed
 */
public class MarkItemsReviewedProcesslet extends BaseProcesslet
{
  private static final Logger log = LoggerFactory.getLogger(MarkItemsReviewedProcesslet.class);
  
  public MarkItemsReviewedProcesslet() throws Exception
  {
    super();
  }
  
  /*
   * (non-Javadoc)
   * @see hoot.services.wps.WPSProcesslet#process(org.deegree.services.wps.ProcessletInputs, org.deegree.services.wps.ProcessletOutputs, org.deegree.services.wps.ProcessletExecutionInfo)
   */
  @Override
  public void process(ProcessletInputs inputParams, ProcessletOutputs outputParams, 
    ProcessletExecutionInfo processletExecInfo) throws ProcessletException
  {
    final String errorMessageStart = "marking items as reviewed";
    Connection conn = DbUtils.createConnection();
    MarkItemsReviewedResponse markItemsReviewedResponse = null;
    try
    {
      //Any changes to these default parameters must also be reflected in 
      //$HOOT_HOME/hoot-services/src/main/webapp/WEB-INF/workspace/MarkItemsReviewedProcesslet.xml
      //and vice versa.
      ReviewInputParamsValidator inputParamsValidator = new ReviewInputParamsValidator(inputParams);
      final String mapId = 
        (String)inputParamsValidator.validateAndParseInputParam("mapId", "", null, null, false, null);
      final boolean markAll = 
        (Boolean)inputParamsValidator.validateAndParseInputParam(
          "markAll", false, null, null, true, false);
      final String reviewedItemsStr = 
        (String)inputParamsValidator.validateAndParseInputParam(
          "reviewedItems", "", null, null, true, null);
      ReviewedItems reviewedItems = null;
      if (reviewedItemsStr != null)
      {
        reviewedItems = (new ObjectMapper()).readValue(reviewedItemsStr, ReviewedItems.class);
      }
      if (!markAll && 
          (reviewedItems == null || reviewedItems.getReviewedItems() == null ||
           reviewedItems.getReviewedItems().length == 0))
      {
        throw new Exception(
          "Invalid input parameter: markAll set to false and " +
          "markItemsReviewedRequest.reviewedItems empty.");
      }
      final String reviewedItemsChangesetStr = 
        (String)inputParamsValidator.validateAndParseInputParam(
          "reviewedItemsChangeset", "", null, null, true, "");
      MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
      markItemsReviewedRequest.setReviewedItems(reviewedItems);
      markItemsReviewedRequest.setReviewedItemsChangeset(reviewedItemsChangesetStr);
      
      log.debug("Initializing transaction manager...");
      PlatformTransactionManager transactionManager = 
        appContext.getBean("transactionManager", PlatformTransactionManager.class);
      
      log.debug("Initializing database connection...");    
        
      //TODO: verify that no other writes are seen during this transaction
      
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
        log.debug("Rolling back database transaction for MarkItemsReviewedProcesslet...");
        transactionManager.rollback(transactionStatus);
        conn.rollback();
        throw e;
      }

      log.debug("Committing MarkItemsReviewedProcesslet database transaction...");
      transactionManager.commit(transactionStatus);
      conn.commit();
      
      if (StringUtils.trimToNull(markItemsReviewedResponse.getChangesetUploadResponse()) != null)
      {
        log.debug("Returning changeset upload response: " + 
          StringUtils.abbreviate(markItemsReviewedResponse.getChangesetUploadResponse(), 100) + " ...");
      }
      else
      {
        log.debug("Returning null changeset upload response...");
      }
      ((LiteralOutput)outputParams.getParameter("changesetUploadResponse")).setValue(
        markItemsReviewedResponse.getChangesetUploadResponse());
      log.debug("Returning number of items marked as reviewed: " + 
        markItemsReviewedResponse.getNumItemsMarkedReviewed() + " ...");
      ((LiteralOutput)outputParams.getParameter("numItemsMarkedReviewed")).setValue(
        String.valueOf(markItemsReviewedResponse.getNumItemsMarkedReviewed()));
      log.debug("Returning changeset ID: " + markItemsReviewedResponse.getChangesetId() + " ...");
      ((LiteralOutput)outputParams.getParameter("changesetId")).setValue(
        String.valueOf(markItemsReviewedResponse.getChangesetId()));
      log.debug("Setting map ID: " + markItemsReviewedResponse.getMapId() + " ...");
      ((LiteralOutput)outputParams.getParameter("mapId")).setValue(
        String.valueOf(markItemsReviewedResponse.getMapId()));
    }
    catch (Exception e)
    {
      try
      {
        ReviewUtils.handleError(e, errorMessageStart, true);
      }
      catch (Exception e2)
      {
        throw (ProcessletException)e2;
      }
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
        try
        {
          ReviewUtils.handleError(e, errorMessageStart, true);
        }
        catch (Exception e2)
        {
          throw (ProcessletException)e2;
        }
      }
    }
  }
}