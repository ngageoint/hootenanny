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

import java.io.File;
import java.io.IOException;
import java.net.JarURLConnection;
import java.net.URL;
import java.util.Collection;
import java.util.Enumeration;
import java.util.LinkedList;
import java.util.List;
import java.util.TreeSet;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;


public class ClassLoaderUtil {
    static class Thief extends ClassLoader {
        Thief(ClassLoader cl) {
            super(cl);
        }

        @Override
        public Package[] getPackages() {
            return super.getPackages();
        }
    }

    public static Collection<String> getMostJars() {
        ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
        try {
            // this seems to populate more jars. Odd.
            _getChildResources("META-INF/services");
            _getChildResources("");
            _getChildResources("/");

        }
        catch (Exception e1) {
            e1.printStackTrace();
        }
        Thief t = new Thief(classLoader);
        Package[] packages = t.getPackages();
        TreeSet<String> result = new TreeSet<String>();

        for (Package p : packages) {
            Enumeration<URL> urls;
            try {
                String path = p.getName().replace(".", "/");

                urls = classLoader.getResources(path);
                while (urls.hasMoreElements()) {
                    URL resource = urls.nextElement();
                    if (resource.getProtocol().equalsIgnoreCase("jar")) {
                        JarURLConnection conn = (JarURLConnection) resource.openConnection();
                        JarFile jarFile = conn.getJarFile();
                        result.add(jarFile.getName());
                    }
                }
            }
            catch (IOException e) {
                e.printStackTrace();
            }
        }

        return result;
    }

    private static List<URL> _getChildResources(String path) throws IOException, ClassNotFoundException {
        List<URL> result = new LinkedList<URL>();

        ClassLoader classLoader = Thread.currentThread().getContextClassLoader();
        Enumeration<URL> p = classLoader.getResources(path);
        while (p.hasMoreElements()) {
            URL resource = p.nextElement();
            // System.out.println("resource: " + resource.toString());
            if (resource.getProtocol().equalsIgnoreCase("FILE")) {
                result.addAll(loadDirectory(resource.getFile()));
            }
            else if (resource.getProtocol().equalsIgnoreCase("JAR")) {
                result.addAll(_loadJar(path, resource));
            }
            else if (resource.getProtocol().equalsIgnoreCase("VFS")) {
                System.out.println("VFS");
            }
            else {
                throw new ClassNotFoundException("Unknown protocol on class resource: " + resource.toExternalForm());
            }
        }

        return result;
    }

    private static List<URL> _loadJar(String path, URL resource) throws IOException {
        JarURLConnection conn = (JarURLConnection) resource.openConnection();
        JarFile jarFile = conn.getJarFile();
        Enumeration<JarEntry> entries = jarFile.entries();
        List<URL> result = new LinkedList<URL>();

        String p = path;
        if (p.endsWith("/") == false) {
            p = p + "/";
        }

        while (entries.hasMoreElements()) {
            JarEntry entry = entries.nextElement();
            if ((!entry.getName().equals(p))
                    && (entry.getName().startsWith(p) || entry.getName().startsWith("WEB-INF/classes/" + p))) {
                URL url = new URL("jar:" + new URL("file", null, jarFile.getName() + "!/" + entry.getName()));
                result.add(url);
            }
        }

        return result;
    }

    public static List<URL> loadDirectory(String filePath) throws IOException {
        List<URL> result = new LinkedList<URL>();
        File directory = new File(filePath);
        if (!directory.isDirectory()) {
            throw new IOException("Invalid directory " + directory.getAbsolutePath());
        }

        File[] files = directory.listFiles();
        for (File file : files) {
            if (file.isDirectory()) {
                loadDirectory(file.getAbsolutePath());
            }
            else {
                result.add(new URL("file", null, file.getAbsolutePath()));
            }
        }
        return result;
    }
}
