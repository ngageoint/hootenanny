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
package hoot.services.validators.review;

import java.sql.Connection;


import hoot.services.db.DbUtils;
import hoot.services.db2.JobStatus;
import hoot.services.db2.QJobStatus;
import hoot.services.db2.QMaps;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewMap;
import hoot.services.job.JobStatusManager.JOB_STATUS;
import hoot.services.models.osm.ModelDaoUtils;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

/**
 * Verifies map data has been prepared for review
 */
public class ReviewMapValidator
{
  private static final Logger log = LoggerFactory.getLogger(ReviewMapValidator.class);

  private Connection conn;

  public ReviewMapValidator(final Connection conn)
  {
    this.conn = conn;
  }

  /**
   * Determines whether a map has been prepared for review
   * 
   * @param mapIdStr map ID; may be a map ID or unique map name
   * @return true if the map has been prepared; false otherwise
   */
  public boolean mapIsPrepared(final String mapIdStr)
  {
  	boolean prepared = false;
  	try
  	{
  		verifyMapPrepared(mapIdStr);
  		prepared = true;
  	}
  	catch (Exception e)
  	{
  	}
  	return prepared;
  }
  
  /**
   * Determines whether a maps data has been prepared for review; throws when false
   *
   * @param mapIdStr map ID; may be a map ID or unique map name
   * @return the map's numeric ID
   * @throws Exception if the map has not had its data prepared for review or its prepare job is
   * in state other than complete
   */
  public long verifyMapPrepared(final String mapIdStr) throws Exception
  {
    log.debug("Checking maps table for map with ID: " + mapIdStr + " ...");
    //this will throw if it doesn't find the map
    QMaps maps = QMaps.maps;
    final long mapIdNum = ModelDaoUtils.getRecordIdForInputString(mapIdStr, conn,
    		maps, maps.id, maps.displayName);

    //Check to see if the map exists in the review_map table.  If not, error out (404).
    log.debug("Checking map review table for map with ID: " + String.valueOf(mapIdNum) + " ...");

    QReviewMap reviewMap = QReviewMap.reviewMap;

    final ReviewMap mapReviewInfo =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapIdStr)).from(reviewMap)
    .where(reviewMap.mapId.eq(mapIdNum))
    .singleResult(reviewMap);
    if (mapReviewInfo == null)
    {
      throw new Exception("The map with ID: " + String.valueOf(mapIdNum) + " has not had " +
        "review data prepared for it.");
    }

    //Check to see if the map has been prepared for review by checking to see that the prepare job
    //associated with it has completed (412).
    log.debug("Map with ID: " + String.valueOf(mapIdNum) + " exists in map review table ...");

    QJobStatus jobStatusTbl = QJobStatus.jobStatus;
    assert(!StringUtils.isEmpty(mapReviewInfo.getReviewPrepareJobId()));
    final JobStatus jobStatusObj =
    	new SQLQuery(conn, DbUtils.getConfiguration(mapIdStr)).from(jobStatusTbl)
       .where(jobStatusTbl.jobId.eq(mapReviewInfo.getReviewPrepareJobId()))
       .singleResult(jobStatusTbl);
    final JOB_STATUS prepareJobStatus = JOB_STATUS.fromInteger(jobStatusObj.getStatus());
    if (!prepareJobStatus.equals(JOB_STATUS.COMPLETE))
    {
      throw new Exception("The prepare job with ID: " + mapReviewInfo.getReviewPrepareJobId() +
        " for the map with ID: " + String.valueOf(mapIdNum) + " has a status of " +
        prepareJobStatus.toString() + ".  The prepare job must complete before attempting to " +
        "retrieve items for review.");
    }

    return mapIdNum;
  }
}
