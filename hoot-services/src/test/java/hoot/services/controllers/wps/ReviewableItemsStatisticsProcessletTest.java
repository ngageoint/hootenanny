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

import java.io.IOException;
import java.math.BigInteger;
import java.util.LinkedList;
import java.util.UUID;

import javax.ws.rs.core.Response.Status;

import org.codehaus.jackson.map.ObjectMapper;
import org.deegree.process.jaxb.java.BoundingBoxInputDefinition;
import org.deegree.process.jaxb.java.LiteralInputDefinition;
import org.deegree.process.jaxb.java.ProcessDefinition;
import org.deegree.process.jaxb.java.ProcessDefinition.OutputParameters;
import org.deegree.services.wps.ProcessExecution;
import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.input.ProcessletInput;
import org.deegree.services.wps.output.LiteralOutputImpl;
import org.deegree.services.wps.output.ProcessletOutput;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.dml.SQLInsertClause;

import hoot.services.IntegrationTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewMap;
import hoot.services.geo.BoundingBox;
import hoot.services.job.JobStatusManager.JOB_STATUS;
import hoot.services.models.review.ReviewableItemsStatistics;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.wps.WpsUtils;

public class ReviewableItemsStatisticsProcessletTest extends OsmResourceTestAbstract
{
  @SuppressWarnings("unused")
  private static final Logger log =
    LoggerFactory.getLogger(ReviewableItemsStatisticsProcessletTest.class);

  public ReviewableItemsStatisticsProcessletTest() throws NumberFormatException, IOException
  {
    super("hoot.services.controllers.job");
  }

  /*
   * This tests mimics the behavior defined in
     $HOOT_HOME/hoot-services/src/main/src/main/java/hoot/services/controllers/review/ReviewableItemsStatisticsProcesslet.java
     So, any changes to that definition must also be reflected in this method and vice versa.
   */
  private ProcessletOutputs execGet(final String mapId, final String reviewScoreThresholdMinimum,
    final String geospatialBounds) throws Exception
  {
    ReviewableItemsStatisticsProcesslet processlet = new ReviewableItemsStatisticsProcesslet();

    LinkedList<ProcessletInput> allInputs = new LinkedList<ProcessletInput>();

    if (mapId != null)
    {
      //default data type for a wps input is string, so no extra params needed here for mapId
      allInputs.add(WpsUtils.createLiteralInput("mapId", String.valueOf(mapId)));
    }
    if (reviewScoreThresholdMinimum != null)
    {
      LiteralInputDefinition inputParamDef = new LiteralInputDefinition();
      LiteralInputDefinition.DataType dataType = new LiteralInputDefinition.DataType();
      inputParamDef.setMinOccurs(new BigInteger("0"));
      inputParamDef.setMaxOccurs(new BigInteger("1"));
      dataType.setReference("http://www.w3.org/TR/xmlschema-2/#double");
      inputParamDef.setDataType(dataType);
      //deegree doesn't have this implemented yet
      /*AllowedValues allowedValues = new AllowedValues();
      allowedValues.getValueOrRange().add(0.01);
      allowedValues.getValueOrRange().add(1.00);
      inputParamDef.setAllowedValues(allowedValues);*/
      inputParamDef.setDefaultValue("0.00");
      allInputs.add(
        WpsUtils.createLiteralInput(
          "reviewScoreThresholdMinimum", reviewScoreThresholdMinimum, inputParamDef));
    }
    if (geospatialBounds != null)
    {
      BoundingBoxInputDefinition inputParamDef = new BoundingBoxInputDefinition();
      inputParamDef.setMinOccurs(new BigInteger("0"));
      inputParamDef.setMaxOccurs(new BigInteger("1"));
      allInputs.add(
        WpsUtils.createBoundingBoxInput(
          "geospatialBounds", new BoundingBox(geospatialBounds), inputParamDef));
    }

    ProcessletInputs in = new ProcessletInputs(allInputs);

    ProcessDefinition def = new ProcessDefinition();
    def.setOutputParameters(new OutputParameters());
    LinkedList<ProcessletOutput> allOutputs = new LinkedList<ProcessletOutput>();

    allOutputs.add(WpsUtils.createLiteralOutput("reviewableItemsStatistics"));

    final ProcessletOutputs out = new ProcessletOutputs(def, allOutputs);

    processlet.process(in, out, new ProcessExecution(null, null, null, null, out));

    return out;
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGet() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());
    ReviewTestUtils.markSomeItemsReviewed();

    final ProcessletOutputs out = execGet(String.valueOf(mapId), null, null);

    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItemsStatistics")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsStatistics response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsStatistics.class);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(54, response.getNumTotalItems());
    Assert.assertEquals(13, response.getNumReviewableItems());
    Assert.assertEquals(3, response.getNumReviewedItems());
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetWithScoreMin() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());
    ReviewTestUtils.markSomeItemsReviewed();

    final ProcessletOutputs out = execGet(String.valueOf(mapId), "0.928", null);

    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItemsStatistics")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsStatistics response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsStatistics.class);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(54, response.getNumTotalItems());
    Assert.assertEquals(5, response.getNumReviewableItems());
    Assert.assertEquals(3, response.getNumReviewedItems());
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetByBounds() throws Exception
  {
    final BoundingBox queryBounds = ReviewTestUtils.createTestQueryBounds();
    ReviewTestUtils.createPreparedData(resource());
    ReviewTestUtils.markSomeItemsReviewed();

    final ProcessletOutputs out =
      execGet(String.valueOf(mapId),null, queryBounds.toServicesString());

    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItemsStatistics")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsStatistics response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsStatistics.class);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(54, response.getNumTotalItems());
    Assert.assertEquals(6, response.getNumReviewableItems());
    Assert.assertEquals(3, response.getNumReviewedItems());
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetNoReviewableData() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    //make a review map and job entry but don't write the reviewed data
    final String jobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(jobId, JOB_STATUS.COMPLETE.toInt(), conn);
    ReviewMap mapReviewInfo = new ReviewMap();
    mapReviewInfo.setMapId(mapId);
    mapReviewInfo.setReviewPrepareJobId(jobId);
    QReviewMap reviewMap = QReviewMap.reviewMap;
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
    .populate(mapReviewInfo).execute();

    final ProcessletOutputs out = execGet(String.valueOf(mapId), null, null);

    //no error should be thrown and there should be an empty set of review items returned
    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItemsStatistics")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsStatistics response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsStatistics.class);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(54, response.getNumTotalItems());
    Assert.assertEquals(0, response.getNumReviewableItems());
    Assert.assertEquals(0, response.getNumReviewedItems());
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetDataNotPrepared() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    try
    {
      execGet(String.valueOf(mapId),null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains(
        "The map with ID: " + mapId + " has not had review data prepared for it."));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.PRECONDITION_FAILED.getStatusCode() + ": " +
          Status.PRECONDITION_FAILED.getReasonPhrase()));

      throw e;
    }
  }

  private void testGetAgainstInvalidJobStatus(final JOB_STATUS jobStatus) throws Exception
  {
    final String jobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(jobId, jobStatus.toInt(), conn);

    ReviewMap mapReviewInfo = new ReviewMap();
    mapReviewInfo.setMapId(mapId);
    mapReviewInfo.setReviewPrepareJobId(jobId);
    QReviewMap reviewMap = QReviewMap.reviewMap;
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
    .populate(mapReviewInfo).execute();
    try
    {
      execGet(String.valueOf(mapId), null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains(
        "The prepare job with ID: " + jobId + " for the map with ID: " + mapId +
        " has a status of " + jobStatus.toString()));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.PRECONDITION_FAILED.getStatusCode() + ": " +
          Status.PRECONDITION_FAILED.getReasonPhrase()));

      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetPrepareJobStillRunning() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.RUNNING);
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetPrepareJobFailed() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.FAILED);
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetPrepareJobUnknown() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.UNKNOWN);
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetMapDoesntExist() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId + 1), null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("No record exists"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.NOT_FOUND.getStatusCode() + ": " + Status.NOT_FOUND.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetMissingMapIdParam() throws Exception
  {
    try
    {
      execGet(null, null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetEmptyMapIdParam() throws Exception
  {
    try
    {
      execGet("", null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetEmptyReviewScoreMinParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), "", null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetInvalidReviewScoreMinParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), "abc", null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetReviewScoreMinParamOutOfRange() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), "1.01", null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("out of the allowable range"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  /*
   * This is a little strange b/c we're catching this from the method that prepares the exec call.
     This should reflect reality, though, b/c the exec call simulates how the processlet is actually
     called in real operation.
   */
  @Test(expected=NumberFormatException.class)
  @Category(IntegrationTest.class)
  public void testGetEmptyBoundsParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, "");
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=Exception.class)
  @Category(IntegrationTest.class)
  public void testGetInvalidBoundsParam() throws Exception
  {
    try
    {
      execGet(
        String.valueOf(mapId), null, "-181,-90,180,90");
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid minimum longitude value"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }
}
