package hoot.services.exceptions.osm;

public class OSMAPIAlreadyDeletedException extends Exception {

    public OSMAPIAlreadyDeletedException(String message, Throwable t) {
        super(message, t);
    }

    public OSMAPIAlreadyDeletedException(String message) {
        super(message);
    }

}
