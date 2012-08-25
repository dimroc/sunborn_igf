//---------------------------------------------------------------------------------
// File Name: zcta015.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef llOllllOll
#define llOllllOll
#ifndef TA_TYPES_H
#include "Types.h"
#endif //
#ifndef TA_COMMON_H
#include "Common.h"
#endif //
#ifndef TA_MATHS_H
#include "Maths.h"
#endif //
na\
mespa\
ce
TA\

{
clas\
s
Serial\
iser\

;
cl\
ass
TACO\
MMON_CL\
ASS
llOl\
l\
l\
llOO
{
pu\
blic
:
llOll\
lllOO\

(
)
;
ll\
OlllllO\
O
(
co\
n\
st\

Ch\
a\
r
*
lllOO\
Ol\
lOl
)
;
llOl\
llllO\
O\

(
co\
n\
st
llO\
ll\
lll\
OO\

&
lO\
O\
lllOOll\
l
)
;
llOl\
llllOO\

(
co\
ns\
t
Char
*
lllO\
OOllO\
l\

,
in\
t
lO\
Ol\
llOl\
lO
)
;
llOl\
llllOO\

(
int\

lOO\
llOO\
ll\
l\
O
)
;
ll\
Olllll\
O\
O
(
float
lO\
OOOOll\
l\
O
)
;
~
llOllll\
l\
OO\

(
)
;
void
Seriali\
se\

(
Serialis\
e\
r
&
lOO\
OlO\
l\
lOl\

)
;
void
lOO\
ll\
l\
OlOO\
l\

(
Seria\
liser
&
lO\
OOlOll\
Ol\

)
;
boo\
l
op\
erator
==
(
cons\
t\

llOll\
lllOO
&
lO\
Ol\
llOOlll\

)
;
boo\
l
ope\
rat\
o\
r
==
(
const
Char
*
lllOOOllO\
l
)
;
bool
op\
er\
ator
!=
(
con\
st
llOl\
llllOO
&
lO\
Oll\
lOOlll\

)
;
bo\
ol
opera\
tor
!=
(
const
Char
*
ll\
lOOOll\
O\
l\

)
;
llOl\
lll\
lOO
&
opera\
tor
=
(
co\
nst
llO\
lllll\
O\
O
&
lO\
OlllOOl\
ll\

)
;
llOl\
llllOO\

&
operator
=
(
con\
st
Ch\
ar
*
ll\
lO\
OOllOl
)
;
ll\
O\
ll\
lllO\
O
&
operator
+=
(
const
llO\
ll\
l\
llOO\

&
lO\
Ol\
ll\
OO\
lll\

)
;
llO\
lllllOO
&
oper\
at\
or\

+=
(
const
Char
*
lllO\
OOllOl
)
;
llOl\
lll\
lO\
O
&
oper\
a\
to\
r
+=
(
int\

lO\
O\
llO\
OlllO
)
;
llO\
ll\
lllOO
&
operator\

+=
(
flo\
at
lOOO\
OOl\
llO
)
;
llOl\
llllOO
opera\
t\
or
+
(
co\
n\
st
llOlllllO\
O
&
lOO\
lllOOl\
ll
)
const
;
llOl\
llll\
OO
op\
er\
ator
+
(
llOllll\
l\
OO
&
lOOlllOO\
lll
)
{
return
(
con\
st
llOll\
l\
l\
lOO
&
)
*
th\
i\
s
+
(
co\
nst
llO\
lllll\
OO
&
)
lOOl\
llOOlll\

;
}
llOlll\
llOO
operat\
or
+
(
con\
st
Cha\
r\

*
lllOO\
OllOl\

)
const
;
llOll\
lll\
OO
op\
er\
ato\
r
+
(
int\

lOOllOOll\
lO
)
const
;
ll\
Ol\
l\
lllOO
op\
era\
tor\

+
(
floa\
t
lOOOO\
Oll\
lO
)
const
;
frie\
nd\

llOl\
llllOO
oper\
ator\

+
(
co\
nst
Char
*
lllO\
Oll\
OlO
,
llOllll\
lOO
&
lllOOllll\
O
)
;
sta\
tic
ll\
Olllll\
O\
O
lOOll\
l\
Oll\
lO\

(
cons\
t\

Ch\
ar
ll\
lO\
l\
O\
Oll\
O\

)
{
Cha\
r
lO\
O\
lll\
Ollll
[
2
]
;
lO\
OlllO\
l\
l\
ll
[
0
]
=
lllO\
lOOllO
;
lO\
O\
lll\
Oll\
l\
l\

[
1
]
=
'\0';
llOlllllOO
lO\
Ollll\
OOOO\

(
lOOlllOllll\

)
;
retur\
n
lOOll\
llOO\
O\
O
;
}
op\
e\
rator
const
Char\

*
(
)
const
{
retur\
n\

lOOl\
l\
lOOllO\

;
}
con\
st
Char\

*
lOO\
llllO\
OOl
(
)
const\

{
retur\
n
lOOl\
ll\
OOllO
;
}
in\
t
lOOOlllOOl\

(
)
co\
nst
{
return
Abs\

(
lOlOOO\
lOO\
l
)
;
}
vo\
id\

Cle\
a\
r\

(
)
;
void\

lO\
O\
lll\
OlOll
(
)
;
vo\
id
lOO\
lll\
O\
ll\
OO\

(
)
;
pr\
iv\
ate
:
int
lOl\
OOO\
lO\
O\
l
;
Cha\
r
*
lO\
Oll\
lOOl\
l\
O\

;
friend
llOlllllO\
O
TAC_CAL\
L\

op\
erato\
r
+
(
co\
nst\

Ch\
a\
r\

*
ll\
lO\
Ol\
lOl\
O
,
co\
n\
s\
t
llOlllllOO
&
lll\
O\
Ollll\
O
)
;
vo\
id
lOOlllO\
lOOO
(
in\
t\

lOOlll\
O\
ll\
O\

)
;
}
;
llOlllllO\
O
TAC_CALL\

op\
er\
ator
+
(
const
Char
*
ll\
lO\
O\
ll\
Ol\
O\

,
cons\
t\

llOlllllOO\

&
lllOOllllO
)
;
const
ll\
Ol\
llllOO\

lO\
O\
l\
lllOOlO
;
}
;
#endif //
