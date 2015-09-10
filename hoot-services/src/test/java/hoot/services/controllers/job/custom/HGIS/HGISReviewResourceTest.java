package hoot.services.controllers.job.custom.HGIS;

import java.util.List;

import javax.ws.rs.core.Response.Status;

import hoot.services.UnitTest;
import hoot.services.models.review.custom.HGIS.PrepareForValidationRequest;
import hoot.services.models.review.custom.HGIS.PrepareForValidationResponse;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.mockito.ArgumentCaptor;
import org.mockito.Mockito;

public class HGISReviewResourceTest {

	@Test
	@Category(UnitTest.class)
	public void TestPrepareItemsForValidationReview() throws Exception
	{
		HGISReviewResource real = new HGISReviewResource();
		HGISReviewResource spy = Mockito.spy(real);
		
		org.mockito.Mockito.doReturn(true).when(spy)._mapExists(org.mockito.Matchers.anyString());
		

		ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

		Mockito.doNothing().when(spy).postJobRquest(org.mockito.Matchers.anyString(), argCaptor.capture());
		PrepareForValidationRequest request = new PrepareForValidationRequest();
		request.setSourceMap("testSrc1");
		request.setOutputMap("out1");
		PrepareForValidationResponse resp = spy.prepareItemsForValidationReview(request);
		
		List<String> args = argCaptor.getAllValues();
		String  param = args.get(0);
		JSONParser parser = new JSONParser();
		JSONObject result = (JSONObject)parser.parse(param);
		Assert.assertEquals(result.get("exec"), "custom/HGIS/PrepareForValidation.sh");
		Assert.assertEquals(result.get("exectype"), "bash");
		Assert.assertNotNull(result.get("params"));
		JSONArray arr = (JSONArray)result.get("params");
		Assert.assertEquals(((JSONObject)arr.get(0)).get("SOURCE"), "postgresql://hoot:hoottest@localhost:5432/hoot/testSrc1");
		Assert.assertEquals(((JSONObject)arr.get(1)).get("OUTPUT"), "postgresql://hoot:hoottest@localhost:5432/hoot/out1");
	}
	

	@Test(expected=javax.ws.rs.WebApplicationException.class)
  @Category(UnitTest.class)
	public void TestInvalidSource() throws Exception
	{

		try
		{
			PrepareForValidationRequest request = new PrepareForValidationRequest();
			request.setSourceMap(null);
			request.setOutputMap("out1");
			HGISReviewResource real = new HGISReviewResource();
			real.prepareItemsForValidationReview(request);
		}
		catch(javax.ws.rs.WebApplicationException e)
		{
			Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
			throw e;
		}

		
	}
	
	
	@Test(expected=javax.ws.rs.WebApplicationException.class)
  @Category(UnitTest.class)
	public void TestInvalidOutput() throws Exception
	{

		try
		{
			PrepareForValidationRequest request = new PrepareForValidationRequest();
			request.setSourceMap("source");
			request.setOutputMap(null);
			HGISReviewResource real = new HGISReviewResource();
			real.prepareItemsForValidationReview(request);
		}
		catch(javax.ws.rs.WebApplicationException e)
		{
			Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
			throw e;
		}		
	}
	
	@Test(expected=javax.ws.rs.WebApplicationException.class)
  @Category(UnitTest.class)
	public void TestInvalidNoMap() throws Exception
	{

		try
		{
			HGISReviewResource real = new HGISReviewResource();
			HGISReviewResource spy = Mockito.spy(real);
			
			org.mockito.Mockito.doReturn(false).when(spy)._mapExists(org.mockito.Matchers.anyString());
			

			ArgumentCaptor<String> argCaptor = ArgumentCaptor.forClass(String.class);

			Mockito.doNothing().when(spy).postJobRquest(org.mockito.Matchers.anyString(), argCaptor.capture());
			PrepareForValidationRequest request = new PrepareForValidationRequest();
			request.setSourceMap("testSrc1");
			request.setOutputMap("out1");
			PrepareForValidationResponse resp = spy.prepareItemsForValidationReview(request);
		}
		catch(javax.ws.rs.WebApplicationException e)
		{
			Assert.assertEquals(Status.BAD_REQUEST.getStatusCode(), e.getResponse().getStatus());
			throw e;
		}

		
	}
}
