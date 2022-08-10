#pragma once


#define Py_TPFLAGS_HAVE_GETCHARBUFFER  (1L<<0)

/* PySequenceMethods contains sq_contains */
#define Py_TPFLAGS_HAVE_SEQUENCE_IN (1L<<1)

/* This is here for backwards compatibility.  Extensions that use the old GC
 * API will still compile but the objects will not be tracked by the GC. */
#define Py_TPFLAGS_GC 0 /* used to be (1L<<2) */

 /* PySequenceMethods and PyNumberMethods contain in-place operators */
#define Py_TPFLAGS_HAVE_INPLACEOPS (1L<<3)

/* PyNumberMethods do their own coercion */
#define Py_TPFLAGS_CHECKTYPES (1L<<4)

/* tp_richcompare is defined */
#define Py_TPFLAGS_HAVE_RICHCOMPARE (1L<<5)

/* Objects which are weakly referencable if their tp_weaklistoffset is >0 */
#define Py_TPFLAGS_HAVE_WEAKREFS (1L<<6)

/* tp_iter is defined */
#define Py_TPFLAGS_HAVE_ITER (1L<<7)

/* New members introduced by Python 2.2 exist */
#define Py_TPFLAGS_HAVE_CLASS (1L<<8)

/* Set if the type object is dynamically allocated */
#define Py_TPFLAGS_HEAPTYPE (1L<<9)

/* Set if the type allows subclassing */
#define Py_TPFLAGS_BASETYPE (1L<<10)

/* Set if the type is 'ready' -- fully initialized */
#define Py_TPFLAGS_READY (1L<<12)

/* Set while the type is being 'readied', to prevent recursive ready calls */
#define Py_TPFLAGS_READYING (1L<<13)

/* Objects support garbage collection (see objimp.h) */
#define Py_TPFLAGS_HAVE_GC (1L<<14)

/* These two bits are preserved for Stackless Python, next after this is 17 */
#ifdef STACKLESS
#define Py_TPFLAGS_HAVE_STACKLESS_EXTENSION (3L<<15)
#else
#define Py_TPFLAGS_HAVE_STACKLESS_EXTENSION 0
#endif

/* Objects support nb_index in PyNumberMethods */
#define Py_TPFLAGS_HAVE_INDEX (1L<<17)

/* Objects support type attribute cache */
#define Py_TPFLAGS_HAVE_VERSION_TAG   (1L<<18)
#define Py_TPFLAGS_VALID_VERSION_TAG  (1L<<19)

/* Type is abstract and cannot be instantiated */
#define Py_TPFLAGS_IS_ABSTRACT (1L<<20)

/* Has the new buffer protocol */
#define Py_TPFLAGS_HAVE_NEWBUFFER (1L<<21)

/* These flags are used to determine if a type is a subclass. */
#define Py_TPFLAGS_INT_SUBCLASS		(1L<<23)
#define Py_TPFLAGS_LONG_SUBCLASS	(1L<<24)
#define Py_TPFLAGS_LIST_SUBCLASS	(1L<<25)
#define Py_TPFLAGS_TUPLE_SUBCLASS	(1L<<26)
#define Py_TPFLAGS_STRING_SUBCLASS	(1L<<27)
#define Py_TPFLAGS_UNICODE_SUBCLASS	(1L<<28)
#define Py_TPFLAGS_DICT_SUBCLASS	(1L<<29)
#define Py_TPFLAGS_BASE_EXC_SUBCLASS	(1L<<30)
#define Py_TPFLAGS_TYPE_SUBCLASS	(1L<<31)

#define Py_TPFLAGS_DEFAULT_EXTERNAL ( \
                             Py_TPFLAGS_HAVE_GETCHARBUFFER | \
                             Py_TPFLAGS_HAVE_SEQUENCE_IN | \
                             Py_TPFLAGS_HAVE_INPLACEOPS | \
                             Py_TPFLAGS_HAVE_RICHCOMPARE | \
                             Py_TPFLAGS_HAVE_WEAKREFS | \
                             Py_TPFLAGS_HAVE_ITER | \
                             Py_TPFLAGS_HAVE_CLASS | \
                             Py_TPFLAGS_HAVE_STACKLESS_EXTENSION | \
                             Py_TPFLAGS_HAVE_INDEX | \
                             0)
#define Py_TPFLAGS_DEFAULT_CORE (Py_TPFLAGS_DEFAULT_EXTERNAL | \
                                 Py_TPFLAGS_HAVE_VERSION_TAG)

#ifdef Py_BUILD_CORE
#define Py_TPFLAGS_DEFAULT Py_TPFLAGS_DEFAULT_CORE
#else
#define Py_TPFLAGS_DEFAULT Py_TPFLAGS_DEFAULT_EXTERNAL
#endif
