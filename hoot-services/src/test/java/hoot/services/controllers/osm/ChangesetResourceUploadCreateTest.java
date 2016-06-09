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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.controllers.osm;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.ws.rs.core.MediaType;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpressionException;

import org.apache.xpath.XPathAPI;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.ClientResponse.Status;
import com.sun.jersey.api.client.UniformInterfaceException;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.Changesets;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentRelations;
import hoot.services.db2.CurrentWayNodes;
import hoot.services.db2.CurrentWays;
import hoot.services.db2.QChangesets;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWayNodes;
import hoot.services.db2.QCurrentWays;
import hoot.services.geo.BoundingBox;
import hoot.services.geo.QuadTileCalculator;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.osm.OsmTestUtils;
import hoot.services.utils.HootCustomPropertiesSetter;
import hoot.services.utils.XmlUtils;


public class ChangesetResourceUploadCreateTest extends OsmResourceTestAbstract {
    private static final Logger log = LoggerFactory.getLogger(ChangesetResourceUploadCreateTest.class);

    private final QCurrentNodes currentNodesTbl = QCurrentNodes.currentNodes;
    private final QCurrentWays currentWaysTbl = QCurrentWays.currentWays;
    private final QCurrentWayNodes currentWayNodesTbl = QCurrentWayNodes.currentWayNodes;
    private final QCurrentRelations currentRelationsTbl = QCurrentRelations.currentRelations;

    public ChangesetResourceUploadCreateTest() {
        super("hoot.services.controllers.osm");
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadCreate() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(null, 0);

            // now update it with new OSM elements; make this create the
            // equivalent of the default dataset
            // which is being created outside of the service call for all other
            // tests, so the default dataset verification methods can be used
            Document responseData = null;
            try {
                responseData =
                        resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
                            "<osmChange version=\"0.3\" generator=\"iD\">" +
                                "<create>" +
                                    "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                         originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                        "<tag k=\"key 1\" v=\"val 1\"/>" + "<tag k=\"key 2\" v=\"val 2\"/>" +
                                    "</node>" +
                                    "<node id=\"-2\" lon=\"" + originalBounds.getMaxLon() + "\" lat=\"" +
                                         originalBounds.getMaxLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "</node>" +
                                    "<node id=\"-3\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                        "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "</node>" +
                                    "<node id=\"-4\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                            "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                        "<tag k=\"key 3\" v=\"val 3\"/>" + "</node>" + "<node id=\"-5\" lon=\"" + originalBounds.getMinLon() +
                                           "\" lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                        "<tag k=\"key 4\" v=\"val 4\"/>" +
                                    "</node>" +
                                    "<way id=\"-6\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                        "<nd ref=\"-1\"></nd>" +
                                        "<nd ref=\"-2\"></nd>" +
                                        "<nd ref=\"-5\"></nd>" +
                                        "<tag k=\"key 1\" v=\"val 1\"/>" +
                                        "<tag k=\"key 2\" v=\"val 2\"/>" +
                                    "</way>" +
                                    "<way id=\"-7\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                        "<nd ref=\"-3\"></nd>" + "<nd ref=\"-2\"></nd>" +
                                    "</way>" +
                                    "<way id=\"-8\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                        "<nd ref=\"-1\"></nd>" + "<nd ref=\"-2\"></nd>" +
                                        "<tag k=\"key 3\" v=\"val 3\"/>" +
                                    "</way>" +
                                    "<relation id=\"-9\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                        "<member type=\"node\" role=\"role1\" ref=\"-1\"></member>" +
                                        "<member type=\"way\" role=\"role3\" ref=\"-7\"></member>" +
                                        "<member type=\"way\" role=\"role2\" ref=\"-6\"></member>" +
                                        "<member type=\"node\" ref=\"-3\"></member>" + "<tag k=\"key 1\" v=\"val 1\"/>" +
                                    "</relation>" +
                                    "<relation id=\"-10\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                        "<member type=\"node\" role=\"role1\" ref=\"-5\"></member>" +
                                        "<member type=\"relation\" role=\"role1\" ref=\"-9\"></member>" +
                                        "<tag k=\"key 2\" v=\"val 2\"/>" + "<tag k=\"key 3\" v=\"val 3\"/>" +
                                    "</relation>" +
                                    "<relation id=\"-11\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                        "<member type=\"way\" role=\"\" ref=\"-7\"></member>" +
                                        "<tag k=\"key 4\" v=\"val 4\"/>" + "</relation>" +
                                    "<relation id=\"-12\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                        "<member type=\"node\" role=\"role1\" ref=\"-3\"></member>" +
                                    "</relation>" +
                                "</create>" +
                                "<modify/>" +
                                "<delete if-unused=\"true\"/>" +
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            Set<Long> nodeIds = new LinkedHashSet<>();
            Set<Long> wayIds = new LinkedHashSet<>();
            Set<Long> relationIds = new LinkedHashSet<>();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(5, returnedNodes.getLength());

                long oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData));
                Assert.assertEquals(-1, oldElementId);
                long newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData));
                Assert.assertNotSame(-1, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData));
                Assert.assertEquals(-2, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData));
                Assert.assertNotSame(-2, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)) + 1,
                        newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@old_id", responseData));
                Assert.assertEquals(-3, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_id", responseData));
                Assert.assertNotSame(-3, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)) + 1,
                        newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@old_id", responseData));
                Assert.assertEquals(-4, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@new_id", responseData));
                Assert.assertNotSame(-4, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_id", responseData)) + 1,
                        newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[5]/@old_id", responseData));
                Assert.assertEquals(-5, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[5]/@new_id", responseData));
                Assert.assertNotSame(-5, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@new_id", responseData)) + 1,
                        newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[5]/@new_version", responseData)));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(3, returnedWays.getLength());

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData));
                Assert.assertEquals(-6, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData));
                Assert.assertNotSame(-6, newElementId);
                wayIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData));
                Assert.assertEquals(-7, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData));
                Assert.assertNotSame(-7, newElementId);
                wayIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[3]/@old_id", responseData));
                Assert.assertEquals(-8, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[3]/@new_id", responseData));
                Assert.assertNotSame(-8, newElementId);
                wayIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[3]/@new_version", responseData)));

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(4, returnedRelations.getLength());

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData));
                Assert.assertEquals(-9, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData));
                Assert.assertNotSame(-9, newElementId);
                relationIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData));
                Assert.assertEquals(-10, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData));
                Assert.assertNotSame(-10, newElementId);
                relationIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[3]/@old_id", responseData));
                Assert.assertEquals(-11, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[3]/@new_id", responseData));
                Assert.assertNotSame(-11, newElementId);
                relationIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[3]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[4]/@old_id", responseData));
                Assert.assertEquals(-12, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[4]/@new_id", responseData));
                Assert.assertNotSame(-12, newElementId);
                relationIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[4]/@new_version", responseData)));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            // changes have actually occurred with the upload of the
            // changeset...what's actually being done here is to compare the state of the default test data set
            // with the dataset we uploaded here, and they should match each other
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadCreateClosedPolygon() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(null, 0);

            Document responseData = null;
            try {
                responseData =
                        resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class,
                            "<osmChange version=\"0.3\" generator=\"iD\">" +
                                "<create>" +
                                    "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                        originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "</node>" +
                                    "<node id=\"-2\" lon=\"" + originalBounds.getMaxLon() + "\" lat=\"" + originalBounds.getMaxLat() +
                                        "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "</node>" +
                                    "<node id=\"-3\" lon=\"" + (originalBounds.getMinLon() - .001) + "\" lat=\"" +
                                        (originalBounds.getMinLat() - .001) + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "</node>" +
                                    "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                        "<nd ref=\"-1\"></nd>" + "<nd ref=\"-2\"></nd>" +
                                        "<nd ref=\"-3\"></nd>" + "<nd ref=\"-1\"></nd>" +
                                    "</way>" +
                                "</create>" +
                                "<modify/>" +
                                "<delete if-unused=\"true\"/>" +
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            List<Long> nodeIds = new ArrayList<>();
            Set<Long> wayIds = new LinkedHashSet<>();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(3, returnedNodes.getLength());

                long oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData));
                Assert.assertEquals(-1, oldElementId);
                long newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData));
                Assert.assertNotSame(-1, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData));
                Assert.assertEquals(-2, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData));
                Assert.assertNotSame(-2, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@old_id", responseData));
                Assert.assertEquals(-3, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_id", responseData));
                Assert.assertNotSame(-3, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_version", responseData)));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(1, returnedWays.getLength());

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData));
                Assert.assertEquals(-1, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData));
                Assert.assertNotSame(-1, newElementId);
                wayIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
            Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
            QChangesets changesets = QChangesets.changesets;

            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(changesets)
                    .where(changesets.id.eq(changesetId))
                    .singleResult(changesets);
            try {
                Map<Long, CurrentNodes> nodes =
                        new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                                .from(currentNodesTbl)
                                .map(currentNodesTbl.id, currentNodesTbl);

                Assert.assertEquals(3, nodes.size());

                CurrentNodes nodeRecord = nodes.get(nodeIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[0], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertEquals(true, nodeRecord.getVisible());

                nodeRecord = nodes.get(nodeIdsArr[1]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMaxLat()), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMaxLon()), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[1], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertEquals(true, nodeRecord.getVisible());

                nodeRecord = nodes.get(nodeIdsArr[2]);
                Assert.assertEquals(new Long(changesetId), nodeRecord.getChangesetId());
                Assert.assertEquals(new Double(originalBounds.getMinLat() - .001), nodeRecord.getLatitude());
                Assert.assertEquals(new Double(originalBounds.getMinLon() - .001), nodeRecord.getLongitude());
                Assert.assertEquals(nodeIdsArr[2], nodeRecord.getId());
                Assert.assertEquals(
                        new Long(QuadTileCalculator.tileForPoint(nodeRecord.getLatitude(), nodeRecord.getLongitude())),
                        nodeRecord.getTile());
                Assert.assertTrue(nodeRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), nodeRecord.getVersion());
                Assert.assertEquals(true, nodeRecord.getVisible());
            }
            catch (Exception e) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Map<Long, CurrentWays> ways = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWaysTbl)
                        .map(currentWaysTbl.id, currentWaysTbl);

                Assert.assertEquals(1, ways.size());
                CurrentWays wayRecord = ways.get(wayIdsArr[0]);
                Assert.assertEquals(new Long(changesetId), wayRecord.getChangesetId());
                Assert.assertEquals(wayIdsArr[0], wayRecord.getId());
                Assert.assertTrue(wayRecord.getTimestamp().before(now));
                Assert.assertEquals(new Long(1), wayRecord.getVersion());
                Assert.assertTrue(wayRecord.getVisible());

                List<CurrentWayNodes> wayNodes = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(currentWayNodesTbl)
                        .where(currentWayNodesTbl.wayId.eq(wayIdsArr[0]))
                        .orderBy(currentWayNodesTbl.sequenceId.asc())
                        .list(currentWayNodesTbl);

                Assert.assertEquals(4, wayNodes.size());
                CurrentWayNodes wayNode = wayNodes.get(0);
                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(1), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(1);
                Assert.assertEquals(nodeIdsArr[1], wayNode.getNodeId());
                Assert.assertEquals(new Long(2), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(2);
                Assert.assertEquals(nodeIdsArr[2], wayNode.getNodeId());
                Assert.assertEquals(new Long(3), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
                wayNode = wayNodes.get(3);
                Assert.assertEquals(nodeIdsArr[0], wayNode.getNodeId());
                Assert.assertEquals(new Long(4), wayNode.getSequenceId());
                Assert.assertEquals(wayRecord.getId(), wayNode.getWayId());
            }
            catch (Exception e) {
                Assert.fail("Error checking ways: " + e.getMessage());
            }

            try {
                Assert.assertNotNull(changeset);
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(4), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                BoundingBox expandedBounds = new BoundingBox(originalBounds);
                BoundingBox updatedBounds = new BoundingBox((originalBounds.getMinLon() - .001),
                        (originalBounds.getMinLat() - .001), originalBounds.getMaxLon(), originalBounds.getMaxLat());
                expandedBounds.expand(updatedBounds,
                        Double.parseDouble(HootProperties.getDefault("changesetBoundsExpansionFactorDeegrees")));

                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                Assert.assertEquals(changesetBounds, expandedBounds);
            }
            catch (Exception e) {
                Assert.fail("Error checking changeset: " + e.getMessage());
            }
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadCreateElementNoTags() throws Exception {
        try {
            BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
            long changesetId = OsmTestUtils.createTestChangeset(originalBounds, 0);

            // now update the changeset with new nodes which have no tags, which is allowed
            Document responseData = null;
            try {
                responseData =
                        resource()
                        .path("api/0.6/changeset/" + changesetId + "/upload")
                        .queryParam("mapId", String.valueOf(mapId))
                        .type(MediaType.TEXT_XML)
                        .accept(MediaType.TEXT_XML)
                        .post(Document.class, "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                    originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                                "<node id=\"-2\" lon=\"" + originalBounds.getMaxLon() + "\" lat=\"" +
                                    originalBounds.getMaxLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                                "<node id=\"-3\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                    originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                                "<node id=\"-4\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                    originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                                "<node id=\"-5\" lon=\"" + originalBounds.getMinLon() + "\" lat=\""
                                    + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                "</node>" +
                                "<way id=\"-6\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"-1\"></nd>" +
                                    "<nd ref=\"-2\"></nd>" +
                                    "<nd ref=\"-5\"></nd>" +
                                "</way>" +
                                "<way id=\"-7\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"-3\"></nd>" + "<nd ref=\"-2\"></nd>" +
                                "</way>" +
                                "<way id=\"-8\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"-1\"></nd>" + "<nd ref=\"-2\"></nd>" +
                                "</way>" +
                                "<relation id=\"-9\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"-1\"></member>" +
                                    "<member type=\"way\" role=\"role3\" ref=\"-7\"></member>" +
                                    "<member type=\"way\" role=\"role2\" ref=\"-6\"></member>" +
                                    "<member type=\"node\" ref=\"-3\"></member>" + "</relation>" +
                                "<relation id=\"-10\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"-5\"></member>" +
                                    "<member type=\"relation\" role=\"role1\" ref=\"-9\"></member>" + "</relation>" +
                                "<relation id=\"-11\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"way\" role=\"\" ref=\"-7\"></member>" + "" +
                                "</relation>" +
                                "<relation id=\"-12\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"-3\"></member>" +
                                "</relation>" +
                                "</create>" +
                                "<modify/>" +
                                "<delete if-unused=\"true\"/>" +
                            "</osmChange>");
            }
            catch (UniformInterfaceException e) {
                ClientResponse r = e.getResponse();
                Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
            }
            Assert.assertNotNull(responseData);

            XPath xpath = XmlUtils.createXPath();
            Set<Long> nodeIds = new LinkedHashSet<>();
            Set<Long> wayIds = new LinkedHashSet<>();
            Set<Long> relationIds = new LinkedHashSet<>();
            try {
                NodeList returnedNodes = XPathAPI.selectNodeList(responseData, "//osm/diffResult/node");
                Assert.assertEquals(5, returnedNodes.getLength());

                long oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@old_id", responseData));
                Assert.assertEquals(-1, oldElementId);
                long newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData));
                Assert.assertNotSame(-1, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@old_id", responseData));
                Assert.assertEquals(-2, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData));
                Assert.assertNotSame(-2, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[1]/@new_id", responseData)) + 1,
                        newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@old_id", responseData));
                Assert.assertEquals(-3, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_id", responseData));
                Assert.assertNotSame(-3, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[2]/@new_id", responseData)) + 1,
                        newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@old_id", responseData));
                Assert.assertEquals(-4, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@new_id", responseData));
                Assert.assertNotSame(-4, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[3]/@new_id", responseData)) + 1,
                        newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[5]/@old_id", responseData));
                Assert.assertEquals(-5, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/node[5]/@new_id", responseData));
                Assert.assertNotSame(-5, newElementId);
                nodeIds.add(newElementId);
                Assert.assertEquals(
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[4]/@new_id", responseData)) + 1,
                        newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/node[5]/@new_version", responseData)));

                NodeList returnedWays = XPathAPI.selectNodeList(responseData, "//osm/diffResult/way");
                Assert.assertEquals(3, returnedWays.getLength());

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@old_id", responseData));
                Assert.assertEquals(-6, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_id", responseData));
                Assert.assertNotSame(-6, newElementId);
                wayIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[1]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@old_id", responseData));
                Assert.assertEquals(-7, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_id", responseData));
                Assert.assertNotSame(-7, newElementId);
                wayIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[2]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[3]/@old_id", responseData));
                Assert.assertEquals(-8, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/way[3]/@new_id", responseData));
                Assert.assertNotSame(-8, newElementId);
                wayIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/way[3]/@new_version", responseData)));

                NodeList returnedRelations = XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation");
                Assert.assertEquals(4, returnedRelations.getLength());

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@old_id", responseData));
                Assert.assertEquals(-9, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_id", responseData));
                Assert.assertNotSame(-9, newElementId);
                relationIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[1]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@old_id", responseData));
                Assert.assertEquals(-10, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_id", responseData));
                Assert.assertNotSame(-10, newElementId);
                relationIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[2]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[3]/@old_id", responseData));
                Assert.assertEquals(-11, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[3]/@new_id", responseData));
                Assert.assertNotSame(-11, newElementId);
                relationIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[3]/@new_version", responseData)));

                oldElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[4]/@old_id", responseData));
                Assert.assertEquals(-12, oldElementId);
                newElementId = Long.parseLong(xpath.evaluate("//osm/diffResult/relation[4]/@new_id", responseData));
                Assert.assertNotSame(-12, newElementId);
                relationIds.add(newElementId);
                Assert.assertEquals(1,
                        Long.parseLong(xpath.evaluate("//osm/diffResult/relation[4]/@new_version", responseData)));
            }
            catch (XPathExpressionException e) {
                Assert.fail("Error parsing response document: " + e.getMessage());
            }

            // see notes by similar call in testUploadCreate
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds, false);
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateMissingNodeTagValue() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(null);

        // Upload a changeset where one of the tags specified has no value
        // attribute. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                    "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"name 1\"/>" + // missing tag value
                                    "<tag k=\"name 2\" v=\"val 2\"/>" +
                                "</node>" +
                                "<node id=\"-2\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                    "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"name 1\" v=\"val 1\"/>" +
                                "</node>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error parsing tag"));

            OsmTestUtils.verifyTestChangesetUnmodified(changesetId);
            Assert.assertFalse(DbUtils.elementDataExistsInServicesDb(conn));

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateInvalidChangesetSpecifiedInUrl() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(null);

        // Upload a changeset where the changeset specified in the URL doesn't
        // exist. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + (changesetId + 1) + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                       originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"name 1\" v=\"val 1\"/>" + "<tag k=\"name 2\" v=\"val 2\"/>" +
                                "</node>" +
                                "<node id=\"-2\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                       "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"name 1\" v=\"val 1\"/>" +
                                "</node>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(404, r.getStatus());
            Assert.assertTrue(r.getEntity(String.class).contains("Changeset to be updated does not exist"));

            OsmTestUtils.verifyTestChangesetUnmodified(changesetId);
            Assert.assertFalse(DbUtils.elementDataExistsInServicesDb(conn));

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateInvalidChangesetIdInNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(null);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" + originalBounds.getMinLat() +
                                      "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"name 1\" v=\"val 1\"/>" + "<tag k=\"name 2\" v=\"val 2\"/>" +
                                "</node>" +
                                "<node id=\"-2\" lon=\"" + originalBounds.getMinLon() + "\" lat=\"" +
                                      originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + (changesetId + 1) + "\">" +
                                    "<tag k=\"name 1\" v=\"val 1\"/>" +
                                "</node>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid changeset ID"));

            OsmTestUtils.verifyTestChangesetUnmodified(changesetId);
            Assert.assertFalse(DbUtils.elementDataExistsInServicesDb(conn));

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateEmptyNodeCoords() throws Exception {
        // Try to update a changeset with nodes that have empty coordinate
        // strings. A failure should occur and no data in system should be modified.
        uploadCreateNodeCoords("", "");
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateNodeCoordsOutOfBounds() throws Exception {
        // Try to update a changeset with nodes that have out of world bounds
        // coordinate values. A failure should occur and no data in system should be modified.
        uploadCreateNodeCoords("91", "-181");
    }

    private void uploadCreateNodeCoords(String lat, String lon) throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(null);

        // Now update the changeset via the service where one of the nodes has a
        // empty coords. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML).accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<modify/>" +
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" " + "lat=\"" +
                                        originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                                "</node>" +
                                "<node id=\"-2\" lon=\"" + lon + "\" " + "lat=\"" + lat + "\" version=\"0\" changeset=\"" +
                                       changesetId + "\">" + "<tag k=\"key 3\" v=\"val 3\"></tag>" + "</node>" + "</create>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error uploading changeset with ID"));

            try {
                // make sure the nodes weren't created
                Assert.assertEquals(0,
                        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl).count());
            }
            catch (Exception e2) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            QChangesets changesets = QChangesets.changesets;
            Changesets changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                    .from(changesets).where(changesets.id.eq(changesetId))
                    .singleResult(changesets);
            try {
                changeset = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                        .from(changesets)
                        .where(changesets.id.eq(changesetId))
                        .singleResult(changesets);

                Assert.assertNotNull(changeset);
                Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
                Assert.assertTrue(changeset.getCreatedAt().before(now));
                Assert.assertTrue(changeset.getClosedAt().after(changeset.getCreatedAt()));
                Assert.assertEquals(new Integer(12), changeset.getNumChanges());
                Assert.assertEquals(new Long(userId), changeset.getUserId());

                // make sure the changeset bounds weren't updated
                Changeset hootChangeset = new Changeset(mapId, changesetId, conn);
                BoundingBox changesetBounds = hootChangeset.getBounds();
                BoundingBox defaultBounds = new BoundingBox();
                Assert.assertEquals(defaultBounds, changesetBounds);
            }
            catch (Exception e2) {
                Assert.fail("Error checking changeset: " + e.getMessage());
            }

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateExistingNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        BoundingBox updateBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();

        // Update the changeset with a node that has an ID equal to that of a
        // previously created node. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<modify/>" +
                            "<create>" +
                                "<node id=\"" + nodeIdsArr[0] + "\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                        "lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 1\" v=\"val 1\"></tag>" +
                                    "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                                "</node>" +
                                "<node id=\"-1\" lon=\"" + updateBounds.getMinLon() + "\" " + "lat=\"" +
                                        updateBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                                "</node>" +
                            "</create>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid OSM element ID for create"));

            // make sure the new nodes weren't created
            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateDuplicateNodeIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        OsmTestUtils.createTestNodes(changesetId, originalBounds);

        // Try to create two nodes with the same ID. A failure should occur and
        // no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<modify/>" +
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                     "lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                                "</node>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                    "lat=\"" + originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                                "</node>" +
                            "</create>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            try {
                // make sure the new nodes weren't created
                Assert.assertEquals(5,
                        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodesTbl).count());
            }
            catch (Exception e2) {
                Assert.fail("Error checking nodes: " + e.getMessage());
            }

            try {
                Assert.assertEquals(4, OsmTestUtils.getTagCountForElementType(mapId, ElementType.Node, conn));
            }
            catch (Exception e2) {
                Assert.fail("Error checking node tags: " + e.getMessage());
            }

            OsmTestUtils.verifyTestChangesetUnmodified(changesetId, originalBounds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateTooFewNodesForWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // A way has to have two or more nodes. Try to upload a way with one
        // node. The request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\">" + "</nd>" +
                                    "<tag k=\"key 1\" v=\"val 1\"/>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Too few nodes specified for way"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateTooManyNodesForWay() throws Exception {
        String originalMaximumWayNodes = HootProperties.getProperty("maximumWayNodes");

        BoundingBox originalBounds = null;
        Long changesetId = null;
        Set<Long> nodeIds = null;
        Set<Long> wayIds = null;
        Set<Long> relationIds = null;

        // A way has to have two or more nodes. Try to upload a way more nodes
        // than is allowed by the system. The request should fail and no data in the system should be modified.
        try {
            originalBounds = OsmTestUtils.createStartingTestBounds();
            changesetId = OsmTestUtils.createTestChangeset(originalBounds);
            nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
            Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
            wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
            relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

            // remember the original value of "maximumWayNodes"
            originalMaximumWayNodes = HootProperties.getProperty("maximumWayNodes");

            // use a lower number of max way nodes then default for efficiency
            HootCustomPropertiesSetter.setProperty("maximumWayNodes", "2");

            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Too many nodes specified for way"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        finally {
            HootCustomPropertiesSetter.setProperty("maximumWayNodes", originalMaximumWayNodes);
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testUploadCreateNoMembersInRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);

        // relations with no members are allowed
        Document responseData = resource()
                .path("api/0.6/changeset/" + changesetId + "/upload")
                .queryParam("mapId", String.valueOf(mapId))
                .type(MediaType.TEXT_XML)
                .accept(MediaType.TEXT_XML)
                .post(Document.class,
                    "<osmChange version=\"0.3\" generator=\"iD\">" +
                        "<create>" +
                            "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                "<tag k=\"key 1\" v=\"val 1\"/>" +
                            "</relation>" +
                        "</create>" +
                        "<modify/>" +
                        "<delete if-unused=\"true\"/>" +
                    "</osmChange>");

        Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/diffResult/relation").getLength());
        Assert.assertEquals(1, new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelationsTbl).count());
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayWithNonExistentNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a way referencing a node that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + (nodeIdsArr[4] + 2) + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithNonExistentNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a relation referencing a node that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + (nodeIdsArr[4] + 2) + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithNonExistentWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a relation referencing a way that doesn't exist. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\">" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + (wayIdsArr[2] + 1) + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithNonExistentRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Try to create a relation referencing a relation that doesn't exist.
        // The request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"relation\" role=\"role1\" ref=\"" + (relationIdsArr[3] + 2) + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.NOT_FOUND, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element(s) being referenced don't exist."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    // haven't been able to find any good way to handle this yet, so I'm
    // choosing not to for now
    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWhereRelationReferencesAnotherRelationThatComesAfterIt() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to create a relation that references a relation that is
        // referenced after it in the request.  This isn't supported by the server, so the request should fail and no
        // data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                         "<osmChange version=\"0.3\" generator=\"iD\">" +
                             "<create>" +
                                 "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                     "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                     "<member type=\"relation\" role=\"role1\" ref=\"-2\"></member>" +
                                 "</relation>" +
                                 "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                     "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                     "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                 "</relation>" +
                             "</create>" +
                             "<modify/>" +
                             "<delete if-unused=\"true\"/>" +
                         "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.PRECONDITION_FAILED, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("does not exist for relation"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayWithInvisibleNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // make one of way nodes invisible
        CurrentNodes invisibleNode = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentNodesTbl)
                .where(currentNodesTbl.id.eq(nodeIdsArr[0]))
                .singleResult(currentNodesTbl);

        Assert.assertNotNull(invisibleNode);

        invisibleNode.setVisible(false);

        int success = ((int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentNodesTbl)
                .where(currentNodesTbl.id.eq(invisibleNode.getId()))
                .set(currentNodesTbl.visible, false)
                .execute());

        Assert.assertEquals(1, success);
        Assert.assertEquals(false, new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentNodesTbl)
                .where(currentNodesTbl.id.eq(nodeIdsArr[0]))
                .singleResult(currentNodesTbl.visible));

        // Try to upload a way which references an invisible node. The request
        // should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("visible for way"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithInvisibleNode() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // make one of relation node members invisible
        CurrentNodes invisibleNode = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentNodesTbl)
                .where(currentNodesTbl.id.eq(nodeIdsArr[0]))
                .singleResult(currentNodesTbl);

        Assert.assertNotNull(invisibleNode);

        invisibleNode.setVisible(false);

        int success = (int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentNodesTbl)
                .where(currentNodesTbl.id.eq(invisibleNode.getId()))
                .set(currentNodesTbl.visible, false)
                .execute();

        Assert.assertEquals(1, success);
        Assert.assertEquals(false, new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentNodesTbl)
                .where(currentNodesTbl.id.eq(nodeIdsArr[0]))
                .singleResult(currentNodesTbl.visible));

        // Try to upload a relation which references an invisible node. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML).post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("visible for relation"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithInvisibleWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // make one of relation way members invisible
        CurrentWays invisibleWay = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentWaysTbl)
                .where(currentWaysTbl.id.eq(wayIdsArr[0]))
                .singleResult(currentWaysTbl);

        Assert.assertNotNull(invisibleWay);

        invisibleWay.setVisible(false);

        int success = (int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentWaysTbl)
                .where(currentWaysTbl.id.eq(invisibleWay.getId()))
                .set(currentWaysTbl.visible, false)
                .execute();

        Assert.assertEquals(1, success);
        Assert.assertEquals(false, new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentWaysTbl)
                .where(currentWaysTbl.id.eq(wayIdsArr[0]))
                .singleResult(currentWaysTbl.visible));

        // Try to upload a relation which references an invisible way. The
        // request should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("visible for relation"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationWithInvisibleRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // make one of relation's relation members invisible
        CurrentRelations invisibleRelation = new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentRelationsTbl)
                .where(currentRelationsTbl.id.eq(relationIdsArr[0]))
                .singleResult(currentRelationsTbl);

        Assert.assertNotNull(invisibleRelation);

        invisibleRelation.setVisible(false);

        int success = (int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentRelationsTbl)
                .where(currentRelationsTbl.id.eq(invisibleRelation.getId()))
                .set(currentRelationsTbl.visible, false)
                .execute();

        Assert.assertEquals(1, success);
        Assert.assertEquals(false, new SQLQuery(conn, DbUtils.getConfiguration(mapId))
                .from(currentRelationsTbl)
                .where(currentRelationsTbl.id.eq(relationIdsArr[0]))
                .singleResult(currentRelationsTbl.visible));

        // Try to upload a relation which references an invisible relation. The request should fail
        // and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                    "<member type=\"relation\" role=\"role1\" ref=\"" + relationIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("visible for relation"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateDuplicateWayIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload two ways with the same ID. The request should fail and no
        // data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateDuplicateRelationIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload two relations with the same ID. The request should fail and no
        // data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[2] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateExistingWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset with a way that has an ID equal to that of a
        // previously created way. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML).accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<modify/>" +
                            "<create>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<way id=\"" + wayIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" + "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid OSM element ID for create"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateExistingRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Update the changeset with a relation that has an ID equal to that of
        // a previously created relation. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<modify/>" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"" + relationIdsArr[0] + "\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid OSM element ID for create"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationReferencingItself() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset with a relation that references itself as a
        // member. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<modify/>" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"relation\" role=\"role1\" ref=\"-1\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[2] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("references itself"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateInvalidChangesetIdInWay() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML).accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<way id=\"-2\" version=\"0\" changeset=\"" + (changesetId + 1) + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid changeset ID"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateInvalidChangesetIdInRelation() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the elements in the changeset lists a
        // changeset ID that doesn't match the changeset ID specified in the URL. A failure should
        // occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-2\" version=\"0\" changeset=\"" + (changesetId + 1) + "\" >" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid changeset ID"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateMissingWayTagValue() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the tags specified has no value
        // attribute. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<way id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[2] + "\"></nd>" +
                                    "<tag k=\"name 1\"/>" + // missing tag value
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error parsing tag"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateMissingRelationTagValue() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Upload a changeset where one of the tags of one of the elements
        // specified has no value attribute. A failure should occur and no data in the system should be
        // modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"way\" role=\"role1\" ref=\"" + wayIdsArr[0] + "\"></member>" +
                                    "<tag k=\"name 1\"/>" + // missing tag value
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Error parsing tag"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateDuplicateWayNodeIds() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Try to upload a way which lists the same node twice. The request
        // should fail and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<way id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Duplicate OSM element ID"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateNodeInvalidVersion() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        BoundingBox updateBounds = OsmTestUtils.createAfterModifiedTestChangesetBounds();

        // Update the changeset where one of the nodes has a version greater
        // than zero, which is what the version should be for all new elements. A failure should occur
        // and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<node id=\"-1\" lon=\"" + originalBounds.getMinLon() + "\" " +
                                      "lat=\"" + originalBounds.getMinLat() + "\" version=\"1\" changeset=\"" + changesetId + "\">" +
                                    "<tag k=\"key 1\" v=\"val 1\"></tag>" + "<tag k=\"key 2\" v=\"val 2\"></tag>" +
                                "</node>" +
                                "<node id=\"-2\" lon=\"" + updateBounds.getMinLon() + "\" " + "lat=\""
                                    + updateBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">"
                                    + "<tag k=\"key 3\" v=\"val 3\"></tag>" +
                                "</node>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayInvalidVersion() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a version greater than
        // zero, which is what the version should be for all new elements. A failure should occur
        // and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<way id=\"-2\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationInvalidVersion() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the relations has a version greater
        // than zero, which is what the version should be for all new elements. A failure should occur
        // and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-2\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayEmptyNodeId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a node with an empty
        // string for its ID. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<way id=\"-2\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"\"></nd>" + "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element in changeset has empty ID."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationEmptyMemberId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID.  A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    // empty member ID
                                    "<member type=\"node\" role=\"role1\" ref=\"\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Element in changeset has empty ID."));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateWayMissingNodeId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the ways has a node with a missing
        // ID attribute. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<way id=\"-1\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                    "<nd ref=\"" + nodeIdsArr[1] + "\"></nd>" +
                                "</way>" +
                                "<way id=\"-2\" version=\"1\" changeset=\"" + changesetId + "\" >" +
                                    "<nd></nd>" + // missing node ID
                                    "<nd ref=\"" + nodeIdsArr[0] + "\"></nd>" +
                                "</way>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.CONFLICT, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid version"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationMissingMemberId() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset with a relation that has a member with a missing
        // ID attribute. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\"\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    // mising member ID
                                    "<member type=\"node\" role=\"role1\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationInvalidMemberType() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID. A failure should occur and no data in the system should be modified.
        try {
            resource()
                    .path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    // invalid member type
                                    "<member type=\"blah\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
            Assert.assertTrue(r.getEntity(String.class).contains("Invalid relation member type"));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }

    @Test(expected = UniformInterfaceException.class)
    @Category(UnitTest.class)
    public void testUploadCreateRelationMissingMemberType() throws Exception {
        BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
        long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // Update the changeset where one of the relations has a member with an
        // empty string for an ID.  A failure should occur and no data in the system should be modified.
        try {
            resource().path("api/0.6/changeset/" + changesetId + "/upload")
                    .queryParam("mapId", String.valueOf(mapId))
                    .type(MediaType.TEXT_XML)
                    .accept(MediaType.TEXT_XML)
                    .post(Document.class,
                        "<osmChange version=\"0.3\" generator=\"iD\">" +
                            "<create>" +
                                "<relation id=\"-1\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    // missing member type
                                    "<member role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                                "<relation id=\"-2\" version=\"0\" changeset=\"" + changesetId + "\" >" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[0] + "\"></member>" +
                                    "<member type=\"node\" role=\"role1\" ref=\"" + nodeIdsArr[1] + "\"></member>" +
                                "</relation>" +
                            "</create>" +
                            "<modify/>" +
                            "<delete if-unused=\"true\"/>" +
                        "</osmChange>");
        }
        catch (UniformInterfaceException e) {
            ClientResponse r = e.getResponse();
            Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));

            OsmTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);

            throw e;
        }
        catch (Exception e) {
            log.error(e.getMessage());
            throw e;
        }
    }
}
