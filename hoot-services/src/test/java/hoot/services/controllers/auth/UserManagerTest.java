package hoot.services.controllers.auth;

import static org.junit.Assert.assertEquals;

import java.sql.Timestamp;

import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.support.AnnotationConfigContextLoader;
import org.springframework.transaction.annotation.Transactional;

import hoot.services.UnitTest;
import hoot.services.jerseyframework.HootServicesJerseyTestAbstract;
import hoot.services.jerseyframework.HootServicesSpringTestConfig;
import hoot.services.models.db.Users;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(classes = HootServicesSpringTestConfig.class, loader = AnnotationConfigContextLoader.class)
@Transactional
public class UserManagerTest extends HootServicesJerseyTestAbstract {
	@Autowired
	private UserManager userManager;

	@Test
	@Category(UnitTest.class)
	public void testParseUser() throws Exception {
		//@formatter:off
		String xml = "\n" +
				"<osm generator=\"OpenStreetMap server\" version=\"0.6\">\n" +
				"    <user account_created=\"2018-08-22T21:30:14Z\" display_name=\"Arni Sumarlidason (work)\" id=\"8723740\">\n" +
				"        <description/>\n" +
				"        <contributor-terms agreed=\"true\" pd=\"false\"/>\n" +
				"        <roles/>\n" +
				"        <changesets count=\"0\"/>\n" +
				"        <traces count=\"0\"/>\n" +
				"        <blocks>\n" +
				"            <received active=\"0\" count=\"0\"/>\n" +
				"        </blocks>\n" +
				"        <languages>\n" +
				"            <lang>en-US</lang>\n" +
				"            <lang>en</lang>\n" +
				"        </languages>\n" +
				"        <messages>\n" +
				"            <received count=\"0\" unread=\"0\"/>\n" +
				"            <sent count=\"0\"/>\n" +
				"        </messages>\n" +
				"    </user>\n" +
				"</osm>";
		//@formatter:on
		Users user = userManager.parseUser(xml);

		assertEquals("Arni Sumarlidason (work)", user.getDisplayName());
		assertEquals(new Long(8723740L), user.getId());
	}

	@Test
	@Category(UnitTest.class)
	public void testParseAccountCreated() throws Exception {
		final String datetime = "2018-08-22T21:30:14Z";
		Timestamp ts = userManager.parseTimestamp(datetime);
		assertEquals(1534973414000L, ts.getTime());
	}

}
