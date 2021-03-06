/**@file main.h
 * @brief main - Program, libs, and logging facilities setup and handling
 * @author Tiago Alves Macambira
 * @version $Id: main.h,v 1.14 2005-06-13 20:58:01 tmacam Exp $
 * 
 * Based on sample code provided with libnids and copyright (c) 1999
 * Rafal Wojtczuk <nergal@avet.com.pl>. All rights reserved.
 * See the file COPYING from libnids for license details.
 *
 *
 * (c) Tiago Alves Macambira
 * See COPYING for licence for further license details
 * 
 */

#ifndef _MAIN__H_
#define _MAIN__H_

#ifdef E2KSNIFFER_JUST_LOG
#undef P4P_SIGNATURE_SHORT_CIRCUIT
#endif

#include "e2k_defs.h"
#include "writers_pool.h"
#include "e2k_zip.h"

/* ********************************************************************  
 *  Global defines - configuration 
 * ******************************************************************** */

#define BE_VERBOSE
#define UNPRIV_USER "nobody"
#define SYSLOG_REPORT_INTERVAL 60
#define LOGROTATE_INTERVAL 15*60
#define LOGROTATE_MAX_SIZE 10*1024*1024
#define LOGROTATE_WITH_N_PACKETS 100;
#define E2K_WRITER_POOL_SAVE_PATH "/tmp/cache/"

/**@see p4p_short_circuit.h */
#ifdef P4P_SIGNATURE_SHORT_CIRCUIT
 #include "p4p_short_circuit.h"
#endif /* P4P_SIGNATURE_SHORT_CIRCUIT */

#define UDP_NICE_INCREMENT 10

/** The size of the biggest packet we are allowed to process */
#define PACKET_MAXIMUM_SIZE 36000

/* ********************************************************************  
 *  Global defines - debug
 * ******************************************************************** */


//#define BEEN_HERE do {printf("BEEN HERE: %03i\n",__LINE__);} while(0) 
#define BEEN_HERE do {} while(0)

/* ********************************************************************  
 *  Global variables
 * ******************************************************************** */

extern writers_pool_t w_pool; /* Defined @ main.c */

/* ********************************************************************  
 *  Strucutres - declaration and typedefs
 * ******************************************************************** */

/**@brief In which state are we rearding the sniffing of a given stream? */
enum sniff_state { 
	/** We are waiting for enought data to read the next header*/
	wait_full_header = 0,
	/** we are just skiping till the begining of the next header */
	skip_full_packet,
	/** we are waiting for enought data to read the next packet */
	wait_full_packet,
	/** the connection is uninteresting now */
	ignore_connection,
};

typedef struct conn_state_t conn_state_t; /* forward declaration */

/**@brief Records the state of the e2k sniffing state machine of one side
 * a given connection.*/
typedef struct half_conn_state_t {
	/** The state of the sniffing state machine on this half-stream*/
	enum sniff_state state;
	/** How many bytes away is the next packet in the half-stream */
	int next_packet_offset;
	/** The connection this half-stream belongs */
	conn_state_t* connection;
	/** says if this side was blessed as a known edonkey connection*/
	int blessed;
} half_conn_state_t;



/** Maximum size a address_str of a conn_state_t can take, null-termination
 * included */
#define CONN_STATE_ADDRESS_STR_SZ 44

/**@brief Records general information about a connection being sniffed
 *
 *  This structure records general information about a connection being
 *  sniffed, such as the state of sniffing state-machine of both sides
 *  of the connection, the address of the end-points of this conection, etc
 */
struct conn_state_t {
	/**Sniffing state-machine information on the client-side of the
	 * connection*/
	half_conn_state_t client;
	/**Sniffing state-machine information on the server-side of the
	 * connection*/
	half_conn_state_t server;
	/**The address of the endpoints of this connection as a string.
	 * Exemple: 111.111.111.111:32000,222.222.222.222:15000 */
	unsigned char address_str[CONN_STATE_ADDRESS_STR_SZ];
	/** Should this connection be ignored ?*/
	int ignore;
	/** Hash of the file being downloaded - if any. One per conn. */
	struct e2k_hash_t download_hash;
	/** writer of the file being downloaded associated writer */
	writers_pool_writer_t download_writer;
	/** serial number of this connections during the current execution */
	dword connection_id;
	/** EMULE COMPRESSED DATA support and state structures.  */
	e2k_zip_state_t zip_state;
};


#endif /* #ifndef _MAIN__H_ */
