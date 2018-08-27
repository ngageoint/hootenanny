package hoot.services.controllers.auth;

public class InvalidUserProfileException extends Exception {

    /**
     *
     */
    private static final long serialVersionUID = 1L;
    private String xml;

    InvalidUserProfileException(String xmlUserProfile) {
        this.xml = xmlUserProfile;
    }

    @Override
    public String toString() {
        return super.toString() + "\nUser Profile:\n" + xml;
    }
}
