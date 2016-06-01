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
-- Data for Name: changesets; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY changesets (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) FROM stdin;
1	1	2015-10-20 06:59:53.562782	\N	\N	\N	\N	2015-10-20 06:59:53.562782	0
\.


--
-- Name: changesets_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('changesets_id_seq', 1, true);


--
-- PostgreSQL database dump complete
--

