package hoot.services.models.db;

public class UserSessions {
    private Long user_id;
    private String session_id;

    public UserSessions(String session_id, Long user_id) {
        this.user_id = user_id;
        this.session_id = session_id;
    }
    public UserSessions(Long user_id, String session_id) {
        this.user_id = user_id;
        this.session_id = session_id;
    }

    public String getSession_id() {
        return session_id;
    }
    public void setSession_id(String session_id) {
        this.session_id = session_id;
    }
    public Long getUser_id() {
        return user_id;
    }
    public void setUser_id(Long user_id) {
        this.user_id = user_id;
    }


}
