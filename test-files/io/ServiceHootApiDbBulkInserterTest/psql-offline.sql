BEGIN TRANSACTION;

COPY current_nodes_70 (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, tags) FROM stdin;
14	38.05	-77.59999999999999	130	t	2018-03-02 14:29:01.607	1704239955	1	\N
13	38.01	-77.65000000000001	130	t	2018-03-02 14:29:01.607	1704239316	1	\N
12	38.01	-77.55	130	t	2018-03-02 14:29:01.607	1704239836	1	\N
11	38.1	-77.59999999999999	130	t	2018-03-02 14:29:01.607	1704240727	1	\N
10	38	-77.7	131	t	2018-03-02 14:29:01.607	1704233711	1	\N
9	38	-77.5	132	t	2018-03-02 14:29:01.607	1704241231	1	\N
8	38.1	-77.40000000000001	132	t	2018-03-02 14:29:01.607	1704242807	1	\N
7	38	-77.40000000000001	132	t	2018-03-02 14:29:01.607	1704241767	1	\N
6	38.1	-77.3	132	t	2018-03-02 14:29:01.607	1704330453	1	\N
5	38	-77.3	133	t	2018-03-02 14:29:01.607	1704329413	1	\N
4	38.1	-77.2	134	t	2018-03-02 14:29:01.607	1704330973	1	\N
3	38	-77.2	134	t	2018-03-02 14:29:01.607	1704329933	1	\N
2	38	-77.09999999999999	134	t	2018-03-02 14:29:01.607	1704331493	1	\N
1	38	-77	134	t	2018-03-02 14:29:01.607	1704332013	1	"building"=>"yes","name"=>"n1 - n2"
\.

COPY current_ways_70 (id, changeset_id, "timestamp", visible, version, tags) FROM stdin;
5	135	2018-03-02 14:29:01.607	t	1	\N
4	136	2018-03-02 14:29:01.607	t	1	\N
3	136	2018-03-02 14:29:01.607	t	1	"name"=>"w3","highway"=>"road"
2	136	2018-03-02 14:29:01.607	t	1	"name"=>"w2","highway"=>"track"
1	136	2018-03-02 14:29:01.607	t	1	"building"=>"yes","area"=>"yes","name"=>"w1"
\.

COPY current_way_nodes_70 (way_id, node_id, sequence_id) FROM stdin;
5	12	1
5	13	2
5	14	3
5	12	4
4	9	1
4	10	2
4	11	3
4	9	4
3	7	1
3	8	2
2	5	1
2	6	2
1	2	1
1	3	2
1	4	3
1	2	4
\.

COPY current_relations_70 (id, changeset_id, "timestamp", visible, version, tags) FROM stdin;
1	137	2018-03-02 14:29:01.607	t	1	"building"=>"yes","type"=>"multipolygon","name"=>"r1"
\.

COPY current_relation_members_70 (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
1	way	4	outer	1
1	way	5	inner	2
\.

COMMIT;