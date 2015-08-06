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

import static org.mockito.Mockito.*;

import javax.ws.rs.core.Response;

import hoot.services.UnitTest;

import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.Matchers;
import org.mockito.Mockito;

public class CookieCutterResourceTest {

	@Test
	@Category(UnitTest.class)
	public void testProcess() throws Exception
	{
		String params = "{\"INPUT1_TYPE\":\"DB\",\"INPUT1\":\"DcGisRoads\",\"INPUT2_TYPE\":\"DB\",\"INPUT2\":\"DcTigerRoads\",";
		params += "\"OUTPUT_NAME\":\"Merged_Roads_3ee\",\"CONFLATION_TYPE\":\"Cookie Cutter\",\"MATCH_THRESHOLD\":\"0.6\",";
		params += "\"MISS_THRESHOLD\":\"0.6\",\"alpha\":\"2500\",\"buffer\":\"0\",\"crop\":\"false\",\"outputbuffer\":\"-1000\",";
		params += "\"cuttershape\":\"DcGisRoads\",\"cuttershapetype\":\"DB\",\"outputname\":\"layer_071\",\"inputtype\":\"DB\",\"input\":\"DcTigerRoads\"}";


		String jobArgs = ",\"exec\":\"makecookiecutter\",\"params\":[{\"CONFLATION_TYPE\":\"Cookie Cutter\"}," +
				"{\"INPUT1_TYPE\":\"DB\"},{\"MISS_THRESHOLD\":\"0.6\"},{\"INPUT2_TYPE\":\"DB\"},{\"cuttershapetype\":\"DB\"}," +
				"{\"OUTPUT_NAME\":\"Merged_Roads_3ee\"},{\"crop\":\"false\"},{\"inputtype\":\"DB\"},{\"buffer\":\"0\"}," +
				"{\"input\":\"DcTigerRoads\"},{\"MATCH_THRESHOLD\":\"0.6\"},{\"outputbuffer\":\"-1000\"},{\"INPUT2\":\"DcTigerRoads\"}," +
				"{\"outputname\":\"layer_071\"},{\"alpha\":\"2500\"},{\"INPUT1\":\"DcGisRoads\"},{\"cuttershape\":\"DcGisRoads\"}]" +
				",\"exectype\":\"make\"},{\"class\":\"hoot.services.controllers.job.ReviewResource\",\"method\":\"prepareItemsForReview\"" +
				",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"Merged_Roads_3ee\",\"paramtype\":\"java.lang.String\"}" +
				",{\"isprimitivetype\":\"true\",\"value\":false,\"paramtype\":\"java.lang.Boolean\"}],\"exectype\":\"reflection\"}" +
				",{\"class\":\"hoot.services.controllers.ingest.RasterToTilesService\",\"method\":\"ingestOSMResourceDirect\"" +
				",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"Merged_Roads_3ee\",\"paramtype\":\"java.lang.String\"}]" +
				",\"exectype\":\"reflection\"}]";


		CookieCutterResource spy = Mockito.spy(new CookieCutterResource());
		Mockito.doNothing().when((JobControllerBase)spy).postChainJobRquest(anyString(), anyString());
		Response resp = spy.process(params);
		String result = resp.getEntity().toString();
		JSONParser parser = new JSONParser();
		JSONObject o = (JSONObject)parser.parse(result);
		String jobId = o.get("jobid").toString();
		verify(spy).postChainJobRquest(Matchers.matches(jobId), Matchers.endsWith(jobArgs));
	}
}
