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
package hoot.services.controllers.osm;

import java.io.IOException;
import java.net.URLDecoder;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Properties;
import java.util.Set;

import javax.ws.rs.core.MediaType;
import javax.xml.xpath.XPath;

import org.junit.Assert;
import org.junit.Ignore;
import org.apache.xpath.XPathAPI;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.CurrentNodes;
import hoot.services.db2.CurrentRelations;
import hoot.services.db2.CurrentWays;
import hoot.services.db2.Maps;
import hoot.services.db2.QCurrentNodes;
import hoot.services.db2.QCurrentRelations;
import hoot.services.db2.QCurrentWays;
import hoot.services.db2.QMaps;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.MapLayer;
import hoot.services.models.osm.MapLayers;
import hoot.services.models.osm.Relation;
import hoot.services.models.osm.RelationMember;
import hoot.services.models.osm.Way;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.osm.OsmTestUtils;
import hoot.services.utils.XmlUtils;

import com.mysema.query.sql.SQLExpressions;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLDeleteClause;
import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.UniformInterfaceException;
import com.sun.jersey.api.client.ClientResponse.Status;

/*
 * @todo Most of these tests could be converted to integration tests and after a refactoring,
 * could be replace with unit tests that test only the internal classes being used by this
 * Jersey resource.
 */
public class MapResourceTest extends OsmResourceTestAbstract
{
  private static final Logger log = LoggerFactory.getLogger(MapResourceTest.class);

  public MapResourceTest() throws NumberFormatException, IOException
  {
    super("hoot.services.controllers.osm");
  }

  private void getMap(final String idOrName, final String multiLayerUniqueElementIdsStr,
    final boolean useMultiLayerUniqueElementIdsParameter) throws Exception
  {
    try
    {
      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
      final Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
      final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

      Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);
      Set<Long> relationIds =
        OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

      //create some nodes that are completely outside the query bounds
      final Set<Long> oobNodeIds =
        OsmTestUtils.createNodesOutsideOfQueryBounds(changesetId, queryBounds);
      final Long[] oobNodeIdsArr = oobNodeIds.toArray(new Long[]{});

      //create a way completely outside the query bounds
      final long oobWayId =
        Way.insertNew(changesetId, mapId, new ArrayList<Long>(oobNodeIds), null, conn);

      //create a way with some nodes inside the query bounds and some outside; The way and the
      //out of bounds nodes it owns should be returned by the query since at least one of the added
      //way nodes is within the bounds.
      Set<Long> partiallyOobNodeIds = new LinkedHashSet<Long>();
      partiallyOobNodeIds.add(nodeIdsArr[0]);
      partiallyOobNodeIds.add(oobNodeIdsArr[0]);
      wayIds.add(
        Way.insertNew(changesetId, mapId, new ArrayList<Long>(partiallyOobNodeIds), null, conn));
      final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

      //create a relation where all members are completely outside of the query bounds
      List<RelationMember> members = new ArrayList<RelationMember>();
      //Only oobNodeIdsArr[1] will make this relation out of bounds, b/c oobNodeIdsArr[0] is used
      //by a way which is partially in bounds and will be returned by the query and, thus, any
      //relations which reference that way and/or its nodes will also be returned.
      members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
      members.add(new RelationMember(oobWayId, ElementType.Way, "role1"));
      Relation.insertNew(changesetId, mapId, members, null, conn);

      //create a relation where some members are inside the query bounds and some are not
      members = new ArrayList<RelationMember>();
      members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
      //see note above for why oobNodeIdsArr[1] is used here
      members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
      members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role1"));
      members.add(new RelationMember(oobWayId, ElementType.Way, "role1"));
      relationIds.add(Relation.insertNew(changesetId, mapId, members, null, conn));
      final Long[] relationIdsArr = relationIds.toArray(new Long[]{});

      //Query the elements back out geospatially.  All but one of the nodes, one of the ways, and
      //one of the relations should be returned.
      Document responseData = null;
      try
      {
        if (useMultiLayerUniqueElementIdsParameter)
        {
          responseData =
            resource()
              .path("api/0.6/map")
              .queryParam("mapId", idOrName)
              .queryParam("bbox", queryBounds.toServicesString())
              .queryParam("multiLayerUniqueElementIds", multiLayerUniqueElementIdsStr)
              .accept(MediaType.TEXT_XML)
              .get(Document.class);
        }
        else
        {
          responseData =
            resource()
              .path("api/0.6/map")
              .queryParam("mapId", idOrName)
              .queryParam("bbox", queryBounds.toServicesString())
              .accept(MediaType.TEXT_XML)
              .get(Document.class);
        }
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      OsmTestUtils.verifyOsmHeader(responseData);
      OsmTestUtils.verifyBounds(responseData, queryBounds);

      XPath xpath = XmlUtils.createXPath();

      final boolean multiLayerUniqueElementIds =
        multiLayerUniqueElementIdsStr.toLowerCase().equals("true");
      try
      {
        Assert.assertEquals(15, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyNode(
            responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId, originalBounds.getMinLat(),
            originalBounds.getMinLon(), false);
          OsmTestUtils.verifyNode(
            responseData, 2, String.valueOf(nodeIdsArr[1]), changesetId, originalBounds.getMaxLat(),
            originalBounds.getMaxLon(), false);
          OsmTestUtils.verifyNode(
            responseData, 3, String.valueOf(nodeIdsArr[2]), changesetId, originalBounds.getMinLat(),
            originalBounds.getMinLon(), false);
          OsmTestUtils.verifyNode(
            responseData, 4, String.valueOf(nodeIdsArr[3]), changesetId, originalBounds.getMinLat(),
            originalBounds.getMinLon(), false);
          OsmTestUtils.verifyNode(
            responseData, 5, String.valueOf(nodeIdsArr[4]), changesetId, originalBounds.getMinLat(),
            originalBounds.getMinLon(), false);
          OsmTestUtils.verifyNode(
            responseData, 6, String.valueOf(oobNodeIdsArr[0]), changesetId, queryBounds.getMinLat() - 5,
            queryBounds.getMinLon() - 5, false);
        }
        else
        {
          OsmTestUtils.verifyNode(
            responseData, 1, mapId + "_n_" + String.valueOf(nodeIdsArr[0]), changesetId,
            originalBounds.getMinLat(), originalBounds.getMinLon(), true);
          OsmTestUtils.verifyNode(
            responseData, 2, mapId + "_n_" + String.valueOf(nodeIdsArr[1]), changesetId,
            originalBounds.getMaxLat(), originalBounds.getMaxLon(), true);
          OsmTestUtils.verifyNode(
            responseData, 3, mapId + "_n_" + String.valueOf(nodeIdsArr[2]), changesetId,
            originalBounds.getMinLat(), originalBounds.getMinLon(), true);
          OsmTestUtils.verifyNode(
            responseData, 4, mapId + "_n_" + String.valueOf(nodeIdsArr[3]), changesetId,
            originalBounds.getMinLat(), originalBounds.getMinLon(), true);
          OsmTestUtils.verifyNode(
            responseData, 5, mapId + "_n_" + String.valueOf(nodeIdsArr[4]), changesetId,
            originalBounds.getMinLat(), originalBounds.getMinLon(), true);
          OsmTestUtils.verifyNode(
            responseData, 6, mapId + "_n_" + String.valueOf(oobNodeIdsArr[0]), changesetId,
            queryBounds.getMinLat() - 5, queryBounds.getMinLon() - 5, true);
        }
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing nodes from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(
          11,
          XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());

        Assert.assertEquals(
          2,
          XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        Assert.assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 1", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        Assert.assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        Assert.assertEquals(
        	"val 2", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        Assert.assertEquals("key 3", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 3", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/node[5]/tag").getLength());
        Assert.assertEquals("key 4", xpath.evaluate("//osm/node[5]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 4", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[5]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing node tags from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
        Set<Long> wayNodeIds = new LinkedHashSet<Long>();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayNodeIds.add(nodeIdsArr[4]);
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyWay(
            responseData, 1, String.valueOf(wayIdsArr[0]), changesetId, wayNodeIds,
            false);
        }
        else
        {
          OsmTestUtils.verifyWay(
            responseData, 1, mapId + "_w_" + String.valueOf(wayIdsArr[0]), changesetId, wayNodeIds,
            true);
        }
        wayNodeIds.clear();
        wayNodeIds.add(nodeIdsArr[2]);
        wayNodeIds.add(nodeIdsArr[1]);
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyWay(
            responseData, 2, String.valueOf(wayIdsArr[1]), changesetId, wayNodeIds,
            false);
        }
        else
        {
          OsmTestUtils.verifyWay(
            responseData, 2, mapId + "_w_" + String.valueOf(wayIdsArr[1]), changesetId, wayNodeIds,
            true);
        }
        wayNodeIds.clear();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyWay(
            responseData, 3, String.valueOf(wayIdsArr[2]), changesetId, wayNodeIds,
            false);
        }
        else
        {
          OsmTestUtils.verifyWay(
            responseData, 3, mapId + "_w_" + String.valueOf(wayIdsArr[2]), changesetId, wayNodeIds,
            true);
        }
        wayNodeIds.clear();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(oobNodeIdsArr[0]);
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyWay(
            responseData, 4, String.valueOf(wayIdsArr[3]), changesetId, wayNodeIds,
            false);
        }
        else
        {
          OsmTestUtils.verifyWay(
            responseData, 4, mapId + "_w_" + String.valueOf(wayIdsArr[3]), changesetId, wayNodeIds,
            true);
        }
        wayNodeIds.clear();
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing ways from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(
          3,
          XPathAPI.selectNodeList(responseData, "//osm/way/tag").getLength());

        Assert.assertEquals(
          2,
          XPathAPI.selectNodeList(responseData, "//osm/way[1]/tag").getLength());
        Assert.assertEquals("key 1", xpath.evaluate("//osm/way[1]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 1", 
        	URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[1]/@v", responseData), "UTF-8"));
        Assert.assertEquals("key 2", xpath.evaluate("//osm/way[1]/tag[2]/@k", responseData));
        Assert.assertEquals(
        	"val 2", 
        	URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[2]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/way[2]/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/way[3]/tag").getLength());
        Assert.assertEquals("key 3", xpath.evaluate("//osm/way[3]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 3", 
        	URLDecoder.decode(xpath.evaluate("//osm/way[3]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing way tags from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(5, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());
        members = new ArrayList<RelationMember>();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[1], ElementType.Way, "role3"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role2"));
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node));
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyRelation(
            responseData, 1, String.valueOf(relationIdsArr[0]), changesetId, members,
            false);
        }
        else
        {
          OsmTestUtils.verifyRelation(
            responseData, 1, mapId + "_r_" + String.valueOf(relationIdsArr[0]), changesetId,
            members, true);
        }

        members.clear();

        members.add(new RelationMember(nodeIdsArr[4], ElementType.Node, "role1"));
        members.add(new RelationMember(relationIdsArr[0], ElementType.Relation, "role1"));
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyRelation(
            responseData, 2, String.valueOf(relationIdsArr[1]), changesetId, members,
            false);
        }
        else
        {
          OsmTestUtils.verifyRelation(
            responseData, 2, mapId + "_r_" + String.valueOf(relationIdsArr[1]), changesetId,
            members, true);
        }
        members.clear();

        members.add(new RelationMember(wayIdsArr[1], ElementType.Way));
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyRelation(
            responseData, 3, String.valueOf(relationIdsArr[2]), changesetId, members,
            false);
        }
        else
        {
          OsmTestUtils.verifyRelation(
            responseData, 3, mapId + "_r_" + String.valueOf(relationIdsArr[2]), changesetId,
            members, true);
        }
        members.clear();

        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node, "role1"));
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyRelation(
            responseData, 4, String.valueOf(relationIdsArr[3]), changesetId, members,
            false);
        }
        else
        {
          OsmTestUtils.verifyRelation(
            responseData, 4, mapId + "_r_" + String.valueOf(relationIdsArr[3]), changesetId,
            members, true);
        }
        members.clear();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
        members.add(new RelationMember(wayIdsArr[0], ElementType.Way, "role1"));
        members.add(new RelationMember(oobWayId, ElementType.Way, "role1"));
        if (!multiLayerUniqueElementIds || !useMultiLayerUniqueElementIdsParameter)
        {
          OsmTestUtils.verifyRelation(
            responseData, 5, String.valueOf(relationIdsArr[4]), changesetId, members,
            false);
        }
        else
        {
          OsmTestUtils.verifyRelation(
            responseData, 5, mapId + "_r_" + String.valueOf(relationIdsArr[4]), changesetId,
            members, true);
        }
        members.clear();
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing relations from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(
          4,
          XPathAPI.selectNodeList(responseData, "//osm/relation/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/relation[1]/tag").getLength());
        Assert.assertEquals("key 1", xpath.evaluate("//osm/relation[1]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 1", 
        	URLDecoder.decode(xpath.evaluate("//osm/relation[1]/tag[1]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          2,
          XPathAPI.selectNodeList(responseData, "//osm/relation[2]/tag").getLength());
        Assert.assertEquals("key 2", xpath.evaluate("//osm/relation[2]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 2", 
        	URLDecoder.decode(xpath.evaluate("//osm/relation[2]/tag[1]/@v", responseData), "UTF-8"));
        Assert.assertEquals("key 3", xpath.evaluate("//osm/relation[2]/tag[2]/@k", responseData));
        Assert.assertEquals(
        	"val 3", 
        	URLDecoder.decode(xpath.evaluate("//osm/relation[2]/tag[2]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/relation[3]/tag").getLength());
        Assert.assertEquals("key 4", xpath.evaluate("//osm/relation[3]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 4", 
        	URLDecoder.decode(xpath.evaluate("//osm/relation[3]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing relation tags from response document: " + e.getMessage());
      }
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetMapById() throws Exception
  {
    getMap(String.valueOf(mapId), "false", true);
  }

  @Test
  @Category(UnitTest.class)
  public void testGetMapByName() throws Exception
  {
    getMap("map-with-id-" + String.valueOf(mapId), "false", true);
  }

  @Test
  @Category(UnitTest.class)
  public void testGetMapMultiLayerUniqueElementIds() throws Exception
  {
    getMap(String.valueOf(mapId), "true", true);
  }

  @Test
  @Category(UnitTest.class)
  public void testGetEmptyMap() throws Exception
  {
    try
    {
      final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();

      //Query an empty map.  No elements should be returned.
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/map")
            .queryParam("mapId", String.valueOf(mapId))
            .queryParam("bbox", queryBounds.toServicesString())
            .accept(MediaType.TEXT_XML)
            .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      OsmTestUtils.verifyOsmHeader(responseData);
      OsmTestUtils.verifyBounds(responseData, queryBounds);

      Assert.assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
      Assert.assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
      Assert.assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetMapNoWays() throws Exception
  {
    try
    {
      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
      final Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
      final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

      Set<Long> relationIds =
        OsmTestUtils.createTestRelationsNoWays(changesetId, nodeIds);

      //create some nodes that are completely outside the query bounds
      final Set<Long> oobNodeIds =
        OsmTestUtils.createNodesOutsideOfQueryBounds(changesetId, queryBounds);
      final Long[] oobNodeIdsArr = oobNodeIds.toArray(new Long[]{});

      //create a relation where all members are completely outside of the query bounds
      List<RelationMember> members = new ArrayList<RelationMember>();
      //Only oobNodeIdsArr[1] will make this relation out of bounds, b/c oobNodeIdsArr[0] is used
      //by a way which is partially in bounds and will be returned by the query and, thus, any
      //relations which reference that way and/or its nodes will also be returned.
      members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
      Relation.insertNew(changesetId, mapId, members, null, conn);

      //create a relation where some members are inside the query bounds and some are not
      members = new ArrayList<RelationMember>();
      members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
      //see note above for why oobNodeIdsArr[1] is used here
      members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
      relationIds.add(Relation.insertNew(changesetId, mapId, members, null, conn));
      final Long[] relationIdsArr = relationIds.toArray(new Long[]{});

      //Query the elements back out geospatially.  All but one of the nodes, one of the ways, and
      //one of the relations should be returned.
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/map")
            .queryParam("mapId", String.valueOf(mapId))
            .queryParam("bbox", queryBounds.toServicesString())
            .accept(MediaType.TEXT_XML)
            .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      OsmTestUtils.verifyOsmHeader(responseData);
      OsmTestUtils.verifyBounds(responseData, queryBounds);

      XPath xpath = XmlUtils.createXPath();

      try
      {
        Assert.assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        OsmTestUtils.verifyNode(
          responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 2, String.valueOf(nodeIdsArr[2]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 3, String.valueOf(nodeIdsArr[3]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 4, String.valueOf(nodeIdsArr[4]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing nodes from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(
          4,
          XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());

        Assert.assertEquals(
          2,
          XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        Assert.assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 1", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        Assert.assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        Assert.assertEquals(
        	"val 2", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());
        Assert.assertEquals("key 3", xpath.evaluate("//osm/node[3]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 3", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[3]/tag[1]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        Assert.assertEquals("key 4", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 4", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing node tags from response document: " + e.getMessage());
      }

      Assert.assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());

      try
      {
        Assert.assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());
        members = new ArrayList<RelationMember>();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node));
        OsmTestUtils.verifyRelation(
          responseData, 1, String.valueOf(relationIdsArr[0]), changesetId, members, false);
        members.clear();

        members.add(new RelationMember(nodeIdsArr[4], ElementType.Node, "role1"));
        members.add(new RelationMember(relationIdsArr[0], ElementType.Relation, "role1"));
        OsmTestUtils.verifyRelation(
          responseData, 2, String.valueOf(relationIdsArr[1]), changesetId, members, false);
        members.clear();

        members.add(new RelationMember(nodeIdsArr[2], ElementType.Node, "role1"));
        OsmTestUtils.verifyRelation(
          responseData, 3, String.valueOf(relationIdsArr[2]), changesetId, members, false);
        members.clear();

        members.add(new RelationMember(nodeIdsArr[0], ElementType.Node, "role1"));
        members.add(new RelationMember(oobNodeIdsArr[1], ElementType.Node, "role1"));
        OsmTestUtils.verifyRelation(
          responseData, 4, String.valueOf(relationIdsArr[3]), changesetId, members, false);
        members.clear();
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing relations from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(
          4,
          XPathAPI.selectNodeList(responseData, "//osm/relation/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/relation[1]/tag").getLength());
        Assert.assertEquals("key 1", xpath.evaluate("//osm/relation[1]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 1", 
        	URLDecoder.decode(xpath.evaluate("//osm/relation[1]/tag[1]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          2,
          XPathAPI.selectNodeList(responseData, "//osm/relation[2]/tag").getLength());
        Assert.assertEquals("key 2", xpath.evaluate("//osm/relation[2]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 2", 
        	URLDecoder.decode(xpath.evaluate("//osm/relation[2]/tag[1]/@v", responseData), "UTF-8"));
        Assert.assertEquals("key 3", xpath.evaluate("//osm/relation[2]/tag[2]/@k", responseData));
        Assert.assertEquals(
        	"val 3", 
        	URLDecoder.decode(xpath.evaluate("//osm/relation[2]/tag[2]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/relation[3]/tag").getLength());
        Assert.assertEquals("key 4", xpath.evaluate("//osm/relation[3]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 4", 
        	URLDecoder.decode(xpath.evaluate("//osm/relation[3]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing relation tags from response document: " + e.getMessage());
      }
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetMapNoRelations() throws Exception
  {
    try
    {
      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
      final Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
      final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

      Set<Long> wayIds = OsmTestUtils.createTestWays(changesetId, nodeIds);

      //create some nodes that are completely outside the query bounds
      final Set<Long> oobNodeIds =
        OsmTestUtils.createNodesOutsideOfQueryBounds(changesetId, queryBounds);
      final Long[] oobNodeIdsArr = oobNodeIds.toArray(new Long[]{});

      //create a way completely outside the query bounds
      /*final long oobWayId =*/
      Way.insertNew(changesetId, mapId, new ArrayList<Long>(oobNodeIds), null, conn);

      //create a way with some nodes inside the query bounds and some outside; The way and the
      //out of bounds nodes it owns should be returned by the query since at least one of the added
      //way nodes is within the bounds.
      Set<Long> partiallyOobNodeIds = new LinkedHashSet<Long>();
      partiallyOobNodeIds.add(nodeIdsArr[0]);
      partiallyOobNodeIds.add(oobNodeIdsArr[0]);
      wayIds.add(
        Way.insertNew(changesetId, mapId, new ArrayList<Long>(partiallyOobNodeIds), null, conn));
      final Long[] wayIdsArr = wayIds.toArray(new Long[]{});

      //Query the elements back out geospatially.  All but one of the nodes, one of the ways, and
      //one of the relations should be returned.
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/map")
            .queryParam("mapId", String.valueOf(mapId))
            .queryParam("bbox", queryBounds.toServicesString())
            .accept(MediaType.TEXT_XML)
            .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      OsmTestUtils.verifyOsmHeader(responseData);
      OsmTestUtils.verifyBounds(responseData, queryBounds);

      XPath xpath = XmlUtils.createXPath();

      try
      {
        Assert.assertEquals(15, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        OsmTestUtils.verifyNode(
          responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 2, String.valueOf(nodeIdsArr[1]), changesetId, originalBounds.getMaxLat(),
          originalBounds.getMaxLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 3, String.valueOf(nodeIdsArr[2]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 4, String.valueOf(nodeIdsArr[3]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 5, String.valueOf(nodeIdsArr[4]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 6, String.valueOf(oobNodeIdsArr[0]), changesetId,
          queryBounds.getMinLat() - 5, queryBounds.getMinLon() - 5, false);
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing nodes from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(
          11,
          XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());

        Assert.assertEquals(
          2,
          XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        Assert.assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 1", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        Assert.assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        Assert.assertEquals(
        	"val 2", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        Assert.assertEquals("key 3", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 3", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/node[5]/tag").getLength());
        Assert.assertEquals("key 4", xpath.evaluate("//osm/node[5]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 4", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[5]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing node tags from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
        Set<Long> wayNodeIds = new LinkedHashSet<Long>();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        wayNodeIds.add(nodeIdsArr[4]);
        OsmTestUtils.verifyWay(
          responseData, 1, String.valueOf(wayIdsArr[0]), changesetId, wayNodeIds, false);
        wayNodeIds.clear();
        wayNodeIds.add(nodeIdsArr[2]);
        wayNodeIds.add(nodeIdsArr[1]);
        OsmTestUtils.verifyWay(
          responseData, 2, String.valueOf(wayIdsArr[1]), changesetId, wayNodeIds, false);
        wayNodeIds.clear();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(nodeIdsArr[1]);
        OsmTestUtils.verifyWay(
          responseData, 3, String.valueOf(wayIdsArr[2]), changesetId, wayNodeIds, false);
        wayNodeIds.clear();
        wayNodeIds.add(nodeIdsArr[0]);
        wayNodeIds.add(oobNodeIdsArr[0]);
        OsmTestUtils.verifyWay(
          responseData, 4, String.valueOf(wayIdsArr[3]), changesetId, wayNodeIds, false);
        wayNodeIds.clear();
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing ways from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(
          3,
          XPathAPI.selectNodeList(responseData, "//osm/way/tag").getLength());

        Assert.assertEquals(
          2,
          XPathAPI.selectNodeList(responseData, "//osm/way[1]/tag").getLength());
        Assert.assertEquals("key 1", xpath.evaluate("//osm/way[1]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 1", 
        	URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[1]/@v", responseData), "UTF-8"));
        Assert.assertEquals("key 2", xpath.evaluate("//osm/way[1]/tag[2]/@k", responseData));
        Assert.assertEquals(
        	"val 2", 
        	URLDecoder.decode(xpath.evaluate("//osm/way[1]/tag[2]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/way[2]/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/way[3]/tag").getLength());
        Assert.assertEquals("key 3", xpath.evaluate("//osm/way[3]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 3", 
        	URLDecoder.decode(xpath.evaluate("//osm/way[3]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing way tags from response document: " + e.getMessage());
      }

      Assert.assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetMapNoWaysOrRelations() throws Exception
  {
    try
    {
      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
      final Set<Long> nodeIds = OsmTestUtils.createTestNodes(changesetId, originalBounds);
      final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

      //create some nodes that are completely outside the query bounds
      /*final Set<Long> oobNodeIds =*/
      OsmTestUtils.createNodesOutsideOfQueryBounds(changesetId, queryBounds);

      //Query the elements back out geospatially.  All but one of the nodes, one of the ways, and
      //one of the relations should be returned.
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/map")
            .queryParam("mapId", String.valueOf(mapId))
            .queryParam("bbox", queryBounds.toServicesString())
            .accept(MediaType.TEXT_XML)
            .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      OsmTestUtils.verifyOsmHeader(responseData);
      OsmTestUtils.verifyBounds(responseData, queryBounds);

      XPath xpath = XmlUtils.createXPath();

      try
      {
        Assert.assertEquals(4, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
        OsmTestUtils.verifyNode(
          responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 2, String.valueOf(nodeIdsArr[2]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 3, String.valueOf(nodeIdsArr[3]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
        OsmTestUtils.verifyNode(
          responseData, 4, String.valueOf(nodeIdsArr[4]), changesetId, originalBounds.getMinLat(),
          originalBounds.getMinLon(), false);
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing nodes from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(
          4,
          XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());

        Assert.assertEquals(
          2,
          XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        Assert.assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 1", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        Assert.assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        Assert.assertEquals(
        	"val 2", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());
        Assert.assertEquals("key 3", xpath.evaluate("//osm/node[3]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 3", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[3]/tag[1]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        Assert.assertEquals("key 4", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 4", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing node tags from response document: " + e.getMessage());
      }

      Assert.assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
      Assert.assertEquals(0, XPathAPI.selectNodeList(responseData, "//osm/relation").getLength());
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetMapWithInvisibleElements() throws Exception
  {
  	QCurrentNodes currentNodes  = QCurrentNodes.currentNodes;
  	QCurrentWays currentWays = QCurrentWays.currentWays;
  	QCurrentRelations currentRelations = QCurrentRelations.currentRelations;

    try
    {
      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
      final Set<Long> nodeIds =
        OsmTestUtils.createTestNodes(changesetId, originalBounds);
      final Long[] nodeIdsArr = nodeIds.toArray(new Long[]{});

      final Set<Long> wayIds =
        OsmTestUtils.createTestWays(changesetId, nodeIds);
      final Long[] wayIdsArr = wayIds.toArray(new Long[]{});
        final Set<Long> relationIds =
        OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);
        final Long[] relationIdsArr = relationIds.toArray(new Long[]{});

      //make one of the nodes invisible, so it shouldn't be returned in a map query
      CurrentNodes invisibleNode =

      		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodes)
          .where(currentNodes.id.eq(nodeIdsArr[3]))
          .singleResult(currentNodes);
      invisibleNode.setVisible(false);



      int success = 
      		(int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentNodes)
      .where(currentNodes.id.eq(invisibleNode.getId()))
   .set(currentNodes.visible, false)
   .execute();


      Assert.assertEquals(1, success);
      Assert.assertEquals(
        false,
        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodes)
        .where(currentNodes.id.eq(nodeIdsArr[3]))
        .singleResult(currentNodes.visible)

      		);

      //make one of the ways invisible, so it shouldn't be returned in a map query
      CurrentWays invisibleWay =

      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWays)
          .where(currentWays.id.eq(wayIdsArr[0]))
          .singleResult(currentWays);
      invisibleWay.setVisible(false);
      success = 
      		(int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentWays)
      .where(currentWays.id.eq(invisibleWay.getId()))
      .set(currentWays.visible, false)
      .execute();
      Assert.assertEquals(1, success);
      Assert.assertEquals(
        false,

        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentWays)
        .where(currentWays.id.eq(wayIdsArr[0]))
        .singleResult(currentWays.visible)
      		);

      //make one of the relations invisible, so it shouldn't be returned in a map query
      CurrentRelations invisibleRelation =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelations)
          .where(currentRelations.id.eq(relationIdsArr[0]))
          .singleResult(currentRelations);
      invisibleRelation.setVisible(false);
      success = 
      		(int) new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId), currentRelations)
      .where(currentRelations.id.eq(invisibleRelation.getId()))
      .set(currentRelations.visible, false)
      .execute();
      Assert.assertEquals(1, success);
      Assert.assertEquals(
        false,
        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentRelations)
        .where(currentRelations.id.eq(relationIdsArr[0]))
        .singleResult(currentRelations.visible)

      		);

      //Query the elements back out geospatially and ensure the invisible node and way do not
      //come back in the results.
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/map")
            .queryParam("mapId", String.valueOf(mapId))
            .queryParam("bbox", queryBounds.toServicesString())
            .accept(MediaType.TEXT_XML)
            .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      OsmTestUtils.verifyOsmHeader(responseData);
      OsmTestUtils.verifyBounds(responseData, queryBounds);

      Assert.assertEquals(8, XPathAPI.selectNodeList(responseData, "//osm/node").getLength());
      OsmTestUtils.verifyNode(
        responseData, 1, String.valueOf(nodeIdsArr[0]), changesetId, originalBounds.getMinLat(),
        originalBounds.getMinLon(), false);
      OsmTestUtils.verifyNode(
        responseData, 2, String.valueOf(nodeIdsArr[1]), changesetId, originalBounds.getMaxLat(),
        originalBounds.getMaxLon(), false);
      OsmTestUtils.verifyNode(
        responseData, 3, String.valueOf(nodeIdsArr[2]), changesetId, originalBounds.getMinLat(),
        originalBounds.getMinLon(), false);
      OsmTestUtils.verifyNode(
        responseData, 4, String.valueOf(nodeIdsArr[4]), changesetId, originalBounds.getMinLat(),
        originalBounds.getMinLon(), false);

      XPath xpath = XmlUtils.createXPath();
      try
      {
        Assert.assertEquals(
          5,
          XPathAPI.selectNodeList(responseData, "//osm/node/tag").getLength());

        Assert.assertEquals(
          2,
          XPathAPI.selectNodeList(responseData, "//osm/node[1]/tag").getLength());
        Assert.assertEquals("key 1", xpath.evaluate("//osm/node[1]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 1", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[1]/@v", responseData), "UTF-8"));
        Assert.assertEquals("key 2", xpath.evaluate("//osm/node[1]/tag[2]/@k", responseData));
        Assert.assertEquals(
        	"val 2", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[1]/tag[2]/@v", responseData), "UTF-8"));

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/node[2]/tag").getLength());

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/node[3]/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/node[4]/tag").getLength());
        Assert.assertEquals("key 4", xpath.evaluate("//osm/node[4]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 4", 
        	URLDecoder.decode(xpath.evaluate("//osm/node[4]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing node tags from response document: " + e.getMessage());
      }

      Assert.assertEquals(2, XPathAPI.selectNodeList(responseData, "//osm/way").getLength());
      Set<Long> wayNodeIds = new LinkedHashSet<Long>();
      wayNodeIds.add(nodeIdsArr[2]);
      wayNodeIds.add(nodeIdsArr[1]);
      OsmTestUtils.verifyWay(
        responseData, 1, String.valueOf(wayIdsArr[1]), changesetId, wayNodeIds, false);
      wayNodeIds.clear();
      wayNodeIds.add(nodeIdsArr[0]);
      wayNodeIds.add(nodeIdsArr[1]);
      OsmTestUtils.verifyWay(
        responseData, 2, String.valueOf(wayIdsArr[2]), changesetId, wayNodeIds, false);
      wayNodeIds.clear();

      try
      {
        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/way/tag").getLength());

        Assert.assertEquals(
          0,
          XPathAPI.selectNodeList(responseData, "//osm/way[1]/tag").getLength());

        Assert.assertEquals(
          1,
          XPathAPI.selectNodeList(responseData, "//osm/way[2]/tag").getLength());
        Assert.assertEquals("key 3", xpath.evaluate("//osm/way[2]/tag[1]/@k", responseData));
        Assert.assertEquals(
        	"val 3", 
        	URLDecoder.decode(xpath.evaluate("//osm/way[2]/tag[1]/@v", responseData), "UTF-8"));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing way tags from response document: " + e.getMessage());
      }

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapNodeLimitExceeded() throws Exception
  {
  	QCurrentNodes currentNodes  = QCurrentNodes.currentNodes;
    try
    {
      final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
      final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
      final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
      final Set<Long> nodeIds =
        OsmTestUtils.createTestNodes(changesetId, originalBounds);

      final Set<Long> wayIds =
        OsmTestUtils.createTestWays(changesetId, nodeIds);
        final Set<Long> relationIds =
        OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

      //use a lower number of max query nodes than default for efficiency
      Properties hootProps = HootProperties.getInstance();
      hootProps.setProperty("maxQueryNodes", "3");
      HootProperties.setProperties(hootProps);
      final long maxQueryNumberOfNodes = 3;
      Assert.assertEquals(
        maxQueryNumberOfNodes,
        Long.parseLong(HootProperties.getInstance().getProperty("maxQueryNodes")));
      Assert.assertTrue(
        maxQueryNumberOfNodes <
        new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(currentNodes)
        .count()
      		);

      try
      {
        //try to run a query that returns more than the maximum allowed results size
        resource()
          .path("api/0.6/map")
          .queryParam("mapId", String.valueOf(mapId))
          .queryParam("bbox", queryBounds.toServicesString())
          .accept(MediaType.TEXT_XML)
          .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
        Assert.assertTrue(r.getEntity(String.class).contains(
          "The maximum number of nodes that may be returned in a map query is " +
            maxQueryNumberOfNodes + ".  This query returned " +
            (maxQueryNumberOfNodes + 1) + " nodes.  Please " +
            "execute a query which returns fewer nodes."));

        OsmTestUtils.verifyTestDataUnmodified(
          originalBounds, changesetId, nodeIds, wayIds, relationIds);

        throw e;
      }
      finally
      {
        //set this back to default now that this test is over
        hootProps.setProperty("maxQueryNodes", HootProperties.getDefault("maxQueryNodes"));
        HootProperties.setProperties(hootProps);
        Assert.assertEquals(
          Long.parseLong(HootProperties.getDefault("maxQueryNodes")),
          Long.parseLong(hootProps.getProperty("maxQueryNodes")));
      }
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapInvalidMap() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

    try
    {
      //try to query nodes from a map that doesn't exist
      resource()
        .path("api/0.6/map")
        .queryParam("mapId", "-1")
        .queryParam("bbox", queryBounds.toServicesString())
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(404, r.getStatus());
      Assert.assertTrue(r.getEntity(String.class).contains("No map exists with ID"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapMissingMapParam() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

    try
    {
      //try to query nodes from a map that doesn't exist
      resource()
        .path("api/0.6/map")
        .queryParam("bbox", queryBounds.toServicesString())
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(404, r.getStatus());
      Assert.assertTrue(r.getEntity(String.class).contains("No map exists with ID: null"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapEmptyMapId() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

    try
    {
      //try to query nodes from a map that doesn't exist
      resource()
        .path("api/0.6/map")
        .queryParam("mapId", "")
        .queryParam("bbox", queryBounds.toServicesString())
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(404, r.getStatus());
      Assert.assertTrue(r.getEntity(String.class).contains("No map exists with ID:"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapByNonUniqueName() throws Exception
  {
  	QMaps maps = QMaps.maps;
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final BoundingBox queryBounds = OsmTestUtils.createTestQueryBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

    //insert another map with the same name as the test map
    Maps map = new Maps();
    SQLQuery query = new SQLQuery(conn, DbUtils.getConfiguration(mapId));
    final long nextMapId = query.uniqueResult(SQLExpressions.nextval(Long.class, "maps_id_seq"));

    map.setId(nextMapId);
    final Timestamp now = new Timestamp(Calendar.getInstance().getTimeInMillis());
    map.setCreatedAt(now);
    final String duplicatedMapName = "map-with-id-" + mapId;
    map.setDisplayName(duplicatedMapName);
    map.setUserId(userId);
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), maps)
    .populate(map).execute();

    try
    {
      //try to query nodes from a map name that is linked to multiple map IDs
      resource()
        .path("api/0.6/map")
        .queryParam("mapId", duplicatedMapName)
        .queryParam("bbox", queryBounds.toServicesString())
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(404, r.getStatus());
      Assert.assertTrue(
        r.getEntity(String.class).contains(
          "Multiple maps exist with name: " + duplicatedMapName + ".  Please specify a " +
          "single, valid map."));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapMissingBounds() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

    try
    {
      //try to query nodes without specifying a bounds
      resource()
        .path("api/0.6/map")
        .queryParam("mapId", String.valueOf(mapId))
        .queryParam("bbox", "")
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains(
        "Error parsing bounding box from bbox param"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }
  
  //TODO: why were these two tests disabled?
 
  @Ignore
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapBoundsOutsideWorld() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

    try
    {
      //try to query nodes with an invalid bounds
      resource()
        .path("api/0.6/map")
        .queryParam("mapId", String.valueOf(mapId))
        .queryParam("bbox", "-181,-90,180,90")
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains(
        "Error parsing bounding box from bbox param"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }

  @Ignore
  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMapBoundsTooLarge() throws Exception
  {
    final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
    final BoundingBox queryBounds =
      new BoundingBox(-79.02265434416296, 37.90089748801109, -77.9224564416296, 39.00085678801109);
    Assert.assertTrue(
      queryBounds.getArea() >
      Double.parseDouble(HootProperties.getDefault("maxQueryAreaDegrees")));
    final long changesetId = OsmTestUtils.createTestChangeset(originalBounds);
    final Set<Long> nodeIds =
      OsmTestUtils.createTestNodes(changesetId, originalBounds);

    final Set<Long> wayIds =
      OsmTestUtils.createTestWays(changesetId, nodeIds);
    final Set<Long> relationIds =
      OsmTestUtils.createTestRelations(changesetId, nodeIds, wayIds);

    try
    {
      //try to query nodes with a bounds that's too large
      resource()
        .path("api/0.6/map")
        .queryParam("mapId", String.valueOf(mapId))
        .queryParam("bbox", queryBounds.toServicesString())
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(Status.BAD_REQUEST, Status.fromStatusCode(r.getStatus()));
      Assert.assertTrue(r.getEntity(String.class).contains("The maximum bbox size is"));

      OsmTestUtils.verifyTestDataUnmodified(
        originalBounds, changesetId, nodeIds, wayIds, relationIds);

      throw e;
    }
  }
  
  @Test
  @Category(UnitTest.class)
  public void testGetMapMissingMultiLayerUniqueElementIdsParam() throws Exception
  {
    getMap(String.valueOf(mapId), "true", false);
  }

  //See ReviewResourceGetStatsTest::testGetEmptyReviewScoreMinParam for an explanation of why
  //this test doesn't throw an error.
  @Test
  @Category(UnitTest.class)
  public void testGetMapInvalidMultiLayerUniqueElementIdsParam() throws Exception
  {
    getMap(String.valueOf(mapId), "blah", true);
  }

  //See ReviewResourceGetStatsTest::testGetEmptyReviewScoreMinParam for an explanation of why
  //this test doesn't throw an error.
  @Test
  @Category(UnitTest.class)
  public void testGetMapEmptyMultiLayerUniqueElementIdsParam() throws Exception
  {
    getMap(String.valueOf(mapId), "", true);
  }

  @Test
  @Category(UnitTest.class)
  public void testGetLayers() throws Exception
  {
    List<Long> mapIds = new ArrayList<Long>();
    //existing test layer
    mapIds.add(mapId);
    //create some more maps
    long mapId2 = DbUtils.insertMap(userId, conn);
    mapIds.add(mapId2);
    long mapId3 = DbUtils.insertMap(userId, conn);
    mapIds.add(mapId3);

    //query out the layers
    final MapLayers mapLayers =
      resource()
        .path("api/0.6/map/layers")
        .accept(MediaType.APPLICATION_JSON)
        .get(MapLayers.class);

    Assert.assertNotNull(mapLayers);
    Assert.assertNotNull(mapLayers.getLayers());
    Assert.assertTrue(mapLayers.getLayers().length >= 3);

    boolean foundFirstId = false;
    boolean foundSecondId = false;
    boolean foundThirdId = false;
    for (int i = 0; i < mapLayers.getLayers().length; i++)
    {
      MapLayer layer = mapLayers.getLayers()[i];
      int mapIdsIndex = -1;
      if (layer.getId() == mapIds.get(0))
      {
      	foundFirstId = true;
      	mapIdsIndex = 0;
      }
      else if (layer.getId() == mapIds.get(1))
      {
      	foundSecondId = true;
      	mapIdsIndex = 1;
      }
      else if (layer.getId() == mapIds.get(2))
      {
      	foundThirdId = true;
      	mapIdsIndex = 2;
      }
      if (mapIdsIndex != -1)
      {
      	Assert.assertEquals("map-with-id-" + mapIds.get(mapIdsIndex), layer.getName());
      }
    }
    Assert.assertTrue(foundFirstId && foundSecondId && foundThirdId);
    
    DbUtils.deleteOSMRecord(conn, mapId2);
    DbUtils.deleteOSMRecord(conn, mapId3);
  }

  @Test
  @Category(UnitTest.class)
  public void testGetDeletedLayer() throws Exception
  {
    //delete the only existing map
  	QMaps maps = QMaps.maps ;
  	new SQLDeleteClause(conn, DbUtils.getConfiguration(mapId), maps)
    .where(maps.id.eq(mapId))
    .execute();

    Assert.assertNull(/*mapDao.findById(mapId)*/

    		new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(maps).where(maps.id.eq(mapId)).singleResult(maps)
    		);

    //query out the layers
    /*final MapLayers mapLayers =
      resource()
        .path("api/0.6/map/layers")
        .accept(MediaType.APPLICATION_JSON)
        .get(MapLayers.class);

    Assert.assertNotNull(mapLayers);
    Assert.assertNull(mapLayers.getLayers());*/
  }

}
