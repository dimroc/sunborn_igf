//---------------------------------------------------------------------------------
// File Name: zcta012.h
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef lOOOOlOOll
#define lOOOOlOOll
#ifndef TA_TYPES_H
#include "Types.h"
#endif //
#ifndef llOllllOll
#include "../Common/zcta015.h"
#endif //
#ifndef TA_DEBUG_H
#include "Debug.h"
#endif //
#ifndef lOlOOlOlOO
#include "../Common/zcta007.h"
#endif //
names\
pace
TA
{
#define lOOllOlOOOO(lOOlOllOOlO)
#define lOOllOlOOOl(lOOlOllOOlO)
class
TA\
C\
OMMON_CL\
ASS
Seri\
ali\
ser
{
pub\
lic\

:
Seriali\
s\
er
(
)
{
lOO\
l\
lOlO\
O\
lO\

=
0
;
}
Se\
ria\
lis\
e\
r
(
lOOOO\
OOl\
OOl
&
lO\
Ol\
lOl\
OO\
ll
)
{
lOOl\
lOl\
OOlO\

=
0
;
Initi\
ali\
se\

(
lOOllOlOO\
l\
l
)
;
}
~
Serialis\
er\

(
)
{
Fi\
nali\
s\
e
(
)
;
}
void
Ini\
tiali\
se\

(
lOOOOO\
OlOOl
&
lOO\
llO\
lOOll\

)
{
TA\
_\
AS\
SE\
RT
(
lOOl\
lOl\
OO\
l\
O\

==
0
)
;
lO\
OllOlOOlO\

=
&
lOOllOlOOll\

;
}
void\

Finalis\
e
(
)
{
lOO\
l\
l\
OlOOlO
=
0
;
}
tem\
pla\
t\
e
<
cla\
ss
Type
>
vo\
id\

Seri\
ali\
s\
e\

(
Typ\
e
&
lOOll\
l\
llOO
)
{
TA_\
ASSERT
(
lOOllOlOOlO\

)
;
lOOll\
OlOOl\
O\

->
lO\
lO\
OllO\
lO
(
&
lOOll\
lll\
O\
O
,
size\
of
(
Typ\
e
)
)
;
}
void
lll\
O\
lO\
l\
OOO
(
void
*
lOOlllOOO\
O
,
int
lOOlll\
OllO
)
{
TA_ASSER\
T
(
lOOll\
O\
l\
OOlO
)
;
lO\
Ol\
lOlOOlO
->
lO\
lOOllOlO
(
lOO\
ll\
lO\
O\
OO
,
lO\
Olll\
Oll\
O\

)
;
}
template
<
cl\
a\
ss\

Type\

>
vo\
id
lOOOlOll\
ll
(
Ty\
pe
&
lOOll\
O\
lOlO\
O
)
{
int
lOOll\
lOllO
=
lO\
O\
ll\
OlOlO\
O\

.
lOOOlllOOl
(
)
;
Serial\
ise\

(
lO\
Olll\
OllO
)
;
if
(
lOOOll\
OOOO
(
)
)
lO\
OllOlOlOO
.
Initialise\

(
lOOlllOll\
O
,
lOOlllOllO\

)
;
ll\
lOl\
O\
lOOO
(
&
lO\
OllOlOl\
OO\

[
0
]
,
lOOlllO\
llO
*
lOO\
llOlOlOO
.
lOlO\
OOOOll
(
)
)
;
}
tem\
p\
late\

<
class
Type
>
voi\
d\

lOOllOlOlOl
(
Type
&
lOOll\
OlO\
l\
OO
)
{
in\
t\

lOOlllOl\
l\
O
=
lOOll\
OlO\
l\
O\
O
.
lOOOlllOOl
(
)
;
Se\
r\
ialis\
e
(
lO\
O\
lllOllO\

)
;
if
(
lOOOllOOO\
O\

(
)
)
lO\
O\
ll\
OlOlO\
O
.
In\
itialise\

(
lOOlllOllO
,
lOOl\
llO\
llO
)
;
for\

(
int
lO\
OOll\
OOO\
l
=
0
;
lOO\
Ol\
lO\
O\
Ol\

<
lOOll\
l\
Ol\
lO
;
lOOOl\
l\
OO\
O\
l
++
)
lO\
Oll\
OlOlO\
O
[
lOOOllOOOl\

]
.
Serialise\

(
*
thi\
s
)
;
}
tem\
plate\

<
class
Type
>
void
lOOl\
lO\
lOllO
(
Typ\
e
&
lOOllOlOlll
)
{
in\
t
lOOll\
l\
OllO
=
lOOllOl\
Olll
.
lOOOll\
lOOl\

(
)
;
Ser\
i\
alis\
e
(
lOOl\
l\
lOl\
lO
)
;
if
(
lllO\
lOOlOl
(
)
)
{
clas\
s
Type
::
Iterat\
o\
r
lO\
lOOlO\
OlO
;
lO\
lO\
OlOOlO
=
lO\
OllOlOlll
.
lOlO\
OOO\
lO\
O\

(
)
;
fo\
r
(
;
!
lO\
l\
OOl\
O\
Ol\
O
.
AtEnd
(
)
;
++
lO\
lOOl\
OOlO
)
(
*
lO\
lOOlOOlO
)
.
Seri\
ali\
se
(
*
th\
is\

)
;
}
else
{
for
(
int\

lOO\
OllOOOl
=
0
;
lOOOllOOOl\

<
lOOll\
lOllO
;
lOOOllO\
O\
Ol
++
)
lOOllOl\
O\
l\
ll
.
lOO\
Oll\
lOOO
(
)
.
Ser\
i\
a\
lis\
e
(
*
th\
is\

)
;
}
}
boo\
l
lO\
OO\
ll\
OOOO
(
)
{
TA_ASSER\
T
(
lO\
OllO\
lOOlO
)
;
retu\
r\
n\

lO\
Oll\
OlOO\
l\
O
->
lOO\
OllO\
OOO\

(
)
;
}
boo\
l
lllOlO\
OlOl
(
)
{
TA_A\
SSERT\

(
lOO\
l\
lOlO\
OlO
)
;
return\

lOOl\
lOlOOl\
O
->
ll\
l\
Ol\
OO\
l\
Ol\

(
)
;
}
pri\
va\
te
:
lO\
OOO\
OOlOOl
*
lO\
OllOlOOlO\

;
}
;
}
;
#endif //