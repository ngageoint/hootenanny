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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "LocalJobRunner.h"

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/InputSplit.h>
#include <pp/Strings.h>
#include <pp/io/FileInputFormat.h>
#include <pp/io/LineRecordReader.h>
#include <pp/io/LineRecordWriter.h>
#include <pp/io/RecordReaderProvider.h>
#include <pp/io/RecordWriterProvider.h>
#include <pp/mapreduce/NullMapper.h>
#include <pp/mapreduce/NullReducer.h>

// Standard
#include <sstream>

using namespace std;


uint qHash(const std::string& str)
{
  QByteArray bytes(str.data(), str.size());
  return qHash(bytes);
}

// When the emit methods are there creator loses its mind and doesn't do markup. Bleh.
#define STUPID_CREATOR

namespace pp
{

class LocalMapContext : public HadoopPipes::MapContext, public RecordReaderProvider
{
public:
  LocalMapContext(Configuration* conf, boost::shared_ptr<HadoopPipes::RecordReader> reader,
    LocalJobRunner::ShuffleMap& shuffle, const InputSplit& is) :
    _conf(conf),
    _is(is),
    _reader(reader),
    _shuffle(shuffle)
  {
    _isStr = _writeSplit(_is);
  }

  virtual ~LocalMapContext() {}

#ifdef STUPID_CREATOR
  virtual void emit(const std::string& key, const std::string& value)
  {
    QByteArray keyBytes(key.data(), key.size());
    QByteArray valueBytes(value.data(), value.size());
    _shuffle[keyBytes].push_back(valueBytes);
  }
#else
# error "Please define STUPID_CREATOR"
#endif

  virtual const std::string& getInputKey() { return _key; }

  virtual const std::string& getInputKeyClass() { return _empty; }

  virtual const string& getInputSplit() { return _isStr; }

  virtual const std::string& getInputValue() { return _value; }

  virtual const std::string& getInputValueClass() { return _empty; }

  virtual const HadoopPipes::JobConf* getJobConf() { return _conf; }

  virtual HadoopPipes::RecordReader* getRecordReader() const { return _reader.get(); }

  virtual bool nextValue() { return _reader->next(_key, _value); }

  virtual void progress() {}

  virtual void setStatus(const std::string&) {}

  virtual Counter*
    getCounter(const std::string&, const std::string&) { return NULL; }

  virtual void incrementCounter(const Counter*, uint64_t) {}

private:
  Configuration* _conf;
  const InputSplit& _is;
  string _empty;
  string _isStr;
  string _key;
  boost::shared_ptr<HadoopPipes::RecordReader> _reader;
  LocalJobRunner::ShuffleMap& _shuffle;
  string _value;

  string _writeSplit(const InputSplit& is)
  {
    stringstream ss(stringstream::out);
    DataOutputStream dos(ss);

    // empty string for the plugin list, not really meaningful in this context.
    dos.writeUTF(QString());
    dos.writeUTF(QString::fromStdString(is.getClassName()));

    string fields;
    is.writeFields(fields);
    dos.writeInt(fields.size());
    dos.write(fields.data(), fields.size());
    return ss.str();
  }

};

class LocalReduceContext : public HadoopPipes::ReduceContext, public RecordWriterProvider
{
public:
  LocalReduceContext(Configuration* conf, boost::shared_ptr<HadoopPipes::RecordWriter> writer) :
    _conf(conf),
    _writer(writer)
  {
  }

  virtual ~LocalReduceContext() {}

#ifdef STUPID_CREATOR
  virtual void emit(const std::string& key, const std::string& value)
  {
    _writer->emit(key, value);
  }
#endif

  virtual const std::string& getInputKey() { return _key; }

  virtual const std::string& getInputKeyClass() { return _empty; }

  virtual const std::string& getInputValue() { return _value; }

  virtual const std::string& getInputValueClass() { return _empty; }

  virtual const HadoopPipes::JobConf* getJobConf() { return _conf; }

  virtual HadoopPipes::RecordWriter* getRecordWriter() const { return _writer.get(); }

  virtual bool nextValue()
  {
    if (_it == _values->end())
    {
      return false;
    }
    else
    {
      _value.resize(_it->size());
      memcpy((void*)_value.data(), _it->data(), _it->size());
      ++_it;
      return true;
    }
  }

  void setValues(const string& key, const vector<QByteArray>* values)
  {
    _key = key;
    _values = values;
    _it = _values->begin();
  }

  virtual void progress() {}

  virtual void setStatus(const std::string&) {}

  virtual Counter*
    getCounter(const std::string&, const std::string&) { return NULL; }

  virtual void incrementCounter(const Counter*, uint64_t) {}

private:
  Configuration* _conf;
  string _empty;
  string _key;
  boost::shared_ptr<HadoopPipes::RecordWriter> _writer;
  vector<QByteArray>::const_iterator _it;
  string _value;
  const vector<QByteArray>* _values;
};


LocalJobRunner::LocalJobRunner(const Configuration& conf) : _conf(conf)
{
  _reducerCount = _conf.getInt("mapred.reduce.tasks", 4);
  // make sure the value is set.
  _conf.setInt("mapred.reduce.tasks", _reducerCount);
}

void LocalJobRunner::_dumpShuffle()
{
  for (ShuffleMap::const_iterator it = _shuffle.begin(); it != _shuffle.end(); ++it)
  {
    const QByteArray& key = it.key();
    QByteArray keyHex = key.toHex();
    cout << "key: " << keyHex.constData() << endl;
    const vector<QByteArray>& values = it.value();
    for (size_t i = 0; i < values.size(); i++)
    {
      QByteArray valueBytes(values[i].data(), values[i].size());
      QByteArray valueHex = valueBytes.toHex();
      cout << "  value: " << valueHex.constData() << endl;
    }
  }
}

void LocalJobRunner::run()
{
  // run the mapper
  _runMapper();

  //_dumpShuffle();

  if (_reducerCount != 0)
  {
    // run the reducer
    _runReducer();
  }
  else
  {
    // if there are no reducers then just write the output.
    ShuffleMap::iterator it = _shuffle.begin();
    boost::shared_ptr<pp::RecordWriter> writer;

    writer.reset(Factory::getInstance().constructObject<pp::RecordWriter>(
      _conf.get(PP_RECORD_WRITER, LineRecordWriter::className())));

    Configuration conf = _conf;
    conf.setInt("mapred.task.partition", 0);
    conf.set("mapred.work.output.dir", conf.get("mapred.output.dir"));
    LocalReduceContext context(&conf, writer);
    pp::ReduceContextConsumer* consumer = dynamic_cast<pp::ReduceContextConsumer*>(writer.get());
    if (consumer)
    {
      consumer->setReduceContext(context);
    }

    while (it != _shuffle.end())
    {
      const vector<QByteArray>& values = it.value();
      for (size_t i = 0; i < values.size(); i++)
      {
        writer->emitRecord(it.key().data(), (size_t)it.key().size(), values[i].data(),
          (size_t)values[i].size());
      }
      ++it;
    }
    writer->close();
  }
}

void LocalJobRunner::_runMapper()
{
  boost::shared_ptr<pp::InputFormat> inputFormat(Factory::getInstance().
    constructObject<pp::InputFormat>(_conf.get(PP_INPUT_FORMAT, FileInputFormat::className())));
  inputFormat->setConfiguration(_conf);

  for (int i = 0; i < inputFormat->getSplitCount(); i++)
  {
    Configuration conf = _conf;
    conf.setInt("mapred.task.partition", i);
    if (_reducerCount == 0)
    {
      conf.set("mapred.work.output.dir", conf.get("mapred.output.dir"));
    }

    // create a new mapper for each split
    boost::shared_ptr<pp::Mapper> mapper(Factory::getInstance().
      constructObject<pp::Mapper>(conf.get(PP_MAPPER, NullMapper::className())));

    boost::shared_ptr<pp::RecordReader> reader(
      Factory::getInstance().constructObject<pp::RecordReader>(
      conf.get(PP_RECORD_READER, LineRecordReader::className())));

    const InputSplit& is = inputFormat->getSplit(i);
    LocalMapContext context(&conf, reader, _shuffle, is);

    //cout << "*** Processing split: " << is.getPath() << endl;

    MapContextConsumer* mcc = dynamic_cast<MapContextConsumer*>(reader.get());
    if (mcc != NULL)
    {
      mcc->setMapContext(context);
    }

    mcc = dynamic_cast<MapContextConsumer*>(mapper.get());
    if (mcc != NULL)
    {
      mcc->setMapContext(context);
    }

    pp::RecordReader* prr = dynamic_cast<pp::RecordReader*>(reader.get());
    if (prr != NULL)
    {
      prr->initialize(is.copy(), context);
    }

    while (context.nextValue())
    {
      //cout << "key: " << context.getInputKey() << " value: " << context.getInputValue() << endl;
      mapper->map(context);
    }

    mapper->close();
  }
}

void LocalJobRunner::_runReducer()
{
  ShuffleMap::iterator it = _shuffle.begin();
  int reduceSize = _shuffle.size() / _reducerCount + 1;
  for (int i = 0; i < _reducerCount; i++)
  {
    boost::shared_ptr<pp::Reducer> reducer;
    boost::shared_ptr<pp::RecordWriter> writer;

    reducer.reset(Factory::getInstance().
      constructObject<pp::Reducer>(_conf.get(PP_REDUCER, NullReducer::className())));

    writer.reset(Factory::getInstance().constructObject<pp::RecordWriter>(
      _conf.get(PP_RECORD_WRITER, LineRecordWriter::className())));

    Configuration conf = _conf;
    conf.setInt("mapred.task.partition", i);
    conf.set("mapred.work.output.dir", conf.get("mapred.output.dir"));
    LocalReduceContext context(&conf, writer);

    ReduceContextConsumer* rcc = dynamic_cast<ReduceContextConsumer*>(reducer.get());
    if (rcc != NULL)
    {
      rcc->setReduceContext(context);
    }

    rcc = dynamic_cast<ReduceContextConsumer*>(writer.get());
    if (rcc != NULL)
    {
      rcc->setReduceContext(context);
    }

    int count = 0;
    while (it != _shuffle.end() && count < reduceSize)
    {
      string keyStr;
      keyStr.resize(it.key().size());
      memcpy((char*)keyStr.data(), it.key().data(), it.key().size());
      context.setValues(keyStr, &it.value());
      ++it;
      count++;

      reducer->reduce(context);
    }
    reducer->close();
    writer->close();
  }
}

}
