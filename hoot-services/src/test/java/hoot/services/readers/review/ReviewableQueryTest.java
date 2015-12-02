package hoot.services.readers.review;

import hoot.services.UnitTest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

public class ReviewableQueryTest {

	@Test
  @Category(UnitTest.class)
  public void testGetQuery() throws Exception
  {
		String expected = "select id from current_relations_10 where "
				+ "tags->'hoot:review:needs' = 'yes' and tags->'hoot:review:sort_order'='2'";
		ReviewableQuery q = new ReviewableQuery(null, 10, 2);
		String actual = q._getQueryString();
		
		
		org.junit.Assert.assertEquals(expected, actual);
  }
}
