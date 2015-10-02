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

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.review.ReviewUtils;
import hoot.services.validators.wps.ReviewInputParamsValidator;
import hoot.services.writers.review.ReviewItemsPreparer;

import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.output.LiteralOutput;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Class implementing a WPS operation to prepare conflated data for review
 */
public class PrepareItemsForReviewProcesslet extends BaseProcesslet
{
  private static final Logger log = LoggerFactory.getLogger(PrepareItemsForReviewProcesslet.class);
  
  //TODO: see #6270
  public static String reviewRecordWriter = "reviewPrepareDbWriter2";
  
  public PrepareItemsForReviewProcesslet() throws Exception
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
    final String errorMessageStart = "preparing items for review";
    Connection conn = DbUtils.createConnection();
    try
    {
      //Any changes to these default parameters must also be reflected in 
      //$HOOT_HOME/hoot-services/src/main/webapp/WEB-INF/workspace/PrepareItemsForReviewProcesslet.xml
      //and vice versa.
      ReviewInputParamsValidator inputParamsValidator = new ReviewInputParamsValidator(inputParams);
      final String mapId = 
        (String)inputParamsValidator.validateAndParseInputParam("mapId", "", null, null, false, null);
      final boolean overwriteExistingData = 
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
      
      //These parameters are passed in by the unit tests only and aren't part of the WPS schema
      //for this method.  With better unit test coverage, these params could probably go away.
      final long testDelayMilliseconds = 
        (Long)inputParamsValidator.validateAndParseInputParam(
          "testDelayMilliseconds", new Long(0), new Long(0), null, true, new Long(0));
      final boolean simulateFailure = 
        (Boolean)inputParamsValidator.validateAndParseInputParam(
          "simulateFailure", false, null, null, true, new Boolean(false));
      
      log.debug("Initializing database connection...");

      ((LiteralOutput)outputParams.getParameter("jobId")).setValue(
        (new ReviewItemsPreparer(
           conn, testDelayMilliseconds, simulateFailure, mapId, reviewRecordWriter))
         .prepare(overwriteExistingData));
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