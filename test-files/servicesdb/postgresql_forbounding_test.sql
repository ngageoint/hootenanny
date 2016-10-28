
SELECT pg_catalog.setval('changesets_id_seq', 2);
SELECT pg_catalog.setval('current_nodes_id_seq', 5);
SELECT pg_catalog.setval('current_ways_id_seq', 3);
SELECT pg_catalog.setval('current_relations_id_seq', 3);


COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
1	1	2016-10-14 18:18:45.436	380000000	389453108	-1064999999	-1039999999	2016-10-14 18:18:45.436	8
\.


COPY current_nodes (id, latitude, longitude, changeset_id, visible, "timestamp", tile, version) FROM stdin;
1	384000000	-1065000000	1	t	2016-10-14 18:18:45.434	1328832880	1
2	380000000	-1040000000	1	t	2016-10-14 18:18:45.435	1329332431	1
3	389453108	-1047213578	1	t	2016-10-14 18:18:45.435	1329042565	1
4	382072214	-1042333805	1	t	2016-10-14 18:18:45.435	1328986390	1
\.


COPY current_node_tags (node_id, k, v) FROM stdin;
1	accuracy1	5
1	highway	road
2	foo	bar
3	accuracy1	5
3	highway	track
4	foo1	bar1
\.


COPY nodes (node_id, latitude, longitude, changeset_id, visible, "timestamp", tile, version, redaction_id) FROM stdin;
1	384000000	-1065000000	1	t	2016-10-14 18:18:45.434	1328832880	1	\N
2	380000000	-1040000000	1	t	2016-10-14 18:18:45.435	1329332431	1	\N
3	389453108	-1047213578	1	t	2016-10-14 18:18:45.435	1329042565	1	\N
4	382072214	-1042333805	1	t	2016-10-14 18:18:45.435	1328986390	1	\N
\.


COPY node_tags (node_id, version, k, v) FROM stdin;
1	1	accuracy1	5
1	1	highway	road
2	1	foo	bar
3	1	accuracy1	5
3	1	highway	track
4	1	foo1	bar1
\.


COPY current_ways (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2016-10-14 18:18:45.435	t	1
2	1	2016-10-14 18:18:45.435	t	1
\.


COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;
1	1	1
1	2	2
2	3	1
2	4	2
\.


COPY current_way_tags (way_id, k, v) FROM stdin;
1	highway	primary
2	highway	footway
\.


COPY ways (way_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2016-10-14 18:18:45.435	1	t	\N
2	1	2016-10-14 18:18:45.435	1	t	\N
\.


COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;
1	1	1	1
1	2	1	2
2	3	1	1
2	4	1	2
\.


COPY way_tags (way_id, version, k, v) FROM stdin;
1	1	highway	primary
2	1	highway	footway
\.


COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2016-10-14 18:18:45.435	t	1
2	1	2016-10-14 18:18:45.435	t	1
\.


COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
1	Way	1	wayrole	1
1	Node	1	noderole	2
2	Way	2	wayrole	1
2	Node	3	noderole	2
\.


COPY current_relation_tags (relation_id, k, v) FROM stdin;
\.


COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2016-10-14 18:18:45.435	1	t	\N
2	1	2016-10-14 18:18:45.435	1	t	\N
\.


COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
1	Way	1	wayrole	1	1
1	Node	1	noderole	1	2
2	Way	2	wayrole	1	1
2	Node	2	noderole	1	2
\.


COPY relation_tags (relation_id, version, k, v) FROM stdin;
\.


