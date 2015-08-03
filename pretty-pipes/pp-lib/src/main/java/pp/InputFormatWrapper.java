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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
package pp;

import java.io.IOException;
import java.util.Iterator;
import java.util.Map;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.mapred.FileInputFormat;
import org.apache.hadoop.mapred.InputFormat;
import org.apache.hadoop.mapred.InputSplit;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.RecordReader;
import org.apache.hadoop.mapred.Reporter;

public class InputFormatWrapper implements InputFormat<FloatWritable, NullWritable>
{
  static
  {
    System.out.println("Initializing...");
    System.out.flush();
    HootHadoop.init();
  }

  static class PipesDummyRecordReader implements RecordReader<FloatWritable, NullWritable>
  {
    float progress = 0.0f;

    public PipesDummyRecordReader(Configuration job, InputSplit split) throws IOException
    {
    }

    public FloatWritable createKey()
    {
      return null;
    }

    public NullWritable createValue()
    {
      return null;
    }

    public synchronized void close() throws IOException
    {
    }

    public synchronized long getPos() throws IOException
    {
      return 0;
    }

    public float getProgress()
    {
      return progress;
    }

    public synchronized boolean next(FloatWritable key, NullWritable value) throws IOException
    {
      progress = key.get();
      return true;
    }
  }

  @Override
  public RecordReader<FloatWritable, NullWritable> getRecordReader(InputSplit genericSplit, JobConf job, Reporter arg2)
    throws IOException
  {
    return new PipesDummyRecordReader(job, genericSplit);
  }

  @Override
  public InputSplit[] getSplits(JobConf job, int numSplits) throws IOException
  {
    String className = job.get("pretty.pipes.input.format");

    pp.Configuration conf = new pp.Configuration();
    
    Iterator<Map.Entry<String, String>> it = job.iterator();
    while (it.hasNext())
    {
      Map.Entry<String, String> e = it.next();
      conf.set(e.getKey(), e.getValue());
    }
    String pluginStr = job.get("pretty.pipes.plugins");
    System.out.println("plugins: " + pluginStr);
    System.out.flush();
    String[] plugins = new String[0];
    if (pluginStr.isEmpty() == false)
    {
      plugins = pluginStr.split(",");
    }
    for (int i = 0; i < plugins.length; i++)
    {
      Plugins.loadLibrary(plugins[i]);
    }

    System.out.println("Creating input format: " + className);
    pp.InputFormat pif = pp.InputFormat.create(className);
   
    pif.setConfiguration(conf);

    InputSplit[] result = new InputSplit[pif.getSplitCount()];
    for (int i = 0; i < pif.getSplitCount(); i++)
    {
      pp.InputSplit pis = pif.getSplit(i);
      result[i] = new InputSplitWrapper(pis);
    }

    return result;
  }

}
