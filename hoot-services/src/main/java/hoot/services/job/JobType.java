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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.job;

/*
 * Order is important here, only add new enums to the end of the list
 */

public enum JobType {
    IMPORT,
    EXPORT,
    CONFLATE,
    CLIP,
    ATTRIBUTES,
    BASEMAP,
    DELETE,
    UNKNOWN,
    DERIVE_CHANGESET,
    UPLOAD_CHANGESET,
    BULK_ADD,
    BULK_REPLACE,
    BULK_DIFFERENTIAL,
    SYNC_WAIT;

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
