package hoot.services.job;

import static org.junit.Assert.assertEquals;

import org.junit.Test;
import org.junit.experimental.categories.Category;

import hoot.services.UnitTest;


public class JobTypeTest {

    @Test
    @Category(UnitTest.class)
    public void test() {
        assertEquals(JobType.IMPORT, JobType.fromInteger(0));
        assertEquals(JobType.EXPORT, JobType.fromInteger(1));
        assertEquals(JobType.CONFLATE, JobType.fromInteger(2));
        assertEquals(JobType.CLIP, JobType.fromInteger(3));
        assertEquals(JobType.ATTRIBUTES, JobType.fromInteger(4));
        assertEquals(JobType.BASEMAP, JobType.fromInteger(5));
        assertEquals(JobType.DELETE, JobType.fromInteger(6));
        assertEquals(JobType.UNKNOWN, JobType.fromInteger(7));
        assertEquals(JobType.UNKNOWN, JobType.fromInteger(-1));
        assertEquals(JobType.UNKNOWN, JobType.fromInteger(99));
    }

}
