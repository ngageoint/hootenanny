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

import javax.ws.rs.core.Response.Status;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathExpressionException;

import org.apache.xpath.XPathAPI;
import org.codehaus.jackson.map.ObjectMapper;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import hoot.services.IntegrationTest;
import hoot.services.models.review.ReviewableItemsStatistics;
import hoot.services.review.ReviewTestUtils;
import hoot.services.utils.XmlDocumentBuilder;
import hoot.services.utils.XmlUtils;
import hoot.services.wps.WpsTestAbstract;

public class ReviewableItemsStatisticsWpsTest extends WpsTestAbstract
{
  @SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewableItemsStatisticsWpsTest.class);;
  
  public ReviewableItemsStatisticsWpsTest() throws NumberFormatException, IOException
  {
    super("hoot.services.controllers.job");
    
    processId = "ReviewableItemsStatistics";
  }
  
  //WPS specifies that both GET and POST be supported, even when one or the other doesn't make
  //sense in context of the request.  It also supports both key/value pair and XML as inputs for
  //both request types.  These tests don't test every possible input parameter combination like 
  //the processlet level tests do.
  
  @Override
  protected String getWpsRequestXml(final String mapId)
  {
    return 
      "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" +
      "<wps:Execute service=\"WPS\" version=\"1.0.0\" " +
        "xmlns:wps=\"http://www.opengis.net/wps/1.0.0\" " + 
        "xmlns:ows=\"http://www.opengis.net/ows/1.1\" " + 
        "xmlns:xlink=\"http://www.w3.org/1999/xlink\" " + 
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" " + 
        "xsi:schemaLocation=\"http://www.opengis../wpsExecute_request.xsd\">" + 
        "<ows:Identifier>" + processId + "</ows:Identifier>" + 
        "<wps:DataInputs>" + 
          "<wps:Input>" + 
            "<ows:Identifier>mapId</ows:Identifier>" + 
            "<wps:Data>" + 
              "<wps:LiteralData>" + String.valueOf(mapId) + "</wps:LiteralData>" + 
            "</wps:Data>" + 
          "</wps:Input>" +
        "</wps:DataInputs>" + 
        "<wps:ResponseForm>" + 
          "<wps:ResponseDocument storeExecuteResponse=\"false\" status=\"false\">" + 
            "<wps:Output>" + 
              "<ows:Identifier>reviewableItemsStatistics</ows:Identifier>" + 
            "</wps:Output>" + 
          "</wps:ResponseDocument>" + 
        "</wps:ResponseForm>" + 
      "</wps:Execute>";
  }
  
  @Override
  protected String getWpsRequestString(final String mapId)
  {
    return 
      "service=WPS&version=1.0.0&request=Execute&identifier=" + processId + "&" + 
      "DataInputs=mapId=" + String.valueOf(mapId);
  }
  
  @Override
  protected String verifyWpsResponse(final String responseStr) throws Exception
  {    
    final Document responseData = XmlDocumentBuilder.parse(responseStr, false);
    Assert.assertNotNull(responseData);
    //System.out.println(XmlDocumentBuilder.toString(responseData));
    
    XPath xpath = XmlUtils.createXPath();    
    try
    {
      Assert.assertEquals(
        processId, xpath.evaluate(".//Process/Identifier", responseData));
      NodeList returnedNodes = XPathAPI.selectNodeList(responseData, ".//ProcessOutputs/Output");
      Assert.assertEquals(1, returnedNodes.getLength());
      Assert.assertEquals(
        "reviewableItemsStatistics", 
        xpath.evaluate(".//ProcessOutputs/Output/Identifier", responseData));
      Assert.assertEquals(
        "string", 
        xpath.evaluate(".//ProcessOutputs/Output/Data/LiteralData/@dataType", responseData));
      final ReviewableItemsStatistics response = 
        (new ObjectMapper()).readValue(
          xpath.evaluate(".//ProcessOutputs/Output/Data/LiteralData", responseData), 
          ReviewableItemsStatistics.class);
      
      Assert.assertEquals(mapId, response.getMapId());
      Assert.assertEquals(54, response.getNumTotalItems());
      Assert.assertEquals(11, response.getNumReviewableItems());
      Assert.assertEquals(3, response.getNumReviewedItems());
    }
    catch (XPathExpressionException e)
    {
      Assert.fail("Error parsing response document: " + e.getMessage());
    }
    
    return null;
  }
  
  @Test
  @Category(IntegrationTest.class)
  public void testWpsGetKvp() throws Exception
  { 
    ReviewTestUtils.createPreparedData(resource());
    ReviewTestUtils.markSomeItemsReviewed();
    
    final String response = testWps(true, true, String.valueOf(mapId));
    verifyWpsResponse(response);
  }
  
  @Test
  @Category(IntegrationTest.class)
  public void testWpsPostXml() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());
    ReviewTestUtils.markSomeItemsReviewed();
    
    final String response = testWps(false, false, String.valueOf(mapId));
    verifyWpsResponse(response);
  }
  
  @Test
  @Category(IntegrationTest.class)
  public void testWpsError() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());
    ReviewTestUtils.markSomeItemsReviewed();
    
    final String response = testWps(true, true, String.valueOf(mapId + 2));
    verifyWpsErrorResponse(response, Status.NOT_FOUND, "No record exists");
  }
}
