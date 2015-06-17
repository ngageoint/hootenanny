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
 * @copyright Copyright (C) 2014, 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.review;

import java.sql.Connection;

import hoot.services.db.DbUtils;
import hoot.services.db2.QMaps;
import hoot.services.models.osm.Changeset;
import hoot.services.models.review.MarkItemsReviewedRequest;
import hoot.services.models.review.MarkItemsReviewedResponse;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.validators.osm.ChangesetUploadXmlValidator;
import hoot.services.validators.review.ReviewMapValidator;
import hoot.services.writers.osm.ChangesetDbWriter;
import hoot.services.writers.review.ReviewedItemsWriter;

import org.apache.commons.lang3.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import com.mysema.query.sql.SQLQuery;

/**
 * Marks reviewable items as reviewed;  This is a wrapper around the process of auto
 * creating/closing a changeset and marking items as reviewed.
 */
public class ReviewItemsMarker
{
  private static final Logger log = LoggerFactory.getLogger(ReviewItemsMarker.class);

  private long mapId;
  private long userId;
  private Connection conn;

  public ReviewItemsMarker(final Connection conn, final String mapId) throws Exception
  {
    this.conn = conn;
    //Check to see if the map exists in the maps table. (404); input mapId may be a map ID or a
    //map name; this will throw if it doesn't find the map
    log.debug("Checking maps table for map with ID: " + mapId + " ...");
    this.mapId = (new ReviewMapValidator(conn)).verifyMapPrepared(mapId);
    //now make sure the user associated with the map exists;
    try
    {
      assert(this.mapId != -1);
      log.debug(
        "Retrieving user ID associated with map having ID: " + String.valueOf(this.mapId) + " ...");

      QMaps maps = QMaps.maps;
      //SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());

      userId =
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(maps)
      .where(maps.id.eq(this.mapId))
      .singleResult(maps.userId);

      log.debug("Retrieved user ID: " + userId);
    }
    catch (Exception e)
    {
      throw new Exception("Error locating user associated with map with ID: " + this.mapId);
    }
  }

  /**
   * Uploads an OSM changeset with items marked as reviewed to the services database and then parses
   * the contents of the changeset, marking items as reviewed based on custom hoot review tags it
   * contains
   *
   * @param markItemsReviewedRequest a request to mark items as reviewed which contains an
   * object describing the items to be reviewed, as well as an optional OSM xml changeset; the two
   * sets of data are not cross validated with each other in any way
   * @param markAll an option to mark all data for the map layer as reviewed; when true, the
   * reviewed items object is no needed and ignored if populated
   * @return an mark items as reviewed response
   * @throws Exception
   */
  public MarkItemsReviewedResponse markItemsReviewed(
    final MarkItemsReviewedRequest markItemsReviewedRequest, final boolean markAll)
    throws Exception
  {
    MarkItemsReviewedResponse markItemsReviewedResponse = new MarkItemsReviewedResponse();

    Document changesetUploadResponse = null;
    long changesetId = -1;
    if (StringUtils.trimToNull(markItemsReviewedRequest.getReviewedItemsChangeset()) != null)
    {
      boolean changesetHasElements = false;
      final Document changesetDiffDoc =
        XmlDocumentBuilder.parse(markItemsReviewedRequest.getReviewedItemsChangeset());
      changesetHasElements = ChangesetUploadXmlValidator.changesetHasElements(changesetDiffDoc);
      if (changesetHasElements)
      {
        changesetId =
          Changeset.createChangeset(
            Changeset.getChangesetCreateDoc("marking items reviewed for map ID: " + mapId),
            mapId,
            userId,
            conn);
        //TODO: There really needs to also be a check in here that makes sure every element tag
        //already has a changeset attribute (or iterate through the element tag DOM attributes.
        //For now, just assuming that iD has already added the changeset attributes.  Whether the
        //value is empty or not doesn't matter, since it will be overwritten here.
        markItemsReviewedRequest.setReviewedItemsChangeset(
          markItemsReviewedRequest.getReviewedItemsChangeset()
            .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\"")
            .replaceAll("changeset=\"\\d+\"", "changeset=\"" + changesetId + "\""));
        //the changeset upload process will catch any elements in the changeset xml which are out
        //of sync with the element versions in the OSM element tables, by design
        changesetUploadResponse =
          (new ChangesetDbWriter(conn)).write(mapId,
            changesetId, markItemsReviewedRequest.getReviewedItemsChangeset());
        Changeset.closeChangeset(mapId, changesetId, conn);

        markItemsReviewedResponse.setChangesetUploadResponse(
          XmlDocumentBuilder.toString(changesetUploadResponse));
      }
    }

    //mark all items as reviewed in review_items; record the changeset ID and other review
    //details
    if (markAll)
    {
      markItemsReviewedRequest.setReviewedItems(
        ReviewUtils.getReviewedItemsCollectionForAllRecords(mapId, conn));
    }
    final int numItemsMarkedReviewed =
      (new ReviewedItemsWriter(conn, mapId, changesetId)).writeReviewedItems(
        markItemsReviewedRequest.getReviewedItems());

    //this will still be = -1 at this point if nothing was in the changeset input parameter or it
    //was invalid
    markItemsReviewedResponse.setChangesetId(changesetId);
    //this will null at this point if nothing was in the changeset input parameter or it was invalid
    markItemsReviewedResponse.setNumItemsMarkedReviewed(numItemsMarkedReviewed);
    return markItemsReviewedResponse;
  }
}
