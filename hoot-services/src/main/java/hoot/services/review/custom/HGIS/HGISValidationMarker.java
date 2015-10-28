package hoot.services.review.custom.HGIS;

import hoot.services.db2.QMaps;
import hoot.services.exceptions.review.custom.HGIS.ReviewMapTagUpdateException;
import hoot.services.models.osm.ModelDaoUtils;
import hoot.services.writers.review.custom.HGIS.HGISValidationDbWriter;

import java.sql.Connection;
import java.sql.SQLException;

public class HGISValidationMarker {
	private long mapId;
  private Connection conn;
  
  public HGISValidationMarker(final Connection cn, final String mapName) throws Exception
  {
  	this.conn = cn;
  	
  	QMaps maps = QMaps.maps;
    this.mapId =
      ModelDaoUtils.getRecordIdForInputString(mapName, conn,
      		maps, maps.id, maps.displayName);
  }
  
  
  public void updateValidationMapTag() throws SQLException, ReviewMapTagUpdateException
  {
  	HGISValidationDbWriter writer = new HGISValidationDbWriter(conn, mapId);
  	writer.updateMapTagWithReviewType();
  }
}
