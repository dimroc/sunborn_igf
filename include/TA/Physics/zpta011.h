//---------------------------------------------------------------------------------
// File Name: zpta011.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lOOllOOOllll
#define lOOllOOOllll
#ifndef lOlOlOlOlO
#include "../Common/zcta009.h"
#endif //
#ifndef TA_PHYSICS_H
#include "Physics.h"
#endif //
#ifndef TA_DYNAMICOBJECT_H
#include "DynamicObject.h"
#endif //
na\
m\
espa\
ce
TA
{
cla\
ss\

St\
at\
i\
cObject
;
struct\

lll\
lOllOlOO
;
clas\
s
lOlOl\
llO\
l\
llO
{
publi\
c
:
typedef
lOlOl\
l\
l\
OOO\

<
lOlOOOOOO\
lO\
l
>
::
lOlOl\
llOOl
::
Iterator
lOllllOO\
OllO\

;
typ\
edef
lOlO\
lll\
OOO
<
Dyn\
amicObject
*
>
::
lO\
lOl\
llO\
Ol\

::
Iterato\
r\

llO\
OOO\
OOllOO
;
class\

Iter\
ator
{
pu\
blic
:
It\
era\
t\
o\
r
(
)
{
}
void\

op\
erat\
or
++
(
)
{
++
ll\
OOlll\
Olll\
l\

;
if
(
llO\
Oll\
lOl\
lll
.
AtEn\
d
(
)
)
{
++
llOOlll\
l\
O\
OOO
;
if
(
!
llOO\
llllO\
O\
O\
O
.
AtEnd
(
)
)
{
ll\
OOlll\
Olll\
l\

=
(
*
llOOllllO\
OOO\

)
.
lO\
l\
OlllO\
O\
Ol\
l\

.
lOl\
OOOOl\
O\
O
(
)
;
TA_ASSE\
RT
(
!
ll\
O\
Ol\
llO\
ll\
ll
.
At\
End\

(
)
)
;
}
}
}
bo\
ol
AtE\
nd
(
)
{
re\
turn\

(
llOO\
llllO\
OOO
.
AtE\
nd
(
)
&&
llOO\
lllOllll
.
At\
E\
n\
d
(
)
)
;
}
Collis\
i\
on
&
operat\
or
*
(
)
{
re\
tur\
n
*
llO\
OlllOll\
ll
;
}
Iter\
ator
&
ope\
ra\
tor\

=
(
cons\
t
Itera\
to\
r
&
lO\
OO\
lll\
lllO
)
{
ll\
OOlllO\
llll
=
lOOO\
l\
ll\
ll\
lO
.
llOOl\
llOll\
ll
;
llOOll\
llOOOO
=
lOO\
OllllllO
.
ll\
OOl\
lllOOO\
O\

;
retur\
n\

*
th\
is
;
}
bool
opera\
tor\

==
(
con\
st\

Iterat\
or
&
lOOOlll\
lllO\

)
co\
ns\
t
{
ret\
u\
r\
n
llO\
Oll\
ll\
OOOO
==
lOO\
Oll\
l\
ll\
lO
.
llOOll\
llOO\
OO
&&
llOO\
lll\
Ollll\

==
lO\
OOlll\
lll\
O\

.
llOOlllOll\
l\
l
;
}
void
Fina\
li\
s\
e
(
)
{
llOOlll\
Ollll\

.
Fina\
lise
(
)
;
llOOllllOOOO
.
Fin\
a\
l\
i\
se
(
)
;
}
pr\
iva\
t\
e
:
friend
cl\
ass
lOl\
OlllO\
lllO
;
typ\
ede\
f
lO\
l\
O\
lllOOO
<
lOl\
OOO\
OOO\
lO\
l
>
::
lOl\
O\
l\
ll\
O\
Ol
::
It\
erator
lOl\
l\
llOOOll\
O
;
typed\
ef
lO\
lOl\
llOOO
<
Co\
l\
lision
>
::
lOl\
O\
lllO\
Ol\

::
Iterator
Coll\
isionIt\
er\
ato\
r
;
Colli\
sionIt\
er\
ato\
r
llOO\
lll\
Ol\
ll\
l
;
lOlOl\
llOOO
<
lOl\
O\
O\
OOOOlOl
>
::
lOlOll\
lOO\
l
::
Iter\
at\
or\

llOOllllOOOO\

;
It\
erat\
or\

(
lOlOlll\
OO\
O
<
lO\
lO\
OO\
OO\
OlOl
>
::
lOl\
Oll\
lOOl
llOOlOlllO\
lO
)
{
ll\
OOllllOOOO
=
llO\
Ol\
Olll\
Ol\
O
.
lOlOOO\
OlOO
(
)
;
if\

(
!
ll\
O\
Oll\
llO\
O\
O\
O
.
At\
End
(
)
)
{
llOOl\
llO\
llll
=
(
*
llOOl\
l\
llOOOO
)
.
lOlOlll\
OOO\
l\
l\

.
lOl\
OO\
OOlOO
(
)
;
TA_\
ASS\
E\
RT
(
!
llOOl\
llO\
l\
lll
.
AtEn\
d
(
)
)
;
}
}
}
;
lOlOlllO\
lllO\

(
)
;
~
lOlOlll\
Olll\
O
(
)
;
Ite\
rato\
r
lOl\
OO\
OOlOO\

(
)
{
ret\
urn
It\
er\
ator
(
llOO\
lO\
lll\
O\
lO
)
;
}
lOllllO\
OOll\
O
lOll\
llO\
O\
lOOO\

(
)
{
retur\
n
llOO\
lOlllOlO
.
lOlOOO\
O\
l\
OO
(
)
;
}
llOOO\
OOO\
l\
lOO
ll\
OOOOOOl\
ll\
O
(
)
{
retu\
r\
n
ll\
OO\
lOlll\
lOO
.
lO\
lOO\
O\
OlOO\

(
)
;
}
int
lOl\
lOl\
l\
l\
OlOl
(
)
con\
st
{
re\
turn
ll\
O\
OlO\
llO\
lOO
;
}
int
lOllO\
ll\
llOOO
(
)
const\

{
retu\
rn
ll\
OOl\
O\
l\
l\
O\
lO\
l
;
}
voi\
d
lOllOll\
llllO
(
bo\
ol
llOO\
l\
lllOOO\
l
)
{
llOOlO\
ll\
O\
llO\

=
ll\
OOl\
l\
l\
l\
OO\
Ol\

;
}
bool
llOOOlO\
OOll\
O\

(
)
{
re\
turn
llOOlO\
llOl\
lO\

;
}
vo\
id
lOl\
lllOlOllO
(
bool
llO\
OlOlllOO\
O
,
int
lOl\
lO\
l\
lOOlll
)
;
vo\
id\

llOOllO\
OlOlO
(
)
;
bool
lO\
llOllOllOO\

(
)
;
bool
ll\
OOOOllOO\
OO
(
)
{
return
llO\
Ol\
Oll\
Olll\

;
}
bool
llO\
Oll\
O\
O\
l\
Ol\
l\

(
cons\
t\

lOl\
OO\
O\
OOOlOl\

*
lOl\
OO\
OO\
Ol\
OlO
)
;
#ifdef _DEBUG
vo\
id
llOOllOOl\
lOO
(
)
;
#else
vo\
id
llOOllOOllO\
O
(
)
{
}
#endif //
private
:
frien\
d\

clas\
s
lOllOlOllOlO
;
llll\
O\
l\
lOlOO
*
lOOll\
Oll\
lO\
lO
;
lO\
l\
O\
ll\
l\
OOO
<
lOlOO\
OOOOl\
Ol\

>
::
lO\
lOlll\
OOl\

ll\
OOlO\
l\
llOl\
O
;
lOlO\
l\
llOOO
<
Dy\
n\
amicOb\
ject\

*
>
::
lOl\
OlllOOl
llOOlOllllOO
;
int\

ll\
OO\
lOl\
lO\
lOO
;
int\

llOOlOllOl\
Ol
;
It\
er\
ator\

ll\
O\
O\
l\
lOO\
llOl
;
lOl\
l\
OlO\
llOl\
O\

*
lOll\
OlOl\
lOO\
l
;
bool
llOO\
lOllOllO
;
bo\
ol\

llOO\
lOllO\
lll
;
void
Initia\
l\
is\
e\

(
lll\
lOllOlOO
*
lOlOOllO\
O\
O\
Ol
,
lOllOlOll\
OlO\

*
llOOl\
l\
OO\
lO\
O\
O
)
;
void
Fina\
l\
i\
s\
e
(
)
;
vo\
i\
d
lO\
lOOO\
Ol\
OO\
Ol
(
lO\
lOOO\
O\
O\
OlOl\

&
lOlOOOOl\
lOl\
O
)
;
}
;
clas\
s\

lOll\
O\
l\
Oll\
OlO
{
pub\
lic
:
typede\
f\

lOlOl\
l\
lOOO
<
lOlOlllO\
ll\
lO
>
::
lOl\
Ol\
llOOl\

::
Iterator
Iterator\

;
typ\
e\
def
lOlO\
lllOOO
<
Col\
lis\
io\
n
>
::
lOl\
Ol\
llO\
Ol\

::
Itera\
tor
llOOllllO\
Ol\
O
;
ty\
pedef\

lOlOll\
lOOO\

<
lOl\
OOO\
OO\
OlOl
>
::
lOOlOOO\
ll\
ll
::
Iter\
ato\
r
lOl\
lllOOOllO
;
lOl\
lOlOl\
l\
Ol\
O
(
)
;
~
lOl\
lOlOllOlO
(
)
;
void
In\
itiali\
se
(
ll\
ll\
OllOlOO\

*
lO\
lO\
OllOOOOl\

,
int\

llOO\
l\
lOl\
OOOl
)
;
void
Fin\
alise
(
)
;
Co\
llision
*
llOOOO\
lllOl\
l
(
lOlOOOOOOlO\
l
&
lOlO\
OOOl\
lOl\
O\

,
const
Co\
ll\
ision
&
llOllllOlOl
)
;
void\

llOOlOOll\
Oll
(
Collision
&
ll\
OOllOllOOO\

,
cons\
t\

Collision\

&
llOl\
lllOlOl
)
;
void
llOOlOO\
ll\
Ol\
O\

(
lO\
lO\
OOOOOlOl\

&
lO\
l\
OO\
O\
Oll\
Ol\
O\

,
Co\
llision
&
llOl\
lllOl\
O\
l
)
;
vo\
id
lOllll\
OlOl\
Ol
(
)
;
void
ll\
OO\
lllO\
lO\
OO
(
lO\
lOO\
O\
OOOlOl
&
lOl\
OOO\
OllO\
l\
O
)
;
Iterator
lOlOOO\
OlOO
(
)
{
return\

ll\
OOllO\
lOllO
.
lOlOOOOlOO
(
)
;
}
lOllllOOO\
llO\

llOO\
lO\
OOOO\
O\
l
(
)
{
re\
t\
urn
llOOlOl\
l\
lOlO
.
lOlO\
OOO\
lOO
(
)
;
}
in\
t
lOll\
llOO\
lllO
(
)
{
return
llOO\
l\
lOOlll\
l
;
}
vo\
id
Cl\
e\
a\
r
(
bool
llOO\
l\
ll\
Ol\
OlO
=
false
)
;
vo\
i\
d
ll\
O\
Ol\
OO\
OlOOO
(
lOlOO\
OOOOlOl
&
lOlOO\
O\
Ol\
lOl\
O\

)
;
void\

lOlOOO\
lO\
OOOO
(
lOlO\
OO\
O\
OOlOl
&
lOlO\
OOO\
ll\
OlO
)
;
void
lOlOOO\
O\
lllOO
(
lO\
lOOO\
O\
OO\
lO\
l
&
lOl\
OOOOllO\
lO
)
;
lOlO\
OOOOOlOl
*
llOOl\
ll\
lO\
Oll\

(
)
{
re\
turn
llO\
Oll\
Ol\
OOO\
O
;
}
lOl\
O\
OOOOOlOl\

*
lOlOO\
OO\
Ollll
(
)
;
void\

lO\
lOO\
OOlOOOl\

(
lOlOOOOOO\
lOl
*
lOlO\
OOO\
Ol\
OlO
)
;
void
ll\
O\
Olll\
OOllO
(
lOlOOOOOO\
l\
Ol\

*
lOlOO\
OOO\
lOlO
)
;
lOl\
OO\
OOO\
O\
lO\
l
*
lOll\
ll\
OlOOlO
(
)
;
lOl\
OO\
OOOOlO\
l
*
lO\
llllOl\
OlOO\

(
)
;
pri\
va\
t\
e
:
fri\
end
cla\
s\
s
lOlOl\
llOl\
l\
lO\

;
llll\
O\
llOlO\
O
*
lOOllO\
lll\
OlO
;
lOlOl\
l\
lOO\
O\

<
Collis\
i\
on
>
llOOl\
lOlOOlO
;
lO\
lOl\
llOOO\

<
DynamicObj\
ect\

*
>
ll\
O\
O\
ll\
OlOOll
;
lOl\
Olll\
O\
O\
O
<
lOlOlllOlllO
>
llOOllOlO\
lOO\

;
lOlOll\
lOOO\

<
lOlO\
lllOlllO
>
::
lOlOl\
llO\
Ol
ll\
OOl\
l\
OlOllO
;
lOlO\
lllO\
OO
<
lOlOO\
OO\
OOlOl\

>
ll\
OOl\
lOl\
Ol\
O\
l\

;
lO\
l\
Oll\
lOOO
<
lOlOO\
OO\
OO\
lOl
>
::
lO\
O\
lOOOllll
llOOl\
Ol\
ll\
OlO\

;
lOlOl\
l\
lO\
OO
<
lOlOOOOO\
O\
l\
O\
l
>
::
lOOlOO\
Olll\
l
llOO\
ll\
lOl\
OOl
;
lOlOOO\
O\
OOlO\
l
*
llOO\
llOlOOO\
O
;
int
llO\
O\
llOO\
lllO
;
int
llO\
Oll\
OOlll\
l
;
lOl\
O\
l\
l\
l\
Ol\
ll\
O
*
llOO\
lllOO\
OOO
(
lOlOl\
llO\
lllO
*
llO\
Ol\
lOlll\
lO\

,
lO\
l\
OlllOl\
ll\
O
*
llOO\
llOl\
ll\
ll
)
;
lOl\
O\
lllOll\
lO
*
ll\
OO\
l\
ll\
OO\
OlO
(
Dy\
na\
m\
ic\
O\
bject
*
lOlOOOOOlOll
,
Dy\
n\
amicObject
*
lOlOOO\
OOllOO
)
;
voi\
d
llOOlllOOO\
O\
l
(
Dy\
na\
mi\
cObje\
c\
t
*
lOlO\
OOOOlOl\
l
,
lOlO\
lllOlllO
*
llOOlllOl\
lOO
)
;
void
llO\
O\
l\
l\
lO\
lllO
(
lOl\
OlllOlllO
&
llOOl\
ll\
OlOll
)
;
void
ll\
OOl\
lO\
OlOO\
l
(
Collision\

*
llO\
llOlOlOl
)
;
}
;
}
#endif //
