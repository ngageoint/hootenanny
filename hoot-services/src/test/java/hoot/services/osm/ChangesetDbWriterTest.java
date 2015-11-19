package hoot.services.osm;

import java.sql.Connection;

import hoot.services.IntegrationTest;
import hoot.services.db.DbUtils;
import hoot.services.db2.QCurrentNodes;
import hoot.services.geo.BoundingBox;
import hoot.services.models.osm.Changeset;
import hoot.services.writers.osm.ChangesetDbWriter;

import org.junit.Assert;
import org.junit.Test;
import org.junit.experimental.categories.Category;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;

@SuppressWarnings("unused")
public class ChangesetDbWriterTest
{
	private static final Logger log = LoggerFactory.getLogger(ChangesetDbWriterTest.class);
	
	private static final QCurrentNodes nodes = QCurrentNodes.currentNodes;

	private static final int NUM_NODES = 1000;
	private static final int NUM_TAGS_PER_NODE = 10;
	
	@Test
	@Category(IntegrationTest.class)
	public void testLargeWrite() throws Exception
	{
		Connection conn  = DbUtils.createConnection();
		final long userId = DbUtils.insertUser(conn);
  	final long mapId = DbUtils.insertMap(userId, conn);
		final long changesetId = Changeset.insertNew(mapId, userId, conn);
		final BoundingBox originalBounds = OsmTestUtils.createStartingTestBounds();
		try
		{
			String changeset = 
				"<osmChange version=\"0.3\" generator=\"iD\">" +
          "<create>";
			for (int i = 0; i < NUM_NODES; i++)
			{
				changeset +=
					  "<node id=\"" + String.valueOf((i + 1) * -1) + "\" lon=\"" + 
				      originalBounds.getMinLon() + "\" lat=\"" +
              originalBounds.getMinLat() + "\" version=\"0\" changeset=\"" + changesetId + "\">";
				for (int j = 0; j < NUM_TAGS_PER_NODE; j++)
				{
					changeset += 
						  "<tag k=\"" + "tag " + String.valueOf(j + 1) + "\" v=\"" + String.valueOf(j + 1) + "\"/>";
				}
				changeset += "</node>";
			}
			changeset += 
				  "</create>" +
          "<modify/>" +
          "<delete if-unused=\"true\"/>" +
         "</osmChange>";
			
			log.info("Create elements test start.");
	    /*final Document response = */new ChangesetDbWriter(conn).write(mapId, changesetId, changeset);
	    log.info("Create elements test end.");
	    
	    Assert.assertEquals(
	    	NUM_NODES, 
	    	(int)new SQLQuery(conn, DbUtils.getConfiguration(mapId))
          .from(nodes)
          .count());
		}
		finally
		{
			//DbUtils.deleteOSMRecord(conn, mapId);
      
      conn.close();
		}
	}
}
