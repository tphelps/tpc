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

#ifndef GRAMMAR_H
#define GRAMMAR_H

#ifndef lint
static const char cvs_GRAMMAR_H[] = "$Id: grammar.h,v 1.10 1999/12/11 16:57:39 phelps Exp $";
#endif /* lint */

/* The grammar type */
typedef struct grammar *grammar_t;


/* Allocates and initializes a new nonterminal grammar_t */
grammar_t grammar_alloc();

/* Releases the resources consumed by the receiver */
void grammar_free(grammar_t self);

/* Adds another production to the grammar */
void grammar_add_production(grammar_t self, production_t production);

/* Pretty-prints the receiver */
void grammar_print(grammar_t self, FILE *out);

#endif /* GRAMMAR_H */
