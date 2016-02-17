package hoot.services.writers.user;

import java.sql.Connection;

import hoot.services.UnitTest;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import com.mysema.query.sql.dml.SQLInsertClause;

public class UserSaverTest {
	@Test
  @Category(UnitTest.class)
	public void testInsert() throws Exception
	{
		Connection conn = null;
		UserSaver saver = new UserSaver(conn);
		SQLInsertClause cl = saver._createInsertClause("test@test.com");
		
		String actual = cl.toString();
		
		String expected = "insert into \"users\" (\"email\", \"display_name\")\n" + 
				"values (?, ?)";
		
		org.junit.Assert.assertEquals(expected, actual);
	}
}
