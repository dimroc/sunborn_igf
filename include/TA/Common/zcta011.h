//---------------------------------------------------------------------------------
// File Name: zcta011.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lOOllOOOOll
#define lOOllOOOOll
#ifndef TA_COMMON_H
#include "Common.h"
#endif //
namespace\

TA
{
templa\
t\
e
<
class
Typ\
e
>
class\

TACOMMO\
N_\
CLASS
lOOllOO\
O\
lOO\

{
public
:
class\

Iter\
a\
tor
{
public
:
~
Ite\
rat\
or
(
)
{
Fin\
al\
i\
se\

(
)
;
}
Ty\
pe\

*
operato\
r
++
(
)
{
if\

(
lO\
Olll\
OlOO\

<=
0
)
retu\
rn
0
;
else\

{
lOO\
ll\
lOl\
O\
O
--
;
re\
turn
*
(
++
lO\
Ol\
lOOOlOl
)
;
}
}
Type
&
op\
er\
ator
*
(
)
{
ret\
u\
rn
*
*
lOOllOO\
OlOl
;
}
bool
AtE\
nd
(
)
{
re\
turn
lO\
OlllO\
lOO
<=
0
;
}
priv\
at\
e
:
friend
lOOllO\
O\
O\
lOO\

;
Itera\
to\
r\

(
)
{
lOOllO\
OO\
lOl
=
0
;
lO\
Oll\
lOlOO
=
0
;
}
void
In\
it\
i\
a\
l\
is\
e
(
Type
*
*
lOOllO\
OOllO\

,
int\

lOOlllOllO
)
{
lOO\
llOO\
O\
lOl
=
lOOllOO\
Ol\
l\
O
;
lO\
OlllOlOO
=
lO\
Ol\
ll\
Ol\
lO
;
}
vo\
id\

Finalise
(
)
{
lOOllOO\
Ol\
Ol\

=
0
;
lOOll\
l\
O\
lO\
O
=
0
;
}
Type
*
*
lOOllOOOlOl
;
int
lOO\
lll\
OlOO
;
}
;
lOOllO\
OOlOO
(
)
;
~
lO\
O\
llOO\
OlOO
(
)
;
vo\
id
Ini\
tia\
lise
(
int
lOO\
lll\
Olll
,
int
lOOllllOOO
,
int
lOOllll\
OOl
=
32
)
;
void\

Final\
i\
se
(
)
;
void\

Clear
(
)
;
Type\

*
ope\
rator
[
]
(
int\

lOO\
OlOOlll
)
;
con\
s\
t
Type
*
ope\
ra\
tor
[
]
(
in\
t\

lOOOl\
O\
Olll\

)
const\

;
int
lOOOll\
l\
OO\
l
(
)
cons\
t\

;
Type
*
lOOO\
l\
ll\
OOO
(
)
;
void
lO\
O\
Ol\
llOOO
(
const
Type
&
lOOlll\
l\
l\
OO
)
;
voi\
d
lOOllllll\
l
(
int\

lO\
OOl\
O\
Oll\
l\

)
;
voi\
d
lOOOOO\
l\
OOll
(
Ty\
p\
e
*
lOOllOOO\
lll\

)
;
Type\

*
lOlOOO\
OOOO\

(
int
lOOOlOO\
lll
)
;
vo\
id
lOl\
OOOOOOl\

(
int
lOl\
OOOOOlO\

)
;
It\
erator
lOlOOOO\
lOO
(
)
;
priv\
a\
te
:
int
lO\
lOOOl\
OOl\

;
int
lOlOOOlOlO
;
int\

lOl\
OOO\
lOll
;
Type\

*
*
lOOl\
lOOlO\
O\
O
;
}
;
}
#include "../Common/zcta008.inl"
#endif //
