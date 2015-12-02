package hoot.services.models.review;

import hoot.services.UnitTest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

public class ReviewableStatisticsTest {
	@Test
  @Category(UnitTest.class)
  public void testToString() throws Exception
  {
		ReviewableStatistics o =  new ReviewableStatistics(5, 3);
		String expected = "{\"totalCount\":5,\"unreviewedCount\":3}";
		String actual = o.toString();
		
		org.junit.Assert.assertEquals(expected, actual);
  }
}
