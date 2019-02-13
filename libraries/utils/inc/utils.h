int printmsg(const char* format, ...);
int charcode(char* str, size_t len);

char* _itoa( int n, char to[]);

char* parse_to(char* input, const char* end, const char seps[]);
char* parse_after(char* input, const char* end, const char seps[]);

typedef	void*	HPARSER;
typedef	void*	HDATA;

typedef void (*PARSER_HANDLER_PTR) ( HPARSER );
typedef	int	 (*PARSER_DATA_PTR)	(HPARSER hp, char c);


HPARSER init_parser(void* initial_state, void* end_state);
void release_parser(HPARSER hp);
void set_parser_context( HPARSER hp, void* context );
void* get_parser_context(HPARSER hp);
void register_state_handler( HPARSER hp, void* state, PARSER_HANDLER_PTR handler_function);
PARSER_HANDLER_PTR get_handler(HPARSER hp, void* state);
HDATA register_data(HPARSER hp, void* data, size_t len);
void* get_parser_data(HDATA hp);
size_t get_parser_datalen(HDATA hp);
void set_current_state(HPARSER hp, void* state);
void* get_current_state(HPARSER hp);
void parse(HPARSER hp, char* buffer, size_t len, PARSER_DATA_PTR f);
size_t get_parsed_len(HPARSER hp);

