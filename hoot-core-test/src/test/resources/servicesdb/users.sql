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
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: hoot
--

COPY users (email, id, pass_crypt, creation_time, display_name, data_public, description, home_lat, home_lon, home_zoom, nearby, pass_salt, image_file_name, email_valid, new_email, creation_ip, languages, status, terms_agreed, consider_pd, openid_url, preferred_editor, terms_seen, description_format, image_fingerprint, changesets_count, traces_count, diary_entries_count, image_use_gravatar, image_content_type) FROM stdin;
hootingest@digitalglobe.com	1		2015-10-20 06:50:40.968242		f		\N	\N	3	50	\N	\N	f	\N	\N	\N	pending	\N	f	\N	\N	f	markdown	\N	0	0	0	t	\N
\.


--
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: hoot
--

SELECT pg_catalog.setval('users_id_seq', 1, true);


--
-- PostgreSQL database dump complete
--

