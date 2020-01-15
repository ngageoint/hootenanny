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
package hoot.services.controllers.grail;

import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


/**
 * Used to construct a changeset-derive-replacement command
 */
class DeriveChangesetReplacementCommand extends GrailCommand {

    DeriveChangesetReplacementCommand(String jobId, GrailParams params, String debugLevel, Class<?> caller) {
        super(jobId, params);

        List<String> options = new LinkedList<>();
        options.add("api.db.email=" + params.getUser().getEmail());

        List<String> bounds = Arrays.asList(params.getBounds().split(","));
        Double width = Double.valueOf(bounds.get(2)) - Double.valueOf(bounds.get(0));
        Double height = Double.valueOf(bounds.get(1)) - Double.valueOf(bounds.get(3));
        Double delta = Math.min(width, height);
        Integer alpha = adaptiveAlpha(delta);
        options.add("cookie.cutter.alpha=" + alpha);

        List<String> hootOptions = toHootOptions(options);

        Map<String, Object> substitutionMap = new HashMap<>();
        substitutionMap.put("INPUT1", params.getInput1());
        substitutionMap.put("INPUT2", params.getInput2());
        substitutionMap.put("BOUNDS", params.getBounds());
        substitutionMap.put("OSC_FILE", params.getOutput());
        substitutionMap.put("HOOT_OPTIONS", hootOptions);
        substitutionMap.put("DEBUG_LEVEL", debugLevel);

        String command = "hoot changeset-derive-replacement --${DEBUG_LEVEL} -C DeriveChangeset.conf ${HOOT_OPTIONS} ${INPUT1} ${INPUT2} ${BOUNDS} ${OSC_FILE} --stats";

        super.configureCommand(command, substitutionMap, caller);
    }

    public static final Pattern pattern = Pattern.compile("[+\\-]?(?:0|[1-9]\\d*)(?:\\.\\d*)?(?:[eE]([+\\-])?(\\d+))?");
    /*
     * returns the power base ten portion of scientific notation for a number
     * ie. .001 will return -3, while 10 will return 1
     */
    public static int exponent(Double d) {
        int exp = 0;
        String scinote = String.format("%.3E", d);
        Matcher matcher = pattern.matcher(scinote);
        if (matcher.find()) {
            exp = Integer.parseInt(scinote.split("E")[1]);
        }

        return exp;
    }

    /*
     * returns an adaptive alpha shape based on exent of dataset
     * (width or height) in decimal degrees
     */
    public static int adaptiveAlpha(Double d) {
        int exp = 4 + exponent(d); //follows table here https://en.wikipedia.org/wiki/Decimal_degrees
        //to provide an alpha distance one order of magnitude less than the extent of the dataset

        return (int)Math.pow(10, exp);
    }
}
