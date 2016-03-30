--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = public, pg_catalog;

--
-- Data for Name: current_relations; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_relations (id, changeset_id, "timestamp", visible, version) FROM stdin;
1	1	2015-10-20 08:04:55.283796	t	1
\.


--
-- Data for Name: current_relation_members; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;
1	Way	1	wayrole	0
1	Node	1	noderole	1
\.


--
-- Data for Name: current_relation_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY current_relation_tags (relation_id, k, v) FROM stdin;
1	type	multistuff
\.


--
-- Name: current_relations_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('current_relations_id_seq', 1, true);

--
-- Name: relations; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY relations (relation_id, changeset_id, "timestamp", version, visible, redaction_id) FROM stdin;
1	1	2015-10-20 08:04:55.283796	1	t	\N
\.


--
-- Data for Name: relation_members; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;
1	Way	1	wayrole	1	0
1	Node	1	noderole	1	1
\.


--
-- Data for Name: relation_tags; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY relation_tags (relation_id, k, v, version) FROM stdin;
1	type	multistuff	1
\.


--
-- PostgreSQL database dump complete
--

