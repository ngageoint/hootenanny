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
package hoot.services.utils;

import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;

import javax.servlet.ServletInputStream;


/**
 * Used for mocking POST request input with Mockito
 */
public class MockServletInputStream extends ServletInputStream {
    private Reader inputReader;
    private StringBuilder charactersRead = new StringBuilder();

    @Override
    public int read() throws IOException {
        int read = inputReader.read();
        if (read >= 0) {
            charactersRead.append((char) read);
        }
        return read;
    }

    /**
     * If you use this constructor, the object will get it's input from the
     * string.
     * 
     * @param inputString
     */
    public MockServletInputStream(String inputString) {
        inputReader = new StringReader(inputString);
    }

    /**
     * This constructor causes the input stream to read from the indicated
     * reader.
     * 
     * @param inputReader
     */
    public MockServletInputStream(Reader inputReader) {
        this.inputReader = inputReader;
    }

    /**
     * Return the characters that have been read by this input stream (so far).
     */
    public String getInput() {
        return charactersRead.toString();
    }
}
