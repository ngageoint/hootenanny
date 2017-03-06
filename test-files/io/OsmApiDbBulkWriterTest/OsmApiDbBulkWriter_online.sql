

COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
1	1	2017-03-06 21:31:21.855	380000000	381000000	-776999999	-769999999	2017-03-06 21:31:21.855	20
\.



COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
15	380500000	-776000000	1	t	2017-03-06 21:31:21.854	1704239955	1
16	380100000	-776500000	1	t	2017-03-06 21:31:21.854	1704239316	1
17	380100000	-775500000	1	t	2017-03-06 21:31:21.854	1704239836	1
18	381000000	-776000000	1	t	2017-03-06 21:31:21.854	1704240727	1
19	380000000	-777000000	1	t	2017-03-06 21:31:21.854	1704233711	1
20	380000000	-775000000	1	t	2017-03-06 21:31:21.854	1704241231	1
21	381000000	-774000000	1	t	2017-03-06 21:31:21.854	1704242807	1
22	380000000	-774000000	1	t	2017-03-06 21:31:21.854	1704241767	1
23	381000000	-773000000	1	t	2017-03-06 21:31:21.854	1704330453	1
24	380000000	-773000000	1	t	2017-03-06 21:31:21.854	1704329413	1
25	381000000	-772000000	1	t	2017-03-06 21:31:21.854	1704330973	1
26	380000000	-772000000	1	t	2017-03-06 21:31:21.854	1704329933	1
27	380000000	-771000000	1	t	2017-03-06 21:31:21.854	1704331493	1
28	380000000	-770000000	1	t	2017-03-06 21:31:21.854	1704332013	1
\.



COPY current_node_tags (node_id, k, v) FROM stdin;
28	building	yes
28	name	n1
\.



COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
15	380500000	-776000000	1	t	2017-03-06 21:31:21.854	1704239955	1	\N
16	380100000	-776500000	1	t	2017-03-06 21:31:21.854	1704239316	1	\N
17	380100000	-775500000	1	t	2017-03-06 21:31:21.854	1704239836	1	\N
18	381000000	-776000000	1	t	2017-03-06 21:31:21.854	1704240727	1	\N
19	380000000	-777000000	1	t	2017-03-06 21:31:21.854	1704233711	1	\N
20	380000000	-775000000	1	t	2017-03-06 21:31:21.854	1704241231	1	\N
21	381000000	-774000000	1	t	2017-03-06 21:31:21.854	1704242807	1	\N
22	380000000	-774000000	1	t	2017-03-06 21:31:21.854	1704241767	1	\N
23	381000000	-773000000	1	t	2017-03-06 21:31:21.854	1704330453	1	\N
24	380000000	-773000000	1	t	2017-03-06 21:31:21.854	1704329413	1	\N
25	381000000	-772000000	1	t	2017-03-06 21:31:21.854	1704330973	1	\N
26	380000000	-772000000	1	t	2017-03-06 21:31:21.854	1704329933	1	\N
27	380000000	-771000000	1	t	2017-03-06 21:31:21.854	1704331493	1	\N
28	380000000	-770000000	1	t	2017-03-06 21:31:21.854	1704332013	1	\N
\.



COPY node_tags (node_id, version, k, v) FROM stdin;
28	1	building	yes
28	1	name	n1
\.



COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
6	1	2017-03-06 21:31:21.854	t	1
7	1	2017-03-06 21:31:21.854	t	1
8	1	2017-03-06 21:31:21.854	t	1
9	1	2017-03-06 21:31:21.854	t	1
10	1	2017-03-06 21:31:21.854	t	1
\.



COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
6	17	1
6	16	2
6	15	3
6	17	4
7	20	1
7	19	2
7	18	3
7	20	4
8	22	1
8	21	2
9	24	1
9	23	2
10	27	1
10	26	2
10	25	3
10	27	4
\.



COPY current_way_tags (way_id, k, v) FROM stdin;
8	name	w3
8	highway	road
9	name	w2
9	highway	track
10	building	yes
10	area	yes
10	name	w1
\.



COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
6	1	2017-03-06 21:31:21.854	1	t	\N
7	1	2017-03-06 21:31:21.854	1	t	\N
8	1	2017-03-06 21:31:21.854	1	t	\N
9	1	2017-03-06 21:31:21.854	1	t	\N
10	1	2017-03-06 21:31:21.854	1	t	\N
\.



COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
6	17	1	1
6	16	1	2
6	15	1	3
6	17	1	4
7	20	1	1
7	19	1	2
7	18	1	3
7	20	1	4
8	22	1	1
8	21	1	2
9	24	1	1
9	23	1	2
10	27	1	1
10	26	1	2
10	25	1	3
10	27	1	4
\.



COPY way_tags (way_id, version, k, v) FROM stdin;
8	1	name	w3
8	1	highway	road
9	1	name	w2
9	1	highway	track
10	1	building	yes
10	1	area	yes
10	1	name	w1
\.



COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
2	1	2017-03-06 21:31:21.855	t	1
\.



COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
2	Way	7	outer	1
2	Way	6	inner	2
\.



COPY current_relation_tags (relation_id, k, v) FROM stdin;
2	building	yes
2	name	r1
\.



COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
2	1	2017-03-06 21:31:21.855	1	t	\N
\.



COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
2	Way	7	outer	1	1
2	Way	6	inner	1	2
\.



COPY relation_tags (relation_id, version, k, v) FROM stdin;
2	1	building	yes
2	1	name	r1
\.




