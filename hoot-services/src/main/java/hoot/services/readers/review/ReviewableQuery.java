package hoot.services.readers.review;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ReviewableQuery {
	private static final Logger log = LoggerFactory.getLogger(ReviewableQuery.class);
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
	
	public ReviewableQuery(final Connection c, final long mapid)
	{
		_conn = c;
		_mapId = mapid;
	}
}
