#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h> //write
#include <err.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>

#include "upper.h"
#include "lower.h"
#include "rot13.h"
#include "utils.h"


#define COMMAND_STR_SIZE		32
#define MODULE_STR_SIZE			128

#define COMMAND_NAME_UPPER		"upper"
#define COMMAND_NAME_LOWER		"lower"
#define COMMAND_NAME_ROT13		"rot13"
#define COMMAND_NAME_LOAD		"load"

#define MODULE_NAME_UPPER		"libupper.so"
#define MODULE_NAME_LOWER		"liblower.so"
#define MODULE_NAME_ROT13		"librot13.so"

#define DYNAMIC_MODULE_FUNCTION	"module_execute_n"

#define ERROR_BUFFER_LENGTH			256
#define SOCKET_CLIENT_BUFFER_SIZE	0x1000



typedef 	void 	(*FUNC_PTR) (const char*, char*, size_t);


int exec_command(const char* command, const char* module, char* parsed, int parsed_length, char error_buffer[])
{
	int res = 0;
	if(strncmp(command,COMMAND_NAME_UPPER,strlen(COMMAND_NAME_UPPER))==0)
	{
		make_upper_n(parsed,parsed,parsed_length);
	}
	else if(strncmp(command,COMMAND_NAME_LOWER,strlen(COMMAND_NAME_LOWER))==0)
	{
		make_lower_n(parsed,parsed,parsed_length);
	}
	else if(strncmp(command,COMMAND_NAME_ROT13,strlen(COMMAND_NAME_ROT13))==0)
	{
		rot13_n(parsed,parsed,parsed_length);
	}
	else if(strncmp(command,COMMAND_NAME_LOAD,strlen(COMMAND_NAME_LOAD))==0)
	{
		void* handle = dlopen(module,RTLD_NOW);
		if(handle)
		{
			//circumvents ISO C cast from obj to function pointer forbidden
			uintptr_t po = (uintptr_t)dlsym(handle,DYNAMIC_MODULE_FUNCTION);
			if(po)
			{
				FUNC_PTR fptr;
				memcpy(&fptr,&po,sizeof(uintptr_t));
				printmsg("call func pointer %x (ptr obj %x)\n",fptr,po);
				(*fptr)(parsed,parsed,parsed_length);
			}
			else
			{
				const char format[] = "function '%s' not found in module '%s'\n";
				sprintf(error_buffer,format,DYNAMIC_MODULE_FUNCTION,module);
				err(-1,format,DYNAMIC_MODULE_FUNCTION,module);
				res = -1;
			}
		}
		else
		{
			sprintf(error_buffer,"cannot load module '%s'\n", module);
			res = -1;
		}
	}
	else
	{
		char cmd[COMMAND_STR_SIZE+16];
		strncpy(cmd,command,COMMAND_STR_SIZE);
		
		sprintf(error_buffer,"unknown command: '%s'\n",cmd);
	}
	return res;
}

enum ParserState { Command, Module, Strings, Escape1, Escape2, Reset, Error } ;

struct ContextData {

	char* 		data_buffer;
	int 		client_sock; 
	size_t		parsed_len;
	size_t		max_len;
	HDATA		h_client_buffer;
	HDATA		h_command;
	HDATA		h_module;
	HDATA		h_error;
	HDATA		h_loop;
};

#define DATA(parser)				((struct ContextData*)get_parser_context(parser))
#define SOCK(parser)				DATA(parser)->client_sock
#define PARSED_LEN(parser)					get_parsed_len(parser)
#define	MAX_LEN(parser)				DATA(parser)->max_len
#define	BUF(parser)					DATA(parser)->data_buffer

#define HDATA(parser,handle,type)		(type*) get_parser_data(DATA(parser)->handle)
#define HDATALEN(parser,handle)			get_parser_datalen(DATA(parser)->handle)
#define MOD(parser)					HDATA(parser,h_module,char)
#define MODLEN(parser)				HDATALEN(parser,h_module)
#define	CMD(parser)					HDATA(parser,h_command,char)
#define	CMDLEN(parser)				HDATALEN(parser,h_command)
#define	CLIENT_BUF(parser)					HDATA(parser,h_client_buffer,char)
#define	CLIENT_BUFLEN(parser)				HDATALEN(parser,h_client_buffer)
#define LOOP(parser)				HDATA(parser,h_loop,bool)
#define ERROR(parser)				HDATA(parser,h_error,char)
#define ERROR_LEN(parser)			HADATALEN(parser,h_error)

#define SET_STATE(parser,state)		set_current_state(parser,(void*)state)


void OnCommand(HPARSER parser)
{
		
		char* command = CMD(parser);
		size_t len = PARSED_LEN(parser);

		if(len==0)
		{
			SET_STATE(parser,Escape1);
			return;
		}

		command[len] = '\0';
		if(strcmp(CMD(parser),COMMAND_NAME_LOAD)==0)
		{
				SET_STATE(parser,Module);
				BUF(parser) = MOD(parser);
				MAX_LEN(parser) = MODLEN(parser);
				strcpy(BUF(parser),"lib");
		}
		else
		{
				SET_STATE(parser,Strings);
				BUF(parser) = CLIENT_BUF(parser);
				MAX_LEN(parser) = CLIENT_BUFLEN(parser);	
		}

		char response[128];
		sprintf(response,"Command '%s' received\n",command);
		write(SOCK(parser), response, strlen(response));
		puts(response);
}

void OnModule(HPARSER parser)
{
				
		if(PARSED_LEN(parser)==0)
		{
			SET_STATE(parser,Escape1);
			return;
		}
		size_t len = PARSED_LEN(parser), i;
		
		for(i=len; i>0; i--)
		{	
			BUF(parser)[i+2] =	BUF(parser)[i-1];
		}
		strncpy(BUF(parser),"lib",4);
		strcpy(BUF(parser)+len+3,".so");
		printmsg("module to load: %s\n",MOD(parser));

		SET_STATE(parser, Strings);
		
		BUF(parser) = CLIENT_BUF(parser);
		MAX_LEN(parser) = CLIENT_BUFLEN(parser);
}


void OnString(HPARSER parser)
{
		if(PARSED_LEN(parser)==0)
		{
			SET_STATE(parser,Escape1);
			return;
		}
		int res = exec_command(CMD(parser), MOD(parser), CLIENT_BUF(parser), PARSED_LEN(parser), ERROR(parser));
		if (res < 0)
		{
				const char error_header[] = "ERROR | ";
				char response [512];

				strcpy(response,error_header);
				strcat(response, ERROR(parser));
				write(SOCK(parser), response, strlen(response));
				SET_STATE(parser,Reset);
		}
		else
		{
				write(SOCK(parser), BUF(parser), PARSED_LEN(parser));
				write(SOCK(parser), "\n", 1);
		}
}

void OnEscape1(HPARSER parser)
{
		if (PARSED_LEN(parser)==0)
		{
				printf("ENTER pressed twice: stopping server\n");
				*LOOP(parser) = false;
				SET_STATE(parser,Escape2);
		}
}

void ParserReset(HPARSER parser)
{
		printmsg("Please enter command\n");
		SET_STATE(parser,Command);
}

void OnError(HPARSER parser)
{
		SET_STATE(parser,Escape2);
		err(-1,"%s",ERROR(parser));
		printmsg("Error: abort program\n");
		write(SOCK(parser),ERROR(parser),strlen(ERROR(parser)));
}

int ParseChar(HPARSER hp, char c)
{
	enum ParserState s = (enum ParserState)get_current_state(hp);
	if( s == Error )
	{
		return 0;
	}
	if(c=='\n' || c=='\r')
	{
			return 0;
	}
	if(	MAX_LEN(hp) < PARSED_LEN(hp) )
	{
			if(s == Command)
			{
					sprintf(ERROR(hp),"Command exceeds maximum allowed size (%d bytes)", COMMAND_STR_SIZE); 
			}
			if (s == Module)
			{
					sprintf(ERROR(hp),"Module name exceeds allowed size (%d bytes)", MODULE_STR_SIZE);
			}

			SET_STATE(hp,Error);
			return 0;
	}
	else
	{
		BUF(hp)[PARSED_LEN(hp)] = c;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client;
	char client_message[SOCKET_CLIENT_BUFFER_SIZE];
	short port = 8888;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1)
	{
		printf("socket creation failed\n");
	}
	printf("Socket created\n");
	
	if(argc>1)
	{
		char* str, *endptr;
		long port_arg;
		errno = 0;
		str = argv[1];

		port_arg = strtol( str, &endptr, 0);
		if (errno == 0)
		{
			port = (short) port_arg;
			printf("set port to %d\n", port);
		}
	}
	//prepare sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( port );
	
	//Bind
	if (bind( socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("bind failed. error");
		return 1;
	}
	printf("bind done\n");

	///listen
	listen(socket_desc, 3);

	//Accept incoming connection
	printf("waiting for incoming connections...\n");

	c = sizeof(struct sockaddr_in);

	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0)
	{
		perror("accept failed");
		return 1;
	}
	printf("Connection accepted\n");

	HPARSER parser = init_parser((void*)Command,(void*)Escape2);
	register_state_handler(parser, (void*)Command, &OnCommand);
	register_state_handler(parser, (void*)Module, &OnModule);
	register_state_handler(parser,(void*) Strings, &OnString);
	register_state_handler(parser,(void*) Escape1, &OnEscape1);
	register_state_handler(parser,(void*) Reset, &ParserReset);
	register_state_handler(parser,(void*) Error, &OnError);

	bool loop = true;	

	printmsg("states registered\n");

	char command[COMMAND_STR_SIZE+0x20];	
	char module[MODULE_STR_SIZE+0x20];
	char error_buffer[ERROR_BUFFER_LENGTH];

	struct ContextData ctx;
	
	ctx.data_buffer = command;
	ctx.max_len = COMMAND_STR_SIZE;
	ctx.client_sock=client_sock; 
	ctx.parsed_len=0;

	set_parser_context(parser,&ctx);

	printmsg("context set\n");
	
	ctx.h_client_buffer = register_data(parser, client_message, SOCKET_CLIENT_BUFFER_SIZE);
	ctx.h_command = register_data(parser, command, COMMAND_STR_SIZE);
	ctx.h_module = register_data(parser,module,MODULE_STR_SIZE);
	ctx.h_loop = register_data(parser,&loop,sizeof(loop));
	ctx.h_error = register_data(parser,error_buffer,ERROR_BUFFER_LENGTH);

	printmsg("data registered\n");
	
	//Receive a message from client
	while(loop && (read_size = recv(client_sock, client_message, SOCKET_CLIENT_BUFFER_SIZE, 0)) > 0)
	{
		parse(parser,client_message,read_size,ParseChar);
	}

	release_parser(parser);

	if (read_size == 0)
	{
		printf("Client disconnected\n");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}
	close(client_sock);
	shutdown(client_sock,2);
	close(socket_desc);
	shutdown(socket_desc,2);
	return 0;
}

	
