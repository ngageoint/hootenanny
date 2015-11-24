package hoot.services.readers.review;

import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItem;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ReviewableQuery extends ReviewableQueryBase implements IReviewableQuery 
{
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewableQuery.class);
	private long _seqId = -1;
	
	public ReviewableQuery(final Connection c, final long mapid, final long seqid)
	{
		super(c, mapid);
		_seqId = seqid;
	}
	
	
	protected String _getQueryString()
	{
		return "select id from current_relations_" + getMapId() + 
				" where tags->'hoot:review:needs' = 'yes' and tags->'hoot:review:sort_order'='" + 
				_seqId + "'";
	}
	
	
	public ReviewQueryMapper execQuery() throws SQLException, Exception
	{
		ReviewableItem ret = null;
		try(
				Statement stmt = getConnection().createStatement();
				ResultSet rs = stmt.executeQuery(_getQueryString())
				)
		{
			long nResCnt = 0;
			long relid = -1;
			while(rs.next())
      {
         //Retrieve by column name
				relid = rs.getLong("id");  
				nResCnt++;
      }
			
			ret = new ReviewableItem(_seqId, getMapId(), relid);
			ret.setResultCount(nResCnt);
		}

		return ret;
	}

}
