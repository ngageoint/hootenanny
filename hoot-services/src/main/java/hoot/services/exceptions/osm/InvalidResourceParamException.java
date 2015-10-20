package hoot.services.exceptions.osm;

@SuppressWarnings("serial")
public class InvalidResourceParamException extends Exception {
	public InvalidResourceParamException(final String msg)
	{
		super(msg);
	}
}
