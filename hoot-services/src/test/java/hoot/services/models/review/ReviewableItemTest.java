package hoot.services.models.review;

import hoot.services.UnitTest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

public class ReviewableItemTest {
	@Test
  @Category(UnitTest.class)
  public void testToString() throws Exception
  {
		ReviewableItem o =  new ReviewableItem(5, 3, 4);
		String expected = "{\"sortorder\":5,\"relationid\":4,\"mapid\":3}";
		String actual = o.toString();
		
		org.junit.Assert.assertEquals(expected, actual);
  }
}
