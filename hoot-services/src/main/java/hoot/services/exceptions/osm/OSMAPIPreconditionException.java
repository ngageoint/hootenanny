package hoot.services.exceptions.osm;

public class OSMAPIPreconditionException extends Exception {

    public OSMAPIPreconditionException(String message, Throwable t) {
        super(message, t);
    }

    public OSMAPIPreconditionException(String message) {
        super(message);
    }
}
