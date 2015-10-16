package hoot.services.writers.review;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
/**
 * Responsible for maintaining review SQL data consistency based on saved OSM changesets
 */
public class ReviewStatusModifier
{
  private static final Logger log = LoggerFactory.getLogger(ReviewStatusModifier.class);
  
  private long mapId;
  private Connection conn;
	
  public ReviewStatusModifier(final Connection conn, final long mapId) throws Exception
  {
    this.conn = conn;
    this.mapId = mapId;
  }
  
  /**
   * Updates all features for the given map as reviewed
   * 
   * @throws Exception 
   */
  public void setAllItemsReviewed() throws Exception
  {
    
  }
}
