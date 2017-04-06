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
 * @copyright Copyright (C) 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/visitors/SplitNameVisitor.h>

// Qt
#include <QDebug>
#include <QDir>

#include "../TestUtils.h"

namespace hoot
{

class SplitNameVisitorTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(SplitNameVisitorTest);
  CPPUNIT_TEST(basicTest);
  CPPUNIT_TEST(bigTest);
  CPPUNIT_TEST_SUITE_END();

public:

  void basicTest()
  {
    SplitNameVisitor v;
    // make the max size smaller so the test is a little easier to read.
    v.setMaxSize(20);

    boost::shared_ptr<Node> n(new Node(Status::Unknown1, -1, Coordinate(0, 0), 5));
    Tags& t = n->getTags();

    t.clear();
    t["name"] = "foo";
    t["alt_name"] = "now is the time for all good men to come to the aid of their country.;bar";
    v.visit(n);
    HOOT_STR_EQUALS("name:0 = bar\n"
                    "alt_name = now is the time f...\n"
                    "name = foo\n",
                    t.toString());

    t.clear();
    t["name"] = "foo;bar;baz;bam bam";
    t["alt_name"] = "bar;the quick brown fox jumped over the lazy dog's back.;now is the time for all good men to come to the aid of their country.;fiddle faddle";
    v.visit(n);
    HOOT_STR_EQUALS("name:0 = the quick brown f...\n"
                    "name:1 = now is the time f...\n"
                    "name:2 = fiddle faddle\n"
                    "alt_name = bar\n"
                    "name = foo;bar;baz;bam bam\n",
                    t.toString());

    t.clear();
    t["name"] = "Riu Valira del Nord";
    t["alt_name"] = QString::fromUtf8("Riu Valira d'Ordino;Riu Valira del Nord;Riu Valira del Nort;Riu Valira d’Ordino;Valira del Nord;Valira del Norte");
    v.visit(n);
    LOG_VAR(TestUtils::toQuotedString(t.toString()));

  }

  void bigTest()
  {
    SplitNameVisitor v;
    // make the max size smaller so the test is a little easier to read.
    v.setMaxSize(255);

    boost::shared_ptr<Node> n(new Node(Status::Unknown1, -1, Coordinate(0, 0), 5));
    Tags& t = n->getTags();

    t.clear();
    t["name"] = "Saint Vincent and the Grenadines";
    t["alt_name"] = QString::fromUtf8("Agios Bikentios kai Grenadines;Colony of Saint Vincent;Mutagatifu Visenti na Gerenadine;Naomh Bhionsant agus Eileanan Greanadach;Noo Winsen as ny Grenadeenyn;Orileede Fisennti ati Genadina;Orílẹ́ède Fisẹnnti ati Genadina;S:t Vincent och Grenadinerna;Saint Vincent;Saint Vincent Colony;Saint Vincent a Grenadiny;Saint Vincent a'r Grenadines;Saint Vincent agus na Grenadines;Saint Vincent an d'Grenadinen;Saint Vincent and the Grenadines;Saint Vincent ati awon Grenadini;Saint Vincent a’r Grenadines;Saint Vincent dan Grenadines;Saint Vincent dan the Grenadines;Saint Vincent e Grenadine;Saint Vincent e Grenadines;Saint Vincent e Grenadinet;Saint Vincent e las Grenadinas;Saint Vincent en de Grenadines;Saint Vincent en die Grenadine;Saint Vincent es Grenadines;Saint Vincent es a Grenadine-szigetek;Saint Vincent eta Grenadinak;Saint Vincent i Grenadyny;Saint Vincent i les Grenadines;Saint Vincent in Grenadine;Saint Vincent ja Grenadiinid;Saint Vincent ja Grenadiinit;Saint Vincent ja Grenadine;Saint Vincent ja Grenadines;Saint Vincent jeung Grenadin;Saint Vincent jeung Grénadin;Saint Vincent kple Grenadine nutome;Saint Vincent lan Grenadines;Saint Vincent na Grenadini;Saint Vincent ne Grenadines;Saint Vincent och Grenadinerna;Saint Vincent og Grenadinane;Saint Vincent og Grenadinene;Saint Vincent og Grenadinerne;Saint Vincent si Grenadines;Saint Vincent u giraven Grenadin;Saint Vincent ug ang Grenadines;Saint Vincent va Grenadines;Saint Vincent ve Grenadines;Saint Vincent ve Grenadinler;Saint Vincent và Grenadines;Saint Vincent àti àwọn Grẹnadínì;Saint Vincent és Grenadines;Saint Vincent és a Grenadine-szigetek;Saint Vincent û giravên Grenadîn;Saint Vincent şi Grenadines;Saint Vincent; Grenadines;Saint Vinsent og Grenadineyjar;Saint Vinsent og Grenadinoyggjar;Saint Vinsent og Grenadíneyjar;Saint Winsent wan Grenadinak;Saint-Vincent-et-les Grenadines;Saint-Vincent-et-les-Grenadines;Saluda-Vinsenteaen e Grenadineaens;Saluda-Vinsenteän e Grenadineäns;San Binsan Da Girnadin;San Uinseann agus na Greanaidini;San Uinseann agus na Greanáidíní;San Vicente at ang Kagranadinahan;San Vicente e Granadinas;San Vicente e Granadinas - Saint Vincent and the Grenadines;San Vicente ihuan in Granadinas;San Vicente ken dagiti Grenadito;San Vicente y Les Granadines;San Vicente y las Granadinas;San Vicente īhuān in Granadinas;San Vicenti i las Granainas;San Vincens e Grenadin-e;San Vincente asin an Granadinas;San Vincente ngan an Grenadines;San Vincenz e Grenadin;San Vincenzo e Grenadinn-e;San Vinçenzo e Grenadinn-e;Sancte Vincente e le Grenadinas;Sanctus Vincentius et Granatinae;Sankta Vincento kaj la Grenadinoj;Sankte Vinsent e li Grenadines;Sankti Vinsent og Grenadineyjar;Sankti Vinsent og Grenadíneyjar;Sant Vicent y as Granadinas;Sant Vincenc e las Grenadinas;Sant Vincenç e las Grenadinas;Sant Visant hag an Inizi Granadinas;Sant Visant hag ar Grenadinez;Sant-Vincent-et-les Grenadenes;Santa Vincent dan Grenadin;Santa Vincent e Grenadini;Santavisenti na Grenadini;Santu Vicent mpe Granadines;Santu Vicent mpé Granadines;Santu vesa ne Ngelenadine;Sao Vicente e Granadinas;Sao Vincente e Granadinas;Saunt Vincent an the Grenadines;See Weesaa e Garnadiin;Seini Viniseni mo Kulenatini;Sejnt Vinsent i Grenadini;Sejnt Vinsunt i Grenadini;Sen Vensan ak Grenadin;Sen Vinsent ha'n Ynysow Grenadinek;Sen-Vensaean na aGrenadini;Sent Vinsent i Grenadini;Sent Vinsent i Grenadinite;Sent Vinsent uonna Grenadinnar;Sent Vinsent va Grenadinlar;Sent Vinsent ve Grenadinler;Sent Vinsentas ir Grenadinai;Sent Vinsunt i Grenadini;Sent Visent-Grenadinner;Sent-Vincento kaj la Grenadinoj;Sent-Vinsent aemae Grenadintae;Sent-Vinsent ba Grenadin;Sent-Vinsent i Grehnadziny;Sent-Vinsent i Grenadini;Sent-Vinsent i Grenadiny;Sent-Vinsent i Grenadyny;Sent-Vinsent və Qrenadina;Senti Vinsenti ne Gurendadiini;Sentvinsenta un Grenadinas;Sentvinsenta un Grenadīnas;Senvinsentin boln Grenadin Orn;Sevensa na Gerenadine;Seynt Vinsent və Qrenada;Sfantul Vicentiu si Grenadine;Sfantul Vincentiu si Grenadine;Sfântul Vicențiu și Grenadine;Sfântul Vincenţiu şi Grenadine;Shen Vincenti dhe Grenadinet;Shën Vincenti dhe Grenadinet;Sint Vincent en de Grenadinen;St. Vincent an Grenadiinen;St. Vincent en de Grenadines;St. Vincent en die Grenadines;St. Vincent og Grenadinane;St. Vincent og Grenadinene;St. Vincent og Grenadinerne;St. Vincent un de Grenadinen;St. Vincent und die Grenadinen;State of Saint Vincent;Sv. Vincent a Grenadiny;Sv. Vincent i Grenadini;Svaety Vincent;Svaety Vincent a Grenadiny;Svaty Vincenc a Grenadiny;Svaty Vincent a Grenadiny;Svatý Vincenc a Grenadiny;Svatý Vincent a Grenadiny;Sventasis Vincentas ir Grenadinai;Sveti Vincent i Grenadini;Sveti Vincent in Grenadini;Sveti Vinsent i Grenadine;Sveti Vinsent i Grenadini;Svätý Vincent;Svätý Vincent a Grenadíny;Sántu vesá mpé Gelenadinɛ;São Vicente e Granadinas;São Vincente e Granadinas;Sên-Vensäan na âGrenadîni;Vinisɛn-Senu-ni-Grenadini;Xan Vin-xen va Gre-na-din;Xan Vin-xen và Grê-na-din;Xanh Vin-xang va Grenad (Saint Vincent va Grenadines);Xanh Vin-xăng và Grênad (Saint Vincent và Grenadines);ceyin vincant marrum kirenatains;ceyint. vincent kirenetins;i-Saint Vincent ne-Grenadines;sant fnsnt wghrnadyn;sant fnsnt wjzr ghrynadyn;sant fnsynt waljrynadynz;sant fynsynt w aljrynadynz;santa vinsenta aura grenada'insa;seinteu binsenteu geulenadin;seinteubinsenteu geulenadin;seinteubinsenteugeulenadin;senda vinsenda grenadinesca;sent vinsent mariyu grenadins;sent vinsent mattu grenadins;sent vinsent mattu grenedains;senta bhinasenta baro grenada'ina;senta bhinasenta ebam di grenadis;senta bhinasenta o dya grenadinasa;senta bhinasenta o grenada'ina dbipapunja;senta bhinasenta o grinedansa;senta bhinsenta ra grenadinsa;senta vhinsenta ani grenada'insa;senta vhinsenta ani grenedinsa;senta vinasainta ate grenadinaza;senta vinasenta enda grenada'insa;senta vinsenta enda dha grenedinsa;sentobinsento・gurenadin;sheng wen sen te he ge lin na ding si;snt wynsnt w grnadyn;snt wynsnt whgrndynym;synt wynsnt w grnadynz;syynt wynsnt whgrndyns;Šventasis Vincentas ir Grenadinai;Άγιος Βικέντιος και Γρεναδίνες;Свети Винсент и Гренадине;Свети Винсент и Гренадини;Свети Винцент и Гренадини;Сейнт Винсент и Гренадини;Сейнт Винсънт и Гренадини;Сенвинсентин болн Гренадин Орн;Сент Винсент и Гренадини;Сент Винсент и Гренадините;Сент Винсент уонна Гренадиннар;Сент Винсънт и Гренадини;Сент-Винсент æмæ Гренадинтæ;Сент-Винсент ба Гренадин;Сент-Винсент және Гренадиндер;Сент-Винсент и Гренадины;Сент-Вінсент і Гренадини;Сент-Вінсент і Грэнадзіны;Сент-Вінсент і Ґренадіни;Сэнт-Вінсэнт і Грэнадыны;Սենթ Վինսենթ և Գրենադիններ;Սենտ Վիսենտ-Գրենադիններ;סיינט וינסנט והגרנדינס;סנט וינסנט ואיי גרנאדין;סנט וינסנט והגרנדינים;سانت فنسنت وجزر غرينادين;سانت فنسنت وغرنادين;سانت فنسينت والجرينادينز;سانت فينسينت و الجرينادينز;سنت وینسنت و گرنادین;سنت وینسنت و گرنادین‌ها;سینت وینسنت و گرنادینز;سینٹ ونسنٹ اور گرینیڈائنز;سینٹ ونسٹنٹ اینڈ دی گریناڈانز;سینٹ وینسینٹ و گریناڈائنز;سېنت ۋىنسېنت ۋە گرېنادىن ئاراللىرى;سېنټ وېنسېنټ او ګرېناډينز;سەینت ڤینسەنت و گرینادینز;ސަންތި ވިސެންޓޭ އަދި ގުރެނާޑީން;सन्त विन्सेण्ट और ग्रेनाडाइन्स;सेंट विनसेंट एंड ग्रेनाडाइन्स;सेंट व्हिन्सेंट आणि ग्रेनडाइन्स;सेंट व्हिन्सेंट आणि ग्रेनेडीन्स;सेंड विन्सेन्ड ग्रेनदिनेश्च;सेन्ट भिन्सेन्ट र ग्रेनाडिन्स;সেন্ট ভিনসেন্ট ও গ্রেনাডাইন দ্বীপপুঞ্জ;সেন্ট ভিনসেন্ট ও দ্যা গ্রেনাডিনস;সেন্ট ভিনসেন্ট বারো গ্রেনাডাইন;ਸੇਂਟ ਵਿਨਸੈਂਟ ਅਤੇ ਗ੍ਰੇਨਾਡੀਨਜ਼;સેંટ વિંસેંટ એંડ ધ ગ્રેનેડિન્સ;ସେଣ୍ଟ ଭିନସେଣ୍ଟ ଏବଂ ଦି ଗ୍ରେନାଡିସ୍;ସେଣ୍ଟ ଭିନସେଣ୍ଟ ଓ ଗ୍ରିନେଡାନ୍ସ;செயிண்ட். வின்செண்ட் கிரெனேடின்ஸ்;செயின் வின்சன்ட் மற்றும் கிரெனடைன்ஸ்;సెంట్ విన్సెంట్ మరియు గ్రెనడీన్స్;ಸೇಂಟ್ ವಿನ್ಸೆಂಟ್ ಮತ್ತು ಗ್ರೆನಡೀನ್ಸ್;ಸೇಂಟ್ ವಿನ್ಸೆಂಟ್ ಮತ್ತು ಗ್ರೆನೆಡೈನ್ಸ್;സെന്‍റ് വിന്‍സന്‍റും ഗ്രനഡൈന്‍സും;സെയ്ന്റ് വിൻസന്റ് ഗ്രനഡീൻസ്;ประเทศเซนต์วินเซนต์และเกรนาดีนส์;เซนต์วินเซนต์และเกรนาดีนส์;སེན་ཊི་བིན་སེན་ཊི་དང་གི་རི་ན་ཌིན་སི།;སེནྚ། ཝིན་སན། དང༌། གྷིརིན་ཌིན།;စိန့်ဗင်းဆင့်နှင့်ဂရိနေဒိုင်နိုင်ငံ;სენტ-ვინსენტი და გრენადინები;ቅዱስ ቪንሴንት እና ግሬናዲንስ;セントビンセント・グレナディーン;セントビンセント・グレナディーン諸島;圣文森特和格林纳丁斯;세인트 빈센트 그레나딘;세인트빈센트 그레나딘;세인트빈센트그레나딘");
    v.visit(n);
    HOOT_STR_EQUALS(QString::fromUtf8("name:16 = Sint Vincent en de Grenadinen;St. Vincent an Grenadiinen;St. Vincent en de Grenadines;St. Vincent en die Grenadines;St. Vincent og Grenadinane;St. Vincent og Grenadinene;St. Vincent og Grenadinerne;St. Vincent un de Grenadinen\n"
                    "name:31 = སེན་ཊི་བིན་སེན་ཊི་དང་གི་རི་ན་ཌིན་སི།;སེནྚ། ཝིན་སན། དང༌། གྷིརིན་ཌིན།;စိန့်ဗင်းဆင့်နှင့်ဂရိနေဒိုင်နိုင်ငံ;სენტ-ვინსენტი და გრენადინები;ቅዱስ ቪንሴንት እና ግሬናዲንስ;セントビンセント・グレナディーン;セントビンセント・グレナディーン諸島;圣文森特和格林纳丁斯;세인트 빈센트 그레나딘;세인트빈센트 그레나딘;세인트빈센트그레나딘\n"
                    "name:17 = St. Vincent und die Grenadinen;State of Saint Vincent;Sv. Vincent a Grenadiny;Sv. Vincent i Grenadini;Svaety Vincent;Svaety Vincent a Grenadiny;Svaty Vincenc a Grenadiny;Svaty Vincent a Grenadiny;Svatý Vincenc a Grenadiny;Svatý Vincent a Grenadiny\n"
                    "name:18 = Sventasis Vincentas ir Grenadinai;Sveti Vincent i Grenadini;Sveti Vincent in Grenadini;Sveti Vinsent i Grenadine;Sveti Vinsent i Grenadini;Svätý Vincent;Svätý Vincent a Grenadíny;Sántu vesá mpé Gelenadinɛ;São Vicente e Granadinas;São Vincente e Granadinas\n"
                    "name:19 = Sên-Vensäan na âGrenadîni;Vinisɛn-Senu-ni-Grenadini;Xan Vin-xen va Gre-na-din;Xan Vin-xen và Grê-na-din;Xanh Vin-xang va Grenad (Saint Vincent va Grenadines);Xanh Vin-xăng và Grênad (Saint Vincent và Grenadines);ceyin vincant marrum kirenatains\n"
                    "name:20 = ceyint. vincent kirenetins;i-Saint Vincent ne-Grenadines;sant fnsnt wghrnadyn;sant fnsnt wjzr ghrynadyn;sant fnsynt waljrynadynz;sant fynsynt w aljrynadynz;santa vinsenta aura grenada'insa;seinteu binsenteu geulenadin;seinteubinsenteu geulenadin\n"
                    "name = Saint Vincent and the Grenadines\n"
                    "name:21 = seinteubinsenteugeulenadin;senda vinsenda grenadinesca;sent vinsent mariyu grenadins;sent vinsent mattu grenadins;sent vinsent mattu grenedains;senta bhinasenta baro grenada'ina;senta bhinasenta ebam di grenadis;senta bhinasenta o dya grenadinasa\n"
                    "name:22 = senta bhinasenta o grenada'ina dbipapunja;senta bhinasenta o grinedansa;senta bhinsenta ra grenadinsa;senta vhinsenta ani grenada'insa;senta vhinsenta ani grenedinsa;senta vinasainta ate grenadinaza;senta vinasenta enda grenada'insa\n"
                    "name:23 = senta vinsenta enda dha grenedinsa;sentobinsento・gurenadin;sheng wen sen te he ge lin na ding si;snt wynsnt w grnadyn;snt wynsnt whgrndynym;synt wynsnt w grnadynz;syynt wynsnt whgrndyns;Šventasis Vincentas ir Grenadinai;Άγιος Βικέντιος και Γρεναδίνες\n"
                    "alt_name = Agios Bikentios kai Grenadines;Colony of Saint Vincent;Mutagatifu Visenti na Gerenadine;Naomh Bhionsant agus Eileanan Greanadach;Noo Winsen as ny Grenadeenyn;Orileede Fisennti ati Genadina;Orílẹ́ède Fisẹnnti ati Genadina;S:t Vincent och Grenadinerna\n"
                    "name:24 = Свети Винсент и Гренадине;Свети Винсент и Гренадини;Свети Винцент и Гренадини;Сейнт Винсент и Гренадини;Сейнт Винсънт и Гренадини;Сенвинсентин болн Гренадин Орн;Сент Винсент и Гренадини;Сент Винсент и Гренадините;Сент Винсент уонна Гренадиннар\n"
                    "name:25 = Сент Винсънт и Гренадини;Сент-Винсент æмæ Гренадинтæ;Сент-Винсент ба Гренадин;Сент-Винсент және Гренадиндер;Сент-Винсент и Гренадины;Сент-Вінсент і Гренадини;Сент-Вінсент і Грэнадзіны;Сент-Вінсент і Ґренадіни;Сэнт-Вінсэнт і Грэнадыны\n"
                    "name:26 = Սենթ Վինսենթ և Գրենադիններ;Սենտ Վիսենտ-Գրենադիններ;סיינט וינסנט והגרנדינס;סנט וינסנט ואיי גרנאדין;סנט וינסנט והגרנדינים;سانت فنسنت وجزر غرينادين;سانت فنسنت وغرنادين;سانت فنسينت والجرينادينز;سانت فينسينت و الجرينادينز;سنت وینسنت و گرنادین\n"
                    "name:27 = سنت وینسنت و گرنادین‌ها;سینت وینسنت و گرنادینز;سینٹ ونسنٹ اور گرینیڈائنز;سینٹ ونسٹنٹ اینڈ دی گریناڈانز;سینٹ وینسینٹ و گریناڈائنز;سېنت ۋىنسېنت ۋە گرېنادىن ئاراللىرى;سېنټ وېنسېنټ او ګرېناډينز;سەینت ڤینسەنت و گرینادینز;ސަންތި ވިސެންޓޭ އަދި ގުރެނާޑީން\n"
                    "name:0 = Saint Vincent;Saint Vincent Colony;Saint Vincent a Grenadiny;Saint Vincent a'r Grenadines;Saint Vincent agus na Grenadines;Saint Vincent an d'Grenadinen;Saint Vincent and the Grenadines;Saint Vincent ati awon Grenadini;Saint Vincent a’r Grenadines\n"
                    "name:28 = सन्त विन्सेण्ट और ग्रेनाडाइन्स;सेंट विनसेंट एंड ग्रेनाडाइन्स;सेंट व्हिन्सेंट आणि ग्रेनडाइन्स;सेंट व्हिन्सेंट आणि ग्रेनेडीन्स;सेंड विन्सेन्ड ग्रेनदिनेश्च;सेन्ट भिन्सेन्ट र ग्रेनाडिन्स;সেন্ট ভিনসেন্ট ও গ্রেনাডাইন দ্বীপপুঞ্জ;সেন্ট ভিনসেন্ট ও দ্যা গ্রেনাডিনস\n"
                    "name:1 = Saint Vincent dan Grenadines;Saint Vincent dan the Grenadines;Saint Vincent e Grenadine;Saint Vincent e Grenadines;Saint Vincent e Grenadinet;Saint Vincent e las Grenadinas;Saint Vincent en de Grenadines;Saint Vincent en die Grenadine\n"
                    "name:29 = সেন্ট ভিনসেন্ট বারো গ্রেনাডাইন;ਸੇਂਟ ਵਿਨਸੈਂਟ ਅਤੇ ਗ੍ਰੇਨਾਡੀਨਜ਼;સેંટ વિંસેંટ એંડ ધ ગ્રેનેડિન્સ;ସେଣ୍ଟ ଭିନସେଣ୍ଟ ଏବଂ ଦି ଗ୍ରେନାଡିସ୍;ସେଣ୍ଟ ଭିନସେଣ୍ଟ ଓ ଗ୍ରିନେଡାନ୍ସ;செயிண்ட். வின்செண்ட் கிரெனேடின்ஸ்;செயின் வின்சன்ட் மற்றும் கிரெனடைன்ஸ்\n"
                    "name:2 = Saint Vincent es Grenadines;Saint Vincent es a Grenadine-szigetek;Saint Vincent eta Grenadinak;Saint Vincent i Grenadyny;Saint Vincent i les Grenadines;Saint Vincent in Grenadine;Saint Vincent ja Grenadiinid;Saint Vincent ja Grenadiinit\n"
                    "name:3 = Saint Vincent ja Grenadine;Saint Vincent ja Grenadines;Saint Vincent jeung Grenadin;Saint Vincent jeung Grénadin;Saint Vincent kple Grenadine nutome;Saint Vincent lan Grenadines;Saint Vincent na Grenadini;Saint Vincent ne Grenadines\n"
                    "name:4 = Saint Vincent och Grenadinerna;Saint Vincent og Grenadinane;Saint Vincent og Grenadinene;Saint Vincent og Grenadinerne;Saint Vincent si Grenadines;Saint Vincent u giraven Grenadin;Saint Vincent ug ang Grenadines;Saint Vincent va Grenadines\n"
                    "name:5 = Saint Vincent ve Grenadines;Saint Vincent ve Grenadinler;Saint Vincent và Grenadines;Saint Vincent àti àwọn Grẹnadínì;Saint Vincent és Grenadines;Saint Vincent és a Grenadine-szigetek;Saint Vincent û giravên Grenadîn;Saint Vincent şi Grenadines\n"
                    "name:6 = Saint Vincent; Grenadines;Saint Vinsent og Grenadineyjar;Saint Vinsent og Grenadinoyggjar;Saint Vinsent og Grenadíneyjar;Saint Winsent wan Grenadinak;Saint-Vincent-et-les Grenadines;Saint-Vincent-et-les-Grenadines;Saluda-Vinsenteaen e Grenadineaens\n"
                    "name:7 = Saluda-Vinsenteän e Grenadineäns;San Binsan Da Girnadin;San Uinseann agus na Greanaidini;San Uinseann agus na Greanáidíní;San Vicente at ang Kagranadinahan;San Vicente e Granadinas;San Vicente e Granadinas - Saint Vincent and the Grenadines\n"
                    "name:8 = San Vicente ihuan in Granadinas;San Vicente ken dagiti Grenadito;San Vicente y Les Granadines;San Vicente y las Granadinas;San Vicente īhuān in Granadinas;San Vicenti i las Granainas;San Vincens e Grenadin-e;San Vincente asin an Granadinas\n"
                    "name:9 = San Vincente ngan an Grenadines;San Vincenz e Grenadin;San Vincenzo e Grenadinn-e;San Vinçenzo e Grenadinn-e;Sancte Vincente e le Grenadinas;Sanctus Vincentius et Granatinae;Sankta Vincento kaj la Grenadinoj;Sankte Vinsent e li Grenadines\n"
                    "name:10 = Sankti Vinsent og Grenadineyjar;Sankti Vinsent og Grenadíneyjar;Sant Vicent y as Granadinas;Sant Vincenc e las Grenadinas;Sant Vincenç e las Grenadinas;Sant Visant hag an Inizi Granadinas;Sant Visant hag ar Grenadinez;Sant-Vincent-et-les Grenadenes\n"
                    "name:11 = Santa Vincent dan Grenadin;Santa Vincent e Grenadini;Santavisenti na Grenadini;Santu Vicent mpe Granadines;Santu Vicent mpé Granadines;Santu vesa ne Ngelenadine;Sao Vicente e Granadinas;Sao Vincente e Granadinas;Saunt Vincent an the Grenadines\n"
                    "name:12 = See Weesaa e Garnadiin;Seini Viniseni mo Kulenatini;Sejnt Vinsent i Grenadini;Sejnt Vinsunt i Grenadini;Sen Vensan ak Grenadin;Sen Vinsent ha'n Ynysow Grenadinek;Sen-Vensaean na aGrenadini;Sent Vinsent i Grenadini;Sent Vinsent i Grenadinite\n"
                    "name:13 = Sent Vinsent uonna Grenadinnar;Sent Vinsent va Grenadinlar;Sent Vinsent ve Grenadinler;Sent Vinsentas ir Grenadinai;Sent Vinsunt i Grenadini;Sent Visent-Grenadinner;Sent-Vincento kaj la Grenadinoj;Sent-Vinsent aemae Grenadintae;Sent-Vinsent ba Grenadin\n"
                    "name:14 = Sent-Vinsent i Grehnadziny;Sent-Vinsent i Grenadini;Sent-Vinsent i Grenadiny;Sent-Vinsent i Grenadyny;Sent-Vinsent və Qrenadina;Senti Vinsenti ne Gurendadiini;Sentvinsenta un Grenadinas;Sentvinsenta un Grenadīnas;Senvinsentin boln Grenadin Orn\n"
                    "name:15 = Sevensa na Gerenadine;Seynt Vinsent və Qrenada;Sfantul Vicentiu si Grenadine;Sfantul Vincentiu si Grenadine;Sfântul Vicențiu și Grenadine;Sfântul Vincenţiu şi Grenadine;Shen Vincenti dhe Grenadinet;Shën Vincenti dhe Grenadinet\n"
                    "name:30 = సెంట్ విన్సెంట్ మరియు గ్రెనడీన్స్;ಸೇಂಟ್ ವಿನ್ಸೆಂಟ್ ಮತ್ತು ಗ್ರೆನಡೀನ್ಸ್;ಸೇಂಟ್ ವಿನ್ಸೆಂಟ್ ಮತ್ತು ಗ್ರೆನೆಡೈನ್ಸ್;സെന്‍റ് വിന്‍സന്‍റും ഗ്രനഡൈന്‍സും;സെയ്ന്റ് വിൻസന്റ് ഗ്രനഡീൻസ്;ประเทศเซนต์วินเซนต์และเกรนาดีนส์;เซนต์วินเซนต์และเกรนาดีนส์\n"),
                    t.toString());
  }
};

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(SplitNameVisitorTest, "quick");

}
