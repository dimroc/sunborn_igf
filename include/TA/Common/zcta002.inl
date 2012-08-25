//---------------------------------------------------------------------------------
// File Name: zcta002.inl
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#ifndef TA_DEBUG_H
#include "Debug.h"
#endif //
#ifndef TA_TYPES_H
#include "Types.h"
#endif //
#ifndef TA_MEMORYMGR_H
#include "MemoryMgr.h"
#endif //
na\
m\
e\
sp\
ace
TA
{
template
<
cl\
ass
Type
,
bool
lO\
OlllOOlO
>
lOO\
O\
OlOllO
<
Typ\
e
,
lO\
Ol\
llOO\
lO
>
::
lO\
O\
OOl\
Ol\
lO\

(
)
{
lOl\
O\
OO\
l\
OOl
=
0
;
lOlOOOlOl\
O
=
0
;
lO\
lOO\
Ol\
lOO
=
0
;
lOlO\
OOlO\
ll
=
0
;
}
templa\
t\
e
<
clas\
s
Ty\
pe
,
bool\

lOOlll\
OOlO
>
lO\
OOOlOllO
<
Type\

,
lO\
Oll\
lOO\
l\
O
>
::
~
lOOOOlOll\
O\

(
)
{
Fin\
alise
(
)
;
}
tem\
plat\
e
<
class
Type
,
bool\

lOOll\
lOOlO
>
vo\
i\
d
lOOOOlOllO\

<
Type\

,
lOO\
lll\
OO\
l\
O
>
::
Initialise
(
int
lOOl\
ll\
Olll\

,
int\

lOOllll\
OOO
,
int
lO\
Ol\
l\
llOOl
)
{
if
(
lO\
l\
OOO\
ll\
OO
)
{
lO\
OOOO\
OllO
(
0
,
"Array::Initialise. Already initialised.")
;
Fi\
n\
al\
ise
(
)
;
}
if\

(
lOOllllO\
OO
<=
0
)
lOOll\
llO\
OO
=
1
;
lOOOOO\
O\
l\
lO
(
lOOllllO\
O\
l
!=
0
,
"Array::Initialise. nBlockSize must be be non-zero.")
;
lOlOOO\
lO\
l\
l\

=
lOOl\
lllO\
Ol
;
if
(
lOOll\
l\
lOOl
>=
0
)
{
int
lOlOOOl\
lOl
=
0
;
}
lOlOOOlOlO
=
lOOllllO\
OO
;
lOlO\
OOl\
OOl
=
lOOlll\
Oll\
l
;
if
(
lO\
lOO\
OlO\
l\
O
<
lOlO\
OOlOOl
)
{
lOOOOOO\
llO\

(
0
,
"Array::Initialise. nInitialSize is > nInitialMaxSize.")
;
lOlOOOlOlO\

=
lO\
l\
OO\
OlOOl
;
}
lOOO\
l\
lllO\
O
(
lOl\
OOOllOO
,
Type
,
lOl\
O\
OOlOlO
)
;
}
templ\
ate
<
clas\
s
Type
,
bo\
ol
lOOlll\
OOlO
>
void\

lOOO\
Ol\
OllO
<
Typ\
e
,
lO\
O\
l\
llO\
OlO\

>
::
Fin\
alis\
e\

(
)
{
if
(
lOlOOOll\
OO\

)
{
lOlOOOll\
lO
(
lOlOOO\
l\
l\
O\
O
,
Type
)
;
lOlOO\
Ol\
l\
OO
=
0
;
}
lOlOOO\
lOOl
=
0
;
lOlOOOlO\
lO
=
0
;
lO\
lOOOl\
Ol\
l
=
0
;
}
temp\
late
<
class\

Type
,
bool
lOOl\
llOOlO\

>
void
lO\
OOOl\
Ol\
lO
<
Type
,
lOOlllO\
OlO
>
::
Cl\
e\
ar\

(
)
{
lO\
lOOOlO\
Ol
=
0
;
}
temp\
lat\
e
<
class
Type
,
bool
lOOlllOOl\
O
>
voi\
d\

lOOO\
Ol\
OllO
<
Type
,
lOOl\
l\
lOOl\
O
>
::
lOO\
llllOl\
O
(
)
{
for
(
u8\

*
lO\
OlllOOOO
=
(
u8
*
)
&
lO\
l\
OOOll\
O\
O
[
0
]
;
lO\
O\
lllOOOO
<
(
u8
*
)
&
lOlOOOll\
O\
O\

[
lO\
lOOOlOOl
]
;
lOOlll\
OOOO
++
)
*
lOO\
ll\
lOOOO
=
0
;
}
templ\
a\
te\

<
class
Ty\
pe
,
bool
lOO\
lllO\
Ol\
O
>
Typ\
e
&
lOOOOlOllO
<
Type
,
lOOlllO\
O\
lO
>
::
opera\
to\
r
[
]
(
int
lO\
OOlOO\
lll\

)
{
TA\
_\
ASS\
ER\
T
(
lO\
lOOO\
llOO\

)
;
lO\
OOOOOllO
(
lOO\
OlOO\
l\
ll
>=
0
,
"Array::operator []. Index out of bounds.")
;
lOOOOOOllO
(
lOOOlO\
Ol\
ll
<
lO\
l\
OOOl\
OO\
l
,
"Array::operator []. Index out of bounds.")
;
ret\
u\
r\
n
lOlO\
OO\
llOO
[
lOO\
Ol\
O\
Ol\
ll
]
;
}
templat\
e
<
cl\
a\
s\
s
Typ\
e
,
bool
lOOlllOO\
lO
>
con\
st
Ty\
pe\

&
lO\
O\
OOlOll\
O
<
Type
,
lOOlllO\
Ol\
O\

>
::
operator
[
]
(
int
lOOOl\
OO\
lll\

)
co\
nst\

{
TA_ASSER\
T\

(
lOlO\
O\
Ol\
lO\
O
)
;
lOOOOO\
OllO
(
lO\
O\
OlOO\
ll\
l
>=
0
,
"Array::operator []. Index out of bounds.")
;
lO\
O\
OO\
OOllO
(
lOO\
O\
lOOll\
l
<
lOlOO\
O\
l\
OOl
,
"Array::operator []. Index out of bounds.")
;
retu\
rn
lOlOOO\
llOO
[
lOOOlO\
Ol\
l\
l
]
;
}
te\
mpla\
t\
e\

<
clas\
s
Typ\
e
,
boo\
l\

lOO\
lll\
OOl\
O
>
int
lOOOOlOl\
lO
<
Type
,
lO\
Ol\
l\
lO\
O\
lO
>
::
lOOO\
l\
l\
lOOl
(
)
con\
st
{
return
lOlOOO\
lOOl
;
}
te\
m\
pl\
ate
<
cla\
ss
Ty\
pe
,
bool
lOOlllOOl\
O
>
int
lOOO\
Ol\
Ol\
lO\

<
Ty\
pe
,
lO\
OlllOOlO\

>
::
lOOl\
lllOll
(
)
co\
nst
{
ret\
u\
rn\

lOlO\
OOl\
OlO
;
}
te\
mplate\

<
cla\
ss
Typ\
e\

,
bool
lOO\
l\
llOO\
l\
O
>
Type
&
lO\
O\
O\
OlOllO
<
Ty\
pe
,
lO\
O\
ll\
l\
O\
OlO
>
::
lOOOlllOO\
O
(
)
{
TA_ASSER\
T
(
lOlOOOllOO\

)
;
TA\
_ASS\
ERT
(
lOl\
OO\
OlO\
Ol
<=
lOl\
O\
OOlOl\
O
)
;
if
(
lO\
l\
O\
OO\
lOOl
==
lOl\
OOOlOlO\

)
{
lO\
lOO\
O\
llll
(
lOO\
lllOO\
l\
O
,
"Array::Append. The static array is already full")
;
Ty\
pe
*
lOl\
OOlOOOO
;
int\

lOl\
OOOO\
OlO
=
lOlOOOlO\
lO
;
if\

(
lOl\
OOOlOll
<
0
)
lOlOO\
OOOlO
*=
2
;
else
lOlOOOOO\
l\
O\

+=
lOlOOOl\
Oll
;
lOOOllllOO
(
lOlOOlO\
OO\
O
,
Type
,
lOlO\
OOOOlO\

)
;
for
(
int
lOOOll\
O\
O\
O\
l\

=
0
;
lOOOl\
lOOOl\

<
lOlO\
OOl\
OO\
l
;
lOO\
Ol\
lO\
OOl
++
)
lOlO\
OlOO\
OO
[
lO\
OOl\
lOO\
Ol
]
=
lOlOOO\
ll\
OO\

[
lO\
OOll\
OO\
Ol
]
;
lOlOOOlllO
(
lOlOOO\
llOO
,
Type
)
;
lOl\
OO\
O\
llOO
=
lOlO\
OlOOO\
O\

;
lOl\
OOOlO\
l\
O
=
lO\
l\
OOOOO\
lO
;
}
lOlO\
OO\
lO\
Ol\

++
;
return
lOlOOO\
llO\
O
[
lO\
l\
OOOlOOl\

-
1
]
;
}
templ\
ate\

<
class
Type\

,
bool
lOOlllOO\
lO
>
vo\
i\
d\

lOOOOlOl\
lO
<
Type
,
lO\
O\
lllO\
OlO\

>
::
lOOOll\
lOOO\

(
co\
n\
st
Type
&
lOOlllllOO
)
{
lO\
O\
OlllO\
O\
O
(
)
=
lOO\
l\
l\
lll\
OO
;
}
temp\
l\
ate
<
cl\
as\
s
Typ\
e
,
boo\
l\

lOOll\
l\
OOlO\

>
voi\
d
lOOOO\
l\
O\
l\
lO
<
Ty\
pe
,
lOOl\
llOOl\
O
>
::
lOOll\
ll\
lO\
l\

(
int
lOOl\
lllllO
)
{
TA_ASSERT\

(
lOlOOOllO\
O\

)
;
TA_\
A\
SSE\
RT
(
lOl\
O\
OOl\
OO\
l
<=
lOlO\
OO\
lOl\
O
)
;
if\

(
lOlOO\
OlOO\
l\

+
lO\
O\
llllllO
>
lOlOOO\
lOlO\

)
{
lOlOOOl\
lll
(
lO\
Oll\
lO\
OlO
,
"Array::IncrementSize. The maximum size of the static array has been exceeded")
;
Type
*
lOlOOlOO\
OO
;
in\
t
lOlO\
O\
OOOl\
O
=
lOlOOO\
lOlO
;
if
(
lO\
lOO\
O\
lO\
ll
<
0
)
{
while
(
lOlOO\
OOOlO
<
lOlOOOlOl\
O
+
lO\
Ol\
l\
llllO
)
lO\
lOO\
O\
O\
O\
lO
*=
2
;
}
else
{
lOlOO\
OOOlO
+=
(
lOO\
ll\
l\
ll\
l\
O\

%
lOlOOOlOll
)
+
lOl\
O\
O\
Ol\
Ol\
l
;
}
lO\
OOllllOO\

(
lOlOOlO\
O\
OO
,
Type\

,
lOlO\
O\
O\
OOlO
)
;
fo\
r\

(
int
lOOOllOO\
Ol
=
0
;
lOOOl\
lOO\
O\
l\

<
lOlOOOlO\
O\
l
;
lOOO\
l\
lO\
OO\
l
++
)
lO\
lO\
Ol\
OOOO\

[
lOO\
OllOOOl
]
=
lOlOOO\
llOO
[
lOOOl\
lOO\
Ol
]
;
lO\
lOOOl\
l\
lO
(
lOl\
OO\
OllO\
O
,
Type\

)
;
lOlOOOllOO
=
lOlOOlOOO\
O\

;
lOl\
O\
O\
Ol\
OlO\

=
lO\
lOOOO\
OlO
;
}
lOl\
OOOlOOl
+=
lOOl\
lllll\
O
;
}
temp\
late
<
class\

Type
,
bool
lOOlllOOlO
>
vo\
id
lOOOOlOllO\

<
Ty\
pe
,
lO\
Ol\
llOOlO\

>
::
lOOl\
l\
ll\
l\
ll
(
int
lOO\
Ol\
OO\
lll
)
{
TA_\
A\
SSERT
(
lO\
lOO\
OllOO\

)
;
lOOO\
OOOllO
(
lOOOlO\
Ol\
ll
>=
0
,
"Array::Remove. Index out of bounds.")
;
lOOO\
OOO\
llO
(
lOOOl\
OOlll
<
lO\
lOO\
OlOOl
,
"Array::Remove. Index out of bounds.")
;
for
(
;
lOO\
OlOOlll
<
lOl\
OOOlO\
O\
l
-
1
;
lOO\
OlOOlll\

++
)
lOlOOOl\
lOO
[
lOO\
O\
lOOl\
ll\

]
=
lOlOO\
OllOO
[
lOOOl\
O\
Olll
+
1
]
;
lOlOOOlOO\
l
--
;
TA_ASSERT\

(
lOl\
O\
OO\
lO\
Ol
>=
0
)
;
if
(
lOlOOOlOOl
>
0
&&
lOlOOOlOl\
O
-
lOl\
OO\
OlOO\
l
>
lOlOOO\
lO\
ll\

)
{
}
}
tem\
pl\
a\
te
<
cla\
ss
Ty\
pe\

,
boo\
l
lOOll\
l\
OO\
lO
>
Type
&
lOOOOlOllO
<
Type
,
lOOll\
l\
OOlO
>
::
lO\
l\
OOOO\
OOO
(
in\
t\

lOOOlO\
Olll
)
{
TA\
_ASSERT\

(
lO\
OlllOO\
lO
)
;
TA_ASSERT\

(
lOl\
OO\
O\
llOO\

)
;
lOlOOlO\
OOl
(
"Array::Insert. Unimplemented")
;
retu\
rn
lO\
lOOOllOO
[
0
]
;
}
temp\
l\
ate
<
cl\
ass\

Type
,
bool
lO\
O\
lllOOlO\

>
vo\
id
lOOOO\
l\
Ol\
lO\

<
Type
,
lOOlllOOlO
>
::
lOlO\
OO\
OO\
Ol
(
in\
t
lOlO\
OOOOlO\

)
{
lOOOOOOl\
lO\

(
lO\
O\
ll\
lOO\
lO\

,
"Array::ReallocateIfNecessary. Not allowed for static arrays.")
;
if\

(
lOlOOOOO\
lO\

>
lO\
l\
OO\
Ol\
OlO
)
{
lOlOO\
OlOlO\

=
lOlOOOOOlO
;
Type
*
lOlOOlO\
OOO
;
lOOO\
ll\
llOO
(
lO\
lOO\
l\
OOO\
O
,
Type
,
lOlOO\
OlOl\
O
)
;
fo\
r\

(
int
lO\
OO\
ll\
OOOl
=
0
;
lOOOllOOOl
<
lO\
lOOOlOOl
;
lOOOll\
OOOl
++
)
lO\
lOO\
lOO\
O\
O
[
lOOOllOO\
Ol
]
=
lOl\
O\
OO\
l\
lOO
[
lOOOl\
lO\
OOl
]
;
lOlOOOll\
lO\

(
lOl\
OO\
Ol\
lO\
O
,
Ty\
p\
e
)
;
lOlOOOl\
l\
O\
O\

=
lOl\
O\
OlO\
OOO
;
}
}
templ\
at\
e
<
class
Type
,
bool
lOOlllOOlO
>
int\

lO\
O\
OOlOll\
O\

<
Type\

,
lOO\
l\
llOO\
lO\

>
::
lOlOO\
OOOll\

(
)
{
retur\
n\

si\
zeof
(
Ty\
p\
e
)
;
}
templat\
e
<
cl\
a\
s\
s
Type
,
boo\
l
lO\
O\
lllOOlO\

>
type\
na\
m\
e
lOOOO\
l\
OllO
<
Type
,
lO\
Oll\
l\
O\
O\
lO
>
::
Ite\
ra\
tor
lOOOO\
l\
O\
llO\

<
Type\

,
lOOlllOOl\
O
>
::
lOlOOOOlOO
(
)
{
Iterat\
o\
r\

lOlOOlOOlO
;
lOlOOlOOlO
.
In\
it\
ial\
ise
(
&
lOlOO\
Ol\
lOO
[
0
]
,
lOlO\
OOlOOl
)
;
return
lOlOOlOO\
l\
O
;
}
tem\
pl\
ate
<
class
Ty\
pe
,
boo\
l
lOOl\
l\
lO\
O\
lO
>
void
lO\
OO\
Ol\
OllO
<
Ty\
pe
,
lOOlllO\
OlO
>
::
lOlOO\
OOl\
Ol
(
)
{
lOl\
OOOlOlO
=
lOlOO\
OlOOl\

;
if\

(
lOl\
OOOlOlO
<=
0
)
lO\
l\
O\
OOlOlO
=
1
;
Ty\
pe
*
lO\
l\
OOlO\
OOO\

;
lOO\
Ollll\
OO
(
lOlOOlOOOO
,
Type\

,
lOlO\
OOlOlO
)
;
for
(
int
lOOOll\
OOO\
l
=
0
;
lOO\
OllOOOl
<
lOlOOOl\
OOl\

;
lO\
O\
O\
llOO\
Ol
++
)
lO\
lO\
O\
lOOOO
[
lO\
OOllO\
OOl\

]
=
lOl\
OOOllOO\

[
lOO\
Oll\
OO\
Ol
]
;
lOlOOO\
lllO
(
lOlOOOll\
O\
O\

,
Type
)
;
lOl\
OO\
OllOO
=
lOlOO\
l\
OOOO
;
}
template
<
clas\
s
Typ\
e
,
bool
lOOl\
llO\
Ol\
O
>
bo\
ol
lO\
OO\
OlOll\
O
<
Type\

,
lOOlllOO\
l\
O
>
::
lOlOOOO\
ll\
O\

(
const
Ty\
pe
*
lOlOO\
O\
O\
lll
)
const
{
int
lO\
lOO\
lOOll
=
(
int
)
(
(
u8
*
)
lOlOOOOll\
l
-
(
u8
*
)
lO\
lOO\
OllO\
O\

)
;
return
(
lO\
lOOlO\
Oll
>=
0
&&
lOlOOl\
OOl\
l\

<
lOlOOOl\
OlO
*
(
int\

)
si\
z\
eof
(
Type\

)
)
;
}
te\
mplate
<
cl\
ass
Type
,
bool
lO\
OlllOOlO
>
int
lOOOO\
l\
OllO
<
Type
,
lOOlllOOl\
O
>
::
lOlOOO\
lOOO\

(
co\
nst
Type
*
lOlOOO\
O\
lll
)
con\
st
{
TA_\
A\
SS\
ERT
(
lO\
lOOO\
OllO
(
lOlO\
OOOl\
ll
)
)
;
in\
t\

lOl\
OOlOO\
ll
=
(
int
)
(
lOl\
OOOOlll
-
lOlOO\
O\
l\
lOO
)
;
ret\
urn
lOlOOlO\
O\
ll
;
}
}
