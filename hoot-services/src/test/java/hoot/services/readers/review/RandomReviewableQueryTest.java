package hoot.services.readers.review;

import hoot.services.UnitTest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

public class RandomReviewableQueryTest {
	
	@Test
  @Category(UnitTest.class)
  public void testGetQuery() throws Exception
  {
		String expected = "select id as relid, tags->'hoot:review:sort_order' as seq from current_relations_10 "
				+ "where tags->'hoot:review:needs' = 'yes' order by random() limit 1";
		RandomReviewableQuery q = new RandomReviewableQuery(null, 10);
		String actual = q._getQueryString();
		
		
		org.junit.Assert.assertEquals(expected, actual);
  }
}
