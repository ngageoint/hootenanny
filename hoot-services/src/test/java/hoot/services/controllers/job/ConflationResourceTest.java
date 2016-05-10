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
package hoot.services.controllers.job;

import static org.mockito.Mockito.*;

import java.io.File;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;

import org.apache.commons.io.FileUtils;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ConflationResourceTest 
{
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ConflationResourceTest.class);
	
	@Test
	@Category(UnitTest.class)
	public void testProcess() throws Exception
	{
		String params = "{\"INPUT1_TYPE\":\"DB\",\"INPUT1\":\"DcGisRoads\",\"INPUT2_TYPE\":\"DB\",\"INPUT2\":\"DcTigerRoads\",";
		params += "\"OUTPUT_NAME\":\"Merged_Roads_e0d\",\"CONFLATION_TYPE\":\"Reference\",\"MATCH_THRESHOLD\":\"0.6\",\"MISS_THRESHOLD\":\"0.6\",\"USER_EMAIL\":\"test@test.com\",\"COLLECT_STATS\":\"false\"}";

		String jobArgs = "\"exec\":\"makeconflate\",\"params\":[{\"USER_EMAIL\":\"test@test.com\"},{\"CONFLATION_TYPE\":\"Reference\"},"
				+ "{\"MATCH_THRESHOLD\":\"0.6\"},{\"INPUT1_TYPE\":\"DB\"},{\"MISS_THRESHOLD\":\"0.6\"},{\"INPUT2_TYPE\":\"DB\"},"
				+ "{\"INPUT2\":\"DcTigerRoads\"},{\"INPUT1\":\"DcGisRoads\"},{\"COLLECT_STATS\":\"false\"},{\"OUTPUT_NAME\":\"Merged_Roads_e0d\"},"
				+ "{\"IS_BIG\":\"false\"}],\"exectype\":\"make\"},{\"class\":\"hoot.services.controllers.osm.MapResource\","
				+ "\"method\":\"updateTagsDirect\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":{\"input2\":\"DcTigerRoads\","
				//+ "\"stats\":\"\\/home\\/vagrant\\/hoot\\/data\\/reports\\/Merged_Roads_e0d-stats.csv\","
				+ "\"params\":\"{\\\\\\\"USER_EMAIL\\\\\\\":\\\\\\\"test@test.com\\\\\\\",\\\\\\\"MATCH_THRESHOLD\\\\\\\":\\\\\\\"0.6\\\\\\\",\\\\\\\"CONFLATION_TYPE\\\\\\\":\\\\\\\"Reference\\\\\\\",\\\\\\\"MISS_THRESHOLD\\\\\\\":\\\\\\\"0.6\\\\\\\",\\\\\\\"INPUT1_TYPE\\\\\\\":\\\\\\\"DB\\\\\\\",\\\\\\\"INPUT2\\\\\\\":\\\\\\\"DcTigerRoads\\\\\\\",\\\\\\\"INPUT2_TYPE\\\\\\\":\\\\\\\"DB\\\\\\\",\\\\\\\"COLLECT_STATS\\\\\\\":\\\\\\\"false\\\\\\\",\\\\\\\"INPUT1\\\\\\\":\\\\\\\"DcGisRoads\\\\\\\",\\\\\\\"OUTPUT_NAME\\\\\\\":\\\\\\\"Merged_Roads_e0d\\\\\\\"}\","
				+ "\"input1\":\"DcGisRoads\"},\"paramtype\":\"java.util.Map\"},{\"isprimitivetype\":\"false\",\"value\":\"Merged_Roads_e0d\","
				+ "\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection\"},{\"class\":\"hoot.services.controllers.ingest.RasterToTilesService\","
				+ "\"method\":\"ingestOSMResourceDirect\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"Merged_Roads_e0d\",\"paramtype\":\"java.lang.String\"},"
				+ "{\"isprimitivetype\":\"false\",\"value\":\"test@test.com\",\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection\"}]";
		ConflationResource spy = Mockito.spy(new ConflationResource());
		Mockito.doNothing().when((JobControllerBase)spy).postChainJobRquest(anyString(), anyString());
		Response resp = spy.process(params);
		String result = resp.getEntity().toString();
		JSONParser parser = new JSONParser();
		JSONObject o = (JSONObject)parser.parse(result);
		String jobId = o.get("jobid").toString();
		verify(spy).postChainJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
	}
	
	@Test
	@Category(UnitTest.class)
	public void testProcessOsmApiDbInput() throws Exception
	{
		final String inputParams = 
		  FileUtils.readFileToString(
        new File(
          Thread.currentThread().getContextClassLoader().getResource(
            "hoot/services/controllers/job/ConflationResourceTestProcessOsmApiDbInputInput.json")
          .getPath()));
				
		ConflationResource spy = Mockito.spy(new ConflationResource());
		Mockito.doNothing().when((JobControllerBase)spy).postChainJobRquest(anyString(), anyString());
		final String jobId = 
			((JSONObject)(new JSONParser()).parse(
				spy.process(inputParams).getEntity().toString())).get("jobid").toString();
		
		//just checking that the request made it the command runner w/o error and that the map tag
		//got added; testProcess checks the generated input at a more detailed level
		verify(spy)
      .postChainJobRquest(
  	    Matchers.matches(jobId), 
  	    //had no luck getting the mockito matcher to take the timestamp regex...validated the 
  	    //regex externally, and it looked good
  	    /*Matchers.matches("\"osm_api_db_export_time\":\"" + DbUtils.TIME_STAMP_REGEX + "\"")*/
  	    Matchers.contains("osm_api_db_export_time"));
	}
	
	//An OSM API DB input must always be a reference layer.  Default ref layer = 1.
	
	@Test(expected=WebApplicationException.class)
	@Category(UnitTest.class)
	public void testOsmApiDbInputAsSecondary() throws Exception
	{
		try
		{
			final String inputParams = 
			  FileUtils.readFileToString(
	        new File(
	          Thread.currentThread().getContextClassLoader().getResource(
	            "hoot/services/controllers/job/ConflationResourceTestOsmApiDbInputAsSecondaryInput.json")
	          .getPath()));
			
			ConflationResource spy = Mockito.spy(new ConflationResource());
			Mockito.doNothing().when((JobControllerBase)spy).postChainJobRquest(anyString(), anyString());
			spy.process(inputParams);
		}
		catch (WebApplicationException e)
    {
			Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
			Assert.assertTrue(
				e.getResponse().getEntity().toString().contains(
				  "OSM_API_DB not allowed as secondary input type"));
      throw e;
    }
	}

	@Test(expected=WebApplicationException.class)
	@Category(UnitTest.class)
	public void testOsmApiDbInputAsSecondary2() throws Exception
	{
		try
		{
			final String inputParams = 
			  FileUtils.readFileToString(
	        new File(
	          Thread.currentThread().getContextClassLoader().getResource(
	            "hoot/services/controllers/job/ConflationResourceTestOsmApiDbInputAsSecondary2Input.json")
	          .getPath()));
			
			ConflationResource spy = Mockito.spy(new ConflationResource());
			Mockito.doNothing().when((JobControllerBase)spy).postChainJobRquest(anyString(), anyString());
			spy.process(inputParams);
		}
		catch (WebApplicationException e)
    {
			Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
			Assert.assertTrue(e.getResponse().getEntity().toString().contains(
				"OSM_API_DB not allowed as secondary input type"));
      throw e;
    }
	}
	
	//An OSM API DB input layer must always be accompanied with an aoi.

	@Test(expected=WebApplicationException.class)
	@Category(UnitTest.class)
	public void testOsmApiDbInputMissingAoi() throws Exception
	{
		try
		{
			final String inputParams = 
			  FileUtils.readFileToString(
	        new File(
	          Thread.currentThread().getContextClassLoader().getResource(
	            "hoot/services/controllers/job/ConflationResourceTestOsmApiDbInputMissingAoiInput.json")
	          .getPath()));
			
			ConflationResource spy = Mockito.spy(new ConflationResource());
			Mockito.doNothing().when((JobControllerBase)spy).postChainJobRquest(anyString(), anyString());
			spy.process(inputParams).getEntity().toString();
		}
		catch (WebApplicationException e)
    {
			Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
			Assert.assertTrue(e.getResponse().getEntity().toString().contains(
				"OSM_API_DB input must be used with the convert.bounding.box setting"));
      throw e;
    }
	}
	
	@Test(expected=WebApplicationException.class)
	@Category(UnitTest.class)
	public void testOsmApiDbInputMissingAo2i() throws Exception
	{
		try
		{
			final String inputParams = 
			  FileUtils.readFileToString(
	        new File(
	          Thread.currentThread().getContextClassLoader().getResource(
	            "hoot/services/controllers/job/ConflationResourceTestOsmApiDbInputMissingAoi2Input.json")
	          .getPath()));
			
			ConflationResource spy = Mockito.spy(new ConflationResource());
			Mockito.doNothing().when((JobControllerBase)spy).postChainJobRquest(anyString(), anyString());
			spy.process(inputParams).getEntity().toString();
		}
		catch (WebApplicationException e)
    {
			Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
			Assert.assertTrue(e.getResponse().getEntity().toString().contains(
				"OSM_API_DB input must be used with the convert.bounding.box setting"));
      throw e;
    }
	}
}
