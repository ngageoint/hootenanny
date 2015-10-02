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
import java.sql.Timestamp;
import java.util.LinkedList;
import java.util.List;
import java.util.Properties;
import java.util.UUID;

import javax.ws.rs.core.Response.Status;

import org.junit.Assert;
import org.apache.commons.lang3.StringUtils;
import org.deegree.services.wps.ProcessExecution;
import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.input.ProcessletInput;
import org.deegree.services.wps.output.LiteralOutputImpl;
import org.deegree.services.wps.output.ProcessletOutput;
import org.deegree.process.jaxb.java.LiteralInputDefinition;
import org.deegree.process.jaxb.java.ProcessDefinition;
import org.deegree.process.jaxb.java.ProcessDefinition.OutputParameters;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;

import hoot.services.HootProperties;
import hoot.services.IntegrationTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.JobStatus;
import hoot.services.db2.QJobStatus;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewMap;

import hoot.services.job.JobStatusWebPoller;
import hoot.services.job.JobStatusManager.JOB_STATUS;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.wps.WpsUtils;

public class PrepareItemsForReviewProcessletTest extends OsmResourceTestAbstract
{
  @SuppressWarnings("unused")
  private static final Logger log =
    LoggerFactory.getLogger(PrepareItemsForReviewProcessletTest.class);

  public PrepareItemsForReviewProcessletTest() throws NumberFormatException, IOException
  {
    super("hoot.services.controllers.job");
  }

  private ProcessletOutputs runPrepareJob(final String mapId, final String overwrite)
    throws Exception
  {
    return runPrepareJob(mapId, overwrite, false);
  }

  /*
   * This tests mimics the behavior defined in
     $HOOT_HOME/hoot-services/src/main/src/main/java/hoot/services/controllers/review/PrepareItemsForReviewProcesslet.java
     So, any changes to that definition must also be reflected in this method and vice versa.
   */
  private ProcessletOutputs runPrepareJob(final String mapId, final String overwrite,
    final boolean simulateFailure) throws Exception
  {
    PrepareItemsForReviewProcesslet processlet = new PrepareItemsForReviewProcesslet();
    PrepareItemsForReviewProcesslet.reviewRecordWriter = "reviewPrepareDbWriter";

    LinkedList<ProcessletInput> allInputs = new LinkedList<ProcessletInput>();
    //I don't think not adding the input params when they aren't specified is the correct
    //behavior here.  I believe that the WPS framework will fill any missing values with defaults
    //for optional params and fail early for missing required params.
    if (mapId != null)
    {
      //default data type for a wps input is string, so no extra params needed here for mapId
      allInputs.add(WpsUtils.createLiteralInput("mapId", mapId));
    }
    if (overwrite != null)
    {
      LiteralInputDefinition inputParamDef = new LiteralInputDefinition();
      inputParamDef.setMinOccurs(new BigInteger("0"));
      inputParamDef.setMaxOccurs(new BigInteger("1"));
      LiteralInputDefinition.DataType dataType = new LiteralInputDefinition.DataType();
      dataType.setReference("http://www.w3.org/TR/xmlschema-2/#boolean");
      inputParamDef.setDataType(dataType);
      inputParamDef.setDefaultValue("false");
      allInputs.add(WpsUtils.createLiteralInput("overwriteExistingData", overwrite, inputParamDef));
    }
    if (simulateFailure)
    {
      allInputs.add(WpsUtils.createLiteralInput("simulateFailure", String.valueOf(true)));
    }
    allInputs.add(
      WpsUtils.createLiteralInput("testDelayMilliseconds", String.valueOf(TEST_JOB_DELAY_MS)));
    ProcessletInputs in = new ProcessletInputs(allInputs);

    ProcessDefinition def = new ProcessDefinition();
    def.setOutputParameters(new OutputParameters());
    LinkedList<ProcessletOutput> allOutputs = new LinkedList<ProcessletOutput>();
    allOutputs.add(WpsUtils.createLiteralOutput("jobId"));
    final ProcessletOutputs out = new ProcessletOutputs(def, allOutputs);

    processlet.process(in, out, new ProcessExecution(null, null, null, null, out));

    return out;
  }

  @Test
  @Category(IntegrationTest.class)
  public void testPrepare() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    final ProcessletOutputs out = runPrepareJob(String.valueOf(mapId), String.valueOf(false));
    final String jobId = ((LiteralOutputImpl)out.getParameter("jobId")).getValue();
    Assert.assertFalse(StringUtils.isEmpty(jobId));

    (new JobStatusWebPoller(resource(), conn)).pollJobStatusUntilCompleteOrFail(jobId, false);

    ReviewTestUtils.jobId = jobId;
    ReviewTestUtils.verifyDataPrepared(ReviewTestUtils.mapId);
  }

  private void testPrepareOverwrite(final boolean createPreviouslyPreparedData) throws Exception
  {
  	QJobStatus jobStatus = QJobStatus.jobStatus;

    String previousJobId = null;
    Timestamp previousJobEndTime = null;
    if (createPreviouslyPreparedData)
    {
      testPrepare();
      previousJobId = ReviewTestUtils.jobId;
      final JobStatus previousJobStatus =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatus)
      .where(jobStatus.jobId.eq(previousJobId))
      .singleResult(jobStatus);
      previousJobEndTime = previousJobStatus.getEnd();
    }
    else
    {
      ReviewTestUtils.createDataToPrepare();
    }

    final ProcessletOutputs out = runPrepareJob(String.valueOf(mapId), String.valueOf(true));
    final String jobId = ((LiteralOutputImpl)out.getParameter("jobId")).getValue();
    Assert.assertFalse(StringUtils.isEmpty(jobId));
    if (createPreviouslyPreparedData)
    {
      Assert.assertNotEquals(previousJobId, jobId);
    }

    (new JobStatusWebPoller(resource(), conn)).pollJobStatusUntilCompleteOrFail(jobId, false);

    //verify a new job was run
    final JobStatus newJobStatus = //jobStatusDao.fetchOneByJobId(jobId);
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatus)
        .where(jobStatus.jobId.eq(jobId))
        .singleResult(jobStatus);
    Assert.assertEquals(JOB_STATUS.COMPLETE.toInt(), (int)newJobStatus.getStatus());
    if (createPreviouslyPreparedData)
    {
      Assert.assertTrue(newJobStatus.getEnd().after(previousJobEndTime));
    }

    ReviewTestUtils.jobId = jobId;
    ReviewTestUtils.verifyDataPrepared(mapId);
  }

  /*
   * If data has been previously prepared for a given map ID, and overwrite = true, the previous
   * data should be overwritten by a new job.
   */
  @Test
  @Category(IntegrationTest.class)
  public void testPrepareOverwriteDataExists() throws Exception
  {
    testPrepareOverwrite(true);
  }

  /*
   * If no data has been previously prepared for a given map ID, regardless of the value of
   * overwrite, a new prepared job should be launched.
   */
  @Test
  @Category(IntegrationTest.class)
  public void testPrepareOverwriteNoPreparedDataExists() throws Exception
  {
    testPrepareOverwrite(false);
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testPrepareInvalidMapIdParam() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    final long fakeMapId = mapId + 2;
    try
    {
      runPrepareJob(String.valueOf(fakeMapId), String.valueOf(false));
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("No record exists"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.NOT_FOUND.getStatusCode() + ": " + Status.NOT_FOUND.getReasonPhrase()));
      ReviewTestUtils.verifyDataNotPrepared(fakeMapId);

      throw e;
    }
  }

  /*
   * currently throwing an exception if a map has no data available for review (no elements with
   * review tags)
   */
  @Test
  @Category(IntegrationTest.class)
  public void testPrepareNoDataAvailableForReview() throws Exception
  {
  	QJobStatus jobStatusTbl = QJobStatus.jobStatus;
    final ProcessletOutputs out = runPrepareJob(String.valueOf(mapId), String.valueOf(false));
    final String jobId = ((LiteralOutputImpl)out.getParameter("jobId")).getValue();
    Assert.assertFalse(StringUtils.isEmpty(jobId));

    (new JobStatusWebPoller(resource(), conn)).pollJobStatusUntilCompleteOrFail(jobId, false);

    //job should have failed
    JobStatus jobStatus =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
        .where(jobStatusTbl.jobId.eq(jobId))
        .singleResult(jobStatusTbl);
    Assert.assertEquals(JOB_STATUS.COMPLETE.toInt(), (int)jobStatus.getStatus());
    //status detail should mention the warning that no records were available for review
    Assert.assertTrue(jobStatus.getStatusDetail().contains("No records available for review"));

    QReviewMap reviewMap = QReviewMap.reviewMap;
    Assert.assertEquals(
      1,
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewMap)
      .where(reviewMap.mapId.eq(mapId))
      .count()


    		);

    //TODO: not convenient to test this if the services test aren't clearing there data out betwee
    //each run; see #6389
    /*QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;
    QReviewItems reviewItems = QReviewItems.reviewItems;
    Assert.assertEquals(
      0,
      (
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(elementIdMappings).count() +
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItems).count()
      		));*/
  }

  private void testPreviouslyExecutedJob(final JOB_STATUS previouslyExecutedJobStatus,
    final boolean overwrite) throws Exception
  {
  	QJobStatus jobStatusTbl = QJobStatus.jobStatus;
    //simulate the prepared data output of a job already run with the same map ID as the job
    //we're about to execute; this is a little inefficient, but the easiest way to do write this
    //prepared data
    testPrepare();
    final String previousJobId = ReviewTestUtils.jobId;
    if (!previouslyExecutedJobStatus.equals(JOB_STATUS.COMPLETE))
    {
      //change the prepared job status
      JobStatus jobStatus =
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
          .where(jobStatusTbl.jobId.eq(previousJobId))
          .singleResult(jobStatusTbl);
      jobStatus.setStatus(previouslyExecutedJobStatus.toInt());

      new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), jobStatusTbl)
      .where(jobStatusTbl.jobId.eq(jobStatus.getJobId()))
      .populate(jobStatus)
      .execute();

      jobStatus =
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
          .where(jobStatusTbl.jobId.eq(previousJobId))
          .singleResult(jobStatusTbl);
      Assert.assertEquals(previouslyExecutedJobStatus.toInt(), (int)jobStatus.getStatus());
    }

    final ProcessletOutputs out = runPrepareJob(String.valueOf(mapId), String.valueOf(overwrite));
    //we don't need to poll job status here, but we still need to give the job thread enough time
    //to create a job record
    Thread.sleep(TEST_JOB_DELAY_MS + 2000);
    final String jobId = ((LiteralOutputImpl)out.getParameter("jobId")).getValue();

    QReviewMap reviewMap = QReviewMap.reviewMap;
    if (previouslyExecutedJobStatus.equals(JOB_STATUS.RUNNING) ||
        (previouslyExecutedJobStatus.equals(JOB_STATUS.COMPLETE) && !overwrite))
    {
      //no new job should have been created
      Assert.assertEquals(previousJobId, jobId);
      List<JobStatus> jobStatuses =
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
          .where(jobStatusTbl.jobId.eq(previousJobId))
          .list(jobStatusTbl);
      Assert.assertEquals(1, jobStatuses.size());
      Assert.assertEquals(previousJobId, jobStatuses.get(0).getJobId());
      Assert.assertEquals(
        previouslyExecutedJobStatus, JOB_STATUS.fromInteger(jobStatuses.get(0).getStatus()));
      //the review map table should still be have a record for the previous job id
      Assert.assertEquals(
        previousJobId, /*reviewMapDao.fetchOneByMapId(mapId).getReviewPrepareJobId()*/
        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewMap)
        .where(reviewMap.mapId.eq(mapId))
        .singleResult(reviewMap.reviewPrepareJobId)
      		);
      //go ahead and set the previous job status to complete, so that verifyDataPrepared doesn't
      //fail
      JobStatus jobStatus = 
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
          .where(jobStatusTbl.jobId.eq(previousJobId))
          .singleResult(jobStatusTbl);
      jobStatus.setStatus(JOB_STATUS.COMPLETE.toInt());

      new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), jobStatusTbl)
      .where(jobStatusTbl.jobId.eq(jobStatus.getJobId()))
      .populate(jobStatus)
      .execute();

      jobStatus = 
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
          .where(jobStatusTbl.jobId.eq(previousJobId))
          .singleResult(jobStatusTbl);
      Assert.assertEquals(JOB_STATUS.COMPLETE.toInt(), (int)jobStatus.getStatus());
      //the data should remain prepared from the previous job
      ReviewTestUtils.verifyDataPrepared(mapId);
    }
    else
    {
      //a new job should have been created
      Assert.assertNotEquals(previousJobId, jobId);
      //maybe not the best check here
      Assert.assertTrue(
        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl).count() >= 2);
      //the old job record should still exist
      Assert.assertNotNull(new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
          .where(jobStatusTbl.jobId.eq(previousJobId))
          .singleResult(jobStatusTbl));
      JobStatus newJob = 
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
          .where(jobStatusTbl.jobId.eq(jobId))
          .singleResult(jobStatusTbl);
      Assert.assertNotNull(newJob);
      Assert.assertEquals(jobId, newJob.getJobId());
      Assert.assertEquals(JOB_STATUS.COMPLETE, JOB_STATUS.fromInteger(newJob.getStatus()));
      //the review map table should still be linked to the new job id
      Assert.assertEquals(jobId, new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewMap)
          .where(reviewMap.mapId.eq(mapId))
          .singleResult(reviewMap.reviewPrepareJobId));
      //manually set the job ID param on ReviewTestUtils so that verifyDataPrepared doesn't fail
      ReviewTestUtils.jobId = jobId;
      //the data should be prepared from the new job
      ReviewTestUtils.verifyDataPrepared(mapId);
    }
  }

  /*
   * Regardless of the value set for overwrite, if a prepare job is currently running when a new
   * one is attempted to be launched, the job ID of the existing job should be returned and no
   * new job launched.
   */
  @Test
  @Category(IntegrationTest.class)
  public void testPrepareJobAlreadyRunning() throws Exception
  {
    testPreviouslyExecutedJob(JOB_STATUS.RUNNING, false);
  }

  /*
   * see testPrepareJobAlreadyRunningWithOverwrite
   */
  @Test
  @Category(IntegrationTest.class)
  public void testPrepareJobAlreadyRunningWithOverwrite() throws Exception
  {
    testPreviouslyExecutedJob(JOB_STATUS.RUNNING, true);
  }

  /*
   * If a prepare job has already completed and overwrite = false, the job ID of the completed job
   * should be returned.
   */
  @Test
  @Category(IntegrationTest.class)
  public void testPrepareJobAlreadyCompleted() throws Exception
  {
    testPreviouslyExecutedJob(JOB_STATUS.COMPLETE, false);
  }

  /*
   * If a prepare job has already completed and overwrite = true, a new job should be launched that
   * overwrites the existing prepared data, and the job ID of the new job should be returned.
   */
  @Test
  @Category(IntegrationTest.class)
  public void testPrepareJobAlreadyCompletedWithOverwrite() throws Exception
  {
    testPreviouslyExecutedJob(JOB_STATUS.COMPLETE, true);
  }

  /*
   * If a prepare job ran previously and failed, we launch a new one.
   */
  @Test
  @Category(IntegrationTest.class)
  public void testPrepareJobPreviouslyFailed() throws Exception
  {
  	QJobStatus jobStatusTbl = QJobStatus.jobStatus;
    final String jobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(jobId, JOB_STATUS.FAILED.toInt(), conn);
    ReviewMap mapReviewInfo = new ReviewMap();
    mapReviewInfo.setMapId(mapId);
    mapReviewInfo.setReviewPrepareJobId(jobId);
    QReviewMap reviewMap = QReviewMap.reviewMap;
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
    .populate(mapReviewInfo).execute();

    testPrepare();

    //maybe not the best check here
    Assert.assertTrue(
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl).count() >= 2);
    List<JobStatus> failedJobs =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
        .where(jobStatusTbl.status.eq(JOB_STATUS.FAILED.toInt()))
        .list(jobStatusTbl);
    //probably too lax
    Assert.assertTrue(failedJobs.size() >= 1);
    List<JobStatus> completedJobs =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
        .where(jobStatusTbl.status.eq(JOB_STATUS.COMPLETE.toInt()))
        .list(jobStatusTbl);
    //probably too lax
    Assert.assertTrue(completedJobs.size() >= 1);
    Assert.assertNotEquals(failedJobs.get(0).getJobId(), completedJobs.get(0).getJobId());
  }

  /*
   * We fail on finding previously run jobs with unknown status.
   */
  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testPrepareJobStatusUnknown() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    final String jobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(jobId, JOB_STATUS.UNKNOWN.toInt(), conn);
    ReviewMap mapReviewInfo = new ReviewMap();
    mapReviewInfo.setMapId(mapId);
    mapReviewInfo.setReviewPrepareJobId(jobId);

    QReviewMap reviewMap = QReviewMap.reviewMap;
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
    .populate(mapReviewInfo).execute();
    try
    {
      runPrepareJob(String.valueOf(mapId), String.valueOf(false));
    }
    catch (ProcessletException e)
    {
      ReviewTestUtils.verifyDataNotPrepared(mapId, true);
      Assert.assertTrue(e.getOWSException().getMessage().contains(
        "Invalid review prepare job status for map with ID: " + mapId));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.PRECONDITION_FAILED.getStatusCode() + ": " +
          Status.PRECONDITION_FAILED.getReasonPhrase()));

      throw e;
    }
  }

  /*
   * There's no good way to simulate a failure during data parsing and test the transaction
   * rollback, since ConflatedDataReviewPreparer handles all the bad data test cases I can think
   * of so far.  So, I'm passing in a parameter that forces it to fail after parsing all the data.
   * None of the prepared review data should be written to the database.
   */
  @Test
  @Category(IntegrationTest.class)
  public void testFailureParsingData() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    final ProcessletOutputs out = runPrepareJob(String.valueOf(mapId), String.valueOf(false), true);

    final String jobId = ((LiteralOutputImpl)out.getParameter("jobId")).getValue();
    Assert.assertFalse(StringUtils.isEmpty(jobId));

    (new JobStatusWebPoller(resource(), conn)).pollJobStatusUntilCompleteOrFail(jobId, true);

    ReviewTestUtils.verifyDataNotPrepared(mapId);
  }

  /*
   * @todo This isn't actually testing that paging works correctly while parsing, but just
   * to see if it the prepare job still succeeds when paging logic kicks in.  A class level unit
   * test against ConflatedDataReviewPreparer needs to be written to test this properly.
   */
  @Test
  @Category(IntegrationTest.class)
  public void testParseReadPaging() throws Exception
  {
    Properties hootProps = null;
    try
    {
      hootProps = HootProperties.getInstance();
      //set this to something lower than the size of the test data to be prepared per element type
      hootProps.setProperty("maxRecordBatchSize", "2");
      HootProperties.setProperties(hootProps);
      final long maxRecordBatchSize = 2;
      Assert.assertEquals(
      	maxRecordBatchSize,
        Integer.parseInt(HootProperties.getInstance().getProperty("maxRecordBatchSize")));

      testPrepare();
    }
    finally
    {
      //set this back to default now that this test is over
      if (hootProps != null)
      {
        hootProps.setProperty("maxRecordBatchSize", HootProperties.getDefault("maxRecordBatchSize"));
        HootProperties.setProperties(hootProps);
        Assert.assertEquals(
          Integer.parseInt(HootProperties.getDefault("maxRecordBatchSize")),
          Integer.parseInt(hootProps.getProperty("maxRecordBatchSize")));
      }
    }
  }

  /*
   * @todo This isn't actually testing that paging works correctly while parsing, but just
   * to see if it the prepare job still succeeds when paging logic kicks in.  A class level unit
   * test against ConflatedDataReviewPreparer needs to be written to test this properly.
   */
  @Test
  @Category(IntegrationTest.class)
  public void testParseWritePaging() throws Exception
  {
    Properties hootProps = null;
    try
    {
      hootProps = HootProperties.getInstance();
      //set this to something lower than the size of the reviewable test data to be prepared per
      //element type
      hootProps.setProperty("maxRecordBatchSize", "1");
      HootProperties.setProperties(hootProps);
      final long maxRecordBatchSize = 1;
      Assert.assertEquals(
        maxRecordBatchSize,
        Integer.parseInt(HootProperties.getInstance().getProperty("maxRecordBatchSize")));

      testPrepare();
    }
    finally
    {
      //set this back to default now that this test is over
      if (hootProps != null)
      {
        hootProps.setProperty("maxRecordBatchSize", HootProperties.getDefault("maxRecordBatchSize"));
        HootProperties.setProperties(hootProps);
        Assert.assertEquals(
          Integer.parseInt(HootProperties.getDefault("maxRecordBatchSize")),
          Integer.parseInt(hootProps.getProperty("maxRecordBatchSize")));
      }
    }
  }

  @Test
  @Category(IntegrationTest.class)
  public void testPrepareElementTypeWithNoTags() throws Exception
  {
    //Because of the way the test data has been set up, the only test of this type that will work
    //is the one which doesn't write way tags.  Trying to not write node or relation tags will result
    //in failure b/c both of them reference each other.  This is ok b/c in a real dataset, such a
    //situation shouldn't arise, and I just want to make sure that if there is no review data
    //available for some element type that the review job will still complete.
    ReviewTestUtils.createDataToPrepare();

    final ProcessletOutputs out = runPrepareJob(String.valueOf(mapId), String.valueOf(false));
    final String jobId = ((LiteralOutputImpl)out.getParameter("jobId")).getValue();
    Assert.assertFalse(StringUtils.isEmpty(jobId));

    (new JobStatusWebPoller(resource(), conn)).pollJobStatusUntilCompleteOrFail(jobId, false);

    //match signature used with createDataToPrepare
    ReviewTestUtils.jobId = jobId;
    ReviewTestUtils.verifyDataPrepared(ReviewTestUtils.mapId);
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testPrepareMissingMapIdParam() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    try
    {
      runPrepareJob(null, String.valueOf(false));
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));

      ReviewTestUtils.verifyDataNotPrepared(mapId);

      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testPrepareEmptyMapIdParam() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    try
    {
      runPrepareJob("", String.valueOf(false));
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));

      ReviewTestUtils.verifyDataNotPrepared(mapId);

      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testPrepareInvalidOverwriteParam() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    try
    {
      runPrepareJob(String.valueOf(mapId), "blah");
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));

      ReviewTestUtils.verifyDataNotPrepared(mapId);

      throw e;
    }
  }

  @Test
  @Category(IntegrationTest.class)
  public void testPrepareMissingOverwriteParam() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    final ProcessletOutputs out = runPrepareJob(String.valueOf(mapId), null);
    final String jobId = ((LiteralOutputImpl)out.getParameter("jobId")).getValue();
    Assert.assertFalse(StringUtils.isEmpty(jobId));

    (new JobStatusWebPoller(resource(), conn)).pollJobStatusUntilCompleteOrFail(jobId, false);

    ReviewTestUtils.jobId = jobId;
    ReviewTestUtils.verifyDataPrepared(mapId);
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testPrepareEmptyOverwriteParam() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    try
    {
      runPrepareJob(String.valueOf(mapId), "");
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));

      ReviewTestUtils.verifyDataNotPrepared(mapId);

      throw e;
    }
  }
}
