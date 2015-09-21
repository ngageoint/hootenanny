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

import javax.ws.rs.core.MediaType;
import javax.xml.transform.TransformerException;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpressionException;

import org.junit.Assert;
import org.apache.xpath.XPathAPI;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;

import hoot.services.HootProperties;
import hoot.services.UnitTest;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.utils.XmlUtils;

import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.UniformInterfaceException;

/*
 * @todo Most of these tests could be converted to integration tests and after a refactoring,
 * could be replace with unit tests that test only the internal classes being used by this
 * Jersey resource.
 */
public class CapabilitiesResourceTest extends OsmResourceTestAbstract
{
  @SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(CapabilitiesResourceTest.class);
  
  public CapabilitiesResourceTest() throws NumberFormatException, IOException
  {
    super("hoot.services.controllers.osm");
  }
  
  @Test
  @Category(UnitTest.class)
  public void testGet() throws IOException, TransformerException
  {
    Document responseData = null;
    try
    {
      responseData = 
        resource()
          .path("api/capabilities")
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
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "osmVersion", HootProperties.getDefault("osmVersion")), 
        xpath.evaluate("//osm/@version", responseData));
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "generator", HootProperties.getDefault("generator")), 
        xpath.evaluate("//osm/@generator", responseData));
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "copyright", HootProperties.getDefault("copyright")), 
        xpath.evaluate("//osm/@copyright", responseData));
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "attribution", HootProperties.getDefault("attribution")), 
        xpath.evaluate("//osm/@attribution", responseData));
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "license", HootProperties.getDefault("license")), 
        xpath.evaluate("//osm/@license", responseData));
      
      Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api").getLength());
      
      Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/version").getLength());
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "osmVersion", HootProperties.getDefault("osmVersion")), 
        xpath.evaluate("//osm/api/version/@minimum", responseData));
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "osmVersion", HootProperties.getDefault("osmVersion")), 
        xpath.evaluate("//osm/api/version/@maximum", responseData));
      
      Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/area").getLength());
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "maxQueryAreaDegrees", HootProperties.getDefault("maxQueryAreaDegrees")), 
        xpath.evaluate("//osm/api/area/@maximum", responseData));
      
      Assert.assertEquals(
        1, XPathAPI.selectNodeList(responseData, "//osm/api/waynodes").getLength());
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "maximumWayNodes", HootProperties.getDefault("maximumWayNodes")), 
        xpath.evaluate("//osm/api/waynodes/@maximum", responseData));
      
      Assert.assertEquals(
        1, XPathAPI.selectNodeList(responseData, "//osm/api/changesets").getLength());
      Assert.assertEquals(
        HootProperties.getInstance().getProperty(
          "maximumChangesetElements", HootProperties.getDefault("maximumChangesetElements")), 
        xpath.evaluate("//osm/api/changesets/@maximum_elements", responseData));
      
      Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/timeout").getLength());
      Assert.assertEquals(
        Integer.parseInt(
          HootProperties.getInstance().getProperty(
            "changesetIdleTimeoutMinutes", 
            HootProperties.getDefault("changesetIdleTimeoutMinutes"))) * 60, 
        Integer.parseInt(xpath.evaluate("//osm/api/timeout/@seconds", responseData)));
      
      Assert.assertEquals(1, XPathAPI.selectNodeList(responseData, "//osm/api/status").getLength());
      Assert.assertEquals("online", xpath.evaluate("//osm/api/status/@database", responseData));
      Assert.assertEquals("online", xpath.evaluate("//osm/api/status/@api", responseData));
      Assert.assertEquals("offline", xpath.evaluate("//osm/api/status/@gpx", responseData));
    }
    catch (XPathExpressionException e)
    {
      Assert.fail("Error parsing response document: " + e.getMessage());
    }
  }
}
