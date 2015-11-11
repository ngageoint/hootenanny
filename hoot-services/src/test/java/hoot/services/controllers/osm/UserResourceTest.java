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
import java.util.LinkedHashSet;
import java.util.Set;

import javax.ws.rs.core.MediaType;
import javax.xml.xpath.XPath;

import org.junit.Assert;
import org.apache.xpath.XPathAPI;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import hoot.services.UnitTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.QChangesets;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Changeset;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.utils.XmlUtils;

import com.mysema.query.sql.SQLQuery;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.UniformInterfaceException;

public class UserResourceTest extends OsmResourceTestAbstract
{
  private static final Logger log = LoggerFactory.getLogger(UserResourceTest.class);

  public UserResourceTest() throws NumberFormatException, IOException
  {
    super("hoot.services.controllers.osm");
  }

  @Test
  @Category(UnitTest.class)
  public void testGetById() throws Exception
  {
    try
    {
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("user/" + userId)
            .accept(MediaType.TEXT_XML)
            .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      XPath xpath = XmlUtils.createXPath();

      try
      {
        Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
        Assert.assertEquals("0.6", xpath.evaluate("//osm[1]/@version", responseData));
        Assert.assertNotNull(xpath.evaluate("//osm[1]/@generator", responseData));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing header from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/user").getLength());
        Assert.assertEquals(userId, Long.parseLong(xpath.evaluate("//osm/user/@id", responseData)));
        Assert.assertEquals(
          "user-with-id-" + userId, xpath.evaluate("//osm/user/@display_name", responseData));
        Assert.assertEquals(
          -1, Long.parseLong(xpath.evaluate("//osm/user/changesets/@count", responseData)));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing user from response document: " + e.getMessage());
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
  public void testGetByName() throws Exception
  {
    try
    {
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("user/" + "user-with-id-" + String.valueOf(userId))
            .accept(MediaType.TEXT_XML)
            .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      XPath xpath = XmlUtils.createXPath();

      try
      {
        Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
        Assert.assertEquals("0.6", xpath.evaluate("//osm[1]/@version", responseData));
        Assert.assertNotNull(xpath.evaluate("//osm[1]/@generator", responseData));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing header from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/user").getLength());
        Assert.assertEquals(userId, Long.parseLong(xpath.evaluate("//osm/user/@id", responseData)));
        Assert.assertEquals(
          "user-with-id-" + userId, xpath.evaluate("//osm/user/@display_name", responseData));
        Assert.assertEquals(
          -1, Long.parseLong(xpath.evaluate("//osm/user/changesets/@count", responseData)));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing user from response document: " + e.getMessage());
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
  public void testGetWithChangesetsModified() throws Exception
  {
    try
    {
      final double originalMinLon = -78.02265434416296;
      final double originalMinLat = 38.90089748801109;
      final double originalMaxLon = -77.9224564416296;
      final double originalMaxLat = 39.00085678801109;
      final BoundingBox originalBounds =
        new BoundingBox(originalMinLon, originalMinLat, originalMaxLon, originalMaxLat);

      //link some changesets to the user
      Set<Long> changesetIds = new LinkedHashSet<Long>();

      long changesetId = Changeset.insertNew(mapId, userId, conn);
      changesetIds.add(changesetId);
      (new Changeset(mapId, changesetId, conn)).setBounds(originalBounds);
      QChangesets changesets = QChangesets.changesets;
      hoot.services.db2.Changesets changeset =
      new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
      .where(changesets.id.eq(changesetId)).singleResult(changesets);


      Assert.assertNotNull(changeset);
      Assert.assertEquals(userId, (long)changeset.getUserId());

      changesetId = Changeset.insertNew(mapId, userId, conn);
      changesetIds.add(changesetId);
      (new Changeset(mapId,changesetId, conn)).setBounds(originalBounds);
      changeset =
          new SQLQuery(conn, DbUtils.getConfiguration(mapId)).from(changesets)
          .where(changesets.id.eq(changesetId)).singleResult(changesets);

      Assert.assertNotNull(changeset);
      Assert.assertEquals(userId, (long)changeset.getUserId());

      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("user/" + userId)
            .accept(MediaType.TEXT_XML)
            .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      XPath xpath = XmlUtils.createXPath();

      try
      {
        Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
        Assert.assertEquals("0.6", xpath.evaluate("//osm[1]/@version", responseData));
        Assert.assertNotNull(xpath.evaluate("//osm[1]/@generator", responseData));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing header from response document: " + e.getMessage());
      }

      try
      {
        Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/user").getLength());
        Assert.assertEquals(userId, Long.parseLong(xpath.evaluate("//osm/user/@id", responseData)));
        Assert.assertEquals(
          "user-with-id-" + userId, xpath.evaluate("//osm/user/@display_name", responseData));
        Assert.assertEquals(
          -1, Long.parseLong(xpath.evaluate("//osm/user/changesets/@count", responseData)));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing user from response document: " + e.getMessage());
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
  public void testGetInvalidUserId() throws Exception
  {
  	//TODO: not the best change in the world, since the ID *could* exist
    final long invalidUserId = /*2*/999999;
    try
    {
      resource()
        .path("user/" + invalidUserId)
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(404, r.getStatus());
      Assert.assertTrue(r.getEntity(String.class).contains("No user exists with ID"));

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetEmptyUserId() throws Exception
  {
    try
    {
      resource()
        .path("user/")
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(404, r.getStatus());

      throw e;
    }
  }

  @Test(expected=UniformInterfaceException.class)
  @Category(UnitTest.class)
  public void testGetMissingUserId() throws Exception
  {
    try
    {
      resource()
        .path("user")
        .accept(MediaType.TEXT_XML)
        .get(Document.class);
    }
    catch (UniformInterfaceException e)
    {
      ClientResponse r = e.getResponse();
      Assert.assertEquals(404, r.getStatus());

      throw e;
    }
  }

  @Test
  @Category(UnitTest.class)
  public void testGetDetails() throws Exception
  {
    //TODO: authentication doesn't exist yet, so this just looks for the first user from a select
    //This test is essentially the same as testGet now but will change after authentication is
    //implemented.
    try
    {
      Document responseData = null;
      try
      {
        responseData =
          resource()
            .path("api/0.6/user/details")
            .accept(MediaType.TEXT_XML)
            .get(Document.class);
      }
      catch (UniformInterfaceException e)
      {
        ClientResponse r = e.getResponse();
        Assert.fail("Unexpected response " + r.getStatus() + " " + r.getEntity(String.class));
      }
      Assert.assertNotNull(responseData);

      XPath xpath = XmlUtils.createXPath();

      try
      {
        Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm").getLength());
        Assert.assertEquals("0.6", xpath.evaluate("//osm[1]/@version", responseData));
        Assert.assertNotNull(xpath.evaluate("//osm[1]/@generator", responseData));
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing header from response document: " + e.getMessage());
      }

      try
      {
      	//probably need a better check than this
        Assert.assertTrue(XPathAPI.selectNodeList(responseData, "//osm/user").getLength() >= 1);
        Assert.assertEquals(
        	DbUtils.getTestUserId(conn), 
        	Long.parseLong(xpath.evaluate("//osm/user/@id", responseData)));
        //TODO: fix
        //Assert.assertEquals(
          //"user-with-id-" + String.valueOf(DbUtils.getTestUserId(conn)), 
          //xpath.evaluate("//osm/user/@display_name", responseData));
        //TODO: can't test this from here if clearing out maps after every test...need a different
        //test for it
        //Assert.assertTrue(
          //Long.parseLong(xpath.evaluate("//osm/user/changesets/@count", responseData)) > 0);
      }
      catch (Exception e)
      {
        Assert.fail("Error parsing user from response document: " + e.getMessage());
      }
    }
    catch (Exception e)
    {
      log.error(e.getMessage());
      throw e;
    }
  }
}
