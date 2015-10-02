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
package hoot.services.writers.review;

import java.sql.Connection;
import java.util.UUID;

import org.apache.commons.lang3.StringUtils;
import org.json.simple.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

import hoot.services.db.DbUtils;
import hoot.services.db2.JobStatus;
import hoot.services.db2.QJobStatus;
import hoot.services.db2.QMaps;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewMap;
import hoot.services.job.JobExecutioner;
import hoot.services.job.JobStatusManager.JOB_STATUS;
import hoot.services.models.osm.ModelDaoUtils;

/**
 * Prepares conflated data for review;  This is a wrapper around the process that prepares conflated
 * data for review.  It first determines whether the data actually needs to be prepared.
 */
public class ReviewItemsPreparer
{
  private static final Logger log = LoggerFactory.getLogger(ReviewItemsPreparer.class);

  private Connection conn;

  private long testDelayMilliseconds = 0;
  private boolean simulateFailure = false;

  private long mapId = -1;
  
  private String writer;

  public ReviewItemsPreparer(final Connection conn, final long testDelayMilliseconds,
    final boolean simulateFailure, final String mapId, final String writer) throws Exception
  {
    this.conn = conn;
    this.testDelayMilliseconds = testDelayMilliseconds;
    this.simulateFailure = simulateFailure;
    log.debug("Checking maps table for map with ID: " + mapId + " ...");
    QMaps maps = QMaps.maps;
    this.mapId =
      ModelDaoUtils.getRecordIdForInputString(mapId, conn,
      		maps, maps.id, maps.displayName);
    this.writer = writer;
  }

  /**
   * Prepares conflated data for a given map for review
   *
   * @param overwriteExistingData if true; any existing review data for the map will be overwritten
   * @return a trackable job ID for the prepare operation
   * @throws Exception
   */
  public String prepare(final boolean overwriteExistingData) throws Exception
  {
    String jobId = null;

    //check to see if the map exists in the review table, which means a prepare job has already
    //been run or is running for it
    log.debug("Checking map review table for map with ID: " + String.valueOf(mapId) + " ...");

    QReviewMap reviewMap = QReviewMap.reviewMap;

    //SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

    final ReviewMap mapReviewInfo =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewMap)
    .where(reviewMap.mapId.eq(mapId))
    .singleResult(reviewMap);


    if (mapReviewInfo != null)
    {
      //if the map exists in the review table, gets its associated prepare job status
      log.debug("Map with ID: " + String.valueOf(mapId) + " exists in map review table ...");
      assert(!StringUtils.isEmpty(mapReviewInfo.getReviewPrepareJobId()));
      QJobStatus jobStatusTbl = QJobStatus.jobStatus;
      final JobStatus jobStatusObj =
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(jobStatusTbl)
      .where(jobStatusTbl.jobId.eq(mapReviewInfo.getReviewPrepareJobId()))
      .singleResult(jobStatusTbl);
      switch (JOB_STATUS.fromInteger(jobStatusObj.getStatus()))
      {
        case RUNNING:

          //if a prepare job is currently running, return its job ID; we're not allowing
          //overwriteExistingData to override and cancel a currently running prepare job or try
          //to launch a second job that conflicts with the job already running
          log.debug(
            "Map with ID: " + String.valueOf(mapId) + " has a prepare job currently running " +
            "with ID: " + mapReviewInfo.getReviewPrepareJobId() + "...");
          jobId = String.valueOf(mapReviewInfo.getReviewPrepareJobId());

          break;

        case COMPLETE:

          log.debug(
            "Map with ID: " + String.valueOf(mapId) + " has been prepared for review by " +
            "job with ID: " + mapReviewInfo.getReviewPrepareJobId() + " ...");
          if (!overwriteExistingData)
          {
            //if the map data has been prepared for review, and if overwriteExistingData is set
            //to false, return an empty string for the job ID, which tells the client the data
            //has already been prepared for review.
            log.debug(
              "Overwrite = false, so returning completed job ID: " +
              mapReviewInfo.getReviewPrepareJobId());
            jobId = String.valueOf(mapReviewInfo.getReviewPrepareJobId());
          }

          break;

        case FAILED:

          //if a previous job failed, let's try again
          log.debug(
            "Map with ID: " + String.valueOf(mapId) + " had a previously failed job with " +
            "ID: " + mapReviewInfo.getReviewPrepareJobId());
          jobId = launchPrepareJob(mapId, testDelayMilliseconds, simulateFailure, writer);

          break;

        case UNKNOWN:

          //we don't support an unknown review status
          throw new Exception(
            "Invalid review prepare job status for map with ID: " + String.valueOf(mapId));

        default:
          //nothing
      }
    }

    //If the output job ID hasn't been set by this point, then either no data has ever been
    //prepared for this map, or the data has been prepared, but the user has specified it to be
    //overwritten by this operation... so, kick off a prepare job
    if (jobId == null)
    {
      jobId = launchPrepareJob(mapId, testDelayMilliseconds, simulateFailure, writer);
    }

    return jobId;
  }
  
  /**
   * Launches a review prepare job in a separate thread
   *
   * @param mapId ID of the map for which to launch the prepare job
   * @param testDelayMilliseconds a pause time in milliseconds for testing purposes
   * @param simulateFailure if true; forces the prepare job to throw an exception after it
   * completes; for testing purposes
   * @return a trackable job ID
   */
  public static String launchPrepareJob(final long mapId, final long testDelayMilliseconds,
    final boolean simulateFailure)
  {
	  //Default to the new writer implementation, and have the tests use the old implementation for 
	  //now.
    return launchPrepareJob(mapId, testDelayMilliseconds, simulateFailure, "reviewPrepareDbWriter2");
  }

  /**
   * Launches a review prepare job in a separate thread
   *
   * @param mapId ID of the map for which to launch the prepare job
   * @param testDelayMilliseconds a pause time in milliseconds for testing purposes
   * @param simulateFailure if true; forces the prepare job to throw an exception after it
   * completes; for testing purposes
   * @param writer the review record writer to use
   * @return a trackable job ID
   */
  @SuppressWarnings("unchecked")
  public static String launchPrepareJob(final long mapId, final long testDelayMilliseconds,
    final boolean simulateFailure, final String writer)
  {
    log.debug("Launching review prepare job for map with ID: " + String.valueOf(mapId) + " ...");

    JSONObject command = new JSONObject();
    command.put("mapId", mapId);
    command.put("execImpl", writer);
    if (testDelayMilliseconds > 0)
    {
      command.put("testDelayMilliseconds", testDelayMilliseconds);
    }
    if (simulateFailure)
    {
      command.put("simulateFailure", true);
    }
    final String jobId = UUID.randomUUID().toString();

    (new JobExecutioner(jobId, command)).start();

    return jobId;
  }
}
