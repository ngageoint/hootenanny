package hoot.services.readers.users;

import java.sql.Connection;

import hoot.services.UnitTest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

public class UsersRetrieverTest {
	@Test
  @Category(UnitTest.class)
	public void testRetrieveAll() throws Exception
	{
		Connection conn = null;
		UsersRetriever r = new UsersRetriever(conn);
		String actual = r._getAllQuery().toString();
		
		String expected = "from \"users\" \"users\"\n" + 
				"order by \"users\".\"display_name\" asc";
		
		org.junit.Assert.assertEquals(expected, actual);
	}
}
