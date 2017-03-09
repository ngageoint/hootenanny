BEGIN TRANSACTION;


COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
2	1	2017-03-09 19:28:30.214	380000000	381000000	-776999999	-775499999	2017-03-09 19:28:30.214	5
3	1	2017-03-09 19:28:30.214	380000000	381000000	-774999999	-772999999	2017-03-09 19:28:30.214	5
4	1	2017-03-09 19:28:30.214	380000000	381000000	-771999999	-769999999	2017-03-09 19:28:30.214	5
5	1	2017-03-09 19:28:30.215	0	-9999999	0	-9999999	2017-03-09 19:28:30.215	5
\.



COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
2	380500000	-776000000	2	t	2017-03-09 19:28:30.213	1704239955	1
3	380100000	-776500000	2	t	2017-03-09 19:28:30.213	1704239316	1
4	380100000	-775500000	2	t	2017-03-09 19:28:30.213	1704239836	1
5	381000000	-776000000	2	t	2017-03-09 19:28:30.213	1704240727	1
6	380000000	-777000000	2	t	2017-03-09 19:28:30.213	1704233711	1
7	380000000	-775000000	3	t	2017-03-09 19:28:30.214	1704241231	1
8	381000000	-774000000	3	t	2017-03-09 19:28:30.214	1704242807	1
9	380000000	-774000000	3	t	2017-03-09 19:28:30.214	1704241767	1
10	381000000	-773000000	3	t	2017-03-09 19:28:30.214	1704330453	1
11	380000000	-773000000	3	t	2017-03-09 19:28:30.214	1704329413	1
12	381000000	-772000000	4	t	2017-03-09 19:28:30.214	1704330973	1
13	380000000	-772000000	4	t	2017-03-09 19:28:30.214	1704329933	1
14	380000000	-771000000	4	t	2017-03-09 19:28:30.214	1704331493	1
15	380000000	-770000000	4	t	2017-03-09 19:28:30.214	1704332013	1
\.



COPY current_node_tags (node_id, k, v) FROM stdin;
15	building	yes
15	name	n1
\.



COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
2	380500000	-776000000	2	t	2017-03-09 19:28:30.213	1704239955	1	\N
3	380100000	-776500000	2	t	2017-03-09 19:28:30.213	1704239316	1	\N
4	380100000	-775500000	2	t	2017-03-09 19:28:30.213	1704239836	1	\N
5	381000000	-776000000	2	t	2017-03-09 19:28:30.213	1704240727	1	\N
6	380000000	-777000000	2	t	2017-03-09 19:28:30.213	1704233711	1	\N
7	380000000	-775000000	3	t	2017-03-09 19:28:30.214	1704241231	1	\N
8	381000000	-774000000	3	t	2017-03-09 19:28:30.214	1704242807	1	\N
9	380000000	-774000000	3	t	2017-03-09 19:28:30.214	1704241767	1	\N
10	381000000	-773000000	3	t	2017-03-09 19:28:30.214	1704330453	1	\N
11	380000000	-773000000	3	t	2017-03-09 19:28:30.214	1704329413	1	\N
12	381000000	-772000000	4	t	2017-03-09 19:28:30.214	1704330973	1	\N
13	380000000	-772000000	4	t	2017-03-09 19:28:30.214	1704329933	1	\N
14	380000000	-771000000	4	t	2017-03-09 19:28:30.214	1704331493	1	\N
15	380000000	-770000000	4	t	2017-03-09 19:28:30.214	1704332013	1	\N
\.



COPY node_tags (node_id, version, k, v) FROM stdin;
15	1	building	yes
15	1	name	n1
\.



COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
2	4	2017-03-09 19:28:30.214	t	1
3	5	2017-03-09 19:28:30.214	t	1
4	5	2017-03-09 19:28:30.214	t	1
5	5	2017-03-09 19:28:30.214	t	1
6	5	2017-03-09 19:28:30.214	t	1
\.



COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
2	4	1
2	3	2
2	2	3
2	4	4
3	7	1
3	6	2
3	5	3
3	7	4
4	9	1
4	8	2
5	11	1
5	10	2
6	14	1
6	13	2
6	12	3
6	14	4
\.



COPY current_way_tags (way_id, k, v) FROM stdin;
4	name	w3
4	highway	road
5	name	w2
5	highway	track
6	building	yes
6	area	yes
6	name	w1
\.



COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
2	4	2017-03-09 19:28:30.214	1	t	\N
3	5	2017-03-09 19:28:30.214	1	t	\N
4	5	2017-03-09 19:28:30.214	1	t	\N
5	5	2017-03-09 19:28:30.214	1	t	\N
6	5	2017-03-09 19:28:30.214	1	t	\N
\.



COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
2	4	1	1
2	3	1	2
2	2	1	3
2	4	1	4
3	7	1	1
3	6	1	2
3	5	1	3
3	7	1	4
4	9	1	1
4	8	1	2
5	11	1	1
5	10	1	2
6	14	1	1
6	13	1	2
6	12	1	3
6	14	1	4
\.



COPY way_tags (way_id, version, k, v) FROM stdin;
4	1	name	w3
4	1	highway	road
5	1	name	w2
5	1	highway	track
6	1	building	yes
6	1	area	yes
6	1	name	w1
\.



COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
2	5	2017-03-09 19:28:30.215	t	1
\.



COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
2	Way	3	outer	1
2	Way	2	inner	2
\.



COPY current_relation_tags (relation_id, k, v) FROM stdin;
2	building	yes
2	name	r1
\.



COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
2	5	2017-03-09 19:28:30.215	1	t	\N
\.



COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
2	Way	3	outer	1	1
2	Way	2	inner	1	2
\.



COPY relation_tags (relation_id, version, k, v) FROM stdin;
2	1	building	yes
2	1	name	r1
\.



COMMIT;