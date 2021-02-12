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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */
package hoot.services.controllers.osm.map;

import static hoot.services.HootProperties.MAX_QUERY_NODES;
import static hoot.services.models.db.QCurrentNodes.currentNodes;
import static hoot.services.models.db.QMaps.maps;
import static hoot.services.utils.DbUtils.createQuery;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.net.URLDecoder;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;
import java.util.stream.Collectors;

import javax.ws.rs.BadRequestException;
import javax.ws.rs.ForbiddenException;
import javax.ws.rs.NotAllowedException;
import javax.ws.rs.NotFoundException;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.xml.xpath.XPath;

import org.apache.xpath.XPathAPI;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.w3c.dom.Document;

import com.querydsl.core.Tuple;
import com.querydsl.sql.SQLExpressions;

import hoot.services.UnitTest;
import hoot.services.controllers.osm.OSMResourceTestAbstract;
import hoot.services.controllers.osm.OSMTestUtils;
import hoot.services.controllers.osm.user.UserResource;
import hoot.services.geo.BoundingBox;
import hoot.services.models.db.CurrentNodes;
import hoot.services.models.db.CurrentRelations;
import hoot.services.models.db.CurrentWays;
import hoot.services.models.db.Maps;
import hoot.services.models.db.QCurrentRelations;
import hoot.services.models.db.QCurrentWays;
import hoot.services.models.db.Users;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.osm.Map;
import hoot.services.models.osm.MapLayer;
import hoot.services.models.osm.MapLayers;
import hoot.services.models.osm.RelationMember;
import hoot.services.utils.DbUtils;
import hoot.services.utils.HootCustomPropertiesSetter;
import hoot.services.utils.MapUtils;
import hoot.services.utils.XmlUtils;


public class MapResourceTest extends OSMResourceTestAbstract {
    private void getMap(String idOrName, String multiLayerUniqueElementIdsStr,
        boolean useMultiLayerUniqueElementIdsParameter) throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // create some nodes that are completely outside the query bounds
        Set<Long> oobNodeIds = OSMTestUtils.createNodesOutsideOfQueryBounds(changesetId, queryBounds);
        Long[] oobNodeIdsArr = oobNodeIds.toArray(new Long[oobNodeIds.size()]);

        // create a way completely outside the query bounds
        long oobWayId = OSMTestUtils.insertNewWay(changesetId, mapId, new ArrayList<>(oobNodeIds), null);

        // Create a way with some nodes inside the query bounds and some
        // outside; The way and the out of bounds nodes it owns should be returned by the query since
        // at least one of the added way nodes is within the bounds.
        Set<Long> partiallyOobNodeIds = new LinkedHashSet<>();
        partiallyOobNodeIds.add(nodeIdsArr[0]);
        partiallyOobNodeIds.add(oobNodeIdsArr[0]);
        wayIds.add(OSMTestUtils.insertNewWay(changesetId, mapId, new ArrayList<>(partiallyOobNodeIds), null));
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        // create a relation where all members are completely outside of the query bounds
        List<RelationMember> members = new ArrayList<>();

        // Only oobNodeIdsArr[1] will make this relation out of bounds, b/c
        // oobNodeIdsArr[0] is used by a way which is partially in bounds and will be returned by the
        // query and, thus, any relations which reference that way and/or its nodes will also be returned.
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
        members.add(new RelationMember(oobWayId, ElementType.Way, "role1"));
        OSMTestUtils.insertNewRelation(changesetId, mapId, members, null);

        // create a relation where some members are inside the query bounds and some are not
        members = new ArrayList<>();
        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));

        // see note above for why oobNodeIdsArr[1] is used here
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role1"));
        members.add(new RelationMember(oobWayId, ElementType.Way, "role1"));
        relationIds.add(OSMTestUtils.insertNewRelation(changesetId, mapId, members, null));
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Query the elements back out geospatially. All but one of the
        // nodes, one of the ways, and one of the relations should be returned.
        Document responseData = null;
        if (useMultiLayerUniqueElementIdsParameter) {
            responseData = target("api/0.6/map/" + idOrName + "/" + queryBounds.toServicesString())
                    .queryParam("multiLayerUniqueElementIds", multiLayerUniqueElementIdsStr)
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        else {
            responseData = target("api/0.6/map/" + idOrName + "/" + queryBounds.toServicesString())
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }

        assertNotNull(responseData);

        OSMTestUtils.verifyOsmHeader(responseData);
        OSMTestUtils.verifyBounds(responseData, queryBounds);

        XPath xpath = XmlUtils.createXPath();

        boolean multiLayerUniqueElementIds = multiLayerUniqueElementIdsStr.toLowerCase().equals("true");
        assertEquals(6, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyNode(responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId,
                    originalBounds.getMinLat(), originalBounds.getMinLon(), false);
            OSMTestUtils.verifyNode(responseData, 2, String.valueOf(nodeIdsArr[1]), changesetId,
                    originalBounds.getMaxLat(), originalBounds.getMaxLon(), false);
            OSMTestUtils.verifyNode(responseData, 3, String.valueOf(nodeIdsArr[2]), changesetId,
                    originalBounds.getMinLat(), originalBounds.getMinLon(), false);
            OSMTestUtils.verifyNode(responseData, 4, String.valueOf(nodeIdsArr[3]), changesetId,
                    originalBounds.getMinLat(), originalBounds.getMinLon(), false);
            OSMTestUtils.verifyNode(responseData, 5, String.valueOf(nodeIdsArr[4]), changesetId,
                    originalBounds.getMinLat(), originalBounds.getMinLon(), false);
            OSMTestUtils.verifyNode(responseData, 6, String.valueOf(oobNodeIdsArr[0]), changesetId,
                    queryBounds.getMinLat() - 5, queryBounds.getMinLon() - 5, false);
        }
        else {
            OSMTestUtils.verifyNode(responseData, 1, mapId + "_n_" + nodeIdsArr[0], changesetId,
                    originalBounds.getMinLat(), originalBounds.getMinLon(), true);
            OSMTestUtils.verifyNode(responseData, 2, mapId + "_n_" + nodeIdsArr[1], changesetId,
                    originalBounds.getMaxLat(), originalBounds.getMaxLon(), true);
            OSMTestUtils.verifyNode(responseData, 3, mapId + "_n_" + nodeIdsArr[2], changesetId,
                    originalBounds.getMinLat(), originalBounds.getMinLon(), true);
            OSMTestUtils.verifyNode(responseData, 4, mapId + "_n_" + nodeIdsArr[3], changesetId,
                    originalBounds.getMinLat(), originalBounds.getMinLon(), true);
            OSMTestUtils.verifyNode(responseData, 5, mapId + "_n_" + nodeIdsArr[4], changesetId,
                    originalBounds.getMinLat(), originalBounds.getMinLon(), true);
            OSMTestUtils.verifyNode(responseData, 6, mapId + "_n_" + oobNodeIdsArr[0], changesetId,
                    queryBounds.getMinLat() - 5, queryBounds.getMinLon() - 5, true);
        }

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        assertEquals("val 1",
                URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        assertEquals("val 2",
                URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        assertEquals("key 3", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        assertEquals("val 3",
                URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node[5]/tag").getLength());
        assertEquals("key 4", xpath.evaluate("//osm/node[5]/tag[1]/@k", responseData));
        assertEquals("val 4",
                URLDecoder.decode(xpath.evaluate("//osm/node[5]/tag[1]/@v", responseData), "UTF-8"));

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
        Set<Long> wayNodeIds = new LinkedHashSet<>();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayNodeIds.add(nodeIdsArr[4]);
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyWay(responseData, 1, String.valueOf(wayIdsArr[0]), changesetId, wayNodeIds,
                    false);
        }
        else {
            OSMTestUtils.verifyWay(responseData, 1, mapId + "_w_" + wayIdsArr[0], changesetId, wayNodeIds,
                    true);
        }
        wayNodeIds.clear();
        wayNodeIds.add(nodeIdsArr[2]);
        wayNodeIds.add(nodeIdsArr[1]);
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyWay(responseData, 2, String.valueOf(wayIdsArr[1]), changesetId, wayNodeIds,
                    false);
        }
        else {
            OSMTestUtils.verifyWay(responseData, 2, mapId + "_w_" + wayIdsArr[1], changesetId, wayNodeIds,
                    true);
        }
        wayNodeIds.clear();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyWay(responseData, 3, String.valueOf(wayIdsArr[2]), changesetId, wayNodeIds,
                    false);
        }
        else {
            OSMTestUtils.verifyWay(responseData, 3, mapId + "_w_" + wayIdsArr[2], changesetId, wayNodeIds,
                    true);
        }
        wayNodeIds.clear();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(oobNodeIdsArr[0]);
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyWay(responseData, 4, String.valueOf(wayIdsArr[3]), changesetId, wayNodeIds,
                    false);
        }
        else {
            OSMTestUtils.verifyWay(responseData, 4, mapId + "_w_" + wayIdsArr[3], changesetId, wayNodeIds,
                    true);
        }
        wayNodeIds.clear();

        assertEquals(3, XPathAPI.selectNodeList(responseData, "//osm/way/tag").getLength());
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/way[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/way[1]/tag[1]/@k", responseData));
        assertEquals("val 1",
                URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 2", xpath.evaluate("//osm/way[1]/tag[2]/@k", responseData));
        assertEquals("val 2",
                URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[2]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way[2]/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/way[3]/tag").getLength());
        assertEquals("key 3", xpath.evaluate("//osm/way[3]/tag[1]/@k", responseData));
        assertEquals("val 3",
                URLDecoder.decode(xpath.evaluate("//osm/way[3]/tag[1]/@v", responseData), "UTF-8"));

        assertEquals(5, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());
        members = new ArrayList<>();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[1], ElementType.Way, "role3"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role2"));
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node));

        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyRelation(responseData, 1, String.valueOf(relationIdsArr[0]), changesetId,
                    members, false);
        }
        else {
            OSMTestUtils.verifyRelation(responseData, 1, mapId + "_r_" + relationIdsArr[0], changesetId,
                    members, true);
        }

        members.clear();

        members.add(new RelationMember(nodeIdsArr[4], ElementType.Node, "role1"));
        members.add(new RelationMember(relationIdsArr[0], ElementType.Relation, "role1"));
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyRelation(responseData, 2, String.valueOf(relationIdsArr[1]), changesetId,
                    members, false);
        }
        else {
            OSMTestUtils.verifyRelation(responseData, 2, mapId + "_r_" + relationIdsArr[1], changesetId,
                    members, true);
        }
        members.clear();

        members.add(new RelationMember(wayIdsArr[1], ElementType.Way));
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyRelation(responseData, 3, String.valueOf(relationIdsArr[2]), changesetId,
                    members, false);
        }
        else {
            OSMTestUtils.verifyRelation(responseData, 3, mapId + "_r_" + relationIdsArr[2], changesetId,
                    members, true);
        }
        members.clear();

        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node, "role1"));
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyRelation(responseData, 4, String.valueOf(relationIdsArr[3]), changesetId,
                    members, false);
        }
        else {
            OSMTestUtils.verifyRelation(responseData, 4, mapId + "_r_" + relationIdsArr[3], changesetId,
                    members, true);
        }
        members.clear();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role1"));
        members.add(new RelationMember(oobWayId, ElementType.Way, "role1"));
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter) {
            OSMTestUtils.verifyRelation(responseData, 5, String.valueOf(relationIdsArr[4]), changesetId,
                    members, false);
        }
        else {
            OSMTestUtils.verifyRelation(responseData, 5, mapId + "_r_" + relationIdsArr[4], changesetId,
                    members, true);
        }
        members.clear();

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/relation/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/relation[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/relation[1]/tag[1]/@k", responseData));
        assertEquals("val 1",
                URLDecoder.decode(xpath.evaluate("//osm/relation[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/relation[2]/tag").getLength());
        assertEquals("key 2", xpath.evaluate("//osm/relation[2]/tag[1]/@k", responseData));
        assertEquals("val 2",
                URLDecoder.decode(xpath.evaluate("//osm/relation[2]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 3", xpath.evaluate("//osm/relation[2]/tag[2]/@k", responseData));
        assertEquals("val 3",
                URLDecoder.decode(xpath.evaluate("//osm/relation[2]/tag[2]/@v", responseData), "UTF-8"));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/relation[3]/tag").getLength());
        assertEquals("key 4", xpath.evaluate("//osm/relation[3]/tag[1]/@k", responseData));
        assertEquals("val 4",
                URLDecoder.decode(xpath.evaluate("//osm/relation[3]/tag[1]/@v", responseData), "UTF-8"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapById() throws Exception {
        getMap(String.valueOf(mapId), "false", true);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapByNegativeOneId() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapByName() throws Exception {
        getMap("map-with-id-" + mapId, "false", true);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapMultiLayerUniqueElementIds() throws Exception {
        getMap(String.valueOf(mapId), "true", true);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetEmptyMap() throws Exception {
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();

        // Query an empty map. No elements should be returned.
        Document responseData = target("api/0.6/map/" + String.valueOf(mapId) + "/" + queryBounds.toServicesString())
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);

        assertNotNull(responseData);

        OSMTestUtils.verifyOsmHeader(responseData);
        OSMTestUtils.verifyBounds(responseData, queryBounds);

        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapNoWays() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

        Set<Long> relationIds = OSMTestUtils.createTestRelationsNoWays(changesetId, nodeIds);

        // create some nodes that are completely outside the query bounds
        Set<Long> oobNodeIds = OSMTestUtils.createNodesOutsideOfQueryBounds(changesetId, queryBounds);
        Long[] oobNodeIdsArr = oobNodeIds.toArray(new Long[oobNodeIds.size()]);

        // create a relation where all members are completely outside of the query bounds
        List<RelationMember> members = new ArrayList<>();

        // Only oobNodeIdsArr[1] will make this relation out of bounds, b/c oobNodeIdsArr[0] is used
        // by a way which is partially in bounds and will be returned by the query and, thus, any
        // relations which reference that way and/or its nodes will also be returned.
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
        OSMTestUtils.insertNewRelation(changesetId, mapId, members, null);

        // create a relation where some members are inside the query bounds and some are not
        members = new ArrayList<>();
        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));

        // see note above for why oobNodeIdsArr[1] is used here
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
        relationIds.add(OSMTestUtils.insertNewRelation(changesetId, mapId, members, null));
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // Query the elements back out geospatially. All but one of the
        // nodes, one of the ways, and one of the relations should be returned.
        Document responseData = target("api/0.6/map/" + String.valueOf(mapId) + "/" + queryBounds.toServicesString())
                .request(MediaType.TEXT_XML)
                .get(Document.class);

        assertNotNull(responseData);

        OSMTestUtils.verifyOsmHeader(responseData);
        OSMTestUtils.verifyBounds(responseData, queryBounds);

        XPath xpath = XmlUtils.createXPath();

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        OSMTestUtils.verifyNode(responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 2, String.valueOf(nodeIdsArr[2]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 3, String.valueOf(nodeIdsArr[3]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 4, String.valueOf(nodeIdsArr[4]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        assertEquals("val 1", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        assertEquals("val 2", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());
        assertEquals("key 3", xpath.evaluate("//osm/node[3]/tag[1]/@k", responseData));
        assertEquals("val 3", URLDecoder.decode(xpath.evaluate("//osm/node[3]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        assertEquals("key 4", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        assertEquals("val 4", URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());

        members = new ArrayList<>();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node));

        OSMTestUtils.verifyRelation(responseData, 1, String.valueOf(relationIdsArr[0]), changesetId, members, false);

        members.clear();

        members.add(new RelationMember(nodeIdsArr[4], ElementType.Node, "role1"));
        members.add(new RelationMember(relationIdsArr[0], ElementType.Relation, "role1"));

        OSMTestUtils.verifyRelation(responseData, 2, String.valueOf(relationIdsArr[1]), changesetId, members, false);

        members.clear();

        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node, "role1"));

        OSMTestUtils.verifyRelation(responseData, 3, String.valueOf(relationIdsArr[2]), changesetId, members, false);

        members.clear();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));

        OSMTestUtils.verifyRelation(responseData, 4, String.valueOf(relationIdsArr[3]), changesetId, members, false);

        members.clear();

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/relation/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/relation[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/relation[1]/tag[1]/@k", responseData));
        assertEquals("val 1", URLDecoder.decode(xpath.evaluate("//osm/relation[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/relation[2]/tag").getLength());
        assertEquals("key 2", xpath.evaluate("//osm/relation[2]/tag[1]/@k", responseData));
        assertEquals("val 2", URLDecoder.decode(xpath.evaluate("//osm/relation[2]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 3", xpath.evaluate("//osm/relation[2]/tag[2]/@k", responseData));
        assertEquals("val 3", URLDecoder.decode(xpath.evaluate("//osm/relation[2]/tag[2]/@v", responseData), "UTF-8"));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/relation[3]/tag").getLength());
        assertEquals("key 4", xpath.evaluate("//osm/relation[3]/tag[1]/@k", responseData));
        assertEquals("val 4", URLDecoder.decode(xpath.evaluate("//osm/relation[3]/tag[1]/@v", responseData), "UTF-8"));
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapNoRelations() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);

        // create some nodes that are completely outside the query bounds
        Set<Long> oobNodeIds = OSMTestUtils.createNodesOutsideOfQueryBounds(changesetId, queryBounds);
        Long[] oobNodeIdsArr = oobNodeIds.toArray(new Long[oobNodeIds.size()]);

        // create a way completely outside the query bounds
        /* long oobWayId = */
        OSMTestUtils.insertNewWay(changesetId, mapId, new ArrayList<>(oobNodeIds), null);

        // create a way with some nodes inside the query bounds and some
        // outside; The way and the
        // out of bounds nodes it owns should be returned by the query since
        // at least one of the added
        // way nodes is within the bounds.
        Set<Long> partiallyOobNodeIds = new LinkedHashSet<>();
        partiallyOobNodeIds.add(nodeIdsArr[0]);
        partiallyOobNodeIds.add(oobNodeIdsArr[0]);
        wayIds.add(OSMTestUtils.insertNewWay(changesetId, mapId, new ArrayList<>(partiallyOobNodeIds), null));
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);

        // Query the elements back out geospatially. All but one of the
        // nodes, one of the ways, and
        // one of the relations should be returned.
        Document responseData = target("api/0.6/map/" + String.valueOf(mapId) + "/" + queryBounds.toServicesString())
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);

        assertNotNull(responseData);

        OSMTestUtils.verifyOsmHeader(responseData);
        OSMTestUtils.verifyBounds(responseData, queryBounds);

        XPath xpath = XmlUtils.createXPath();

        assertEquals(6, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        OSMTestUtils.verifyNode(responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 2, String.valueOf(nodeIdsArr[1]), changesetId,
                originalBounds.getMaxLat(), originalBounds.getMaxLon(), false);
        OSMTestUtils.verifyNode(responseData, 3, String.valueOf(nodeIdsArr[2]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 4, String.valueOf(nodeIdsArr[3]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 5, String.valueOf(nodeIdsArr[4]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 6, String.valueOf(oobNodeIdsArr[0]), changesetId,
                queryBounds.getMinLat() - 5, queryBounds.getMinLon() - 5, false);

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        assertEquals("val 1", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        assertEquals("val 2", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        assertEquals("key 3", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        assertEquals("val 3", URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node[5]/tag").getLength());
        assertEquals("key 4", xpath.evaluate("//osm/node[5]/tag[1]/@k", responseData));
        assertEquals("val 4", URLDecoder.decode(xpath.evaluate("//osm/node[5]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());

        Set<Long> wayNodeIds = new LinkedHashSet<>();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayNodeIds.add(nodeIdsArr[4]);

        OSMTestUtils.verifyWay(responseData, 1, String.valueOf(wayIdsArr[0]), changesetId, wayNodeIds, false);

        wayNodeIds.clear();

        wayNodeIds.add(nodeIdsArr[2]);
        wayNodeIds.add(nodeIdsArr[1]);

        OSMTestUtils.verifyWay(responseData, 2, String.valueOf(wayIdsArr[1]), changesetId, wayNodeIds, false);

        wayNodeIds.clear();

        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);

        OSMTestUtils.verifyWay(responseData, 3, String.valueOf(wayIdsArr[2]), changesetId, wayNodeIds, false);

        wayNodeIds.clear();

        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(oobNodeIdsArr[0]);

        OSMTestUtils.verifyWay(responseData, 4, String.valueOf(wayIdsArr[3]), changesetId, wayNodeIds, false);

        wayNodeIds.clear();

        assertEquals(3, XPathAPI.selectNodeList(responseData, "//osm/way/tag").getLength());
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/way[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/way[1]/tag[1]/@k", responseData));
        assertEquals("val 1", URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 2", xpath.evaluate("//osm/way[1]/tag[2]/@k", responseData));
        assertEquals("val 2", URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[2]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way[2]/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/way[3]/tag").getLength());
        assertEquals("key 3", xpath.evaluate("//osm/way[3]/tag[1]/@k", responseData));
        assertEquals("val 3", URLDecoder.decode(xpath.evaluate("//osm/way[3]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapNoWaysOrRelations() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);

        // create some nodes that are completely outside the query bounds
        /* Set<Long> oobNodeIds = */
        OSMTestUtils.createNodesOutsideOfQueryBounds(changesetId, queryBounds);

        // Query the elements back out geospatially. All but one of the
        // nodes, one of the ways, and one of the relations should be returned.
        Document responseData = target("api/0.6/map/" + String.valueOf(mapId) + "/" + queryBounds.toServicesString())
                .request(MediaType.TEXT_XML)
                .get(Document.class);

        assertNotNull(responseData);

        OSMTestUtils.verifyOsmHeader(responseData);
        OSMTestUtils.verifyBounds(responseData, queryBounds);

        XPath xpath = XmlUtils.createXPath();

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        OSMTestUtils.verifyNode(responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 2, String.valueOf(nodeIdsArr[2]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 3, String.valueOf(nodeIdsArr[3]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 4, String.valueOf(nodeIdsArr[4]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        assertEquals("val 1", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        assertEquals("val 2", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());
        assertEquals("key 3", xpath.evaluate("//osm/node[3]/tag[1]/@k", responseData));
        assertEquals("val 3", URLDecoder.decode(xpath.evaluate("//osm/node[3]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        assertEquals("key 4", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        assertEquals("val 4", URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapWithInvisibleElements() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Long[] nodeIdsArr = nodeIds.toArray(new Long[nodeIds.size()]);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Long[] wayIdsArr = wayIds.toArray(new Long[wayIds.size()]);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        Long[] relationIdsArr = relationIds.toArray(new Long[relationIds.size()]);

        // make one of the nodes invisible, so it shouldn't be returned in a map query
        CurrentNodes invisibleNode = createQuery(mapId)
                .select(currentNodes)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[3]))
                .fetchFirst();

        assertNotNull(invisibleNode);

        invisibleNode.setVisible(false);

        long success = createQuery(mapId).update(currentNodes)
                .where(currentNodes.id.eq(invisibleNode.getId()))
                .set(currentNodes.visible, false)
                .execute();

        assertEquals(1, success);

        assertEquals(false, createQuery(mapId)
                .select(currentNodes.visible)
                .from(currentNodes)
                .where(currentNodes.id.eq(nodeIdsArr[3]))
                .fetchOne());

        // make one of the ways invisible, so it shouldn't be returned in a map query
        QCurrentWays currentWays = QCurrentWays.currentWays;
        CurrentWays invisibleWay = createQuery(mapId)
                .select(currentWays)
                .from(currentWays)
                .where(currentWays.id.eq(wayIdsArr[0]))
                .fetchOne();

        assertNotNull(invisibleWay);

        invisibleWay.setVisible(false);

        success = createQuery(mapId).update(currentWays)
                .where(currentWays.id.eq(invisibleWay.getId()))
                .set(currentWays.visible, false)
                .execute();

        assertEquals(1, success);

        assertEquals(false, createQuery(mapId)
                .select(currentWays.visible)
                .from(currentWays)
                .where(currentWays.id.eq(wayIdsArr[0]))
                .fetchOne());

        // make one of the relations invisible, so it shouldn't be returned in a map query
        QCurrentRelations currentRelations = QCurrentRelations.currentRelations;
        CurrentRelations invisibleRelation = createQuery(mapId)
                .select(currentRelations)
                .from(currentRelations)
                .where(currentRelations.id.eq(relationIdsArr[0]))
                .fetchOne();

        assertNotNull(invisibleRelation);

        invisibleRelation.setVisible(false);

        success = createQuery(mapId).update(currentRelations)
                .where(currentRelations.id.eq(invisibleRelation.getId()))
                .set(currentRelations.visible, false)
                .execute();

        assertEquals(1, success);

        assertEquals(false, createQuery(mapId)
                .select(currentRelations.visible)
                .from(currentRelations)
                .where(currentRelations.id.eq(relationIdsArr[0]))
                .fetchOne());

        // Query the elements back out geospatially and ensure the invisible
        // node and way do not come back in the results.
        Document responseData = target("api/0.6/map/" + String.valueOf(mapId) + "/" + queryBounds.toServicesString())
                .request(MediaType.TEXT_XML)
                .get(Document.class);

        assertNotNull(responseData);

        OSMTestUtils.verifyOsmHeader(responseData);
        OSMTestUtils.verifyBounds(responseData, queryBounds);

        assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        OSMTestUtils.verifyNode(responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 2, String.valueOf(nodeIdsArr[1]), changesetId,
                originalBounds.getMaxLat(), originalBounds.getMaxLon(), false);
        OSMTestUtils.verifyNode(responseData, 3, String.valueOf(nodeIdsArr[2]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);
        OSMTestUtils.verifyNode(responseData, 4, String.valueOf(nodeIdsArr[4]), changesetId,
                originalBounds.getMinLat(), originalBounds.getMinLon(), false);

        XPath xpath = XmlUtils.createXPath();
        assertEquals(3, XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        assertEquals("val 1", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        assertEquals("val 2", URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        assertEquals("key 4", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        assertEquals("val 4", URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));
        assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());

        Set<Long> wayNodeIds = new LinkedHashSet<>();
        wayNodeIds.add(nodeIdsArr[2]);
        wayNodeIds.add(nodeIdsArr[1]);

        OSMTestUtils.verifyWay(responseData, 1, String.valueOf(wayIdsArr[1]), changesetId, wayNodeIds, false);

        wayNodeIds.clear();

        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);

        OSMTestUtils.verifyWay(responseData, 2, String.valueOf(wayIdsArr[2]), changesetId, wayNodeIds, false);
        wayNodeIds.clear();

        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/way/tag").getLength());
        assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way[1]/tag").getLength());
        assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/way[2]/tag").getLength());
        assertEquals("key 3", xpath.evaluate("//osm/way[2]/tag[1]/@k", responseData));
        assertEquals("val 3", URLDecoder.decode(xpath.evaluate("//osm/way[2]/tag[1]/@v", responseData), "UTF-8"));

        OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testGetMapNodeLimitExceeded() throws Exception {
        String originalMaxQueryNodes = MAX_QUERY_NODES;
        try {
            long maxQueryNumberOfNodes = 3;

            // use a lower number of max query nodes than default for efficiency
            HootCustomPropertiesSetter.setProperty("MAX_QUERY_NODES", String.valueOf(maxQueryNumberOfNodes));

            BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
            BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
            long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
            Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
            Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
            Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

            assertTrue(maxQueryNumberOfNodes < createQuery(mapId)
                    .from(currentNodes)
                    .fetchCount());

            final String expectedErrorMsg = "The maximum number of nodes that may be returned in a map query is "
                    + maxQueryNumberOfNodes + ". This query returned " + (maxQueryNumberOfNodes + 1)
                    + " nodes. Please " + "execute a query which returns fewer nodes.";
            try {
                // try to run a query that returns more than the maximum allowed
                // results size
                target("api/0.6/map/" + String.valueOf(mapId) + "/" + queryBounds.toServicesString())
                        .request(MediaType.TEXT_XML).get(Document.class);
            }
            catch (BadRequestException e) {
                Response r = e.getResponse();
                assertEquals(Response.Status.BAD_REQUEST, Response.Status.fromStatusCode(r.getStatus()));
                assertEquals(r.readEntity(String.class), expectedErrorMsg);

                OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
                throw e;
            }
        }
        finally {
            HootCustomPropertiesSetter.setProperty("MAX_QUERY_NODES", originalMaxQueryNodes);
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetMapInvalidMap() throws Exception {
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
        try {
            // try to query nodes from a map that doesn't exist
            target("api/0.6/map/-2/" + queryBounds.toServicesString()).request(MediaType.TEXT_XML).get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(404, r.getStatus());
            assertEquals(r.readEntity(String.class), "No map with that id exists");

            // Map::query() throws an exception because there is no map "table"
            // by that name corrupting the current transaction it is not
            // possible to execute any additional queries without a rollback.
            //
            // A rollback would undo any changes anyway. This test should be
            // added to a successful map fetch =>
            // testGetMapWithInvisibleElements() does this for us.
            //
            // OSMTestUtils.verifyTestDataUnmodified(originalBounds,
            // changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = NotFoundException.class)
    @Category(UnitTest.class)
    public void testGetMapEmptyMapId() throws Exception {
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
        try {
            // try to query nodes from a map that doesn't exist
            //
            // 2018-10-04: with the change from query params to path params
            // and double slash URIs being rejected by spring security
            // this response has changed to a 404 due to the invalid
            // URI.
            // https://docs.spring.io/spring-security/site/docs/5.0.0.RELEASE/reference/htmlsingle/#request-matching
            target("api/0.6/map//" + queryBounds.toServicesString())
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotFoundException e) {
            Response r = e.getResponse();
            assertEquals(404, r.getStatus());
            assertEquals(r.readEntity(String.class), "");
            // Map::query() throws an exception because there is no map "table"
            // by that name corrupting the current transaction it is not
            // possible to execute any additional queries without a rollback.
            //
            // A rollback would undo any changes anyway. This test should be
            // added to a successful map fetch =>
            // testGetMapWithInvisibleElements() does this for us.
            //
            // OSMTestUtils.verifyTestDataUnmodified(originalBounds,
            // changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    @Test(expected = BadRequestException.class)
    @Category(UnitTest.class)
    public void testGetMapByNonUniqueName() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        BoundingBox queryBounds = OSMTestUtils.createTestQueryBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        // insert another map with the same name as the test map
        Maps map = new Maps();
        long nextMapId = createQuery(mapId)
                .select(SQLExpressions.nextval(Long.class, "maps_id_seq"))
                .from()
                .fetchFirst();

        map.setId(nextMapId);
        Timestamp now = super.getCurrentDBTime();
        map.setCreatedAt(now);
        String duplicatedMapName = "map-with-id-" + mapId;
        map.setDisplayName(duplicatedMapName);
        map.setUserId(userId);

        createQuery(mapId).insert(maps).populate(map).execute();
        final String expectedErrorMsg = "Multiple records exist with NAME = " + duplicatedMapName
                + " in 'maps' table. Please specify a single, valid record.";
        try {
            // try to query nodes from a map name that is linked to multiple map IDs
            target("api/0.6/map/" + duplicatedMapName + "/" + queryBounds.toServicesString())
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (BadRequestException e) {
            Response r = e.getResponse();
            assertEquals(400, r.getStatus());
            assertEquals(r.readEntity(String.class), expectedErrorMsg);
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    // 10/09/2018 Since the introduction of HTTP VERBS, when
    // paths collide, you get NotAllowed instead of NotFound
    @Test(expected = NotAllowedException.class)
    @Category(UnitTest.class)
    public void testGetMapMissingBounds() throws Exception {
        BoundingBox originalBounds = OSMTestUtils.createStartingTestBounds();
        long changesetId = OSMTestUtils.createTestChangeset(originalBounds);
        Set<Long> nodeIds = OSMTestUtils.createTestNodes(changesetId, originalBounds);
        Set<Long> wayIds = OSMTestUtils.createTestWays(changesetId, nodeIds);
        Set<Long> relationIds = OSMTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

        try {
            // try to query nodes without specifying a bounds
            target("api/0.6/map/" + String.valueOf(mapId) + "/")
                    .request(MediaType.TEXT_XML)
                    .get(Document.class);
        }
        catch (NotAllowedException e) {
            Response r = e.getResponse();
            assertEquals(Response.Status.METHOD_NOT_ALLOWED, Response.Status.fromStatusCode(r.getStatus()));
            OSMTestUtils.verifyTestDataUnmodified(originalBounds, changesetId, nodeIds, wayIds, relationIds);
            throw e;
        }
    }

    // As of 02/10/2017 Boundary check is still disabled on the server side.
    // There is an issue (https://github.com/ngageoint/hootenanny/issues/1199) in github that's related to this test.
    @Ignore
    @Test(/*expected = WebApplicationException.class*/)
    @Category(UnitTest.class)
    public void testGetMapBoundsOutsideWorld() throws Exception {
        // Try to query nodes with invalid bounds.
        target("api/0.6/map/" + String.valueOf(mapId) + "/-181,-90,180,90").request(MediaType.TEXT_XML)
                .get(Document.class);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapMissingMultiLayerUniqueElementIdsParam() throws Exception {
        getMap(String.valueOf(mapId), "true", false);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapInvalidMultiLayerUniqueElementIdsParam() throws Exception {
        getMap(String.valueOf(mapId), "blah", true);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapEmptyMultiLayerUniqueElementIdsParam() throws Exception {
        getMap(String.valueOf(mapId), "", true);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetLayers() throws Exception {
        List<Long> mapIds = new ArrayList<>();

        // existing test layer
        mapIds.add(mapId);

        // create some more maps
        long mapId2 = MapUtils.insertMap(userId);
        mapIds.add(mapId2);

        long mapId3 = MapUtils.insertMap(userId);
        mapIds.add(mapId3);

        // query out the layers
        MapLayers mapLayers = target("api/0.6/map/layers").request(MediaType.APPLICATION_JSON).get(MapLayers.class);

        assertNotNull(mapLayers);
        assertNotNull(mapLayers.getLayers());
        assertTrue(mapLayers.getLayers().length >= 3);

        boolean foundFirstId = false;
        boolean foundSecondId = false;
        boolean foundThirdId = false;

        for (int i = 0; i < mapLayers.getLayers().length; i++) {
            MapLayer layer = mapLayers.getLayers()[i];
            int mapIdsIndex = -1;
            if (layer.getId() == mapIds.get(0)) {
                foundFirstId = true;
                mapIdsIndex = 0;
            }
            else if (layer.getId() == mapIds.get(1)) {
                foundSecondId = true;
                mapIdsIndex = 1;
            }
            else if (layer.getId() == mapIds.get(2)) {
                foundThirdId = true;
                mapIdsIndex = 2;
            }
            if (mapIdsIndex != -1) {
                assertEquals("map-with-id-" + mapIds.get(mapIdsIndex), layer.getName());
            }
        }

        assertTrue(foundFirstId && foundSecondId && foundThirdId);
    }

    @Test
    @Category(UnitTest.class)
    public void testGetDeletedLayer() throws Exception {
        createQuery(mapId).delete(maps).where(maps.id.eq(mapId)).execute();

        assertNull(createQuery(mapId).select(maps).from(maps).where(maps.id.eq(mapId)).fetchOne());

        // query out the layers
        MapLayers mapLayers = target("api/0.6/map/layers").request(MediaType.APPLICATION_JSON).get(MapLayers.class);

        assertNotNull(mapLayers);

        MapLayer[] mLayers = mapLayers.getLayers();

        if (mLayers != null) {
            for (MapLayer layer : mLayers) {
                if (layer.getId() == mapId) {
                    fail("Map with ID = " + mapId + " must not be present!");
                }
            }
        }
    }

    @Test
    @Category(UnitTest.class)
    public void testGenerateExtentOSM() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testGetTileNodesCounts() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteLayers() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testDeleteStaleLayers() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testGetStaleLayers() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testModifyName() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testUpdateFolderMapLink() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testGetTags() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testGetAllIds() throws Exception {
        //TODO
    }

    @Test
    @Category(UnitTest.class)
    public void testGetMapForAdmin() throws Exception {

        MapUtils.cleanupTestUsers();

        long userId = MapUtils.insertUser();
        assertTrue(userId < 0);

        long advancedId = MapUtils.insertAdvancedUser();
        long adminId = MapUtils.insertAdminUser();
        long advAdminId = MapUtils.insertAdvancedAdminUser();

        Users user = MapUtils.getUser(userId);
        assertNotNull(user);
        assertFalse(UserResource.adminUserCheck(user));
        assertFalse(UserResource.userPrivilegeCheck(user, "advanced"));

        Users advancedUser = MapUtils.getUser(advancedId);
        assertFalse(UserResource.adminUserCheck(advancedUser));
        assertTrue(UserResource.userPrivilegeCheck(advancedUser, "advanced"));

        Users adminUser = MapUtils.getUser(adminId);
        assertTrue(UserResource.adminUserCheck(adminUser));
        assertFalse(UserResource.userPrivilegeCheck(adminUser, "advanced"));

        Users advancedAdminUser = MapUtils.getUser(advAdminId);
        assertTrue(UserResource.adminUserCheck(advancedAdminUser));
        assertTrue(UserResource.userPrivilegeCheck(advancedAdminUser, "advanced"));

        long mapId = MapUtils.insertMap(userId);
        long userFolderId = MapUtils.addPrivateFolder("userFolder", userId);
        DbUtils.updateFolderMapping(mapId, userFolderId);

        long adminMapId = MapUtils.insertMap(adminId);
        long adminFolderId = MapUtils.addPrivateFolder("adminFolder", adminId);
        DbUtils.updateFolderMapping(adminMapId, adminFolderId);

        //user should not see admin private folder
//        FolderResource.get(folders, f, advancedAdminUser)
        assertNotNull(FolderResource.getFolderForUser(user, userFolderId));
        assertNotNull(FolderResource.getFolderForUser(adminUser, userFolderId));

        //user should not see admin private map
        List<Tuple> userMaps = DbUtils.getMapsForUser(user);
        assertTrue(userMaps.stream().map(m -> m.get(maps).getId()).collect(Collectors.toList()).contains(mapId));
        assertFalse(userMaps.stream().map(m -> m.get(maps).getId()).collect(Collectors.toList()).contains(adminMapId));

        //admin should see user private folder
//      FolderResource.folderIsPublic(folders, f, advancedAdminUser)
        try {
            FolderResource.getFolderForUser(user, adminFolderId);
            fail();
        } catch (ForbiddenException ex) {
            assertTrue(true);
        }
        assertNotNull(FolderResource.getFolderForUser(adminUser, adminFolderId));


        //admin should see user private map
        List<Tuple> adminMaps = DbUtils.getMapsForUser(adminUser);
        assertTrue(adminMaps.stream().map(m -> m.get(maps).getId()).collect(Collectors.toList()).contains(mapId));
        assertTrue(adminMaps.stream().map(m -> m.get(maps).getId()).collect(Collectors.toList()).contains(adminMapId));

        Map userMap = new Map(mapId);
        userMap.setUserId(userId);
        Map adminMap = new Map(adminMapId);
        adminMap.setUserId(adminId);
        assertTrue(userMap.isVisibleTo(user));
        assertTrue(userMap.isVisibleTo(adminUser));
        assertFalse(adminMap.isVisibleTo(user));
        assertTrue(adminMap.isVisibleTo(adminUser));
    }

}
