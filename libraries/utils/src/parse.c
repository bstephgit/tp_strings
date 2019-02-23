#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "../inc/utils.h"

char* parse_to(char* input, const char* end, const char seps[])
{
	char* p = input;
	while(p < end)
	{
#ifdef DEBUG
	charcode(p,1);
#endif
		size_t sep_len = strlen(seps);
		unsigned int i;
		for(i=0; i<sep_len; i += 1)
		{
				char sep = seps[i];
				if (*p == sep)
				{
#ifdef DEBUG
						printf("\nseparator found ==> return\n");
#endif
						return p;
				}
		}
		p += 1;
	}
	return p;
}

char* parse_after(char* input, const char* end, const char seps[])
{

	size_t sep_len = strlen(seps);
	char* p = input;
	bool is_equal = true;
	unsigned int i;

	while(p < end)
	{
#ifdef DEBUG
		charcode(p,1);
#endif
		is_equal = false;
		for(i=0; !is_equal && i<sep_len; i += 1)
		{
				is_equal = (*p == seps[i]);
		}
		if(!is_equal)
			return p;
		p += 1;
	}
	return p;

}


struct Handler {
	void* state;
	PARSER_HANDLER_PTR		handler;	
	struct Handler* next;
};
struct Data {

	void* data;
	size_t len;
	struct Data* next;
};

struct ParserInfo {
	
	void*				current_state;
	void*				end_state;
	char				*pos, *end, *start;	
	int					parsing;
	
	struct Handler* 	handlers;
	struct Data*		data;
	void*				context;
};

HPARSER init_parser(void* initial_state, void* end_state)
{
	
	struct ParserInfo* parser = (struct ParserInfo*) malloc(sizeof(struct ParserInfo));
	parser->current_state = initial_state;
	parser->end_state = end_state;

	parser->handlers = NULL;		
	parser->data = NULL;
	parser->context = NULL;

	return (HPARSER)parser;
}

void release_parser(HPARSER h)
{

	struct ParserInfo* parser = (struct ParserInfo*) h;

	struct Handler *ph = parser->handlers;
	while(ph)
	{
		struct Handler* temp = ph;
		ph = temp->next;
		free(temp);	
	}
	struct Data *p = parser->data;
	while(p)
	{
		struct Data* temp = p;
		p = temp->next;
		free(temp);	
	}
	free(parser);
}

void set_parser_context( HPARSER hp, void* context )
{

	struct ParserInfo* parser = (struct ParserInfo*)hp;
	parser->context = context;
}

void* get_parser_context(HPARSER hp)
{

	struct ParserInfo* parser = (struct ParserInfo*)hp;
	return parser->context;
}

void register_state_handler( HPARSER h, void* state, PARSER_HANDLER_PTR handler_function)
{

	struct ParserInfo* parser = (struct ParserInfo*) h;

	struct Handler** pph = &(parser->handlers);
	struct Handler *p = NULL;

	while( *pph )
	{
		p = *pph;
		if( p->state == state)
		{
			p->handler = handler_function;
			return;
		}
		pph = &(p->next);
	}

	p = (struct Handler*) malloc( sizeof(struct Handler) );
	p->handler = handler_function;
	p->state = state;
	p->next = NULL;
	*pph = p;
}

PARSER_HANDLER_PTR get_handler(HPARSER h, void* state)
{
	struct ParserInfo* parser = (struct ParserInfo*) h;
	struct Handler *ph = parser->handlers;
	
	while(ph)
	{	
		if(ph->state==state)
		{
			return ph->handler;
		}
		ph = ph->next;
	}
	return NULL;
}
	

HDATA register_data(HPARSER h, void* data, size_t len)
{

	struct ParserInfo* parser = (struct ParserInfo*) h;
	struct Data** ppd = &parser->data;
	struct Data* p;

	while( *ppd )
	{

		p = *ppd;
		if ( p->data == data )
		{
			p->len = len;
			return (HDATA)p;
		}
		ppd = &p->next;

	}
	p = (struct Data*) malloc ( sizeof(struct Data));
		
	p->data = data;
	p->len = len;
	p->next = NULL;
	*ppd = p;
	return (HDATA)p;
}

void* get_parser_data(HDATA h)
{
	return ((struct Data*)h)->data;
}

size_t get_parser_datalen(HDATA h)
{
	return ((struct Data*)h)->len;
}

void set_current_state(HPARSER hp, void* state)
{
	
	struct ParserInfo* parser = (struct ParserInfo*)hp;
	
	parser->current_state = state;
}

void* get_current_state(HPARSER hp)
{
	
	struct ParserInfo* parser = (struct ParserInfo*)hp;
	
	return parser->current_state;
}


void parse(HPARSER hp, char* buffer, size_t len, PARSER_DATA_PTR f)
{
	
	struct ParserInfo* parser = (struct ParserInfo*)hp;

	parser->pos = buffer;
	parser->end = buffer + len;
	parser->start = parser->pos;
	parser->parsing = 1;
	
	while(parser->pos < parser->end)
	{
		int next_state = f(hp,*parser->pos);

		if(parser->parsing==1 && next_state!=parser->parsing) //client stopped parsing
		{
			PARSER_HANDLER_PTR phdlr = get_handler(hp,parser->current_state);						
			parser->parsing = 0;
			if(phdlr)
			{
				(*phdlr)(hp);
			}
			if(parser->current_state==parser->end_state)
			{	
				PARSER_HANDLER_PTR phdlr_last = get_handler(hp,parser->current_state);
				if(phdlr_last && phdlr != phdlr_last)
				{
					(*phdlr_last)(hp);
				}
				return;
			}
		}
		if (parser->parsing == 0 && next_state != parser->parsing)
		{
			parser->parsing = 1;
			parser->start = parser->pos;
		}
		parser->pos += 1;
	}
}

size_t get_parsed_len(HPARSER hp)
{
	struct ParserInfo* parser = (struct ParserInfo*)hp;

	return parser->pos - parser->start;
}

