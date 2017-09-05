BEGIN TRANSACTION;


COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
3	1	2017-03-27 19:05:20.716	0	-9999999	0	-9999999	2017-03-27 19:05:20.716	5
4	1	2017-03-27 19:05:20.716	0	-9999999	0	-9999999	2017-03-27 19:05:20.716	5
5	1	2017-03-27 19:05:20.717	0	-9999999	0	-9999999	2017-03-27 19:05:20.717	5
6	1	2017-03-27 19:05:20.718	0	-9999999	0	-9999999	2017-03-27 19:05:20.718	5
\.



COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
4	380500000	-776000000	3	t	2017-03-27 19:05:20.716	1704239955	1
5	380100000	-776500000	3	t	2017-03-27 19:05:20.716	1704239316	1
6	380100000	-775500000	3	t	2017-03-27 19:05:20.716	1704239836	1
7	381000000	-776000000	3	t	2017-03-27 19:05:20.716	1704240727	1
8	380000000	-777000000	3	t	2017-03-27 19:05:20.716	1704233711	1
9	380000000	-775000000	4	t	2017-03-27 19:05:20.716	1704241231	1
10	381000000	-774000000	4	t	2017-03-27 19:05:20.716	1704242807	1
11	380000000	-774000000	4	t	2017-03-27 19:05:20.716	1704241767	1
12	381000000	-773000000	4	t	2017-03-27 19:05:20.716	1704330453	1
13	380000000	-773000000	4	t	2017-03-27 19:05:20.716	1704329413	1
14	381000000	-772000000	5	t	2017-03-27 19:05:20.716	1704330973	1
15	380000000	-772000000	5	t	2017-03-27 19:05:20.716	1704329933	1
16	380000000	-771000000	5	t	2017-03-27 19:05:20.716	1704331493	1
17	380000000	-770000000	5	t	2017-03-27 19:05:20.716	1704332013	1
\.



COPY current_node_tags (node_id, k, v) FROM stdin;
17	building	yes
17	name	n1
\.



COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
4	380500000	-776000000	3	t	2017-03-27 19:05:20.716	1704239955	1	\N
5	380100000	-776500000	3	t	2017-03-27 19:05:20.716	1704239316	1	\N
6	380100000	-775500000	3	t	2017-03-27 19:05:20.716	1704239836	1	\N
7	381000000	-776000000	3	t	2017-03-27 19:05:20.716	1704240727	1	\N
8	380000000	-777000000	3	t	2017-03-27 19:05:20.716	1704233711	1	\N
9	380000000	-775000000	4	t	2017-03-27 19:05:20.716	1704241231	1	\N
10	381000000	-774000000	4	t	2017-03-27 19:05:20.716	1704242807	1	\N
11	380000000	-774000000	4	t	2017-03-27 19:05:20.716	1704241767	1	\N
12	381000000	-773000000	4	t	2017-03-27 19:05:20.716	1704330453	1	\N
13	380000000	-773000000	4	t	2017-03-27 19:05:20.716	1704329413	1	\N
14	381000000	-772000000	5	t	2017-03-27 19:05:20.716	1704330973	1	\N
15	380000000	-772000000	5	t	2017-03-27 19:05:20.716	1704329933	1	\N
16	380000000	-771000000	5	t	2017-03-27 19:05:20.716	1704331493	1	\N
17	380000000	-770000000	5	t	2017-03-27 19:05:20.716	1704332013	1	\N
\.



COPY node_tags (node_id, version, k, v) FROM stdin;
17	1	building	yes
17	1	name	n1
\.



COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
3	5	2017-03-27 19:05:20.717	t	1
4	6	2017-03-27 19:05:20.717	t	1
5	6	2017-03-27 19:05:20.717	t	1
6	6	2017-03-27 19:05:20.717	t	1
7	6	2017-03-27 19:05:20.717	t	1
\.



COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
3	6	1
3	5	2
3	4	3
3	6	4
4	9	1
4	8	2
4	7	3
4	9	4
5	11	1
5	10	2
6	13	1
6	12	2
7	16	1
7	15	2
7	14	3
7	16	4
\.



COPY current_way_tags (way_id, k, v) FROM stdin;
5	name	w3
5	highway	road
6	name	w2
6	highway	track
7	building	yes
7	area	yes
7	name	w1
\.



COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
3	5	2017-03-27 19:05:20.717	1	t	\N
4	6	2017-03-27 19:05:20.717	1	t	\N
5	6	2017-03-27 19:05:20.717	1	t	\N
6	6	2017-03-27 19:05:20.717	1	t	\N
7	6	2017-03-27 19:05:20.717	1	t	\N
\.



COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
3	6	1	1
3	5	1	2
3	4	1	3
3	6	1	4
4	9	1	1
4	8	1	2
4	7	1	3
4	9	1	4
5	11	1	1
5	10	1	2
6	13	1	1
6	12	1	2
7	16	1	1
7	15	1	2
7	14	1	3
7	16	1	4
\.



COPY way_tags (way_id, k, v, version) FROM stdin;
5	name	w3	1
5	highway	road	1
6	name	w2	1
6	highway	track	1
7	building	yes	1
7	area	yes	1
7	name	w1	1
\.



COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
3	6	2017-03-27 19:05:20.717	t	1
\.



COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
3	Way	4	outer	1
3	Way	3	inner	2
\.



COPY current_relation_tags (relation_id, k, v) FROM stdin;
3	building	yes
3	name	r1
\.



COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
3	6	2017-03-27 19:05:20.717	1	t	\N
\.



COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
3	Way	4	outer	1	1
3	Way	3	inner	1	2
\.



COPY relation_tags (relation_id, k, v, version) FROM stdin;
3	building	yes	1
3	name	r1	1
\.



COMMIT;