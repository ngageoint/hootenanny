package hoot.services.db.postgres;

import hoot.services.UnitTest;

import java.util.Map;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;

public class PostgresUtilsTest {
	@Test
	@Category(UnitTest.class)
	public void TestPostgresObjToHStore() throws Exception
	{
		String tagsStr = "\"test1\"=>\"val1\", \"test2\"=>\"{\\\"label\\\":\\\"Confirmed\\\","
				+ "\\\"description\\\":\\\"You can look at the point and tell what it is (e.g. mosque or airport)\\\","
				+ "\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"confirmed\\\",\\\"hgis:accuracy\\\":"
				+ "\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:"
				+ "sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\","
				+ "\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}";
		Map<String,String> ret = PostgresUtils._parseTags(tagsStr);
		
		Assert.assertTrue(ret.size() == 2);
		Assert.assertTrue(ret.get("test1").toString().equals("val1"));
		Assert.assertEquals("{\\\"label\\\":\\\"Confirmed\\\",\\\"description\\\":"
				+ "\\\"You can look at the point and tell what it is (e.g. mosque or airport)\\\","
				+ "\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"confirmed\\\","
				+ "\\\"hgis:accuracy\\\":\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":"
				+ "\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\","
				+ "\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":"
				+ "\\\"${BASEMAP_IMAGE_ID}\\\"}}}", ret.get("test2").toString());
		
	}

}
