package hoot.services.readers.review;

import java.sql.Connection;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.SQLQuery;
import com.mysema.query.types.OrderSpecifier;

import hoot.services.db.DbUtils;
import hoot.services.db2.QReviewTags;
import hoot.services.db2.ReviewTags;

public class ReviewTagRetriever {

	@SuppressWarnings("unused")
  private static final Logger log = LoggerFactory.getLogger(ReviewTagRetriever.class);
	
	private Connection _conn;
	private QReviewTags _reviewTags = QReviewTags.reviewTags;
	
	public ReviewTagRetriever(final Connection cn)
	{
		this._conn = cn;
	}
	
	/**
	 * Retrieves single review tag using map id and relation id.
	 * 
	 * @param mapId
	 * @param relationId
	 * @return
	 * @throws Exception
	 */
	public List<ReviewTags> retrieve(final long mapId, final long relationId) throws Exception
	{		

		
		List<ReviewTags> res = null;
		try
		{
			SQLQuery query = _getQuery(mapId, relationId);
			res = query.list(_reviewTags);
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return res;
	}
	
	
	/**
	 * Retrieves all review tags
	 * 
	 * @param orderByCol - order by column to sort
	 * @param isAsc - is order by asc | desc
	 * @param limit - limit for numbers of returned results
	 * @param offset - offset row for paging
	 * @return - list of Review tags
	 * @throws Exception
	 */
	public List<ReviewTags> retrieveAll(final String orderByCol, 
			final boolean isAsc, final long limit, final long offset) throws Exception
	{		

		
		List<ReviewTags> res = null;
		try
		{
			SQLQuery query = _getAllQuery(orderByCol, isAsc, limit, offset);
			res = query.list(_reviewTags);
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return res;
	}
	
	
	/**
	 * Get the total counts of all review tags
	 * 
	 * @return - numbers of toal count
	 * @throws Exception
	 */
	public long getTagsCount() throws Exception
	{
		long res = -1;
		
		try
		{
			SQLQuery query = new SQLQuery(this._conn, DbUtils.getConfiguration());
			res = query.from(_reviewTags).count();
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return res;
	}
	
	/**
	 * SQL Query for retrieving review tag
	 * 
	 * @param mapId
	 * @param relationId
	 * @return - SQLQuery
	 * @throws Exception
	 */
	protected SQLQuery _getQuery(final long mapId, final long relationId) throws Exception
	{
		SQLQuery query = new SQLQuery(this._conn, DbUtils.getConfiguration());
		try
		{
			query.from(_reviewTags)
			.where(_reviewTags.mapId.eq(mapId).and(_reviewTags.relationId.eq(relationId)));
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return query;
	}
	
	/**
	 * SQL Query for retrieving all tags
	 * 
	 * @param orderByCol
	 * @param isAsc
	 * @param limit
	 * @param offset
	 * @return - SQLQuery
	 * @throws Exception
	 */
	protected SQLQuery _getAllQuery(final String orderByCol, 
			final boolean isAsc, final long limit, final long offset) throws Exception
	{
		SQLQuery query = new SQLQuery(this._conn, DbUtils.getConfiguration());
		try
		{
			query.from(_reviewTags).orderBy(_getSpecifier(orderByCol, isAsc));
			if(limit > -1)
			{
				query.limit(limit);
			}
			
			if(offset > -1)
			{
				query.offset(offset);
			}
			

		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		
		return query;
	}
	
	/**
	 * Filter for allowed columns for order by
	 * 
	 * @param orderByCol - String representation of order by column
	 * @param isAsc - asc | dsc
	 * @return - OrderSpecifier
	 */
	protected OrderSpecifier _getSpecifier(final String orderByCol, 
			final boolean isAsc)
	{
		OrderSpecifier res = _reviewTags.id.asc();
		try
		{
			if(orderByCol != null)
			{
				switch(orderByCol)
				{
				case "id" :
					res =  (isAsc) ? _reviewTags.id.asc() : _reviewTags.id.desc();				
					break;
				case "createdAt" :
					res = (isAsc) ? _reviewTags.createdAt.asc() : _reviewTags.createdAt.desc();	
					break;
				case "createdBy" :
					res = (isAsc) ? _reviewTags.createdBy.asc() : _reviewTags.createdBy.desc();	
					break;
				case "lastModifiedAt" :
					res = (isAsc) ? _reviewTags.lastModifiedAt.asc() : _reviewTags.lastModifiedAt.desc();	
					break;
				case "lastModifiedBy" :
					res = (isAsc) ? _reviewTags.lastModifiedBy.asc() : _reviewTags.lastModifiedBy.desc();	
					break;
				case "mapId" :
					res = (isAsc) ? _reviewTags.mapId.asc() : _reviewTags.mapId.desc();	
					break;
				case "relationId" :
					res = (isAsc) ? _reviewTags.relationId.asc() : _reviewTags.relationId.desc();	
					break;
				default :
					res = _reviewTags.id.asc();
					break;				
				}
			}
		}
		catch(Exception ex)
		{
			log.warn("Defualting to reviewtags.id asc order: " + ex.getMessage());
		}
		
		
		return res;
	}
	

}
