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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
package hoot.services.wps;

import java.io.FileNotFoundException;

import javax.xml.stream.XMLStreamException;

import org.deegree.commons.tom.ows.LanguageString;
import org.deegree.geometry.Envelope;
import org.deegree.geometry.standard.DefaultEnvelope;
import org.deegree.geometry.standard.primitive.DefaultPoint;
import org.deegree.process.jaxb.java.BoundingBoxInputDefinition;
import org.deegree.process.jaxb.java.CodeType;
import org.deegree.process.jaxb.java.ComplexOutputDefinition;
import org.deegree.process.jaxb.java.LiteralInputDefinition;
import org.deegree.process.jaxb.java.LiteralOutputDefinition;
import org.deegree.services.wps.input.BoundingBoxInputImpl;
import org.deegree.services.wps.input.LiteralInputImpl;
import org.deegree.services.wps.output.ComplexOutputImpl;
import org.deegree.services.wps.output.LiteralOutputImpl;
import org.deegree.services.wps.storage.OutputStorage;

import hoot.services.geo.BoundingBox;


public class WpsUtils {
    public static LiteralInputImpl createLiteralInput(String key, String value) {
        LiteralInputDefinition lid = new LiteralInputDefinition();
        CodeType ct = new CodeType();
        ct.setValue(key);
        lid.setIdentifier(ct);
        LiteralInputImpl lii = new LiteralInputImpl(lid, new LanguageString(key, null), new LanguageString(key, null),
                value, null);
        return lii;
    }

    public static LiteralInputImpl createLiteralInput(String key, String value, LiteralInputDefinition lid) {
        CodeType ct = new CodeType();
        ct.setValue(key);
        lid.setIdentifier(ct);
        LiteralInputImpl lii = new LiteralInputImpl(lid, new LanguageString(key, null), new LanguageString(key, null),
                value, null);
        return lii;
    }

    public static LiteralOutputImpl createLiteralOutput(String key) {
        LiteralOutputDefinition lod = new LiteralOutputDefinition();
        CodeType ct = new CodeType();
        ct.setValue(key);
        lod.setIdentifier(ct);
        LiteralOutputImpl loi = new LiteralOutputImpl(lod, null, true);
        return loi;
    }

    public static LiteralOutputImpl createLiteralOutput(String key, LiteralOutputDefinition lod) {
        CodeType ct = new CodeType();
        ct.setValue(key);
        lod.setIdentifier(ct);
        LiteralOutputImpl loi = new LiteralOutputImpl(lod, null, true);
        return loi;
    }

    public static ComplexOutputImpl createComplexOutput(String key, ComplexOutputDefinition cod)
            throws FileNotFoundException, XMLStreamException {
        CodeType ct = new CodeType();
        ct.setValue(key);
        cod.setIdentifier(ct);
        OutputStorage output = null; // I don't know what to do with this...
        ComplexOutputImpl coi = new ComplexOutputImpl(cod, output, true, cod.getDefaultFormat().getMimeType(), null,
                cod.getDefaultFormat().getEncoding());
        return coi;
    }

    public static BoundingBoxInputImpl createBoundingBoxInput(String key, BoundingBox bounds) {
        BoundingBoxInputDefinition bid = new BoundingBoxInputDefinition();
        CodeType ct = new CodeType();
        ct.setValue(key);
        bid.setIdentifier(ct);
        DefaultPoint min = new DefaultPoint(null, null, null, new double[] { bounds.getMinLon(), bounds.getMinLat() });
        DefaultPoint max = new DefaultPoint(null, null, null, new double[] { bounds.getMaxLon(), bounds.getMaxLat() });
        Envelope env = new DefaultEnvelope(min, max);
        BoundingBoxInputImpl lii = new BoundingBoxInputImpl(bid, new LanguageString(key, null),
                new LanguageString(key, null), env);
        return lii;
    }

    public static BoundingBoxInputImpl createBoundingBoxInput(String key, BoundingBox bounds,
            BoundingBoxInputDefinition bid) {
        CodeType ct = new CodeType();
        ct.setValue(key);
        bid.setIdentifier(ct);
        DefaultPoint min = new DefaultPoint(null, null, null, new double[] { bounds.getMinLon(), bounds.getMinLat() });
        DefaultPoint max = new DefaultPoint(null, null, null, new double[] { bounds.getMaxLon(), bounds.getMaxLat() });
        Envelope env = new DefaultEnvelope(min, max);
        BoundingBoxInputImpl lii = new BoundingBoxInputImpl(bid, new LanguageString(key, null),
                new LanguageString(key, null), env);
        return lii;
    }
}
