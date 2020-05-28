This tests many to one area matching. In this scenario, we want to generate a reviews for areas in the second dataset that mostly overlaps with 
the single large area in the first dataset. There is only one area in the second dataset that doesn't overlap. Since we're treating diff reviews
as matches in this test config, everything but the one non-overlapping area from the second input should drop out of the diff output.
