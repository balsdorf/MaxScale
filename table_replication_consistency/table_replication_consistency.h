/*
Copyright (C) 2013, SkySQL Ab


This file is distributed as part of the SkySQL Gateway. It is free
software: you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation,
version 2.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51
Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

Author: Jan Lindström jan.lindstrom@skysql.com

Created: 20-06-2013
Updated:

*/

#ifndef TABLE_REPLICATION_CONSISTENCY_H
#define TABLE_REPLICATION_CONSISTENCY_H

/* Structure definition for replication listener */
typedef struct replication_listener {
	char *server_url;            /*!< in: Server address e.g.
				     mysql://root:pw@127.0.0.1:3308 */
	unsigned long binlog_pos;    /*!< in: Binlog position where to start
				     listening. */
	int use_mariadb_gtid;        /*!< in: 1 if MariaDB global
				     transaction id should be used for
				     binlog start position. */
	int use_mysql_gtid;          /*!< in: 1 if MySQL global
				     transaction id should be used for
				     binlog start position. */
	int use_binlog_pos;          /*!< in: 1 if binlog position
				     should be used for binlog start
				     position. */
	char *gtid;                  /*!< in: Global transaction identifier
				     or NULL */
	int is_master;               /*!< in: Is this server a master 1 =
				     yes, 0 = no. */
	int gateway_slave_server_id; /*!< in: replication listener slave 
				     server id. */
	int listener_id;             /*!< in: listener id */
	int connection_suggesfull;   /*!< out: 0 if connection successfull
				     or error number. */
	char *error_message;         /*!< out: error message in case of
				     error. */
} replication_listener_t;

/* Structure definition for table consistency query */
typedef struct table_consistency_query {
	char *db_dot_table;          /*!< in: Fully qualified database and
				     table, e.g. Production.Orders. */
} table_consistency_query_t;

/* Structure definition for table consistency result */
typedef struct table_consistency {
	char *db_dot_table;         /*!< out: Fully qualified database and
				    table, e.g. Production.Orders. */
	unsigned int server_id;     /*!< out: Server id where the consitency
				    information is from. */
	int mariadb_gtid_known;     /*!< out: 1 if MariaDB global
				    transaction id is known. */
	int mysql_gtid_known;       /*!< out: 1 if MySQL global
				    transaction id is known. */
	unsigned long binlog_pos;   /*!< out: Last seen binlog position
				    on this server. */
	char *gtid;                 /*!< out: If global transacition id
				    is known, will contain the id or NULL. */
	int error_code;             /*!< out: 0 if table consistency query
				    for this server succesfull or error
				    code. */
	char *error_message;        /*!< out: Error message if table
				    consistency query failed for this
				    server failed. */
} table_consistency_t;

/* Interface functions */

/***********************************************************************//**
This function will register replication listener for every server
provided and initialize all internal data structures and starts listening
the replication stream.
@return 0 on success, error code at failure. */
int 
tb_replication_consistency_init(
/*============================*/
	replication_listener_t *rpl,              /*!< in: Server
						  definition. */
	int                    n_servers,         /*!< in: Number of servers */
	unsigned int           gateway_server_id);/*!< in: Gateway slave
						  server id. */

/***********************************************************************//**
With this fuction client can request table consistency status for a
single table. As a return client will receive a number of consistency
status structures. Client must allocate memory for consistency result
array and provide the maximum number of values returned. At return
there is information how many results where available.
@return 0 on success, error code at failure. */
int 
tb_replication_consistency_query(
/*=============================*/
	table_consistency_query_t *tb_query, /*!< in: Table consistency
					     query. */ 
	table_consistency_t *tb_consistency, /*!< in: Table consistency
					     status structure.*/
	int *n_servers);                     /*!< inout: Number of
					     servers where to get table
					     consistency status. Out: Number
					     of successfull consistency
					     query results. */

/***********************************************************************//**
This function will reconnect replication listener to a server
provided.
@return 0 on success, error code at failure. */
int
tb_replication_consistency_reconnect(
/*=================================*/
	replication_listener_t* rpl,     /*!< in: Server definition.*/
	unsigned int gateway_server_id); /*!< in: Gateway slave
					 server id. */

/***********************************************************************//**
This function is to shutdown the replication listener and free all
resources on table consistency. This function (TODO) will store
the current status on metadata to MySQL server.
@return 0 on success, error code at failure. */
int
tb_replication_consistency_shutdown(
/*=================================*/
        char ** error_message);         /*!< out: error_message*/


#endif
