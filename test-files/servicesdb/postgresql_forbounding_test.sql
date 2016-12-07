
SELECT pg_catalog.setval('changesets_id_seq', 2);
SELECT pg_catalog.setval('current_nodes_id_seq', 5);
SELECT pg_catalog.setval('current_ways_id_seq', 3);
SELECT pg_catalog.setval('current_relations_id_seq', 3);


COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
1	1	2016-10-14 18:18:45.436	380000000	389453108	-1064999999	-1039999999	2016-10-14 18:18:45.436	8
\.


COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
1	389008975	-780226543	1	t	2016-10-14 18:18:45.434	1704300837	1
2	399008568	-770224564	1	t	2016-10-14 18:18:45.434	1707233245	1
3	389008975	-780226543	1	t	2016-10-14 18:18:45.434	1704300837	1
4	389008975	-780226543	1	t	2016-10-14 18:18:45.434	1704300837	1
5	389008975	-780226543	1	t	2016-10-14 18:18:45.434	1704300837	1
6	339008975	-830226543	1	t	2016-10-14 18:18:45.434	1696640828	1
7	289008975	-880226543	1	t	2016-10-14 18:18:45.434	1682123204	1
\.


COPY current_node_tags (node_id, k, v) FROM stdin;
1	node1	node1
2	node2	node2
3	node3	node3
4	node4	node4
5	node5	node5
6	node6	node6
7	node7	node7
\.


COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
1	389008975	-780226543	1	t	2016-10-14 18:18:45.434	1704300837	1	\N
2	399008568	-770224564	1	t	2016-10-14 18:18:45.434	1707233245	1	\N
3	389008975	-780226543	1	t	2016-10-14 18:18:45.434	1704300837	1	\N
4	389008975	-780226543	1	t	2016-10-14 18:18:45.434	1704300837	1	\N
5	389008975	-780226543	1	t	2016-10-14 18:18:45.434	1704300837	1	\N
6	339008975	-830226543	1	t	2016-10-14 18:18:45.434	1696640828	1	\N
7	289008975	-880226543	1	t	2016-10-14 18:18:45.434	1682123204	1	\N
\.


COPY node_tags (node_id, version, k, v) FROM stdin;
1	1	node1	node1
2	1	node2	node2
3	1	node3	node3
4	1	node4	node4
5	1	node5	node5
6	1	node6	node6
7	1	node7	node7
\.


COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2016-10-14 18:18:45.435	t	1
2	1	2016-10-14 18:18:45.435	t	1
3	1	2016-10-14 18:18:45.435	t	1
4	1	2016-10-14 18:18:45.435	t	1
5	1	2016-10-14 18:18:45.435	t	1
\.


COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
1	1	1
1	2	2
1	5	3
2	3	1
2	2	2
3	1	1
3	2	2
4	6	1
4	7	2
5	1	1
5	6	2
\.


COPY current_way_tags (way_id, k, v) FROM stdin;
1	way1	way1
2	way2	way2
3	way3	way3
4	way4	way4
5	way5	way5
\.


COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2016-10-14 18:18:45.435	1	t	\N
2	1	2016-10-14 18:18:45.435	1	t	\N
3	1	2016-10-14 18:18:45.435	1	t	\N
4	1	2016-10-14 18:18:45.435	1	t	\N
5	1	2016-10-14 18:18:45.435	1	t	\N
\.


COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
1	1	1	1
1	2	1	2
1	5	1	3
2	3	1	1
2	2	1	2
3	1	1	1
3	2	1	2
4	6	1	1
4	7	1	2
5	1	1	1
5	6	1	2
\.


COPY way_tags (way_id, version, k, v) FROM stdin;
1	1	way1	way1
2	1	way2	way2
3	1	way3	way3
4	1	way4	way4
5	1	way5	way5
\.


COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2016-10-14 18:18:45.435	t	1
2	1	2016-10-14 18:18:45.435	t	1
3	1	2016-10-14 18:18:45.435	t	1
4	1	2016-10-14 18:18:45.435	t	1
5	1	2016-10-14 18:18:45.435	t	1
6	1	2016-10-14 18:18:45.435	t	1
\.


COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
1	Node	1	role1	1
1	Way	2	role3	2
1	Way	1	role2	3
1	Node	3	empty	4
2	Node	5	role1	1
2	Relation	1	role1	2
3	Way	2	empty	1
4	Node	3	role1	1
5	Node	7	role1	1
5	Way	4	role1	2
6	Node	7	role1	1
6	Way	1	role1	2
6	Way	4	role1	3
\.


COPY current_relation_tags (relation_id, k, v) FROM stdin;
1	relation1	relation1
2	relation2	relation2
3	relation3	relation3
4	relation4	relation4
5	relation5	relation5
6	relation6	relation6
\.


COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2016-10-14 18:18:45.435	1	t	\N
2	1	2016-10-14 18:18:45.435	1	t	\N
3	1	2016-10-14 18:18:45.435	1	t	\N
4	1	2016-10-14 18:18:45.435	1	t	\N
5	1	2016-10-14 18:18:45.435	1	t	\N
6	1	2016-10-14 18:18:45.435	1	t	\N
\.


COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
1	Node	1	role1	1	1
1	Way	2	role3	1	2
1	Way	1	role2	1	3
1	Node	3	role1	1	4
2	Node	5	role1	1	1
2	Relation	1	role1	1	2
3	Way	2	role2	1	1
4	Node	3	role1	1	1
5	Node	7	role1	1	1
5	Way	4	role1	1	2
6	Node	7	role1	1	1
6	Way	1	role1	1	2
6	Way	4	role1	1	3
\.


COPY relation_tags (relation_id, version, k, v) FROM stdin;
1	1	relation1	relation1
2	1	relation2	relation2
3	1	relation3	relation3
4	1	relation4	relation4
5	1	relation5	relation5
6	1	relation6	relation6
\.


