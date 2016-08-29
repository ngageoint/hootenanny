/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

template <class T, class U>
std::ostream& operator<<(std::ostream& o, const std::pair<U, T>& p)
{
  o << "(" << p.first << ", " << p.second << ")";
  return o;
}


template <class T>
std::ostream& operator<<(std::ostream & o, const std::vector<T> & vec)
{
  int n = vec.size();
  o << "[" << n << "]{";
  for (int i = 0; i < n; ++i)
  {
    o << vec[i];
    if (i != n - 1)
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

template <class T>
std::ostream& operator<<(std::ostream & o, const std::deque<T> & vec)
{
  int n = vec.size();
  o << "[" << n << "]{";
  for (int i = 0; i < n; ++i)
  {
    o << vec[i];
    if (i != n - 1)
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

template<class T>
std::ostream& operator<<(std::ostream& o, const std::list<T>& l)
{
  typedef typename std::list<T>::const_iterator const_iterator;
  int n = l.size();
  o << "[" << n << "]{";
  int i = 0;
  for (const_iterator it = l.begin(); it != l.end(); ++it)
  {
    o << *it;
    if (i != n - 1)
    {
      o << ", ";
    }
    i++;
  }
  o << "}";
  return o;
}

template <class K, class V>
std::ostream & operator << (std::ostream & o, const std::map<K, V> & map)
{
  int n = map.size();
  o << "[" << n << "]{";

  for (typename std::map<K, V>::const_iterator it = map.begin(); it != map.end();)
  {
    o << "(" << it->first << ", " << it->second << ")";
    it++;
    if (it != map.end())
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

template <class K, class V>
std::ostream & operator << (std::ostream & o, const std::multimap<K, V> & map)
{
  int n = map.size();
  o << "[" << n << "]{";

  for (typename std::multimap<K, V>::const_iterator it = map.begin(); it != map.end();)
  {
    o << "(" << it->first << ", " << it->second << ")";
    it++;
    if (it != map.end())
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

template <class K, class V>
std::ostream & operator << (std::ostream & o, const HashMap<K, V> & map)
{
  int n = map.size();
  o << "[" << n << "]{";

  for (typename HashMap<K, V>::const_iterator it = map.begin(); it != map.end();)
  {
    o << "(" << it->first << ", " << it->second << ")";
    it++;
    if (it != map.end())
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

#ifdef QT_CORE_LIB

inline std::ostream& operator << (std::ostream& o, const QString& str)
{
  o << str.toUtf8().data();
  return o;
}

inline std::ostream& operator << (std::ostream& o, const QVariant& v)
{
  o << v.toString();
  return o;
}

template <class K, class V>
std::ostream & operator << (std::ostream& o, const QMap<K, V>& map)
{
  int n = map.size();
  o << "[" << n << "]{";

  for (typename QMap<K, V>::const_iterator it = map.constBegin(); it != map.constEnd();)
  {
    o << "(" << it.key() << ", " << it.value() << ")";
    it++;
    if (it != map.end())
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

template <class K, class V>
std::ostream & operator << (std::ostream& o, const QHash<K, V>& map)
{
  int n = map.size();
  o << "[" << n << "]{";

  for (typename QHash<K, V>::const_iterator it = map.constBegin(); it != map.constEnd();)
  {
    o << "(" << it.key() << ", " << it.value() << ")";
    it++;
    if (it != map.end())
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

template <class T>
std::ostream & operator << (std::ostream& o, const QVector<T>& v)
{
  int n = v.size();
  o << "[" << n << "]{";
  for (int i = 0; i < n; i++)
  {
    o << v.at(i);
    if (i != n - 1)
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

inline std::ostream & operator << (std::ostream& o, const QHash<QString, QString>& map)
{
  int n = map.size();
  o << "[" << n << "]{";

  for (QHash<QString, QString>::const_iterator it = map.constBegin(); it != map.constEnd();)
  {
    o << "(" << it.key() << ", " << it.value() << ")";
    it++;
    if (it != map.end())
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

template<class T>
std::ostream& operator<<(std::ostream& o, const QList<T>& l)
{
  int n = l.size();
  o << "[" << n << "]{";
  for (int i = 0; i < n; i++)
  {
    o << l.at(i);
    if (i != n - 1)
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

template<class T>
std::ostream& operator<<(std::ostream& o, const QSet<T>& l)
{
  o << "[";
  int i = 0;
  foreach (const T& t, l)
  {
    if (i != 0)
    {
      o << ", ";
    }
    o << t;
    ++i;
  }
  o << "]";
  return o;
}


// If the class has a "QString toString() const" method then automagically call it.
#define HAS_MEM_FUNC(func, name)                                        \
    template<typename T, typename Sign>                                 \
    struct name {                                                       \
        typedef char yes[1];                                            \
        typedef char no [2];                                            \
        template <typename U, U> struct type_check;                     \
        template <typename _1> static yes &chk(type_check<Sign, &_1::func> *); \
        template <typename   > static no  &chk(...);                    \
        static bool const value = sizeof(chk<T>(0)) == sizeof(yes);     \
    }

template<bool C, typename T = void>
struct enable_if {
  typedef T type;
};

template<typename T>
struct enable_if<false, T> { };

HAS_MEM_FUNC(toString, has_to_string);

template<typename T>
typename enable_if< has_to_string<T, QString(T::*)() const>::value, std::ostream& >::type
operator<<(std::ostream& o, const T& t)
{
  o << t.toString();
  return o;
}

template<typename T>
typename enable_if< has_to_string<T, std::string(T::*)() const>::value, std::ostream& >::type
operator<<(std::ostream& o, const T& t)
{
  o << t.toString();
  return o;
}

#endif

template<class T, class C>
std::ostream& operator<<(std::ostream& o, const std::set<T, C>& s)
{
  typedef typename std::set<T, C>::const_iterator const_iterator;
  int n = s.size();
  o << "[" << n << "]{";
  int i = 0;
  for (const_iterator it = s.begin(); it != s.end(); ++it)
  {
    o << *it;
    if (i != n - 1)
    {
      o << ", ";
    }
    i++;
  }
  o << "}";
  return o;
}

template <class T>
std::ostream& operator<<(std::ostream & o, const shared_ptr<T>& v)
{
  if (v == 0)
  {
    o << "null";
  }
  else
  {
    o << *v;
  }

  return o;
}

template <class T>
std::ostream& operator<<(std::ostream & o, const shared_ptr<const T>& v)
{
  if (v.get() == 0)
  {
    o << "null";
  }
  else
  {
    o << *v;
  }

  return o;
}

template<class V>
V conv(const std::string& s)
{
  std::istringstream i(s);
  V x;
  if (!(i >> x))
  {
    std::stringstream strm;
    const std::type_info &info = typeid(V);
    strm << "Bad conversion '" << s << "' to " << info.name();
    throw Tgs::Exception(strm.str());
  }
  return x;
}

template<class V>
QString toString(const V& v)
{
  std::stringstream ss;
  ss << v;
  return QString::fromStdString(ss.str());
}
