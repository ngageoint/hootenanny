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
				+ "\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}\"";
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


		String fullExample = "\"poi\"=>\"yes\", \"name\"=>\"Garden of the Gods\", \"uuid\"=>\"{82e7efa6-1295-44c7-b79c-5ae022d7b3a1}\", "
		+ "\"leisure\"=>\"park\", \"hoot:status\"=>\"1\", \"error:circular\"=>\"1000\", \"hoot:review:note\"=>\"Flagged for imagery validation\", \"hoot:review:needs\"=>\"yes\", "
		+ "\"hoot:review:choices:1\"=>\"{\\\"label\\\":\\\"Confirmed\\\",\\\"description\\\":\\\"You can look at the point and tell what it is (e.g. mosque or airport)\\\",\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"confirmed\\\",\\\"hgis:accuracy\\\":\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}\", "
		+ "\"hoot:review:choices:2\"=>\"{\\\"label\\\":\\\"Assessed\\\",\\\"description\\\":\\\"The point is on a building, but you can't verify its type (e.g. hair salon).\\\",\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"assessed\\\",\\\"hgis:accuracy\\\":\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}\", "
		+ "\"hoot:review:choices:3\"=>\"{\\\"label\\\":\\\"Reported\\\",\\\"description\\\":\\\"Imagery is pixelated or cloudy -- can not be assessed.\\\",\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"reported\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}\"";

		Map<String,String> tags = PostgresUtils._parseTags(fullExample);
		Assert.assertNotNull(tags);
		Assert.assertEquals(tags.size(), 11);
		Assert.assertEquals(tags.get("poi").toString(), "yes");
		Assert.assertEquals(tags.get("hoot:review:note").toString(), "Flagged for imagery validation");

		String expected = "{\\\"label\\\":\\\"Assessed\\\",\\\"description\\\":\\\"The point is on a building, but you can't verify its type (e.g. hair salon).\\\",\\\"changes\\\":{\\\"replaceTags\\\":{\\\"hgis:imagery_confirmed\\\":\\\"assessed\\\",\\\"hgis:accuracy\\\":\\\"high\\\"},\\\"appendTags\\\":{\\\"source:geometry\\\":\\\"${BASEMAP_IMAGE_SOURCE}\\\",\\\"source:geometry:sensor\\\":\\\"${BASEMAP_IMAGE_SENSOR}\\\",\\\"source:geometry:date\\\":\\\"${BASEMAP_IMAGE_DATETIME}\\\",\\\"source:geometry:id\\\":\\\"${BASEMAP_IMAGE_ID}\\\"}}}";

		Assert.assertEquals(expected,
				tags.get("hoot:review:choices:2").toString());
	}

}

