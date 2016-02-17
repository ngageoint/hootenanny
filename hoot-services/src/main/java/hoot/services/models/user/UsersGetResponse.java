package hoot.services.models.user;

import hoot.services.db2.Users;

import java.util.List;

import javax.xml.bind.annotation.XmlRootElement;

@XmlRootElement
public class UsersGetResponse {
	private List<Users> users;
	
	public List<Users> getUsers()
	{
		return this.users;
	}
	
	public void setUers(final List<Users> usrs)
	{
		this.users = usrs;
	}
	
	
	public UsersGetResponse(final List<Users> usrs)
	{
		this.users = usrs;
	}
	
	public UsersGetResponse()
	{
		
	}

}
