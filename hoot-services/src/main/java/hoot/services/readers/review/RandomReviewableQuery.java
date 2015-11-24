package hoot.services.readers.review;

import hoot.services.models.review.ReviewQueryMapper;
import hoot.services.models.review.ReviewableItem;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class RandomReviewableQuery  extends ReviewableQueryBase implements IReviewableQuery 
{
	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(RandomReviewableQuery.class);
	
	public RandomReviewableQuery(final Connection c, final long mapid)
	{
		super(c,mapid);
	}

	
	public ReviewQueryMapper execQuery() throws SQLException, Exception
	{
		ReviewableItem ret = new ReviewableItem(-1, getMapId(), -1);
		

		
		long relId = -1;
		String seqId = "-1";
	
		try(
				Statement stmt = getConnection().createStatement();
				ResultSet rs = stmt.executeQuery(_getQueryString())
				)
		{
			long nResCnt = 0;
			while(rs.next())
			{
				relId = rs.getLong("relid");
				seqId = rs.getString("seq");
				nResCnt++;
			}
			
			ret.setRelationId(relId);
			ret.setSortOrder(Long.parseLong(seqId));
			ret.setResultCount(nResCnt);
		}

		return ret;
	}
	
	protected String _getQueryString()
	{
		return "select id as relid, tags->'hoot:review:sort_order' as seq from current_relations_" + getMapId() + 
				" where tags->'hoot:review:needs' = 'yes' order by random() limit 1";
	}
	

}
