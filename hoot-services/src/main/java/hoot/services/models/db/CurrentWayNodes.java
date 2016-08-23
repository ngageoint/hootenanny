package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * CurrentWayNodes is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class CurrentWayNodes {

    private Long nodeId;

    private Long sequenceId;

    private Long wayId;

    public Long getNodeId() {
        return nodeId;
    }

    public void setNodeId(Long nodeId) {
        this.nodeId = nodeId;
    }

    public Long getSequenceId() {
        return sequenceId;
    }

    public void setSequenceId(Long sequenceId) {
        this.sequenceId = sequenceId;
    }

    public Long getWayId() {
        return wayId;
    }

    public void setWayId(Long wayId) {
        this.wayId = wayId;
    }

}

