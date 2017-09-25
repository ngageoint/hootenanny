BEGIN TRANSACTION;

COPY changesets_22 (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes, tags) FROM stdin;
1	3	2017-09-19 14:39:32.974	0	-1	0	-1	2017-09-19 14:39:32.974	5	"bot"=>"yes","created_by"=>"hootenanny"
2	3	2017-09-19 14:39:32.974	0	-1	0	-1	2017-09-19 14:39:32.974	5	"bot"=>"yes","created_by"=>"hootenanny"
3	3	2017-09-19 14:39:32.974	0	-1	0	-1	2017-09-19 14:39:32.974	5	"bot"=>"yes","created_by"=>"hootenanny"
4	3	2017-09-19 14:39:32.974	0	-1	0	-1	2017-09-19 14:39:32.974	5	"bot"=>"yes","created_by"=>"hootenanny"
\.

COPY current_nodes_22 (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, tags) FROM stdin;
1	38.05	-77.59999999999999	1	t	2017-09-19 14:39:32.974	1704239955	1	\N
2	38.01	-77.65000000000001	1	t	2017-09-19 14:39:32.974	1704239316	1	\N
3	38.01	-77.55	1	t	2017-09-19 14:39:32.974	1704239836	1	\N
4	38.1	-77.59999999999999	1	t	2017-09-19 14:39:32.974	1704240727	1	\N
5	38	-77.7	1	t	2017-09-19 14:39:32.974	1704233711	1	\N
6	38	-77.5	2	t	2017-09-19 14:39:32.974	1704241231	1	\N
7	38.1	-77.40000000000001	2	t	2017-09-19 14:39:32.974	1704242807	1	\N
8	38	-77.40000000000001	2	t	2017-09-19 14:39:32.974	1704241767	1	\N
9	38.1	-77.3	2	t	2017-09-19 14:39:32.974	1704330453	1	\N
10	38	-77.3	2	t	2017-09-19 14:39:32.974	1704329413	1	\N
11	38.1	-77.2	3	t	2017-09-19 14:39:32.974	1704330973	1	\N
12	38	-77.2	3	t	2017-09-19 14:39:32.974	1704329933	1	\N
13	38	-77.09999999999999	3	t	2017-09-19 14:39:32.974	1704331493	1	\N
14	38	-77	3	t	2017-09-19 14:39:32.974	1704332013	1	"building"=>"yes","name"=>"n1 - n2"
\.

COPY current_ways_22 (id, changeset_id, "timestamp", visible, version, tags) FROM stdin;
1	3	2017-09-19 14:39:32.974	t	1	\N
2	4	2017-09-19 14:39:32.974	t	1	\N
3	4	2017-09-19 14:39:32.974	t	1	\N
4	4	2017-09-19 14:39:32.974	t	1	\N
5	4	2017-09-19 14:39:32.974	t	1	\N
\.

COPY current_way_nodes_22 (way_id, node_id, sequence_id) FROM stdin;
1	3	1
1	2	2
1	1	3
1	3	4
2	6	1
2	5	2
2	4	3
2	6	4
3	8	1
3	7	2
4	10	1
4	9	2
5	13	1
5	12	2
5	11	3
5	13	4
\.

COPY current_relations_22 (id, changeset_id, "timestamp", visible, version, tags) FROM stdin;
1	4	2017-09-19 14:39:32.974	t	1	\N
\.

COPY current_relation_members_22 (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
1	way	2	outer	1
1	way	1	inner	2
\.

COMMIT;
