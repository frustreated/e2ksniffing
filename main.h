/* 
 * @author Tiago Alves Macambira
 * @version $Id: main.h,v 1.4 2004-03-03 06:40:38 tmacam Exp $
 * 
 * 
 * 
 */


#define BE_VERBOSE


//#define BEEN_HERE do {printf("BEEN HERE: %03i\n",__LINE__);} while(0) 
#define BEEN_HERE do {} while(0)

/**@brief In which state are we rearding the sniffing of a given stream? */
enum sniff_state { 
	/** We are waiting for enought data to read the next header*/
	wait_full_header,
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
};
