/***************************************************************

  Copyright (C) DSTC Pty Ltd (ACN 052 372 577) 1995.
  Unpublished work.  All Rights Reserved.

  The software contained on this media is the property of the
  DSTC Pty Ltd.  Use of this software is strictly in accordance
  with the license agreement in the accompanying LICENSE.DOC
  file.  If your distribution of this software does not contain
  a LICENSE.DOC file then you have no rights to use this
  software in any manner and should contact DSTC at the address
  below to determine an appropriate licensing arrangement.

     DSTC Pty Ltd
     Level 7, Gehrmann Labs
     University of Queensland
     St Lucia, 4072
     Australia
     Tel: +61 7 3365 4310
     Fax: +61 7 3365 4311
     Email: enquiries@dstc.edu.au

  This software is being provided "AS IS" without warranty of
  any kind.  In no event shall DSTC Pty Ltd be liable for
  damage of any kind arising out of or in connection with
  the use or performance of this software.

****************************************************************/

#ifndef lint
static const char cvsid[] = "$Id: production.c,v 1.1 1999/12/11 16:58:48 phelps Exp $";
#endif /* lint */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"
#include "production.h"

struct production
{
    /* The production's index */
    int index;

    /* The production's nonterminal (left-hand-side) */
    component_t nonterminal;

    /* The number of components on the right-hand-side */
    int count;

    /* The components */
    component_t *components;

    /* The production's reduction function */
    char *function;
};

/* Allocates and initializes a new empty production_t */
production_t production_alloc(int index)
{
    production_t self;

    /* Allocate memory for the new production_t */
    if ((self = (production_t)malloc(sizeof(struct production))) == NULL)
    {
	return NULL;
    }

    /* Initialize its contents to sane values */
    self -> index = index;
    self -> nonterminal = NULL;
    self -> count = 0;
    self -> components = NULL;
    self -> function = NULL;
    return self;
}

/* Releases the resources consumed by the receiver */
void production_free(production_t self)
{
    int index;

    if (self -> nonterminal != NULL)
    {
	component_free(self -> nonterminal);
    }

    for (index = 0; index < self -> count; index++)
    {
	component_free(self -> components[index]);
    }

    if (self -> function != NULL)
    {
	free(self -> function);
    }

    free(self);
}

/* Sets the receiver's nonterminal */
void production_set_nonterminal(production_t self, component_t nonterminal)
{
    self -> nonterminal = nonterminal;
}

/* Adds another component to the end of the production's list */
void production_add_component(production_t self, component_t component)
{
    self -> components = (component_t *)realloc(
	self -> components, (self -> count + 1) * sizeof(component_t));
    self -> components[self -> count++] = component;
}

/* Sets the receiver's function */
void production_set_function(production_t self, char *function)
{
    self -> function = strdup(function);
}

/* Pretty-prints the receiver */
void production_print(production_t self, FILE *out)
{
    production_print_with_offset(self, out, -1);
}

/* Pretty-prints the receiver with a `*' after the nth element */
void production_print_with_offset(production_t self, FILE *out, int offset)
{
    int index;

    /* Print the left-hand-side */
    component_print(self -> nonterminal, out);

    /* Print the `derives' operator */
    fprintf(out, "::= ");

    /* Print the right-hand-side */
    for (index = 0; index < self -> count; index++)
    {
	if (index == offset)
	{
	    fprintf(out, "* ");
	}

	component_print(self -> components[index], out);
    }

    /* Watch for a final `*' */
    if (self -> count == offset)
    {
	fprintf(out, "* ");
    }
}
