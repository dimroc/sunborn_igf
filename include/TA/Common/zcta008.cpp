//---------------------------------------------------------------------------------
// File Name: zcta008.cpp
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

#include "../Common/zcta004.h"
#ifndef lOOOOlOOll
#include "../Common/zcta012.h"
#endif //
na\
mes\
pace
TA
{
void
lllOlOO\
OO\
O
::
Ini\
ti\
alise\

(
cons\
t
Char
*
lllO\
l\
O\
OOOl
,
co\
n\
st
Version
&
lllO\
lOO\
O\
lO\

)
{
lllOl\
O\
OOl\
l
=
lll\
O\
l\
OOOOl
;
lllOl\
OOlOO\

=
ll\
lOlOOO\
l\
O
;
}
Ve\
rsion
ll\
lO\
lO\
OOOO
::
Seria\
li\
se
(
Se\
ri\
al\
is\
er
&
lOOOl\
Ol\
lOl
)
const
{
if
(
lO\
OOlO\
ll\
Ol
.
lllOl\
OO\
lO\
l\

(
)
)
{
int
lO\
OOllOOOl
;
for
(
lOOOllO\
O\
Ol
=
0
;
lllOl\
OOO\
ll
[
lOOOllOOOl
]
;
lOO\
OllOO\
Ol
++
)
{
ch\
ar\

lllOlOOl\
lO\

=
lll\
OlOOO\
ll
[
lOOOllO\
OO\
l\

]
;
lOOO\
lO\
ll\
Ol
.
Seria\
lise
(
lll\
O\
lOO\
llO
)
;
}
int
lllO\
l\
OO\
l\
ll
=
0
;
if
(
lOO\
OllOOO\
l
&
4
)
lOOO\
lOll\
Ol
.
lllOl\
OlOOO
(
&
lllOlOO\
ll\
l
,
lO\
OOll\
OO\
O\
l
&
4
)
;
lOOO\
l\
O\
llO\
l
.
ll\
lOlOlOOO
(
(
void
*
)
&
lllOlOOlOO\

,
size\
o\
f\

(
lll\
OlOO\
l\
OO\

)
)
;
ret\
urn
ll\
lO\
lO\
OlO\
O
;
}
else
{
int
lOOO\
llO\
O\
Ol
;
for
(
lO\
OO\
l\
lO\
OO\
l
=
0
;
lll\
OlOOO\
ll\

[
lOOO\
llOOO\
l
]
;
lOOOl\
l\
OOO\
l
++
)
{
ch\
a\
r
lll\
OlOOll\
O
;
lOOO\
l\
OllO\
l\

.
Se\
ria\
lise\

(
lllOlOOllO
)
;
if
(
lllO\
lOOllO
!=
lllOlO\
OOl\
l
[
lOOOllOO\
O\
l
]
)
{
lOOOOOOll\
O\

(
0
,
"FileHeader::Serialise. The header name doesn't match")
;
re\
tu\
r\
n
ll\
lO\
lOOlO\
O\

;
}
}
in\
t
lll\
Ol\
OOlll
=
0
;
if
(
lO\
O\
O\
l\
l\
OO\
Ol
&
4
)
lO\
OOlOllOl
.
lll\
O\
lOlOOO
(
&
lllOlOOlll
,
lO\
OO\
l\
lOOOl
&
4
)
;
if\

(
lllOlO\
Olll
!=
0
)
{
lOOOO\
O\
O\
llO\

(
0
,
"FileHeader::Serialise. The header version doesn't match")
;
retu\
r\
n
ll\
l\
Ol\
O\
OlOO
;
}
Vers\
i\
on
lll\
OlO\
OO\
lO
;
lOO\
OlOllOl\

.
Seri\
a\
li\
se
(
lllOlO\
O\
OlO
)
;
retur\
n\

lllO\
l\
OOO\
lO
;
}
}
}