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

import static org.mockito.Mockito.verify;

import java.util.List;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;

import hoot.services.UnitTest;
import hoot.services.job.JobStatusManager;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.ArgumentCaptor;
import org.mockito.ArgumentMatcher;
import org.mockito.Mockito;
import org.mockito.Matchers;

@Ignore
public class JobResourceTest {
	@Test
	@Category(UnitTest.class)
	public void testTerminate() throws Exception
	{
		JSONObject mockRet = new JSONObject();

		mockRet.put("jobId", "dae4be8a-4964-4a9a-8d7d-4e8e738a5b58");
		mockRet.put("status", "running");

		JSONObject statDetailObj = new JSONObject();
		statDetailObj.put("chainjobstatus", "dae4be8a-4964-4a9a-8d7d-4e8e738a5b58");

		JSONArray childrenArr = new JSONArray();
		JSONObject childObj = new JSONObject();
		childObj.put("id", "e16282c6-2432-4a45-a582-b0dd3f2f0d9f");
		childObj.put("detail", "success");
		childObj.put("status", "complete");
		childrenArr.add(childObj);

		childObj = new JSONObject();
		childObj.put("id", "66d53cfc-29b6-49eb-9459-9a74794d34b2");
		childObj.put("detail", "running");
		childObj.put("status", "running");
		childrenArr.add(childObj);

		statDetailObj.put("children", childrenArr);

		mockRet.put("statusDetail", statDetailObj.toJSONString());


		JobResource real = new JobResource();
		JobResource spy = Mockito.spy(real);
		org.mockito.Mockito.when(spy.getJobStatusObj(org.mockito.Matchers.anyString())).thenReturn(mockRet);

		Mockito.doNothing().when(spy)._terminateJob(org.mockito.Matchers.anyString());

		spy.terminateJob("1234", null);

		verify(spy)._terminateJob(Matchers.matches("66d53cfc-29b6-49eb-9459-9a74794d34b2"));
	}

	/**
	 * Tests processChainJob in happy route
	 *
	 * @throws Exception
	 */
	@Test
	@Category(UnitTest.class)
	public void testprocessChainJob() throws Exception
	{

		String jobStr = "[{\"caller\":\"FileUploadResource\",\"exec\":\"makeetl\"," +
				"\"params\":[{\"INPUT\":\"upload\\/81898818-2ca3-4a15-9421-50eb91952586\\/GroundPhotos.shp\"}," +
				"{\"INPUT_TYPE\":\"OGR\"},{\"TRANSLATION\":\"translations\\/UTP.js\"},{\"INPUT_NAME\":\"GroundPhotos\"}]," +
				"\"exectype\":\"make\"},{\"class\":\"hoot.services.controllers.ingest.RasterToTilesService\"," +
				"\"method\":\"ingestOSMResource\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"GroundPhotos\"," +
				"\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection\"}]";

		// Create Mock JobStatusManager Class
		JobStatusManager mockJobStatusManager = Mockito.mock(JobStatusManager.class);
		Mockito.doNothing().when(mockJobStatusManager).addJob(org.mockito.Matchers.anyString());
		Mockito.doNothing().when(mockJobStatusManager).updateJob(org.mockito.Matchers.anyString(), org.mockito.Matchers.anyString());
		Mockito.doNothing().when(mockJobStatusManager).setComplete(org.mockito.Matchers.anyString(), org.mockito.Matchers.anyString());
		Mockito.doNothing().when(mockJobStatusManager).setFailed(org.mockito.Matchers.anyString(), org.mockito.Matchers.anyString());

		// Mock child info
		JSONObject mockChild = new JSONObject();
		mockChild.put("id", "test_child_123");
		mockChild.put("detail", "processing");
		mockChild.put("status", "running");

		JobResource real = new JobResource();
		JobResource spy = Mockito.spy(real);


		org.mockito.Mockito.when(spy._createDbConnection()).thenReturn(null);
		Mockito.doNothing().when(spy)._closeDbConnection(org.mockito.Matchers.any(java.sql.Connection.class));

		// so I use this to avoid actual call
		org.mockito.Mockito.doReturn(new JSONObject()).when(spy)._processJob(org.mockito.Matchers.anyString(),org.mockito.Matchers.any(JSONObject.class));

		org.mockito.Mockito.when(spy._createJobStatusMananger(org.mockito.Matchers.any(java.sql.Connection.class))).thenReturn(mockJobStatusManager);

		org.mockito.Mockito.doReturn(mockChild).when(spy)._execReflection(org.mockito.Matchers.anyString(),
				org.mockito.Matchers.any(JSONObject.class), org.mockito.Matchers.any(JobStatusManager.class));

		spy.processChainJob("test_job_id_1234", jobStr);


		class validParam2Matcher extends ArgumentMatcher<JSONObject>
		{
			public boolean matches(Object oParam)
			{
				JSONObject param = (JSONObject)oParam;
				boolean val1 = param.get("class").toString().equals("hoot.services.controllers.ingest.RasterToTilesService");
				boolean val2 = param.get("method").toString().equals("ingestOSMResource");
				boolean val3 = param.get("exectype").toString().equals("reflection");

				JSONArray params = (JSONArray)param.get("params");
				JSONObject innerParam = (JSONObject)params.get(0);
				boolean val4 = innerParam.get("isprimitivetype").toString().equals("false");
				boolean val5 = innerParam.get("value").toString().equals("GroundPhotos");
				boolean val6 = innerParam.get("paramtype").toString().equals("java.lang.String");

				return val1 && val2 && val3 && val4 && val5 && val6;
			}
		}
		org.mockito.Mockito.verify(spy)._execReflection( Matchers.matches("test_job_id_1234"), org.mockito.Matchers.argThat(new validParam2Matcher()),
				org.mockito.Matchers.any(JobStatusManager.class) );


		ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

		org.mockito.Mockito.verify(mockJobStatusManager, org.mockito.Mockito.times(3)).updateJob(org.mockito.Matchers.anyString(), argCaptor.capture());

		JSONParser parser = new JSONParser();

		List<String> args = argCaptor.getAllValues();
		JSONObject status = (JSONObject) parser.parse(args.get(0));
		JSONArray children = (JSONArray) status.get("children");
		JSONObject child = (JSONObject) children.get(0);

		Assert.assertEquals(child.get("detail").toString(), "processing");
		Assert.assertEquals(child.get("status").toString(), "running");


		status = (JSONObject) parser.parse(args.get(1));
		children = (JSONArray) status.get("children");
		Assert.assertEquals(1, children.size());

		child = (JSONObject) children.get(0);
		Assert.assertEquals(child.get("detail").toString(), "success");
		Assert.assertEquals(child.get("status").toString(), "complete");


		status = (JSONObject) parser.parse(args.get(2));
		children = (JSONArray) status.get("children");
		Assert.assertEquals(2, children.size());

		child = (JSONObject) children.get(0);
		Assert.assertEquals(child.get("detail").toString(), "success");
		Assert.assertEquals(child.get("status").toString(), "complete");

		child = (JSONObject) children.get(1);
		Assert.assertEquals(child.get("detail").toString(), "success");
		Assert.assertEquals(child.get("status").toString(), "complete");


		ArgumentCaptor<String> setCompleteArgCaptor = ArgumentCaptor.forClass(String.class);

		org.mockito.Mockito.verify(mockJobStatusManager, org.mockito.Mockito.times(1)).setComplete(org.mockito.Matchers.anyString(), setCompleteArgCaptor.capture());

		args = setCompleteArgCaptor.getAllValues();
		status = (JSONObject) parser.parse(args.get(0));
		children = (JSONArray) status.get("children");

		Assert.assertEquals(2, children.size());

		child = (JSONObject) children.get(0);
		Assert.assertEquals(child.get("detail").toString(), "success");
		Assert.assertEquals(child.get("status").toString(), "complete");

		child = (JSONObject) children.get(1);
		Assert.assertEquals(child.get("detail").toString(), "success");
		Assert.assertEquals(child.get("status").toString(), "complete");


	}



	/**
	 * Tests the processChainJob failure by _execReflection
	 *
	 * @throws Exception
	 */
	@Test
	@Category(UnitTest.class)
	public void testprocessChainJobFailure() throws Exception
	{

		String jobStr = "[{\"caller\":\"FileUploadResource\",\"exec\":\"makeetl\"," +
				"\"params\":[{\"INPUT\":\"upload\\/81898818-2ca3-4a15-9421-50eb91952586\\/GroundPhotos.shp\"}," +
				"{\"INPUT_TYPE\":\"OGR\"},{\"TRANSLATION\":\"translations\\/UTP.js\"},{\"INPUT_NAME\":\"GroundPhotos\"}]," +
				"\"exectype\":\"make\"},{\"class\":\"hoot.services.controllers.ingest.RasterToTilesService\"," +
				"\"method\":\"ingestOSMResource\",\"params\":[{\"isprimitivetype\":\"false\",\"value\":\"GroundPhotos\"," +
				"\"paramtype\":\"java.lang.String\"}],\"exectype\":\"reflection\"}]";

		// Create Mock JobStatusManager Class
		JobStatusManager mockJobStatusManager = Mockito.mock(JobStatusManager.class);
		Mockito.doNothing().when(mockJobStatusManager).addJob(org.mockito.Matchers.anyString());
		Mockito.doNothing().when(mockJobStatusManager).updateJob(org.mockito.Matchers.anyString(), org.mockito.Matchers.anyString());
		Mockito.doNothing().when(mockJobStatusManager).setComplete(org.mockito.Matchers.anyString(), org.mockito.Matchers.anyString());
		Mockito.doNothing().when(mockJobStatusManager).setFailed(org.mockito.Matchers.anyString(), org.mockito.Matchers.anyString());

		// Mock child info
		JSONObject mockChild = new JSONObject();
		mockChild.put("id", "test_child_123_FAIL");
		mockChild.put("detail", "processing");
		mockChild.put("status", "running");

		JobResource real = new JobResource();
		JobResource spy = Mockito.spy(real);


		org.mockito.Mockito.when(spy._createDbConnection()).thenReturn(null);
		Mockito.doNothing().when(spy)._closeDbConnection(org.mockito.Matchers.any(java.sql.Connection.class));

		// so I use this to avoid actual call
		org.mockito.Mockito.doReturn(new JSONObject()).when(spy)._processJob(org.mockito.Matchers.anyString(),org.mockito.Matchers.any(JSONObject.class));

		org.mockito.Mockito.when(spy._createJobStatusMananger(org.mockito.Matchers.any(java.sql.Connection.class))).thenReturn(mockJobStatusManager);

		// failure point
		org.mockito.Mockito.doThrow(new Exception("Mock failure for testing Process Chain Job failure. (Not real failure!!!)")).when(spy)
		._execReflection(org.mockito.Matchers.anyString(),
				org.mockito.Matchers.any(JSONObject.class), org.mockito.Matchers.any(JobStatusManager.class));

		try
		{
			spy.processChainJob("test_job_id_1234_FAIL", jobStr);
		}
		catch(WebApplicationException wex)
		{
			Response res = wex.getResponse();
			Assert.assertEquals(res.getStatus(), 500);
		}

		// There should be one success for first part being completed. Second would be setFailed which updates db directly so update is not called.
		ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

		org.mockito.Mockito.verify(mockJobStatusManager, org.mockito.Mockito.times(2)).updateJob(org.mockito.Matchers.anyString(), argCaptor.capture());

		JSONParser parser = new JSONParser();

		List<String> args = argCaptor.getAllValues();
		JSONObject status = (JSONObject) parser.parse(args.get(0));
		JSONArray children = (JSONArray) status.get("children");
		JSONObject child = (JSONObject) children.get(0);

		Assert.assertEquals(child.get("detail").toString(), "processing");
		Assert.assertEquals(child.get("status").toString(), "running");


		status = (JSONObject) parser.parse(args.get(1));
		children = (JSONArray) status.get("children");
		Assert.assertEquals(1, children.size());

		child = (JSONObject) children.get(0);
		Assert.assertEquals(child.get("detail").toString(), "success");
		Assert.assertEquals(child.get("status").toString(), "complete");


		// Check for setFailed invocation
		ArgumentCaptor<String> setFailedArgCaptor = ArgumentCaptor.forClass(String.class);

		org.mockito.Mockito.verify(mockJobStatusManager, org.mockito.Mockito.times(1)).setFailed(org.mockito.Matchers.anyString(), setFailedArgCaptor.capture());

		args = setFailedArgCaptor.getAllValues();
		status = (JSONObject) parser.parse(args.get(0));
		children = (JSONArray) status.get("children");

		Assert.assertEquals(1, children.size());

		child = (JSONObject) children.get(0);
		Assert.assertEquals(child.get("detail").toString(), "Mock failure for testing Process Chain Job failure. (Not real failure!!!)");
		Assert.assertEquals(child.get("status").toString(), "failed");



	}


	/**
	 * Tests getJobStatus where it should respond with current jobstatus with percentage completed
	 *
	 * @throws Exception
	 */
	@Test
	@Category(UnitTest.class)
	public void testGetJobStatus() throws Exception
	{
		JobResource real = new JobResource();
		JobResource spy = Mockito.spy(real);

		/* Create something like this for mock status
		 *
		 * {"jobId":"d9ebbe43-c319-410c-95d5-c339bb16f084",
		 * "statusDetail":"{\"chainjobstatus\":\"d9ebbe43-c319-410c-95d5-c339bb16f084\",
		 * \"childrencount\":\"2\",\"children\":[{\"id\":\"74722711-7f40-4067-a49c-6aedd26655a9\",
		 * \"detail\":\"processing\",\"status\":\"running\"}]}","status":"running"}
		 *
		 * */

		JSONObject mockStatus = new JSONObject();
		mockStatus.put("jobId", "test-id-1234");
		mockStatus.put("status", "running");

		JSONObject statusDetail = new JSONObject();
		statusDetail.put("chainjobstatus", "test-id-1234");
		statusDetail.put("childrencount", "2");

		JSONArray children = new JSONArray();
		JSONObject child = new JSONObject();
		child.put("id", "child-id-1234");
		child.put("detail", "processing");
		child.put("status", "running");
		children.add(child);

		statusDetail.put("children", children);
		mockStatus.put("statusDetail", statusDetail.toJSONString());


		org.mockito.Mockito.when(spy.getJobStatusObj(org.mockito.Matchers.anyString())).thenReturn(mockStatus);


		/* Create mock progress info like following
		 *
		 * { "source": "ogr2osm", "status": { "message": "Finished successfully.", "state": "Successful",
		 * "jobFinished": "true", "percentComplete" : 100 } }
		 *
		 * */

		JSONObject mockProgressInfo = new JSONObject();
		mockProgressInfo.put("source", "ogr2osm");

		JSONObject mockProgDetail = new JSONObject();
		mockProgDetail.put("message", "Reading ogr features");
		mockProgDetail.put("state", "Running");
		mockProgDetail.put("jobFinished", "false");
		mockProgDetail.put("percentComplete", 60);

		mockProgressInfo.put("status", mockProgDetail);

		org.mockito.Mockito.when(spy._getProgressText(org.mockito.Matchers.anyString())).thenReturn(mockProgressInfo.toJSONString());

		Response resp = spy.getJobStatus("child-id-1234");
		String respStr = resp.getEntity().toString();


		JSONParser p = new JSONParser();
		JSONObject oRes = (JSONObject)p.parse(respStr);

		long percent = (Long)oRes.get("percentcomplete");

		Assert.assertEquals(30, percent);


		String jobId = oRes.get("jobId").toString();
		Assert.assertEquals("test-id-1234", jobId);


		String statusDetailStr = oRes.get("statusDetail").toString();

		String expectedStatDetail = "{\"chainjobstatus\":\"test-id-1234\",\"childrencount\":\"2\",\"children\":" +
				"[{\"id\":\"child-id-1234\",\"detail\":\"processing\",\"status\":\"running\"}]}";
		Assert.assertEquals(expectedStatDetail, statusDetailStr);
	}

}
