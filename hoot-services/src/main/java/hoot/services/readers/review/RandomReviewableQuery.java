package hoot.services.readers.review;

import hoot.services.HootProperties;
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
  private static final Logger log = LoggerFactory.getLogger(RandomReviewableQuery.class);
	
	public RandomReviewableQuery(final Connection c, final long mapid)
	{
		super(c,mapid);
		
		try
		{
			//TODO: Since this code will affect all subsequent calls to random(), it is better moved to
			//a more centralized location.  Given that this is the only class useing random() in a SQL
			//query so far, no harm is done for the time being.
			if (Boolean.parseBoolean(
		      HootProperties.getInstance().getProperty(
            "seedRandomQueries", HootProperties.getDefault("seedRandomQueries"))))
	    {
		    final double seed = 
			    Double.parseDouble(
	          HootProperties.getInstance().getProperty(
              "randomQuerySeed", HootProperties.getDefault("randomQuerySeed")));
		    if (seed >= -1.0 && seed <= 1.0)
		    {
		  	  Statement stmt = null;
		  	  try
		  	  {
		  		  stmt = getConnection().createStatement();
		  		  //After executing this, all subsequent calls to random() will be seeded.
		  	    stmt.executeQuery("select setseed(" + seed + ");");
		  	  }
		  	  finally
		  	  {
		  		  if (stmt != null)
		  		  {
		  			  stmt.close();
		  		  }
		  	  }
		    }
	    }
		}
		catch (Exception e)
		{
			log.error("Unable to seed random review query.");
		}
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
		return 
			"select id as relid, tags->'hoot:review:sort_order' as seq from current_relations_" + 
			getMapId() + " where tags->'hoot:review:needs' = 'yes' order by random() limit 1";
	}
}
