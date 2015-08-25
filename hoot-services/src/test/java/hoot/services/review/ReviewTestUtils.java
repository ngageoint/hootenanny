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
package hoot.services.review;

import java.io.File;
import java.io.IOException;
import java.sql.Connection;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpressionException;

import org.apache.commons.io.FileUtils;
import org.apache.commons.lang3.StringUtils;

import org.junit.Assert;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.sun.jersey.api.client.WebResource;

import hoot.services.HootProperties;
import hoot.services.db.DbUtils;
import hoot.services.db2.Changesets;
import hoot.services.db2.ElementIdMappings;
import hoot.services.db2.QChangesets;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QElementIdMappings;
import hoot.services.db2.QReviewItems;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewItems;
import hoot.services.db2.ReviewMap;
import hoot.services.geo.BoundingBox;
import hoot.services.job.JobStatusWebPoller;
import hoot.services.job.JobStatusManager.JOB_STATUS;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element;
import hoot.services.models.osm.ElementFactory;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.review.MarkItemsReviewedRequest;
import hoot.services.models.review.ReviewedItem;
import hoot.services.models.review.ReviewedItems;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.writers.osm.ChangesetDbWriter;

/*
 * The methods in this class are based off a handpicked test scenario.  The files in
 * src/main/resources/hoot/services/review are used to populate the test data.
 */
public class ReviewTestUtils
{
  @SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewTestUtils.class);

  public static Connection conn;

  public static long mapId = -1;
  public static long secondMapId = -1;
  public static long userId = -1;
  public static long changesetId = -1;
  public static long secondChangesetId = -1;
  //TODO: the handling of job ID's by this class is a little confusing and inconsistent
  public static String jobId;
  public static String secondJobId;

  //maps old element ID's (ID's from creation request) to new element ID's
  public static Map<Long, Long> nodeIds = new HashMap<Long, Long>();
  public static Map<Long, Long> wayIds = new HashMap<Long, Long>();
  public static Map<Long, Long> relationIds = new HashMap<Long, Long>();

  private static void populateOsmIdMappings(final Document changesetUploadResponse,
    final ElementType elementType, Map<Long, Long> mappings, final int size)
    throws XPathExpressionException
  {
    XPath xpath = XmlDocumentBuilder.createXPath();
    mappings.clear();

    //there aren't more than 100 elements of any type in the source file
    for (int i = 1; i < size + 1; i++)
    {
      final long oldId = -1 * i;
      final String newIdStr =
        xpath.evaluate(
          "//osm/diffResult/" + elementType.toString().toLowerCase() +
            "[@old_id=\"" + oldId + "\"]/@new_id", changesetUploadResponse);
      if (StringUtils.trimToNull(newIdStr) != null)
      {
        final long newId = Long.parseLong(newIdStr);
        mappings.put(oldId, newId);
      }
    }
  }

  public static void createDataToPrepare() throws Exception
  {
  	//conn = DbUtils.createConnection();
    //write the reviewable data to the OSM tables
    changesetId = Changeset.insertNew(mapId, userId, conn);
    ChangesetDbWriter elementWriter = new ChangesetDbWriter(conn);
    final Document response =
      elementWriter.write(
      		mapId,
        changesetId,
        FileUtils.readFileToString(
          new File(
            Thread.currentThread().getContextClassLoader().getResource(
              "hoot/services/review/review-create.osm")
            .getPath()))
        .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\""));

    //we're not testing the changeset service here, so no actual validation of the response is being
    //done

    //collect only the written new element ID's that had review tags
    populateOsmIdMappings(response, ElementType.Node, nodeIds, 76);
    populateOsmIdMappings(response, ElementType.Way, wayIds, 44);
    populateOsmIdMappings(response, ElementType.Relation, relationIds, 3);

    //create a second map with the same data to ensure the correct map's data is being
    //modified throughout the tests (this map's data should never be modified)
    secondMapId = DbUtils.insertMap(userId, conn);
    secondChangesetId = Changeset.insertNew(secondMapId, userId, conn);
    elementWriter.write(
    		secondMapId,
      secondChangesetId,
      FileUtils.readFileToString(
        new File(
          Thread.currentThread().getContextClassLoader().getResource(
            "hoot/services/review/review-create.osm")
          .getPath()))
      .replaceAll("changeset=\"\"", "changeset=\"" + secondChangesetId + "\""));
  }

  private static void verifyElementIdMappings()
  {
    SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration(mapId));
    QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;

    final Map<String, ElementIdMappings> idMappings =
    query.from(elementIdMappings)
    	.where(elementIdMappings.mapId.eq(mapId))
    	.map(elementIdMappings.elementId, elementIdMappings);

    Assert.assertEquals(15, idMappings.size());

    //nodes

    ElementIdMappings elementIdMapping =
      idMappings
        .get("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(DbUtils.nwr_enum.node, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{99b917d0-8c23-4469-b028-b749a38a8841}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(nodeIds.get((long)-71), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.node, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{5c131c00-a30f-11e3-a5e2-0800200c9a66}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(nodeIds.get((long)-65), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.node, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{de285618-a4df-4536-b218-5009c25f7107}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(nodeIds.get((long)-67), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.node, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    //ways

    elementIdMapping =
      idMappings
        .get("{46d98769-dee2-489e-8fd6-ff265842ed55};{28fe6088-cb30-45a2-88dd-1146092dcd0e}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(wayIds.get((long)-43), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.way, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{46d98769-dee2-489e-8fd6-ff265842ed55};{f917b7e1-412b-4294-acdc-37bd0fb13b4c}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(wayIds.get((long)-42), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.way, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{46d98769-dee2-489e-8fd6-ff265842ed55};{dfcd4a0e-04d5-46ee-b7a0-1a2c591ddf54}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(wayIds.get((long)-41), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.way, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{46d98769-dee2-489e-8fd6-ff265842ed55};{81d2aff0-88e5-480e-95cb-c502d120f51f}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(wayIds.get((long)-40), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.way, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{81d2aff0-88e5-480e-95cb-c502d120f51f}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(wayIds.get((long)-36), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.way, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{46d98769-dee2-489e-8fd6-ff265842ed55};{f5639540-aaf4-47ac-8c5e-82ad05abc29f}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(wayIds.get((long)-24), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.way, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{f5639540-aaf4-47ac-8c5e-82ad05abc29f}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(wayIds.get((long)-20), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.way, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{2258bbcc-b474-41fe-aa81-76ffc6e2be42}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(wayIds.get((long)-18), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.way, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{51cd8890-a312-11e3-a5e2-0800200c9a66}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(wayIds.get((long)-44), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.way, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    //relations

    elementIdMapping =
      idMappings
        .get("{7ea9ac60-a310-11e3-a5e2-0800200c9a66}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(relationIds.get((long)-1), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.relation, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());

    elementIdMapping =
      idMappings
        .get("{0c9f69f0-dad5-11e3-9c1a-0800200c9a66}");
    Assert.assertNotNull(elementIdMapping);
    Assert.assertEquals((Long)mapId, elementIdMapping.getMapId());
    Assert.assertEquals(relationIds.get((long)-3), elementIdMapping.getOsmElementId());
    Assert.assertEquals(DbUtils.nwr_enum.relation, (DbUtils.nwr_enum)elementIdMapping.getOsmElementType());
  }

  public static void verifyDataPrepared(final long mapId) throws Exception
  {
    //verify job content in review_map table

  	SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration());
    QReviewMap reviewMap = QReviewMap.reviewMap;

    ReviewMap reviewMapInfo  =
    query.from(reviewMap)
    	.where(reviewMap.mapId.eq(mapId))
    	.singleResult(reviewMap);

    Assert.assertNotNull(reviewMapInfo);
    Assert.assertEquals(jobId, reviewMapInfo.getReviewPrepareJobId());

    //verify job completed
    Assert.assertEquals(JOB_STATUS.COMPLETE.toInt(), DbUtils.getJobStatus(jobId, conn));

    //verify content in element_id_mappings table
    verifyElementIdMappings();

    //verify content in review_items table
    query = new SQLQuery(conn, DbUtils.getConfiguration(mapId));
    QReviewItems reviewItemsTbl = QReviewItems.reviewItems;

    final List<ReviewItems> reviewItems =
    query.from(reviewItemsTbl)
    	.where(reviewItemsTbl.mapId.eq(mapId))
    	.orderBy(reviewItemsTbl.reviewId.asc())
    	.list(reviewItemsTbl);



    Assert.assertEquals(16, reviewItems.size());

    //nodes

    ReviewItems reviewItem = reviewItems.get(0);
    reviewItem.getReviewableItemId().equals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}");
    Assert.assertEquals(0.750459, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{5c131c00-a30f-11e3-a5e2-0800200c9a66}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(1);
    reviewItem.getReviewableItemId().equals("{de285618-a4df-4536-b218-5009c25f7107}");
    Assert.assertEquals(0.750459, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{de285618-a4df-4536-b218-5009c25f7107}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(2);
    reviewItem.getReviewableItemId().equals("{99b917d0-8c23-4469-b028-b749a38a8841}");
    Assert.assertEquals(1.0, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    //ways

    reviewItem = reviewItems.get(3);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{28fe6088-cb30-45a2-88dd-1146092dcd0e}");
    Assert.assertEquals(0.750459, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(4);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{28fe6088-cb30-45a2-88dd-1146092dcd0e}");
    Assert.assertEquals(0.750459, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(5);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{f917b7e1-412b-4294-acdc-37bd0fb13b4c}");
    Assert.assertEquals(0.800795, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(6);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{f917b7e1-412b-4294-acdc-37bd0fb13b4c}");
    Assert.assertEquals(0.800795, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(7);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{dfcd4a0e-04d5-46ee-b7a0-1a2c591ddf54}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(8);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{dfcd4a0e-04d5-46ee-b7a0-1a2c591ddf54}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(9);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{81d2aff0-88e5-480e-95cb-c502d120f51f}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(10);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{81d2aff0-88e5-480e-95cb-c502d120f51f}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(11);
    reviewItem.getReviewableItemId().equals("{81d2aff0-88e5-480e-95cb-c502d120f51f}");
    Assert.assertEquals(1.0, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{81d2aff0-88e5-480e-95cb-c502d120f51f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(12);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{f5639540-aaf4-47ac-8c5e-82ad05abc29f}");
    Assert.assertEquals(0.928129, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(13);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{f5639540-aaf4-47ac-8c5e-82ad05abc29f}");
    Assert.assertEquals(0.928129, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(14);
    reviewItem.getReviewableItemId().equals("{51cd8890-a312-11e3-a5e2-0800200c9a66}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{7ea9ac60-a310-11e3-a5e2-0800200c9a66}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());

    //relations

    reviewItem = reviewItems.get(15);
    reviewItem.getReviewableItemId().equals("{0c9f69f0-dad5-11e3-9c1a-0800200c9a66}");
    Assert.assertEquals(0.939793, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{de285618-a4df-4536-b218-5009c25f7107}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(DbUtils.review_status_enum.unreviewed, (DbUtils.review_status_enum)reviewItem.getReviewStatus());
  }

  public static void createPreparedData(WebResource resource) throws Exception
  {

     createDataToPrepare();

    //TODO: probably change these to reviewPrepareDbWriter2; see #6270
    jobId = ReviewItemsPreparer.launchPrepareJob(mapId, 0, false, "reviewPrepareDbWriter");
    (new JobStatusWebPoller(resource, conn)).pollJobStatusUntilCompleteOrFail(jobId, false);

    secondJobId = 
      ReviewItemsPreparer.launchPrepareJob(secondMapId, 0, false, "reviewPrepareDbWriter");
    (new JobStatusWebPoller(resource, conn)).pollJobStatusUntilCompleteOrFail(jobId, false);
  }

  public static void verifyDataNotPrepared(final long mapId) throws Exception
  {
    verifyDataNotPrepared(mapId, false);
  }

  public static void verifyDataNotPrepared(final long mapId, final boolean allowReviewMapEntry)
    throws Exception
  {

  	QReviewMap reviewMap = QReviewMap.reviewMap;
  	QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;
    //no review data for the current map ID should exist in the element ID mappings and review map
    //tables
    if (!allowReviewMapEntry)
    {
      Assert.assertEquals(
        0,
        (new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewMap)
        		.where(reviewMap.mapId.eq(mapId)).count() +
         new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(elementIdMappings).
         			where(elementIdMappings.mapId.eq(mapId)).count()));
    }
    //also, no unique id entry in the element ID mappings should have an entry in the review items
    //table
    final List<String> elementIdsForMap =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(elementIdMappings)
    			.where(elementIdMappings.mapId.eq(mapId))
    			.list(elementIdMappings.elementId);

    QReviewItems reviewItems = QReviewItems.reviewItems;
    List<String> reviewItemIdsMatchingElementIds = new ArrayList<String>();
    if(elementIdsForMap.size() > 0)
    {
      reviewItemIdsMatchingElementIds =
        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItems)
    		.where(reviewItems.reviewableItemId.in(elementIdsForMap))
    		.list(reviewItems.reviewableItemId);
    }
    Assert.assertEquals(0, reviewItemIdsMatchingElementIds.size());
  }

  public static void verifyNoDataPrepared()
  {
  	QReviewMap reviewMap = QReviewMap.reviewMap;
  	QElementIdMappings elementIdMappings = QElementIdMappings.elementIdMappings;
  	QReviewItems reviewItems = QReviewItems.reviewItems;

    Assert.assertEquals(
      0,
      (new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewMap).count() +
      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(elementIdMappings).count() +
       new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItems).count()));
  }

  public static BoundingBox createTestQueryBounds() throws Exception
  {
    BoundingBox bounds = new BoundingBox(-77.0548, 38.9025, -77.054, 39.903);
    BoundingBox expandedBounds = new BoundingBox();
    expandedBounds.expand(
      bounds,
      Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));
    return expandedBounds;
  }

  public static MarkItemsReviewedRequest createMarkItemsReviewedRequest() throws IOException,
    Exception
  {
    MarkItemsReviewedRequest markItemsReviewedRequest = new MarkItemsReviewedRequest();
    markItemsReviewedRequest.setReviewedItems(createReviewedItems());
    markItemsReviewedRequest.setReviewedItemsChangeset(
      XmlDocumentBuilder.toString(createReviewedItemsChangeset()));
    //System.out.println(markItemsReviewedRequest.getReviewedItemsChangeset());
    return markItemsReviewedRequest;
  }

  public static Document createReviewedItemsChangeset() throws Exception
  {
    //TODO: this hardcoded string replace logic needs to be replaced with something more generic;
    //see the convertOldOsmIdsInChangesetRequestToNewOsmIds method in the source control history
    //of this class for an example of how it could be done
    return
      XmlDocumentBuilder.parse(
        FileUtils.readFileToString(
          new File(
            Thread.currentThread().getContextClassLoader().getResource(
              "hoot/services/review/review-update.osm")
            .getPath()))
        .replaceAll(
          "node visible=\"true\" id=\"-64\"",
          "node visible=\"true\" id=\"" + String.valueOf(nodeIds.get((long)-64)) + "\"")
        .replaceAll(
          "node visible=\"true\" id=\"-67\"",
          "node visible=\"true\" id=\"" + String.valueOf(nodeIds.get((long)-67)) + "\"")
        .replaceAll(
          "way visible=\"true\" id=\"-43\"",
          "way visible=\"true\" id=\"" + String.valueOf(wayIds.get((long)-43)) + "\"")
        .replaceAll(
          "nd ref=\"-33\"",
          "nd ref=\"" + String.valueOf(nodeIds.get((long)-33)) + "\"")
        .replaceAll(
          "nd ref=\"-34\"",
          "nd ref=\"" + String.valueOf(nodeIds.get((long)-34)) + "\"")
        .replaceAll(
          "nd ref=\"-35\"",
          "nd ref=\"" + String.valueOf(nodeIds.get((long)-35)) + "\"")
        .replaceAll(
          "nd ref=\"-36\"",
          "nd ref=\"" + String.valueOf(nodeIds.get((long)-36)) + "\"")
        .replaceAll(
          "way visible=\"true\" id=\"-41\"",
          "way visible=\"true\" id=\"" + String.valueOf(wayIds.get((long)-41)) + "\"")
        .replaceAll(
          "nd ref=\"-32\"",
          "nd ref=\"" + String.valueOf(nodeIds.get((long)-32)) + "\"")
        .replaceAll(
          "nd ref=\"-45\"",
          "nd ref=\"" + String.valueOf(nodeIds.get((long)-45)) + "\"")
        .replaceAll(
          "nd ref=\"-60\"",
          "nd ref=\"" + String.valueOf(nodeIds.get((long)-60)) + "\"")
        .replaceAll(
          "way visible=\"true\" id=\"-16\"",
          "way visible=\"true\" id=\"" + String.valueOf(wayIds.get((long)-16)) + "\"")
        .replaceAll(
          "nd ref=\"-52\"",
          "nd ref=\"" + String.valueOf(nodeIds.get((long)-52)) + "\"")
        .replaceAll(
          "way id=\"-18\"",
          "way id=\"" + String.valueOf(wayIds.get((long)-18)) + "\"")
        .replaceAll(
          "relation visible=\"true\" id=\"-3\"",
          "relation visible=\"true\" id=\"" + String.valueOf(relationIds.get((long)-3)) + "\"")
        .replaceAll(
          "member type=\"way\" role=\"role1\" ref=\"-16\"",
          "member type=\"way\" role=\"role1\" ref=\"" +
            String.valueOf(wayIds.get((long)-16)) + "\"")
        .replaceAll(
          "member type=\"node\" role=\"role2\" ref=\"-64\"",
          "member type=\"node\" role=\"role2\" ref=\"" +
            String.valueOf(nodeIds.get((long)-64)) + "\"")
        .replaceAll(
          "node id=\"-71\"",
          "node id=\"" + String.valueOf(nodeIds.get((long)-71)) + "\"")
        .replaceAll(
          "way id=\"-42\"",
          "way id=\"" + String.valueOf(wayIds.get((long)-42)) + "\"")
        .replaceAll(
          "way id=\"-40\"",
          "way id=\"" + String.valueOf(wayIds.get((long)-40)) + "\"")
        .replaceAll(
          "way id=\"-18\"",
          "way id=\"" + String.valueOf(wayIds.get((long)-18)) + "\""));
  }

  public static ReviewedItems createReviewedItems()
  {
    ReviewedItems reviewedItems = new ReviewedItems();
    ReviewedItem[] reviewedItemsArr = new ReviewedItem[5];

    ReviewedItem reviewedItem = new ReviewedItem();
    reviewedItem.setId(nodeIds.get((long)-64));
    reviewedItem.setType(Element.ElementType.Node.toString().toLowerCase());
    reviewedItem.setReviewedAgainstId(nodeIds.get((long)-65));
    reviewedItem.setReviewedAgainstType(Element.ElementType.Node.toString());
    reviewedItemsArr[0] = reviewedItem;

    reviewedItem = new ReviewedItem();
    reviewedItem.setId(nodeIds.get((long)-71));
    reviewedItem.setType(Element.ElementType.Node.toString().toLowerCase());
    reviewedItem.setReviewedAgainstId(nodeIds.get((long)-64));
    reviewedItem.setReviewedAgainstType(Element.ElementType.Node.toString());
    reviewedItemsArr[1] = reviewedItem;

    reviewedItem = new ReviewedItem();
    reviewedItem.setId(wayIds.get((long)-43));
    reviewedItem.setType(Element.ElementType.Way.toString().toLowerCase());
    reviewedItem.setReviewedAgainstId(wayIds.get((long)-20));
    reviewedItem.setReviewedAgainstType(Element.ElementType.Way.toString());
    reviewedItemsArr[2] = reviewedItem;

    reviewedItem = new ReviewedItem();
    reviewedItem.setId(wayIds.get((long)-42));
    reviewedItem.setType(Element.ElementType.Way.toString().toLowerCase());
    reviewedItem.setReviewedAgainstId(wayIds.get((long)-18));
    reviewedItem.setReviewedAgainstType(Element.ElementType.Way.toString());
    reviewedItemsArr[3] = reviewedItem;

    reviewedItem = new ReviewedItem();
    reviewedItem.setId(relationIds.get((long)-3));
    reviewedItem.setType(Element.ElementType.Relation.toString().toLowerCase());
    reviewedItem.setReviewedAgainstId(nodeIds.get((long)-67));
    reviewedItem.setReviewedAgainstType(Element.ElementType.Node.toString());
    reviewedItemsArr[4] = reviewedItem;

    reviewedItems.setReviewedItems(reviewedItemsArr);
    return reviewedItems;
  }

  private static void verifyReviewStatuses(final DbUtils.review_status_enum[] expectedReviewStatuses)
  {
    QReviewItems reviewItemsTbl = QReviewItems.reviewItems;
    final List<ReviewItems> reviewItems =
    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(reviewItemsTbl)
    		.where(reviewItemsTbl.mapId.eq(mapId))
    		.orderBy(reviewItemsTbl.reviewId.asc())
    		.list(reviewItemsTbl);


    Assert.assertEquals(16, reviewItems.size());

    //nodes

    ReviewItems reviewItem = reviewItems.get(0);
    reviewItem.getReviewableItemId().equals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}");
    Assert.assertEquals(0.750459, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{5c131c00-a30f-11e3-a5e2-0800200c9a66}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[0], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(1);
    reviewItem.getReviewableItemId().equals("{de285618-a4df-4536-b218-5009c25f7107}");
    Assert.assertEquals(0.750459, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{de285618-a4df-4536-b218-5009c25f7107}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[1], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(2);
    reviewItem.getReviewableItemId().equals("{99b917d0-8c23-4469-b028-b749a38a8841}");
    Assert.assertEquals(1.0, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[2], reviewItem.getReviewStatus());

    //ways

    reviewItem = reviewItems.get(3);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{28fe6088-cb30-45a2-88dd-1146092dcd0e}");
    Assert.assertEquals(0.750459, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[3], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(4);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{28fe6088-cb30-45a2-88dd-1146092dcd0e}");
    Assert.assertEquals(0.750459, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[4], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(5);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{f917b7e1-412b-4294-acdc-37bd0fb13b4c}");
    Assert.assertEquals(0.800795, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[5], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(6);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{f917b7e1-412b-4294-acdc-37bd0fb13b4c}");
    Assert.assertEquals(0.800795, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[6], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(7);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{dfcd4a0e-04d5-46ee-b7a0-1a2c591ddf54}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[7], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(8);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{dfcd4a0e-04d5-46ee-b7a0-1a2c591ddf54}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[8], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(9);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{81d2aff0-88e5-480e-95cb-c502d120f51f}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[9], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(10);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{81d2aff0-88e5-480e-95cb-c502d120f51f}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[10], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(11);
    reviewItem.getReviewableItemId().equals("{81d2aff0-88e5-480e-95cb-c502d120f51f}");
    Assert.assertEquals(1.0, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{81d2aff0-88e5-480e-95cb-c502d120f51f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[11], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(12);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{f5639540-aaf4-47ac-8c5e-82ad05abc29f}");
    Assert.assertEquals(0.928129, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[12], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(13);
    reviewItem.getReviewableItemId().equals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{f5639540-aaf4-47ac-8c5e-82ad05abc29f}");
    Assert.assertEquals(0.928129, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[13], reviewItem.getReviewStatus());

    reviewItem = reviewItems.get(14);
    reviewItem.getReviewableItemId().equals("{51cd8890-a312-11e3-a5e2-0800200c9a66}");
    Assert.assertEquals(0.832799, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{7ea9ac60-a310-11e3-a5e2-0800200c9a66}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[14], reviewItem.getReviewStatus());

    //relations

    reviewItem = reviewItems.get(15);
    reviewItem.getReviewableItemId().equals("{0c9f69f0-dad5-11e3-9c1a-0800200c9a66}");
    Assert.assertEquals(0.939793, reviewItem.getReviewScore(), 0.0);
    Assert.assertEquals(
      "{de285618-a4df-4536-b218-5009c25f7107}", reviewItem.getReviewAgainstItemId());
    Assert.assertEquals(expectedReviewStatuses[15], reviewItem.getReviewStatus());
  }

  public static void verifyAllDataMarkedAsReviewed() throws Exception
  {
    //the set of element ID unique ID values will be unchanged from when the data was prepared
    verifyElementIdMappings();

    //verify content in review_items table - no items will have been deleted as a result of the
    //mark operation, so the number should be the same as when the data was prepared; only the
    //review status of the items should change
    DbUtils.review_status_enum[] expectedReviewStatuses = new DbUtils.review_status_enum[16];
    for (int i = 0; i < expectedReviewStatuses.length; i++)
    {
      expectedReviewStatuses[i] = DbUtils.review_status_enum.reviewed;
    }
    verifyReviewStatuses(expectedReviewStatuses);

    //TODO: verify tags on all reviewed data
  }

  public static void verifyDataMarkedAsReviewed(final boolean validChangesetUploaded)
    throws Exception
  {
    //the set of element ID unique ID values will be unchanged from when the data was prepared
    verifyElementIdMappings();

    //verify content in review_items table - no items will have been deleted as a result of the
    //mark operation, so the number should be the same as when the data was prepared; only the
    //review status of the items should change
    DbUtils.review_status_enum[] expectedReviewStatuses = new DbUtils.review_status_enum[16];
    for (int i = 0; i < expectedReviewStatuses.length; i++)
    {
      expectedReviewStatuses[i] = DbUtils.review_status_enum.unreviewed;
    }
    expectedReviewStatuses[0] =  DbUtils.review_status_enum.reviewed;
    expectedReviewStatuses[2] =  DbUtils.review_status_enum.reviewed;
    expectedReviewStatuses[3] =  DbUtils.review_status_enum.reviewed;
    expectedReviewStatuses[6] =  DbUtils.review_status_enum.reviewed;
    expectedReviewStatuses[15] =  DbUtils.review_status_enum.reviewed;
    verifyReviewStatuses(expectedReviewStatuses);

    if (validChangesetUploaded)
    {
      //Make sure the changeset exists and is valid
      final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
      QChangesets changesets = QChangesets.changesets;
      Changesets changeset =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
      .where(changesets.id.eq(changesetId)).singleResult(changesets);

      Assert.assertNotNull(changeset);
      Assert.assertTrue(changeset.getCreatedAt().before(now));
      Assert.assertTrue(changeset.getClosedAt().before(now));
      Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
    }

    //verify tags on all reviewed data
    /*
     1. reviewable item = {25e1aa70-a30f-11e3-a5e2-0800200c9a66} = node -64 - no more items to
     review against; all review tags get dropped; uuid gets appended to

     review against item = {5c131c00-a30f-11e3-a5e2-0800200c9a66} = node -65 - no more items to
     be reviewed against; another item still needs to be reviewed against this, but all its review
     tags should still get dropped
    */
    Element element =
      ElementFactory.getInstance().create(mapId, ElementType.Node, nodeIds.get((long)-64), conn);
    //Assert.assertFalse(element.hasTagsStartingWithText("hoot:review"));
    Assert.assertEquals(
      "{25e1aa70-a30f-11e3-a5e2-0800200c9a66}",
      element.getTags().get("uuid"));
    element =
      ElementFactory.getInstance().create(
        mapId, ElementType.Node, nodeIds.get((long)-65), conn);
    Assert.assertEquals(
      "{5c131c00-a30f-11e3-a5e2-0800200c9a66}", element.getTags().get("uuid"));

    if (validChangesetUploaded)
    {
      /*
      2. reviewable item = {99b917d0-8c23-4469-b028-b749a38a8841} = node -71 - this was deleted in
      the changeset

      review against item = {25e1aa70-a30f-11e3-a5e2-0800200c9a66} = node -64 - this review against
      item was checked in the previous step, so skip
     */
    QCurrentNodes currentNodes = QCurrentNodes.currentNodes;
     Assert.assertEquals(
       0,
       new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodes)
       .where(currentNodes.id.eq(nodeIds.get((long)-71))).count()
    		 );
    }
    else
    {
      /*
      2. reviewable item = {99b917d0-8c23-4469-b028-b749a38a8841} = node -71 - nothing else to
      review against, so drop review tags and append uuid of reviewed against items

      review against item = {25e1aa70-a30f-11e3-a5e2-0800200c9a66} = node -64 - this review against
      item was checked in the previous step, so skip
     */
     element =
       ElementFactory.getInstance().create(mapId, ElementType.Node, nodeIds.get((long)-71), conn);
     //Assert.assertFalse(element.hasTagsStartingWithText("hoot:review"));
     Assert.assertEquals(
       "{99b917d0-8c23-4469-b028-b749a38a8841}",
       element.getTags().get("uuid"));
    }

    /*
     3. {46d98769-dee2-489e-8fd6-ff265842ed55};{28fe6088-cb30-45a2-88dd-1146092dcd0e} = way -43 -
     this still has an item to be reviewed against so review tags not dropped; uuid gets appended to

     review against item = {f5639540-aaf4-47ac-8c5e-82ad05abc29f} = way -20 - this has invalid
     review tags; the system doesn't strip out invalid review tags from a set if any of them are
     invalid, so they should still be here
    */
    element =
      ElementFactory.getInstance().create(mapId, ElementType.Way, wayIds.get((long)-43), conn);
    Assert.assertTrue(element.hasTagsStartingWithText("hoot:review"));
    Assert.assertEquals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{28fe6088-cb30-45a2-88dd-1146092dcd0e}",
      element.getTags().get("uuid"));
    element =
      ElementFactory.getInstance().create(mapId, ElementType.Way, wayIds.get((long)-20), conn);
    Assert.assertTrue(element.hasTagsStartingWithText("hoot:review"));
    Assert.assertEquals("{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", element.getTags().get("uuid"));

    if (validChangesetUploaded)
    {
      /*
        4. reviewable item =
        {46d98769-dee2-489e-8fd6-ff265842ed55};{f917b7e1-412b-4294-acdc-37bd0fb13b4c} = way -42 -
        even though this still has an item to be reviewed against, it was deleted in
        the changeset (system doesn't guard against that)

        review against item = {2258bbcc-b474-41fe-aa81-76ffc6e2be42} = way -18 - this was deleted in
        the changeset
       */
    	QCurrentWays currentWays = QCurrentWays.currentWays;
       Assert.assertEquals(
         0,
         new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWays)
         .where(currentWays.id.eq(wayIds.get((long)-42))).count()
);
       Assert.assertEquals(
         0,
         new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWays)
         .where(currentWays.id.eq(wayIds.get((long)-18))).count());
    }
    else
    {
      /*
        4. reviewable item =
        {46d98769-dee2-489e-8fd6-ff265842ed55};{f917b7e1-412b-4294-acdc-37bd0fb13b4c} = way -42 -
        this still has other items to review against, so don't drop review tags; append uuid of
        reviewed against items

        review against item = {2258bbcc-b474-41fe-aa81-76ffc6e2be42} = way -18 - this has invalid
        review tags; the system doesn't strip out invalid review tags from a set if any of them are
        invalid, so they should still be here
       */
       element =
         ElementFactory.getInstance().create(mapId, ElementType.Way, wayIds.get((long)-42), conn);
       Assert.assertTrue(element.hasTagsStartingWithText("hoot:review"));
       Assert.assertEquals(
         "{46d98769-dee2-489e-8fd6-ff265842ed55};{f917b7e1-412b-4294-acdc-37bd0fb13b4c}",
         element.getTags().get("uuid"));
       element =
         ElementFactory.getInstance().create(mapId, ElementType.Way, wayIds.get((long)-18), conn);
       Assert.assertTrue(element.hasTagsStartingWithText("hoot:review"));
       Assert.assertEquals("{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", element.getTags().get("uuid"));
    }

    //check that nothing in the other map was modified
    jobId = secondJobId;  //hack
    verifyDataPrepared(secondMapId);
  }

  public static void verifyDataNotMarkedAsReviewed() throws Exception
  {
  	QChangesets changesets = QChangesets.changesets;
    verifyDataPrepared(mapId);
    jobId = secondJobId;  //hack
    verifyDataPrepared(secondMapId);
    //also check that no new changeset was created; just the two from the two prepare jobs launched
    //should be present; none for the mark job
    long firstCnt = new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets).count();
    long secondCnt = new SQLQuery(conn, DbUtils.getConfiguration(secondMapId)).from(changesets).count();
    Assert.assertEquals(2, (
    		firstCnt + secondCnt));
  }

  public static void markSomeItemsReviewed() throws Exception
  {
  	QReviewItems reviewItems = QReviewItems.reviewItems;
    final String[] uniqueIdStrs =
      new String[]{
        //node; 1 review
        "{25e1aa70-a30f-11e3-a5e2-0800200c9a66}",
        //way; 2 reviews
        "{46d98769-dee2-489e-8fd6-ff265842ed55};{28fe6088-cb30-45a2-88dd-1146092dcd0e}"
        };
    for (int i = 0; i < uniqueIdStrs.length; i++)
    {
      if (
      		new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), reviewItems)
      		.where(reviewItems.reviewableItemId.eq(uniqueIdStrs[i]).and(reviewItems.mapId.eq(mapId)))
          .set(reviewItems.reviewStatus, DbUtils.review_status_enum.reviewed)
          .execute() <= 0
      		)
      {
        throw new Exception("Error updating item.");
      }
    }
  }

  public static void deleteReviewableItem() throws Exception
  {
    final long changesetId = Changeset.insertNew(mapId, userId, conn);
    ChangesetDbWriter elementWriter = new ChangesetDbWriter(conn);
    /*final Document response =*/
    elementWriter.write(
    		mapId,
      changesetId,
      FileUtils.readFileToString(
        new File(
          Thread.currentThread().getContextClassLoader().getResource(
            "hoot/services/review/review-update-delete.osm")
            .getPath()))
      .replaceAll("changeset=\"\"", "changeset=\"" + changesetId + "\"")
      .replaceAll(
        "way id=\"-41\"",
        "way id=\"" + wayIds.get((long)-41) + "\""));
  }
}
