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
package hoot.services.controllers.wps;

import java.io.File;
import java.io.IOException;
import java.math.BigInteger;
import java.util.LinkedList;
import java.util.UUID;

import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.codehaus.jackson.map.ObjectMapper;
import org.deegree.process.jaxb.java.BoundingBoxInputDefinition;
import org.deegree.process.jaxb.java.LiteralInputDefinition;
import org.deegree.process.jaxb.java.ProcessDefinition;
import org.deegree.process.jaxb.java.ProcessDefinition.OutputParameters;
import org.deegree.services.wps.ProcessExecution;
import org.deegree.services.wps.ProcessletException;
import org.deegree.services.wps.ProcessletInputs;
import org.deegree.services.wps.ProcessletOutputs;
import org.deegree.services.wps.input.ProcessletInput;
import org.deegree.services.wps.output.LiteralOutputImpl;
import org.deegree.services.wps.output.ProcessletOutput;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.dml.SQLInsertClause;
import com.mysema.query.sql.dml.SQLUpdateClause;

import hoot.services.HootProperties;
import hoot.services.IntegrationTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewItems;
import hoot.services.db2.QReviewMap;
import hoot.services.db2.ReviewMap;
import hoot.services.geo.BoundingBox;
import hoot.services.job.JobStatusManager.JOB_STATUS;
import hoot.services.models.osm.Changeset;
import hoot.services.models.osm.Element.ElementType;
import hoot.services.models.review.ReviewAgainstItem;
import hoot.services.models.review.ReviewableItem;
import hoot.services.models.review.ReviewableItemsResponse;
import hoot.services.osm.OsmResourceTestAbstract;
import hoot.services.review.ReviewTestUtils;
import hoot.services.wps.WpsUtils;
import hoot.services.writers.osm.ChangesetDbWriter;

public class GetReviewableItemsProcessletTest extends OsmResourceTestAbstract
{
  @SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(GetReviewableItemsProcessletTest.class);

  public GetReviewableItemsProcessletTest() throws NumberFormatException, IOException
  {
    super("hoot.services.controllers.job");
  }

  /*
   * This tests mimics the behavior defined in
     $HOOT_HOME/hoot-services/src/main/src/main/java/hoot/services/controllers/review/GetReviewableItemsProcesslet.java
     So, any changes to that definition must also be reflected in this method and vice versa.
   */
  private ProcessletOutputs execGet(final String mapId, final String numItems,
    final String highestReviewScoreFirst, final String reviewScoreThresholdMinimum,
    final String geospatialBounds) throws Exception
  {
    GetReviewableItemsProcesslet processlet = new GetReviewableItemsProcesslet();

    LinkedList<ProcessletInput> allInputs = new LinkedList<ProcessletInput>();

    if (mapId != null)
    {
      //default data type for a wps input is string, so no extra params needed here for mapId
      allInputs.add(WpsUtils.createLiteralInput("mapId", String.valueOf(mapId)));
    }
    if (numItems != null)
    {
      LiteralInputDefinition inputParamDef = new LiteralInputDefinition();
      LiteralInputDefinition.DataType dataType = new LiteralInputDefinition.DataType();
      inputParamDef.setMinOccurs(new BigInteger("0"));
      inputParamDef.setMaxOccurs(new BigInteger("1"));
      dataType.setReference("http://www.w3.org/TR/xmlschema-2/#integer");
      inputParamDef.setDataType(dataType);
      //deegree doesn't have this implemented yet
      /*AllowedValues allowedValues = new AllowedValues();
      allowedValues.getValueOrRange().add(1);
      allowedValues.getValueOrRange().add(50000);
      inputParamDef.setAllowedValues(allowedValues);*/
      inputParamDef.setDefaultValue("1");
      allInputs.add(WpsUtils.createLiteralInput("numItems", numItems, inputParamDef));
    }
    if (highestReviewScoreFirst != null)
    {
      LiteralInputDefinition inputParamDef = new LiteralInputDefinition();
      LiteralInputDefinition.DataType dataType = new LiteralInputDefinition.DataType();
      inputParamDef.setMinOccurs(new BigInteger("0"));
      inputParamDef.setMaxOccurs(new BigInteger("1"));
      dataType.setReference("http://www.w3.org/TR/xmlschema-2/#boolean");
      inputParamDef.setDataType(dataType);
      inputParamDef.setDefaultValue("true");
      allInputs.add(
        WpsUtils.createLiteralInput(
          "highestReviewScoreFirst", highestReviewScoreFirst, inputParamDef));
    }
    if (reviewScoreThresholdMinimum != null)
    {
      LiteralInputDefinition inputParamDef = new LiteralInputDefinition();
      LiteralInputDefinition.DataType dataType = new LiteralInputDefinition.DataType();
      inputParamDef.setMinOccurs(new BigInteger("0"));
      inputParamDef.setMaxOccurs(new BigInteger("1"));
      dataType.setReference("http://www.w3.org/TR/xmlschema-2/#double");
      inputParamDef.setDataType(dataType);
      //deegree doesn't have this implemented yet
      /*AllowedValues allowedValues = new AllowedValues();
      allowedValues.getValueOrRange().add(0.01);
      allowedValues.getValueOrRange().add(1.00);
      inputParamDef.setAllowedValues(allowedValues);*/
      inputParamDef.setDefaultValue("0.50");
      allInputs.add(
        WpsUtils.createLiteralInput(
          "reviewScoreThresholdMinimum", reviewScoreThresholdMinimum, inputParamDef));
    }
    if (geospatialBounds != null)
    {
      BoundingBoxInputDefinition inputParamDef = new BoundingBoxInputDefinition();
      inputParamDef.setMinOccurs(new BigInteger("0"));
      inputParamDef.setMaxOccurs(new BigInteger("1"));
      allInputs.add(
        WpsUtils.createBoundingBoxInput(
          "geospatialBounds", new BoundingBox(geospatialBounds), inputParamDef));
    }

    ProcessletInputs in = new ProcessletInputs(allInputs);

    ProcessDefinition def = new ProcessDefinition();
    def.setOutputParameters(new OutputParameters());
    LinkedList<ProcessletOutput> allOutputs = new LinkedList<ProcessletOutput>();

    allOutputs.add(WpsUtils.createLiteralOutput("reviewableItems"));

    final ProcessletOutputs out = new ProcessletOutputs(def, allOutputs);

    processlet.process(in, out, new ProcessExecution(null, null, null, null, out));

    return out;
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGet() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final ProcessletOutputs out = execGet(String.valueOf(mapId), null, null, null, null);

    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItems")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsResponse response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsResponse.class);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(1, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    final ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    final ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetWithNumItems() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final ProcessletOutputs out = execGet(String.valueOf(mapId), "2", null, null, null);

    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItems")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsResponse response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsResponse.class);
    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(2, response.getNumItemsRequested());
    Assert.assertEquals(2, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(2, itemsToReview.length);

    ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[1];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-36), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{81d2aff0-88e5-480e-95cb-c502d120f51f}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.054806,38.9010842,-77.0515,38.904085", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-36), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{81d2aff0-88e5-480e-95cb-c502d120f51f}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetDontSortByScore() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final ProcessletOutputs out = execGet(String.valueOf(mapId), null, "false", null, null);

    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItems")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsResponse response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsResponse.class);
    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(false, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(1, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    final ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.750459, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    final ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-65), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{5c131c00-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetWithScoreMin() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    final ProcessletOutputs out = execGet(String.valueOf(mapId), "10", null, "1.0", null);

    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItems")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsResponse response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsResponse.class);
    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(1.0, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(10, response.getNumItemsRequested());
    Assert.assertEquals(2, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(2, itemsToReview.length);

    ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[1];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-36), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{81d2aff0-88e5-480e-95cb-c502d120f51f}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.054806,38.9010842,-77.0515,38.904085", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-36), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{81d2aff0-88e5-480e-95cb-c502d120f51f}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetByBounds() throws Exception
  {
    final BoundingBox queryBounds = ReviewTestUtils.createTestQueryBounds();
    ReviewTestUtils.createPreparedData(resource());

    final ProcessletOutputs out =
      execGet(String.valueOf(mapId), "20", null, null, queryBounds.toServicesString());

    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItems")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsResponse response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsResponse.class);

    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(20, response.getNumItemsRequested());
    //discrepancy here between this and ReviewableItemsStatisticsProcesslet::testGetByBounds;
    //this method is correct and ReviewableItemsStatisticsProcesslet::testGetByBounds is wrong
    Assert.assertEquals(7, response.getNumItemsReturned());
    Assert.assertEquals(queryBounds, new BoundingBox(response.getGeospatialBounds()));
    Assert.assertEquals("ESPG:4326", response.getCoordSys());

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(7, itemsToReview.length);

    ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.relationIds.get((long)-3), reviewableItem.getId());
    Assert.assertEquals(ElementType.Relation.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.939793, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{0c9f69f0-dad5-11e3-9c1a-0800200c9a66}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    // Disable due to precision issue
    /*Assert.assertEquals(
      "-77.05624759999999,38.900034,-77.0515,38.9040842", itemsToReview[0].getDisplayBounds());*/
    ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-67), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{de285618-a4df-4536-b218-5009c25f7107}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[1];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-41), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{dfcd4a0e-04d5-46ee-b7a0-1a2c591ddf54}",
      reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05625099999999,38.9009821,-77.0515,38.9045", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-20), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[2];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-41), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{dfcd4a0e-04d5-46ee-b7a0-1a2c591ddf54}",
      reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05625099999999,38.9009821,-77.0515,38.9045", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-18), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[3];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-40), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{81d2aff0-88e5-480e-95cb-c502d120f51f}",
      reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.056242,38.9009821,-77.0515,38.904085", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-20), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{f5639540-aaf4-47ac-8c5e-82ad05abc29f}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[4];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-40), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals(
      "{46d98769-dee2-489e-8fd6-ff265842ed55};{81d2aff0-88e5-480e-95cb-c502d120f51f}",
      reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.056242,38.9009821,-77.0515,38.904085", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-18), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Way.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{2258bbcc-b474-41fe-aa81-76ffc6e2be42}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[5];
    Assert.assertEquals(
      (long)ReviewTestUtils.wayIds.get((long)-44), reviewableItem.getId());
    Assert.assertEquals(ElementType.Way.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.832799, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{51cd8890-a312-11e3-a5e2-0800200c9a66}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    // Disabling due to precision issue
    /*Assert.assertEquals(
      "-77.05625069999999,38.900034,-77.0515,38.9045", reviewableItem.getDisplayBounds());*/
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.relationIds.get((long)-1), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Relation.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{7ea9ac60-a310-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());

    reviewableItem = itemsToReview[6];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(0.750459, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-65), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{5c131c00-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetDataNotPrepared() throws Exception
  {
    ReviewTestUtils.createDataToPrepare();

    try
    {
      execGet(String.valueOf(mapId), null, null, null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains(
        "The map with ID: " + mapId + " has not had review data prepared for it."));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.PRECONDITION_FAILED.getStatusCode() + ": " +
          Status.PRECONDITION_FAILED.getReasonPhrase()));

      throw e;
    }
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetNoReviewableData() throws Exception
  {
  	QReviewMap reviewMap = QReviewMap.reviewMap;
    //make a review map and job entry but don't write the reviewed data
    final String jobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(jobId, JOB_STATUS.COMPLETE.toInt(), conn);
    ReviewMap mapReviewInfo = new ReviewMap();
    mapReviewInfo.setMapId(mapId);
    mapReviewInfo.setReviewPrepareJobId(jobId);
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
    .populate(mapReviewInfo).execute();

    final ProcessletOutputs out = execGet(String.valueOf(mapId), null, null, null, null);

    //no error should be thrown and there should be an empty set of review items returned
    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItems")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsResponse response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsResponse.class);
    Assert.assertEquals(mapId, response.getMapId());
    Assert.assertEquals(true, response.getHighestReviewScoreFirst());
    Assert.assertEquals(0.5, response.getReviewScoreThresholdMinimum(), 0);
    Assert.assertEquals(1, response.getNumItemsRequested());
    Assert.assertEquals(0, response.getNumItemsReturned());
    Assert.assertEquals(BoundingBox.worldBounds(), new BoundingBox(response.getGeospatialBounds()));

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    //this behavior is different than the Jersey resource
    Assert.assertNotNull(itemsToReview);
    Assert.assertEquals(0, itemsToReview.length);
  }

  private void testGetAgainstInvalidJobStatus(final JOB_STATUS jobStatus) throws Exception
  {
  	QReviewMap reviewMap = QReviewMap.reviewMap;
    final String jobId = UUID.randomUUID().toString();
    DbUtils.insertJobStatus(jobId, jobStatus.toInt(), conn);

    ReviewMap mapReviewInfo = new ReviewMap();
    mapReviewInfo.setMapId(mapId);
    mapReviewInfo.setReviewPrepareJobId(jobId);
    new SQLInsertClause(conn, DbUtils.getConfiguration(mapId), reviewMap)
    .populate(mapReviewInfo).execute();

    try
    {
      execGet(String.valueOf(mapId), null, null, null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains(
        "The prepare job with ID: " + jobId + " for the map with ID: " + mapId +
        " has a status of " + jobStatus.toString()));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.PRECONDITION_FAILED.getStatusCode() + ": " +
          Status.PRECONDITION_FAILED.getReasonPhrase()));

      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetPrepareJobStillRunning() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.RUNNING);
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetPrepareJobFailed() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.FAILED);
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetPrepareJobUnknown() throws Exception
  {
    testGetAgainstInvalidJobStatus(JOB_STATUS.UNKNOWN);
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetMapDoesntExist() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId + 1), null, null, null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("No record exists"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.NOT_FOUND.getStatusCode() + ": " + Status.NOT_FOUND.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetMissingMapIdParam() throws Exception
  {
    try
    {
      execGet(null, null, null, null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetEmptyMapIdParam() throws Exception
  {
    try
    {
      execGet("", null, null, null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetEmptyNumItemsParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), "", null, null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetInvalidNumItemsParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), "abc", null, null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetNumItemsParamOutOfRange() throws Exception
  {
    final int maxReviewGetSize =
      Integer.parseInt(
        HootProperties.getInstance().getProperty(
          "reviewGetMaxReviewSize", HootProperties.getDefault("reviewGetMaxReviewSize")));
    try
    {
      execGet(String.valueOf(mapId), String.valueOf(maxReviewGetSize + 1), null, null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("out of the allowable range"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetEmptyReviewScoreSortingParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, "", null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetInvalidReviewScoreSortingParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, "abc", null, null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetEmptyReviewScoreMinParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, null, "", null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetInvalidReviewScoreMinParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, null, "abc", null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test(expected=ProcessletException.class)
  @Category(IntegrationTest.class)
  public void testGetReviewScoreMinParamOutOfRange() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, null, "1.01", null);
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("out of the allowable range"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  /*
   * This is a little strange b/c we're catching this from the method that prepares the exec call.
     This should reflect reality, though, b/c the exec call simulates how the processlet is actually
     called in real operation.
   *
   * @todo probably need a better way to test this
   */
  @Test(expected=NumberFormatException.class)
  @Category(IntegrationTest.class)
  public void testGetEmptyBoundsParam() throws Exception
  {
    try
    {
      execGet(String.valueOf(mapId), null, null, null, "");
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid input parameter"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  /*
   * @todo need a better way to test this; see testGetEmptyBounds
   */
  @Test(expected=Exception.class)
  @Category(IntegrationTest.class)
  public void testGetInvalidBoundsParam() throws Exception
  {
    try
    {
      execGet(
        String.valueOf(mapId), null, null, null, "-181,-90,180,90");
    }
    catch (ProcessletException e)
    {
      Assert.assertTrue(e.getOWSException().getMessage().contains("Invalid minimum longitude value"));
      Assert.assertTrue(
        e.getOWSException().getExceptionCode().contains(
          Status.BAD_REQUEST.getStatusCode() + ": " + Status.BAD_REQUEST.getReasonPhrase()));
      throw e;
    }
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetWithReviewedItemsPresent() throws Exception
  {
  	QReviewItems reviewItems = QReviewItems.reviewItems;
    ReviewTestUtils.createPreparedData(resource());

    //now mark one of the items as reviewed
    if (
    		new SQLUpdateClause(conn, DbUtils.getConfiguration(mapId),reviewItems)
    		.where(reviewItems.reviewableItemId.eq("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}"))
    		.set(reviewItems.reviewStatus, DbUtils.review_status_enum.reviewed)
    		.execute() <= 0


    		)
    {
      throw new Exception("Error updating item.");
    }

    final ProcessletOutputs out = execGet(String.valueOf(mapId), null, null, null, null);

    //the reviewed item should not be returned in the results
    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItems")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsResponse response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsResponse.class);

    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(1, itemsToReview.length);

    final ReviewableItem reviewableItem = itemsToReview[0];
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-71), reviewableItem.getId());
    Assert.assertEquals(ElementType.Node.toString().toLowerCase(), reviewableItem.getType());
    Assert.assertEquals(1.0, reviewableItem.getReviewScore(), 0);
    Assert.assertEquals("{99b917d0-8c23-4469-b028-b749a38a8841}", reviewableItem.getUuid());
    //System.out.println(reviewableItem.getDisplayBounds());
    Assert.assertEquals(
      "-77.05624759999999,38.9010842,-77.0515,38.9040842", reviewableItem.getDisplayBounds());
    final ReviewAgainstItem itemToReviewAgainst = reviewableItem.getItemToReviewAgainst();
    Assert.assertEquals(
      (long)ReviewTestUtils.nodeIds.get((long)-64), itemToReviewAgainst.getId());
    Assert.assertEquals(
      ElementType.Node.toString().toLowerCase(), itemToReviewAgainst.getType());
    Assert.assertEquals("{25e1aa70-a30f-11e3-a5e2-0800200c9a66}", itemToReviewAgainst.getUuid());
  }

  @Test
  @Category(IntegrationTest.class)
  public void testGetReviewItemAssociatedWithElementThatNoLongerExists() throws Exception
  {
    ReviewTestUtils.createPreparedData(resource());

    //Currently, we simply don't return a reviewable item if its OSM element is deleted before it
    //has been marked as reviewed.  It *might* make more sense to throw an error in this situation.
    //Or, eventually, we could set FKC between the review and OSM tables.
    //long temp = ReviewTestUtils.wayIds.get((long)-40);
    //now delete one of the element records associated with a reviewable item
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
        "way id=\"" + ReviewTestUtils.wayIds.get((long)-41) + "\""));

    final ProcessletOutputs out = execGet(String.valueOf(mapId), "20", null, null, null);
    final String responseStr =
      ((LiteralOutputImpl)out.getParameter("reviewableItems")).getValue();
    Assert.assertNotNull(responseStr);
    final ReviewableItemsResponse response =
      (new ObjectMapper()).readValue(responseStr, ReviewableItemsResponse.class);

    //we should get back two fewer items from this query
    final ReviewableItem[] itemsToReview = response.getReviewableItems();
    Assert.assertEquals(14, itemsToReview.length);
  }
}
