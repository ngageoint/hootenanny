package hoot.services.models.user;

import hoot.services.db2.Users;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class UserSaveResponse {
	private Users user;
	
	public Users getUser()
	{
		return user;
	}
	
	public void setUser(final Users u)
	{
		user = u;
	}
	
	public UserSaveResponse(final Users u)
	{
		user = u;
	}
	
	public UserSaveResponse()
	{
		
	}
}
