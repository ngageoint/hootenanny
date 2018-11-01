package hoot.services.models.db;

public class SpringSessionAttributes {
	private String session_id;
	private String attribute_name;
	// private byte[] attribute_bytes;
//	public byte[] getAttribute_bytes() {
//		return attribute_bytes;
//	}
//	public void setAttribute_bytes(byte[] attribute_bytes) {
//		this.attribute_bytes = attribute_bytes;
//	}
	public String getAttribute_name() {
		return attribute_name;
	}
	public void setAttribute_name(String attribute_name) {
		this.attribute_name = attribute_name;
	}
	public String getSession_id() {
		return session_id;
	}
	public void setSession_id(String session_id) {
		this.session_id = session_id;
	}
}
