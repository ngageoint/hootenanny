package hoot.services.readers.review;

import hoot.services.UnitTest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

public class ReviewableStatisticsQueryTest {

	@Test
  @Category(UnitTest.class)
  public void testGetQuery() throws Exception
  {
		String expected = "select count(*) as totalcnt from current_relations_10 where tags->'type' = 'review'";
		
		ReviewableStatisticsQuery q = new ReviewableStatisticsQuery(null, 10);
		String actual = q._getTotalReviewableCountQueryString();
		org.junit.Assert.assertEquals(expected, actual);
		
		expected = "select count(*) as remaining from current_relations_10 where tags->'hoot:review:needs' = 'yes'";
		
		actual = q._getUnreviewedCountQueryString();
		org.junit.Assert.assertEquals(expected, actual);
		
		
		
  }
	
}
