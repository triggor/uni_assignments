/*===========Projekt: System wspomagający organizację konferencji==========*/
/*==========Utworzenie od nowa bazy. Wykonał: Igor Tryhub, 275235==========*/


CREATE ROLE student SUPERUSER CREATEDB CREATEROLE LOGIN REPLICATION PASSWORD 'student';
CREATE DATABASE student WITH OWNER=student TEMPLATE template0;
\connect student
\i db_project_physical.sql
