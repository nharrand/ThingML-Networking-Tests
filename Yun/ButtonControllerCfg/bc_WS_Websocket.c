#include <lws_config.h>
#include <stdio.h>
#include <stdlib.h>
#include <libwebsockets.h>

typedef enum { false, true } bool;

//externalMessageEnqueue(uint8_t * msg, uint8_t msgSize, uint16_t listener_id);



struct Websocket_instance_type {
    uint16_t listener_id;
    //Connector// Pointer to receiver list
struct Msg_Handler ** WS_receiver_list_head;
struct Msg_Handler ** WS_receiver_list_tail;
// Handler Array
struct Msg_Handler * WS_handlers;

};

extern struct Websocket_instance_type Websocket_instance;


struct libwebsocket * Websocket_clients[16];
int Websocket_nb_client;

struct lws_context_creation_info Websocket_info;
struct libwebsocket_context *Websocket_context;


uint16_t add_client(struct libwebsocket *wsi) {
	uint16_t i = 0;
	bool done = false;
	while ((!done) && (i < 16)) {
		if(Websocket_clients[i] == NULL) {
			Websocket_clients[i] = wsi;
			done = true;
		}
		i++;
	}
	if (!done) {
            printf("[PosixWSForward] Client list overflow\n");
            return -1;
	} else {
            Websocket_nb_client++;
            i=i-1;
            return i;
	}
}

uint16_t remove_client(struct libwebsocket *wsi) {
	uint16_t i = 0;
	bool done = false;
	while ((!done) && (i < 16)) {
            if(Websocket_clients[i] == wsi) {
                Websocket_clients[i] = NULL;
                done = true;
            }
            i++;
	}
	if (!done) {
            printf("[PosixWSForward] Client not found\n");
            return -1;
	} else {
            Websocket_nb_client--;
            i=i-1;
            return i;
	}
}


static int callback_http(struct libwebsocket_context * this,
                         struct libwebsocket *wsi,
                         enum libwebsocket_callback_reasons reason, void *user,
                         void *in, size_t len)
{
    return 0;
}

static int callback_ThingML_protocol(struct libwebsocket_context * this,
                                   struct libwebsocket *wsi,
                                   enum libwebsocket_callback_reasons reason,
                                   void *user, void *in, size_t len)
{
   
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED: // just log message that someone is connecting
            printf("[PosixWSForward] New Client\n");
            uint16_t clientID = add_client(wsi);
		
            break;

        case LWS_CALLBACK_RECEIVE: {
            int len = strlen((char *) in);
            printf("[PosixWSForward] l:%i\n", len);
            if ((len % 3) == 0) {
                    unsigned char msg[len % 3];
                    unsigned char * p = in;

                    int buf = 0;
                    int index = 0;
                    bool everythingisfine = true;
                    while ((index < len) && everythingisfine) {
                            if((*p - 48) < 10) {
                                    buf = (*p - 48) + 10 * buf;
                            } else {
                                    everythingisfine = false;
                            }
                            if ((index % 3) == 2) {
                                    if(buf < 256) {
                                            msg[(index-2) / 3] =  (uint8_t) buf;
                                    } else {
                                            everythingisfine = false;
                                    }
                                    buf = 0;
                            }
                            index++;
                            p++;
                    }
                    if(everythingisfine) {
                            int j;
                            externalMessageEnqueue(msg, (len / 3), Websocket_instance.listener_id);
                            printf("[PosixWSForward] Message received");

                    } else {
                            printf("[PosixWSForward] incorrect message '%s'\n", (char *) in);
                    }
            } else {
                printf("[PosixWSForward] incorrect message '%s'\n", (char *) in);
            }


			
          
            // send response
            // just notice that we have to tell where exactly our response starts. That's
            // why there's `buf[LWS_SEND_BUFFER_PRE_PADDING]` and how long it is.
            // we know that our response has the same length as request because
            // it's the same message in reverse order.
            //libwebsocket_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], len, LWS_WRITE_TEXT);
           
            // release memory back into the wild
            //free(buf);
            break;
        }
		
        case LWS_CALLBACK_WSI_DESTROY: {
                uint16_t clientID = remove_client(wsi);
		
                printf("[PosixWSForward] Wsi destroyed\n");
        }

        case LWS_CALLBACK_CLOSED: {
                printf("[PosixWSForward] Connexion with client closed\n");
                uint16_t clientID = remove_client(wsi);
		
        }

        default:
            break;
    }
   
   
    return 0;
}

static struct libwebsocket_protocols protocols[] = {
    /* first protocol must always be HTTP handler */
    {
        "http-only",   // name
        callback_http, // callback
        0              // per_session_data_size
    },
    {
        "ThingML-protocol", // protocol name - very important!
        callback_ThingML_protocol,   // callback
        0                          // we don't use any per session data
    },
    {
        NULL, NULL, 0   /* End of list */
    }
};

void Websocket_set_listener_id(uint16_t id) {
	Websocket_instance.listener_id = id;
}

void Websocket_setup() {
    memset(&Websocket_info, 0, sizeof Websocket_info);

    int port = 9000;
    const char *interface = NULL;
    // we're not using ssl
    const char *cert_path = NULL;
    const char *key_path = NULL;
    // no special options
    int opts = 0;
   
    Websocket_info.port = port;
    Websocket_info.iface = interface;
    Websocket_info.protocols = protocols;
    Websocket_info.extensions = libwebsocket_get_internal_extensions();
    Websocket_info.ssl_cert_filepath = NULL;
    Websocket_info.ssl_private_key_filepath = NULL;

    Websocket_info.gid = -1;
    Websocket_info.uid = -1;
    Websocket_info.options = opts;
    printf("[PosixWSForward] Init WS Server on port:%i\n", port);
}

void Websocket_start_receiver_process() {

    printf("[PosixWSForward] Start running WS Server\n");
    // create libwebsocket Websocket_context representing this server
    Websocket_context = libwebsocket_create_context(&Websocket_info);
   
    if (Websocket_context == NULL) {
        fprintf(stderr, "[PosixWSForward] libwebsocket init failed\n");
        return -1;
    }
	
    printf("[PosixWSForward] Starting server...\n");
	
    // infinite loop, to end this server send SIGTERM. (CTRL+C)
    while (1) {
        libwebsocket_service(Websocket_context, 50);
    }
	
    libwebsocket_context_destroy(Websocket_context);
}

void Websocket_forwardMessage(char * msg, int length) {
	int n, m, i;
	unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + (length * 3 + 1) +
						  LWS_SEND_BUFFER_POST_PADDING];
	unsigned char *p = &buf[LWS_SEND_BUFFER_PRE_PADDING];	
	unsigned char *q = p;
	n = 0;
	for(i = 0; i < length; i++) {
		//printf("%03i -> ", (unsigned char) msg[i]);
		n += sprintf((unsigned char *)q, "%03i", (unsigned char) msg[i]);
		//printf("%s\n", q);
		n--;
		q += 3;
	}
	*q = '\0';
	n++;
	printf("[PosixWSForward] Trying to send:\n%s \n", p);


       for(i = 0; i < Websocket_nb_client; i++) {
m = libwebsocket_write(Websocket_clients[i], p, (length * 3 + 1), LWS_WRITE_TEXT);
}
	//for(i = 0; i < Websocket_nb_client; i++) {
	//	m = libwebsocket_write(Websocket_clients[i], p, (length * 3 + 1), LWS_WRITE_TEXT);
	//}
}

