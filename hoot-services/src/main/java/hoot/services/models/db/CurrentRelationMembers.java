package hoot.services.models.db;

import javax.annotation.Generated;

/**
 * CurrentRelationMembers is a Querydsl bean type
 */
@Generated("com.querydsl.codegen.BeanSerializer")
public class CurrentRelationMembers {

    private Long memberId;

    private String memberRole;

    private Object memberType;

    private Long relationId;

    private Integer sequenceId;

    public Long getMemberId() {
        return memberId;
    }

    public void setMemberId(Long memberId) {
        this.memberId = memberId;
    }

    public String getMemberRole() {
        return memberRole;
    }

    public void setMemberRole(String memberRole) {
        this.memberRole = memberRole;
    }

    public Object getMemberType() {
        return memberType;
    }

    public void setMemberType(Object memberType) {
        this.memberType = memberType;
    }

    public Long getRelationId() {
        return relationId;
    }

    public void setRelationId(Long relationId) {
        this.relationId = relationId;
    }

    public Integer getSequenceId() {
        return sequenceId;
    }

    public void setSequenceId(Integer sequenceId) {
        this.sequenceId = sequenceId;
    }

}

