/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
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
