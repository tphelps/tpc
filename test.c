#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 1024

static int state_stack[STACK_SIZE];
static int *state_top = state_stack;

static void *value_stack[STACK_SIZE];
static void **value_top = value_stack;

static void *Accept()
{
    printf("Accept!\n");
    return NULL;
}

static void *NameFromId()
{
    printf("NameFromId\n");
    return NULL;
}

static void *CreateInt32Function()
{
    printf("CreateInt32Function\n");
    return NULL;
}

static void *Function()
{
    return NULL;
}

#include "e4new.h"

/* Push a state and value onto the stack */
static void Push(int state, void *value)
{
    *(++state_top) = state;
    *(value_top++) = value;
}

/* Pop states and values off the stack */
static void Pop(int count)
{
    state_top -= count;
    value_top -= count;
}

/* Answers the top state on the stack */
static int Top()
{
    return *state_top;
}


static void ShiftReduce(terminal_t type, void *value)
{
    while (1)
    {
	int state = Top();
	int action = sr_table[state][type];
	struct production *production;
	int reduction;
	void *result;

	/* Watch for errors */
	if (IS_ERROR(action))
	{
	    fprintf(stderr, "*** ERROR (state=%d, type=%d)\n", state, type);
	    exit(0);
	}

	/* Accept if we can */
	if (IS_ACCEPT(action))
	{
	    Accept();
	    return;
	}

	/* Shift if we can */
	if (IS_SHIFT(action))
	{
	    printf("s%d\n", SHIFT_GOTO(action));
	    Push(SHIFT_GOTO(action), value);
	    return;
	}

	/* Locate the production we're going to use to reduce */
	reduction = REDUCTION(action);
	production = &productions[reduction];

	printf("r%d, ", reduction);
	fflush(stdout);

	/* Pop stuff off of the stack */
	Pop(production -> count);

	/* Reduce */
	result = (production -> function)();

	/* Push the result of the reduction back onto the stack */
	Push(REDUCE_GOTO(Top(), production), result);
    }
}


int main(int argc, char *argv[])
{
    *state_top = 0;
    printf("hello sailor\n");

    ShiftReduce(TT_IS_INT32, NULL);
    ShiftReduce(TT_LPAREN, NULL);
    ShiftReduce(TT_ID, NULL);
    ShiftReduce(TT_RPAREN, NULL);
    ShiftReduce(TT_BOOL_AND, NULL);
    ShiftReduce(TT_ID, NULL);
    ShiftReduce(TT_EQ, NULL);
    ShiftReduce(TT_INT32, NULL);
    ShiftReduce(TT_EOF, NULL);
    return 0;
}