package hoot.services.job;

public enum JobType {
    IMPORT, EXPORT, CONFLATE, CLIP, ATTRIBUTES, BASEMAP, DELETE, UNKNOWN;

    public static JobType fromInteger(int value) {
        if ((value >= 0) && (value < JobType.values().length)) {
            return JobType.values()[value];
        }
        return UNKNOWN;
    }

    @Override
    public String toString() {
        return this.name().toLowerCase();
    }
}
