package hoot.services.writers.user;

import hoot.services.db.DbUtils;
import hoot.services.db2.QUsers;
import hoot.services.db2.Users;

import java.sql.Connection;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.mysema.query.sql.Configuration;
import com.mysema.query.sql.SQLQuery;
import com.mysema.query.sql.dml.SQLInsertClause;

public class UserSaver {
	private static final Logger log = LoggerFactory.getLogger(UserSaver.class);
	
private Connection _conn;
	
	public UserSaver(final Connection cn) 
	{
		_conn = cn;
	}
	
	public Users getOrSaveByEmail(final String userEmail) throws Exception
	{
		Users ret = null;
		
		try
		{
			QUsers users = QUsers.users;

	   
			ret = (new SQLQuery(_conn, DbUtils.getConfiguration()))
					.from(users).where(users.email.equalsIgnoreCase(userEmail)).singleResult(users);
			// none then create
			if(ret == null)
			{
				long nCreated = insert(userEmail);
				if(nCreated > 0)
				{

					ret = (new SQLQuery(_conn, DbUtils.getConfiguration())).from(users)
							.where(users.email.equalsIgnoreCase(userEmail)).singleResult(users);
				}
			}
		}
		catch (Exception ex)
		{
			log.error("Failed to save user by email: " + ex.getMessage());
			throw ex;
		}
		return ret;
	}
	
	public long insert(final String email) throws Exception
	{
		long nInserted = 0;
		try
		{
			SQLInsertClause cl = _createInsertClause(email);
			if(cl != null)
			{
				nInserted = cl.execute();
			}
			else 
			{
				throw new Exception("Invalid insert clause.");
			}

		}
		catch(Exception ex)
		{
			String err = ex.getMessage();
			log.error(err);
			throw ex;
		}
		return nInserted;
	}
	
	protected SQLInsertClause _createInsertClause(final String email) throws Exception
	{
		SQLInsertClause cl = null;
		try
		{
			Configuration configuration = DbUtils.getConfiguration();
			QUsers users = QUsers.users;
			cl = new SQLInsertClause(_conn, configuration,users)
			.columns(users.email, users.displayName)
			.values(email, email);
		}
		catch (Exception ex)
		{
			log.error(ex.getMessage());
			throw ex;
		}
		return cl;
	}
}
