package hoot.services.writers.review.custom.HGIS;

import hoot.services.db.DbUtils;
import hoot.services.exceptions.review.custom.HGIS.ReviewMapTagUpdateException;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

//import org.slf4j.Logger;
//import org.slf4j.LoggerFactory;


public class HGISValidationDbWriter {
	//private static final Logger log = LoggerFactory.getLogger(HGISValidationDbWriter.class);
	
	private Connection conn;
  private long mapId;
  
  public HGISValidationDbWriter(final Connection cn, final long mapid)
  {
  	this.conn = cn;
  	this.mapId = mapid;
  }
  
  public void updateMapTagWithReviewType() throws SQLException, ReviewMapTagUpdateException
  {
  	Map<String, String> tags = new HashMap<>();
  	tags.put("reviewtype", "hgisvalidation");
  	final long resCnt = DbUtils.updateMapsTableTags(tags, this.mapId, conn);
  	if(resCnt < 1)
  	{
  		throw new ReviewMapTagUpdateException("Failed to update maps table for mapid:" + this.mapId);
  	}

  }
}
