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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.wps;

import java.sql.Connection;
import java.util.List;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.geo.BoundingBox;
import hoot.services.models.review.ReviewableItem;
import hoot.services.readers.review.ReviewableItemRetriever;
import hoot.services.review.DisplayBoundsCalculator;
import hoot.services.review.ReviewUtils;
import hoot.services.validators.wps.ReviewInputParamsValidator;
import hoot.services.writers.review.ReviewableItemsResponseWriter;

import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletExecutionInfo;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.output.LiteralOutput;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Class implementing a WPS operation to retrieve conflated data marked for review
 */
public class GetReviewableItemsProcesslet extends BaseProcesslet
{
  private static final Logger log = LoggerFactory.getLogger(GetReviewableItemsProcesslet.class);
  
  public GetReviewableItemsProcesslet() throws Exception
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
    final String errorMessageStart = "retrieving reviewable items";
    Connection conn = DbUtils.createConnection();
    try
    {
      //Any changes to these default parameters must also be reflected in 
      //$HOOT_HOME/hoot-services/src/main/webapp/WEB-INF/workspace/GetReviewableItemsProcesslet.xml
      //and vice versa.
      ReviewInputParamsValidator inputParamsValidator = new ReviewInputParamsValidator(inputParams);
      final String mapId = 
        (String)inputParamsValidator.validateAndParseInputParam("mapId", "", null, null, false, null);
      final int numItems = 
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
      final boolean highestReviewScoreFirst = 
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
      final double reviewScoreThresholdMinimum = 
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
      final BoundingBox geospatialBounds = 
        (BoundingBox)inputParamsValidator.validateAndParseInputParam(
          "geospatialBounds", 
          "", 
          null, 
          null, 
          true, 
          HootProperties.getInstance().getProperty(
            "reviewGetGeospatialBoundsDefault", 
            HootProperties.getDefault("reviewGetGeospatialBoundsDefault")));
      final double displayBoundsZoomAdjust = 
        (Double)inputParamsValidator.validateAndParseInputParam(
          "displayBoundsZoomAdjust", 
          new Double(0.0), 
          new Double(-1.0), 
          new Double(1.0), 
          true, 
          Double.parseDouble(
            HootProperties.getInstance().getProperty(
              "reviewDisplayBoundsZoomAdjustDefault", 
              HootProperties.getDefault("reviewDisplayBoundsZoomAdjustDefault"))));
      //TODO: this isn't validating the validity of the actual enum value yet; need better error
      //checking here
      final DisplayBoundsCalculator.DisplayMethod boundsDisplayMethodEnumVal = 
        DisplayBoundsCalculator.DisplayMethod.valueOf(
          (String)inputParamsValidator.validateAndParseInputParam(
            "boundsDisplayMethod", 
            "", 
            null, 
            null, 
            true, 
            HootProperties.getInstance().getProperty(
              "reviewDisplayBoundsMethod", 
              HootProperties.getDefault("reviewDisplayBoundsMethod"))));
      
      log.debug("Initializing database connection...");
   
      
      //query for review items
      ReviewableItemRetriever itemRetriever = new ReviewableItemRetriever(conn, mapId);
      final List<ReviewableItem> reviewItems = 
        itemRetriever.getReviewItems(
          numItems, highestReviewScoreFirst, reviewScoreThresholdMinimum, geospatialBounds,
          displayBoundsZoomAdjust, boundsDisplayMethodEnumVal);
      assert(reviewItems.size() <= numItems);
       
      //write the items out to the response besides string output, deegree only supports XML and 
      //binary types, so not returning as application/json
      ((LiteralOutput)outputParams.getParameter("reviewableItems")).setValue(
        (new ReviewableItemsResponseWriter()).writeResponseString(
          reviewItems, itemRetriever.getMapId(), numItems, highestReviewScoreFirst, 
          reviewScoreThresholdMinimum, geospatialBounds));
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
