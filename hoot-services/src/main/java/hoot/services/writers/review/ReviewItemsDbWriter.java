package hoot.services.writers.review;

import hoot.services.db2.QMaps;
import hoot.services.models.osm.ModelDaoUtils;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ReviewItemsDbWriter {

	private static final Logger log = LoggerFactory.getLogger(ReviewItemsDbWriter.class);
	
	private static final String POSTGRESQL_DRIVER = "org.postgresql.Driver";
	
	private long mapId;
  public long getMapId() { return mapId; }

  private Connection conn;
	
	public ReviewItemsDbWriter(final Connection conn, final String mapId) throws Exception
	{	
    this.conn = conn;
    
    QMaps maps = QMaps.maps;
    this.mapId =
      ModelDaoUtils.getRecordIdForInputString(mapId, conn,
      		maps, maps.id, maps.displayName);
		
	}
	
	protected String _getRawSql() throws Exception
	{
		return "INSERT INTO review_items " + 
    		"(map_id, reviewable_item_id, review_score, review_against_item_id, review_status,reviewable_item_rel_id )" +
    		"SELECT " + mapId + ",'', to_number(tags->'hoot:review:score', '99G999D9S') as score, '', 'unreviewed', id  from " + 
    				"current_relations_" + mapId + " where EXIST(tags, 'hoot:review:needs') and EXIST(tags, 'hoot:review:score')";
	}
	
	public void populateTable() throws Exception
	{


    Statement stmt = null;
    try
    {
      Class.forName(POSTGRESQL_DRIVER);

      stmt = conn.createStatement();

      String sql = _getRawSql();
      stmt.executeUpdate(sql);

    }
    catch (Exception e)
    {
      throw new Exception("Error inserting review items.");
    }

    finally
    {
      // finally block used to close resources
      try
      {
        if (stmt != null)
          stmt.close();
      }
      catch (SQLException se2)
      {
      	throw se2;
      }// nothing we can do

    }// end try
  
	}
}
