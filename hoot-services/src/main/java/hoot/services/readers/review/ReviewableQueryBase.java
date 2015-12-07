package hoot.services.readers.review;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * 
 */
public class ReviewableQueryBase 
{
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewableQueryBase.class);
	private Connection _conn;
	private long _mapId = -1;
	
	protected Connection getConnection()
	{
		return _conn;
	}
	
	protected long getMapId()
	{
		return _mapId;
	}
	
	public ReviewableQueryBase(final Connection c, final long mapid)
	{
		_conn = c;
		_mapId = mapid;
	}
}
