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
package hoot.services.controllers.job;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import javax.ws.rs.core.Response;

import hoot.services.UnitTest;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;

public class ConflationResourceTest {

	@Test
	@Category(UnitTest.class)
	public void testProcess() throws Exception
	{
		String params = "{\"INPUT1_TYPE\":\"DB\",\"INPUT1\":\"DcGisRoads\",\"INPUT2_TYPE\":\"DB\",\"INPUT2\":\"DcTigerRoads\",";
		params += "\"OUTPUT_NAME\":\"Merged_Roads_e0d\",\"CONFLATION_TYPE\":\"Reference\",\"MATCH_THRESHOLD\":\"0.6\",\"MISS_THRESHOLD\":\"0.6\"}";

		String jobArgs = "\"exec\":\"makeconflate\",\"params\":[{\"CONFLATION_TYPE\":\"Reference\"}," +
				"{\"MATCH_THRESHOLD\":\"0.6\"},{\"INPUT1_TYPE\":\"DB\"},{\"MISS_THRESHOLD\":\"0.6\"}," +
				"{\"INPUT2_TYPE\":\"DB\"},{\"INPUT2\":\"DcTigerRoads\"},{\"INPUT1\":\"DcGisRoads\"}," +
				"{\"OUTPUT_NAME\":\"Merged_Roads_e0d\"},{\"IS_BIG\":\"false\"}],\"exectype\":\"make\"}," +
				"{\"class\":\"hoot.services.controllers.job.ReviewResource\",\"method\":\"prepareItemsForReview\",\"params\":" +
				"[{\"isprimitivetype\":\"false\",\"value\":\"Merged_Roads_e0d\",\"paramtype\":\"java.lang.String\"}," +
				"{\"isprimitivetype\":\"true\",\"value\":false,\"paramtype\":\"java.lang.Boolean\"}],\"exectype\":\"reflection\"}," +
				"{\"class\":\"hoot.services.controllers.ingest.RasterToTilesService\",\"method\":\"ingestOSMResourceDirect\",\"params\":" +
				"[{\"isprimitivetype\":\"false\",\"value\":\"Merged_Roads_e0d\",\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection\"}]";
		ConflationResource spy = Mockito.spy(new ConflationResource());
		Mockito.doNothing().when((JobControllerBase)spy).postChainJobRquest(anyString(), anyString());
		Response resp = spy.process(params);
		String result = resp.getEntity().toString();
		JSONParser parser = new JSONParser();
		JSONObject o = (JSONObject)parser.parse(result);
		String jobId = o.get("jobid").toString();
		verify(spy).postChainJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
	}
	/*
	@Test
	@Category(UnitTest.class)
	public void testgetAdvOpts() throws Exception
	{
		ConflationResource spy = new ConflationResource();
		JSONObject ret = spy.getAdvOpts();
		JSONArray mergers = (JSONArray)ret.get("merger");
		JSONArray matchers = (JSONArray)ret.get("matcher");
		
		assertNotNull(mergers);
		assertNotNull(matchers);
		
		int nMerger = 0;
		for(Object o: mergers)
		{
			JSONObject mrs = (JSONObject)o;
			String cName = mrs.get("className").toString();
			if(cName.contains("MergerCreator"))
			{
				nMerger++;
			}
		}
		assertTrue(nMerger > 0);
		
		int nMatch = 0;
		for(Object o: matchers)
		{
			JSONObject mrs = (JSONObject)o;
			String cName = mrs.get("className").toString();
			if(cName.contains("MatchCreator"))
			{
				nMatch++;
			}
		}
		assertTrue(nMatch > 0);
	}*/
}
