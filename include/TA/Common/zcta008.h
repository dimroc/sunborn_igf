//---------------------------------------------------------------------------------
// File Name: zcta008.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lOOOOOOllOl
#define lOOOOOOllOl
#ifndef TA_DEBUG_H
#include "Debug.h"
#endif //
#ifndef TA_MEMORYMGR_H
#include "MemoryMgr.h"
#endif //
na\
mesp\
a\
c\
e\

TA
{
templat\
e
<
cl\
ass\

Ty\
p\
e\

>
cl\
ass
TAC\
O\
M\
MO\
N\
_\
C\
LASS
List
{
prot\
ec\
ted
:
st\
r\
uct
lO\
Ol\
lOOOOO\

;
public
:
cla\
s\
s
Iterator
{
public
:
It\
e\
ra\
tor\

(
)
{
lOOOOOO\
lllO
=
0
;
}
~
Iterator\

(
)
{
Finalise
(
)
;
}
void\

opera\
tor
++
(
)
{
if
(
lOOOOO\
Olll\
O
)
lOOOOO\
O\
lllO
=
lOOOOOOlll\
O\

->
lOOOOOOlll\
l\

;
}
Type
&
oper\
at\
or
*
(
)
{
retu\
rn
lO\
OOOOOlllO\

->
lOOOOO\
lOO\
OO\

;
}
bo\
ol
AtE\
nd
(
)
{
re\
t\
urn
lOO\
OOOOl\
ll\
O
==
0
;
}
pri\
v\
a\
te
:
friend
cl\
ass
List
;
void\

Ini\
ti\
a\
lis\
e\

(
lOOll\
OOOOO
*
lOO\
l\
l\
l\
OO\
Ol
)
{
lOOOO\
O\
OlllO
=
lOOll\
lO\
O\
Ol\

;
}
void
Fin\
a\
li\
se\

(
)
{
lO\
OOOOOlllO
=
0
;
}
lOOllO\
OO\
OO
*
lO\
OO\
OOOlllO\

;
}
;
cl\
ass
lOOOOO\
lOOOl\

{
publi\
c\

:
lOOO\
OOlO\
OO\
l
(
)
{
lOOOOOOl\
llO
=
0
;
}
~
lO\
OO\
OOlOO\
Ol\

(
)
{
Finali\
s\
e
(
)
;
}
void
operat\
or
++
(
)
{
if
(
lOOO\
OOOl\
llO
)
lOOOOOO\
ll\
l\
O
=
lOOOOOO\
lllO
->
lOOOOOO\
ll\
l\
l
;
}
co\
nst
Ty\
pe
&
op\
e\
rat\
or
*
(
)
{
ret\
urn
lO\
OOOOOlll\
O
->
lOOOOOlOO\
OO
;
}
bool
At\
E\
n\
d
(
)
{
re\
turn\

lO\
OOOOO\
ll\
l\
O
==
0
;
}
priva\
te
:
fr\
ien\
d
clas\
s\

List
;
voi\
d
Initial\
i\
s\
e
(
lOOllOOO\
OO
*
lOOlllO\
OO\
l
)
{
lOOOOOOl\
l\
lO\

=
lOOl\
llOOO\
l\

;
}
vo\
id
Fin\
a\
lise
(
)
{
lO\
OOO\
OO\
lllO\

=
0
;
}
cons\
t
lOOllOO\
OO\
O\

*
lO\
OOO\
OOll\
l\
O
;
}
;
List
(
)
;
~
List
(
)
;
void\

Fi\
na\
lise
(
)
;
voi\
d
Cle\
ar
(
)
;
int
lOOO\
l\
llOOl
(
)
co\
nst\

;
bool
lOO\
OOO\
lO\
OlO
(
)
co\
nst
;
Typ\
e
&
op\
era\
t\
or
[
]
(
int
lOO\
OlOO\
lll
)
;
const\

Typ\
e
&
operator
[
]
(
int
lOO\
Ol\
OO\
l\
ll
)
const
;
Type\

&
lOO\
Ol\
llO\
OO\

(
)
;
vo\
i\
d
lOO\
O\
ll\
l\
O\
OO\

(
const
Type
&
lO\
O\
O\
l\
O\
O\
ll\
O\

)
{
lOO\
O\
lll\
O\
OO
(
)
=
lOOOlOOllO
;
}
void
lOO\
OOOlOOll\

(
const
Type
&
lOOOlOOllO
)
;
void
lOOlllll\
ll
(
co\
nst
Typ\
e\

*
lOOOO\
O\
lOl\
OO\

)
;
Iterat\
o\
r
lOl\
OOOOl\
OO
(
)
{
Iterator
lOlOO\
lOOlO
;
lO\
l\
OOlO\
OlO\

.
Init\
i\
a\
l\
is\
e\

(
lOOOO\
OlOlOl
)
;
return
lO\
lOOlOOlO\

;
}
lOOOOOlOOO\
l
lOOOOO\
lOllO
(
)
co\
nst\

{
lOO\
OOOlOO\
Ol
lOlOO\
lOOlO
;
lO\
lOOlOO\
lO\

.
Initiali\
se
(
lOOOO\
OlOlOl
)
;
return\

lOlOOlOO\
lO
;
}
pr\
otect\
ed\

:
st\
ruct
lOO\
llO\
OOO\
O
{
Type
lOOOOOlOO\
OO
;
lOO\
llOO\
OOO
*
lO\
O\
OOOO\
ll\
ll
;
}
;
lO\
Ol\
l\
OOOOO
*
lOOOOOlO\
lOl\

;
lOOllO\
O\
O\
O\
O\

*
lOOOOO\
l\
O\
lll
;
}
;
}
#include "../Common/zcta003.inl"
#endif //