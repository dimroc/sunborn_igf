//---------------------------------------------------------------------------------
// File Name: zpta002.inl
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

{
TA\
_ASSE\
RT
(
llOOO\
lOOlO\

->
lOllOl\
lOl\
l
<
lO\
lllOOlOOl
)
;
int
lOO\
Ol\
lO\
OO\
l
;
int
lOlllO\
OlOl\
O\

[
2
]
=
{
0
,
0
}
;
in\
t\

lOlll\
OOllO
=
0
;
for
(
lO\
lllOOllO
=
0
;
lO\
l\
llOOllO
<
2
;
lOlll\
OOllO
++
)
{
bool
lOlll\
O\
OlO\
ll
=
false
;
TA_ASSERT
(
lOll\
lOOl\
OOl\

<
32
)
;
TA_\
ASS\
ER\
T
(
ll\
OOOlOOlO
->
lO\
ll\
OllOll
<
32
)
;
int
lOlllO\
OllOO
=
0
;
{
co\
nst
Ve\
c3
&
lOlll\
OOllOl
=
lOlllOOll\
lO
[
lOl\
ll\
OOll\
O\

]
;
flo\
a\
t
lOlllO\
ll\
O\
O\

=
lOlllO\
Olll\
l\

(
lOlllO\
OllOl\

)
;
if\

(
lO\
lllOllOO
<=
lOll\
lO\
lOOOO\

.
lO\
lOlOOlOOO\

&&
lOlllOllO\
O
>=
-
lOl\
llO\
lOO\
O\
O
.
lO\
lOlOOlO\
OO
)
{
flo\
at
lOlllOlO\
OOl
=
lOll\
lOl\
OOOO
.
lO\
l\
OlOOlO\
OO
*
lO\
lll\
Ol\
OOO\
O
.
lOlOl\
OOl\
OOO\

-
lOlllOllOO\

*
lOlllO\
llO\
O
;
lOlllOOlOll
=
tru\
e
;
for
(
lOO\
OllOOO\
l
=
0
;
lOOOllOOO\
l
<
llOO\
O\
l\
OOlO
->
lOllO\
llO\
l\
l
;
lO\
OOllOOOl
++
)
{
const
Ve\
c3
&
lOlllOl\
OO\
lO
=
lO\
lllOlOOll
(
lOOOllO\
OO\
l
)
;
fl\
o\
a\
t\

lOll\
lOlOlO\
O
=
ll\
OOOlOOlO
->
llOllllOOO
[
lO\
OOllOOOl
]
.
lOlll\
OlOlO\
l
.
Dot
(
lOlllO\
l\
OO\
lO
-
lO\
lllO\
Ol\
lOl
)
;
if
(
lOl\
ll\
OlOlO\
O
*
Fabs
(
lOll\
lO\
lOlO\
O
)
>
lO\
lllO\
lOOOl\

)
{
lOlllOOlOll
=
false
;
break
;
}
if\

(
lO\
lllOl\
OlO\
O\

<
0.0f
)
{
if
(
lO\
lllO\
lOlOO
*
Fa\
b\
s\

(
lOlll\
OlO\
lO\
O
)
<
lO\
lllOlOOOl
+
0.001f
)
{
int\

lO\
l\
llO\
l\
OllO
=
lO\
ll\
lOlOll\
l
(
lOOOllOOOl
,
ll\
O\
OOlOOlO
->
lOllO\
l\
lOll
)
;
con\
s\
t
Vec3\

&
lO\
ll\
lOllOOO\

=
lOlllO\
lO\
Ol\
l
(
lOlll\
OlOll\
O
)
;
if
(
(
lO\
lll\
OlO\
OlO
-
lOl\
ll\
OllOOO
)
.
Dot\

(
lOlllO\
l\
lOOO\

-
lO\
l\
l\
lOOllOl
)
<=
0.0f
)
{
if
(
(
lOlllOlOOlO\

-
lO\
lll\
O\
llO\
OO
)
.
Dot
(
lOll\
lOl\
OOlO
-
lOl\
l\
lOOl\
lOl
)
>=
0.0f
)
{
lOll\
l\
Oll\
OOl
(
lO\
lll\
OlOOlO\

,
lOl\
llOll\
OOO
,
1
+
lO\
ll\
l\
OOl\
lO
)
;
lOlllO\
OllOO\

|=
1
<<
lOOO\
llO\
OOl
;
}
}
}
}
els\
e
{
lOlllOOl\
O\
l\
l\

=
fa\
lse
;
int\

lOlllOlOllO
=
lOlllOl\
Olll
(
lO\
OOll\
OOOl
,
llOO\
O\
lOOlO
->
lOll\
Ol\
l\
Ol\
l\

)
;
con\
st
Vec3\

&
lO\
l\
ll\
Oll\
OOO\

=
lOlll\
Ol\
OOll
(
lOlllOlOllO
)
;
if\

(
(
lOlllOlOOlO
-
lOlllOll\
OO\
O
)
.
Dot
(
lOlllOl\
lOOO
-
lOlllOO\
l\
lOl\

)
<=
0.0f
)
{
if
(
(
lOlll\
OlOOlO
-
lO\
l\
l\
l\
Ol\
lOO\
O
)
.
Dot\

(
lOlllO\
lOOlO
-
lOlllOOllO\
l\

)
>=
0.0f
)
{
Ve\
c3
lOlllO\
llOl\
O\

=
lOlllO\
Oll\
Ol\

-
ll\
OO\
OlO\
Ol\
O
->
lO\
OOlOOOO\
O\

*
lOl\
l\
lO\
ll\
OO
+
llOOOl\
OOl\
O
->
llOl\
lll\
OO\
O
[
lOOOllOOOl\

]
.
lO\
lll\
OlOlOl
*
lOlll\
O\
lOlO\
O\

;
Vec3\

lOOOlO\
OOOO
=
lOlllOOll\
Ol\

-
lOlllO\
llOlO\

;
float\

lOllllOOll
=
lOOOlOO\
O\
OO
.
GetM\
agn\
itu\
d\
e
(
)
;
if\

(
lOl\
lllOOll
>
0.0001f
)
lOOOl\
O\
OO\
OO\

/=
lOllllOO\
l\
l
;
else
lOOOlOOOOO
=
llOOOlOO\
l\
O
->
lOOOl\
O\
OO\
O\
O
;
lOll\
lOll\
O\
ll
(
0.0f
,
lOllll\
O\
Oll
,
llO\
OO\
lOO\
lO
,
lOlllO\
l\
OOlO\

,
lOlllOllO\
OO
,
lOl\
ll\
Ol\
lOl\
O
,
lOOOlOOO\
O\
O\

,
1
+
lO\
lllOOl\
lO
)
;
lOll\
lOO\
ll\
OO
|=
1
<<
lOOOllOOOl
;
}
}
}
}
if\

(
lOO\
OllO\
OO\
l
==
llOOOlO\
OlO\

->
lO\
l\
lOl\
lOl\
l
)
{
fo\
r
(
lOO\
Ol\
l\
OOOl
=
0
;
lOO\
Ol\
l\
OO\
Ol
<
llOOOl\
OOlO\

->
lOllOllOl\
l
;
lOOOllOOO\
l
++
)
{
const\

Vec3
&
llOOl\
ll\
l\
OO
=
lOlllO\
l\
O\
Oll
(
lOOOllOOO\
l
)
;
if\

(
(
lO\
lllOOllOl
-
ll\
OOl\
ll\
l\
OO
)
.
Dot
(
lOlllOOl\
l\
lO\

[
lOlllO\
Oll\
O
]
-
lOl\
llOOl\
ll\
O\

[
(
lO\
lllOOl\
lO\

+
1
)
&
1
]
)
>
0.0f
)
con\
tinue
;
if
(
(
lO\
lll\
OOll\
Ol
-
ll\
OOlll\
lOO
)
.
GetMagnitudeSqrd\

(
)
>
lOlllO\
lOOO\
O\

.
lO\
l\
Ol\
O\
Ol\
OOO
*
lOlll\
OlOOOO\

.
lOlOlOO\
lOOO
)
co\
ntin\
ue
;
con\
st
in\
t
lOlll\
OlllOO\

=
lOl\
llOl\
llOl\

(
lOO\
O\
llOOO\
l
,
llOOOl\
OOlO
->
lOllOllOll
)
;
if\

(
lOlll\
O\
OlOl\
l
||
(
lO\
l\
ll\
OO\
llOO
&
(
(
1
<<
lOl\
l\
lOlllOO
)
|
(
1
<<
lOOOl\
lOOOl
)
)
)
)
{
lOlllO\
llll\
O
(
llOO\
lll\
lO\
O\

)
;
}
els\
e
{
Vec3\

lOOOlOO\
O\
OO
=
lOl\
l\
l\
O\
Ol\
lOl
-
llOOl\
lllOO
;
float
lOllll\
O\
Oll\

=
lOOOlOOOOO\

.
Get\
Magni\
tude
(
)
;
if
(
lOllllO\
O\
ll
>
0.0001f
)
lOOOl\
OOOO\
O
/=
lOl\
lllOO\
ll
;
els\
e
lO\
O\
Ol\
OO\
OO\
O
=
ll\
OOO\
lO\
O\
lO
->
lOOOlOOOO\
O
;
lOll\
lOlllll
(
0.0f
,
lOll\
l\
lOOll
,
llOOO\
lO\
O\
l\
O
,
lOOOllO\
OOl
,
false
,
llOOllllOO
,
llO\
Ol\
lllO\
O
,
lO\
O\
OlOOO\
OO
)
;
}
lOl\
llO\
OlOl\
O
[
lOlllO\
O\
ll\
O\

]
|=
1
<<
lOOOllOO\
Ol
;
}
}
if\

(
lO\
ll\
lOO\
lOll
)
{
lOll\
llOO\
OOO\

(
0.0f
,
lOl\
llO\
lO\
OOO
.
lO\
lOlO\
Ol\
OOO
-
lOlll\
OllOO\

,
llOOOlO\
OlO
,
lOlll\
OO\
llOl
-
llOOOlO\
O\
l\
O\

->
lO\
O\
O\
lO\
O\
OOO
*
lO\
lllOllOO
,
llOOOlOOlO
->
lO\
O\
Ol\
OOOO\
O
)
;
}
}
}
if
(
lO\
ll\
lO\
OOO\
O\
l\

->
lOllOlllOOl
)
{
const\

Ve\
c3\

&
lOO\
l\
OlOll\
l
=
lO\
lllOOlllO
[
lOll\
lO\
Ol\
l\
O
]
;
cons\
t
Vec\
3
&
lOOlO\
llOO\
l
=
lOl\
lllOOOOl
[
lO\
l\
l\
lO\
OllO
]
;
fl\
oa\
t
lO\
l\
OllOOlO
=
lO\
lll\
OOll\
ll\

(
lOOl\
OlO\
lll
)
-
lOll\
lOlOOOO
.
lOlOlO\
Ol\
OOO
;
floa\
t
lO\
l\
OllOO\
l\
l
=
lO\
lllOO\
llll
(
lOO\
l\
O\
llO\
Ol\

)
-
lOll\
l\
O\
lO\
OO\
O
.
lOlOlOOl\
OOO\

;
if\

(
lO\
l\
O\
llO\
O\
ll
<
0
&&
lO\
lO\
ll\
OOlO
>
0
)
{
TA\
_\
ASSERT\

(
lO\
lOll\
OOlO
-
lOlO\
llOOll\

>
0.0f
)
;
flo\
a\
t
lO\
llllOOOlO
=
lOlOllOOlO
/
(
lOlOl\
lOOl\
O
-
lOlO\
l\
lOOl\
l
)
;
Vec\
3
lOl\
ll\
Ol\
lOlO\

=
lOOlO\
lOl\
ll\

+
(
lOO\
lOllO\
Ol
-
lOOl\
OlOlll\

)
*
lO\
l\
l\
llOO\
Ol\
O
;
bool
lOllllOOOll\

=
!
lOl\
llO\
OlOl\
l
&&
lOl\
ll\
OOll\
OO
==
0
&&
lO\
l\
llOOlOlO
[
lOl\
llOO\
llO
]
==
0
;
fo\
r\

(
lOOO\
llO\
O\
Ol\

=
0
;
lOOO\
l\
lOOOl
<
llOOOlOOlO\

->
lOllOl\
l\
O\
l\
l
;
lOOOll\
OO\
Ol
++
)
{
cons\
t
Ve\
c3
&
lOll\
ll\
O\
Ol\
OO\

=
lO\
ll\
lOlO\
O\
ll
(
lO\
O\
Ol\
lOOO\
l
)
;
in\
t
lO\
ll\
lOl\
OllO
=
lO\
lllOlOl\
ll\

(
lOOOll\
OO\
Ol
,
llOO\
Ol\
OO\
lO\

->
lOllO\
l\
lOll\

)
;
const
Vec3
&
lOlll\
O\
llOOO\

=
lO\
lllOlOOl\
l
(
lOlllO\
lOllO
)
;
float
lOll\
l\
O\
lOl\
OO
=
llOOOlO\
Ol\
O\

->
llOlll\
lOO\
O
[
lOOOllO\
OOl
]
.
lO\
lllOlOl\
O\
l
.
Dot\

(
lOllllOOl\
OO
-
lOlllOl\
lO\
lO
)
;
float
llllOlOOOl
;
Vec3
lOlll\
lOOOl\

;
Vec3
lOlll\
lO\
Ol\
O
;
flo\
at
llllOlllll
=
lO\
l\
l\
lOlO\
OOO
.
lOlOlO\
O\
lOO\
O
;
if
(
lOll\
l\
Ol\
O\
lOO\

<
0.0f
)
llllOll\
lll
+=
0.001f
;
if
(
!
(
lOll\
lOOll\
OO\

&
1
<<
lOOO\
l\
lOOOl
)
&&
Ge\
o\
m\
etry
::
TestSp\
h\
e\
re\
MovementAg\
ainstLi\
ne
(
lOOlO\
lOlll
,
lOOlOl\
lO\
Ol
,
lOllll\
OOl\
OO\

,
lOl\
llOllOOO
,
ll\
l\
lOl\
l\
lll
,
lll\
l\
Ol\
O\
OOl
,
lOllllO\
OOl
,
lOllllO\
O\
lO\

)
)
{
lO\
lll\
OOllOO
|=
1
<<
lOOOllO\
OO\
l
;
if
(
lOlll\
OlOl\
O\
O\

<
0.0f
)
{
lO\
ll\
l\
O\
l\
lOO\
l
(
lOl\
ll\
lOO\
lOO
,
lOlll\
OllOOO
,
1
+
lO\
lll\
O\
OllO
)
;
}
el\
se
{
lO\
lllOllOll
(
ll\
l\
lOlOO\
Ol
,
0.0f
,
ll\
OO\
O\
lOOlO
,
lO\
llllO\
Ol\
O\
O\

,
lOlll\
OllOOO
,
lO\
Ol\
OlOlll
-
lOl\
lll\
OO\
lO
*
lOl\
ll\
OlO\
OO\
O\

.
lOlOlOO\
lOO\
O
,
lOll\
llOO\
lO
,
1
+
lOll\
l\
OO\
llO
)
;
lOllllO\
OOll
=
fal\
s\
e
;
}
}
el\
se
{
if
(
lOlllOl\
OlOO
>
0.0f
)
{
lOllll\
OOOll
=
fals\
e
;
}
}
}
if
(
lO\
llllOOOll\

)
{
if
(
lO\
OO\
ll\
OOOl\

==
llOOOlOOlO
->
lOllOllO\
l\
l\

)
{
lOllll\
OOOOO
(
lO\
llllOOO\
lO
,
0.0f
,
llOOO\
lOO\
lO\

,
lOOl\
Ol\
Olll\

-
llOO\
OlOOl\
O
->
lO\
OOlOO\
OOO
*
lOlllOlOOO\
O\

.
lO\
lO\
lOO\
lO\
OO\

,
ll\
OOOlOOlO
->
lOO\
OlOOOO\
O\

)
;
lOlllO\
OlOll
=
true\

;
}
}
fo\
r
(
lOOO\
llOOOl
=
0
;
lO\
OO\
llOOO\
l\

<
ll\
OOO\
lOO\
lO
->
lOllOllOll
;
lOOO\
llOOOl
++
)
{
con\
st\

Vec\
3\

&
lOlllOlOOlO
=
lOlll\
OlOOll
(
lOOOl\
l\
O\
O\
O\
l\

)
;
flo\
a\
t\

llllOlOOOl
;
Vec\
3
lO\
ll\
l\
l\
OOO\
l
;
Vec3
lOlll\
l\
OOlO
;
if
(
!
(
lO\
l\
ll\
OOl\
OlO
[
lO\
l\
llOOll\
O
]
&
1
<<
lOOOllOOOl\

)
&&
Geometry\

::
TestPoin\
t\
MovementAgai\
nstSphe\
r\
e
(
lOOlOlOlll
,
lOOl\
OllO\
Ol
,
lOl\
ll\
OlO\
OlO
,
lOlllOlOO\
OO
.
lOl\
OlOOl\
OOO
,
lll\
l\
OlOOO\
l
,
lOlll\
l\
O\
OOl\

,
lOll\
ll\
OO\
lO
)
)
{
if
(
lOllll\
O\
OlO\

.
Dot\

(
lOl\
ll\
OOlllO
[
lOlllO\
Oll\
O
]
-
lOl\
llO\
Olll\
O
[
(
lOl\
llO\
Ol\
lO
+
1
)
&
1
]
)
<
0.0f
)
{
conti\
nue
;
}
lOl\
llOO\
l\
OlO
[
lOlllO\
OllO
]
|=
1
<<
lOOOllOOOl
;
cons\
t
int\

lOlllOlll\
OO
=
lOll\
lOlllOl
(
lOOOllOO\
O\
l
,
llOO\
Ol\
OOl\
O\

->
lOllOl\
lOll
)
;
if
(
lOl\
ll\
OOlOll\

||
(
lOlll\
OO\
l\
l\
OO
&
(
(
1
<<
lOl\
l\
l\
Ol\
l\
lOO
)
|
(
1
<<
lOO\
OllOOOl
)
)
)
)
{
lOll\
l\
Oll\
l\
lO
(
lOl\
ll\
OlOOlO
)
;
}
else
{
lOll\
l\
Oll\
ll\
l
(
llllOl\
OOO\
l\

,
0.0f
,
llOOO\
lOOlO
,
lO\
OO\
llOOOl
,
false
,
lOl\
llOlOO\
l\
O
,
lO\
OlOlOl\
ll\

-
lOll\
llO\
OlO\

*
lOl\
llOlOO\
OO
.
lO\
lOlO\
Ol\
OOO
,
lOll\
l\
l\
OOlO
)
;
}
}
}
}
}
}
{
con\
st
Ve\
c3
&
lO\
OlOl\
Olll\

=
lO\
l\
llOOlllO
[
0
]
;
co\
nst
Vec3
&
lOOlOllOO\
l
=
lOlllOOll\
l\
O
[
1
]
;
floa\
t
lO\
lOllOOlO
=
lOll\
l\
OOllll\

(
lOOl\
OlOlll\

)
;
float
lOlO\
llOOll
=
lOlllO\
Oll\
ll\

(
lOOlOll\
OOl
)
;
if
(
!
Sign\
sEqual\

(
lOl\
OllOOl\
l
,
lO\
l\
OllOOlO
)
)
{
TA\
_ASSERT
(
Fa\
b\
s
(
lOlOllO\
OlO
-
lOl\
OllOOll
)
>
0.0f
)
;
float
lO\
llll\
OOOlO
=
(
lO\
lO\
l\
lOOlO
)
/
(
lO\
l\
OllOOl\
O
-
lOlOllO\
O\
ll
)
;
Vec3
lOlllOl\
lO\
l\
O
=
lOOlOl\
Olll
+
(
lO\
Ol\
Oll\
O\
O\
l\

-
lOO\
lO\
lO\
lll
)
*
lOl\
ll\
lOOOl\
O
;
for\

(
lOOOllOOOl
=
0
;
lOOO\
llOOO\
l
<
llOOOlOOlO
->
lOllOllO\
ll
;
lO\
O\
O\
ll\
OOO\
l
++
)
{
con\
s\
t
Vec3
&
lO\
ll\
lOlOOlO
=
lOlllO\
lO\
Oll
(
lO\
O\
OllOO\
O\
l
)
;
if
(
llOOOl\
OOl\
O
->
llOll\
l\
lO\
O\
O
[
lOO\
OllO\
OOl
]
.
lO\
l\
llOlOlO\
l
.
Dot\

(
lOlll\
OllOlO
-
lO\
ll\
lOlO\
O\
lO
)
<
0.0f
)
brea\
k
;
}
if
(
lO\
OOll\
OOO\
l\

==
llO\
OOlOOl\
O
->
lOllOllOl\
l
)
{
lO\
llll\
O\
OOOO\

(
0.0f
,
lOll\
lOlOO\
O\
O
.
lOlOlO\
O\
lOOO
,
llOOO\
l\
OO\
lO\

,
lO\
lllOllOlO
,
ll\
O\
OO\
lO\
O\
lO\

->
lOO\
Ol\
O\
OOOO
)
;
}
}
for
(
lO\
OOllOOOl\

=
0
;
lOOOllOO\
Ol\

<
llO\
OOlO\
OlO
->
lOllO\
l\
lOl\
l\

;
lOOO\
l\
lOOO\
l\

++
)
{
co\
n\
st
Ve\
c3
&
lOl\
lllOOlO\
O
=
lOlllO\
l\
O\
Ol\
l
(
lO\
OOl\
lOOOl\

)
;
con\
st
in\
t
lOl\
l\
lOl\
OllO
=
lOO\
Oll\
OOO\
l
+
1
<
llOOOlO\
OlO
->
lOllOllOll
?
lO\
OOllOOOl
+
1
:
0
;
co\
nst\

Vec\
3\

&
lOlllOllOO\
O\

=
lO\
l\
llOl\
OOll
(
lOlllOlOll\
O
)
;
flo\
at
llll\
O\
lOOOl
=
0.0f
;
flo\
a\
t
lOl\
lllOOl\
Ol
=
0.0f
;
Vec3
lOllllOOlO\

;
Vec\
3
lO\
lll\
lOO\
Ol
;
Vec3
lllllOOl\
ll
;
Vec3
lll\
llOll\
Ol
;
boo\
l
lO\
ll\
llOO\
llO
=
false\

;
if
(
Geome\
t\
r\
y
::
Fi\
ndClose\
stPointO\
n\
LineToLineUnC\
l\
am\
pe\
d
(
lO\
OlOlOlll
,
lOO\
lOllOOl
,
lOllll\
OOl\
O\
O\

,
lO\
ll\
lO\
llOOO
,
llll\
lOOlll
,
lllllOll\
Ol\

)
)
{
Vec3
lOOO\
lO\
OO\
OO
=
lllllOO\
lll
-
llll\
lOll\
Ol
;
flo\
at
lll\
OOlOlOl
=
lOOO\
lOOOOO
.
GetMagnitude\
S\
qrd
(
)
;
if
(
ll\
l\
OOlOlOl
<
lO\
l\
l\
l\
Ol\
O\
OOO
.
lOlOlOO\
l\
OOO
*
lOl\
ll\
Ol\
OOO\
O\

.
lOlOl\
OOlO\
OO\

)
{
lOllllOOO\
l
=
lll\
ll\
OllOl
;
co\
nst
float
lO\
llllOOl\
l
=
Sqr\
t
(
lll\
O\
O\
l\
O\
lOl\

)
;
const\

floa\
t
lOl\
lOl\
Ol\
Ol
=
0.001f
*
0.001f
;
if
(
ll\
lOOlOlOl
>
lOl\
lOlOl\
Ol
)
lOO\
OlOOOOO
=
lOO\
Ol\
OOO\
OO\

/
lO\
ll\
llO\
Oll
;
else\

lOOOlOOO\
OO
=
llOOOlOO\
l\
O
->
lO\
OO\
lOOOOO\

;
lOll\
llOOl\
O
=
lOOO\
lOOOOO\

;
lOllll\
O\
Ol\
O\
l
=
lO\
lllOl\
OOOO\

.
lOlOlO\
O\
lO\
OO
-
lOl\
lll\
OOll
;
lOl\
ll\
lOOl\
lO
=
true
;
}
}
if\

(
lO\
ll\
l\
OO\
OOOl\

->
lOllOlllO\
O\
l\

&&
!
lOllllOO\
ll\
O
&&
Ge\
om\
et\
r\
y\

::
Test\
Li\
neMove\
men\
tA\
gainstC\
apsul\
e
(
lOl\
l\
l\
O\
O\
l\
l\
lO
[
0
]
,
lOl\
l\
lOOlllO
[
1
]
,
lOlll\
lOOO\
O\
l\

[
0
]
,
lOlll\
lOOOOl
[
1
]
,
lO\
llll\
OOlOO
,
lO\
l\
l\
lO\
ll\
OOO
,
lOl\
ll\
Ol\
O\
O\
OO
.
lOlOl\
OO\
lOOO
,
llllOlOO\
Ol
,
lO\
llllOOOl
,
lOllllOOlO
)
)
{
lOllllOO\
ll\
O
=
tru\
e\

;
}
if
(
lOllllOOl\
lO\

)
{
if\

(
lO\
l\
l\
llOOlll
&&
lOlll\
lOOlO
.
Do\
t\

(
llOOOlOOlO\

->
lOOOlO\
O\
O\
OO\

)
<
0.0f
)
lO\
llllOOlO
=
-
lOllllO\
Ol\
O
;
if
(
lOllll\
OO\
l\
O
.
Dot
(
ll\
OOOlOOl\
O
->
llOllllOOO\

[
lOOO\
ll\
O\
OOl
]
.
lOll\
lO\
lOlO\
l
)
<
0.0f
)
{
lOl\
llOllOll
(
lll\
lO\
lOOOl
,
lOllllOOlOl
,
llOOOl\
OOl\
O
,
lOllllO\
O\
lOO
,
lOlllO\
llOOO\

,
lOllllOOOl
,
lOl\
lllO\
OlO\

,
0
)
;
}
el\
se
{
lOll\
lOllOO\
l
(
lOllll\
OOl\
O\
O\

,
lO\
lll\
OllOO\
O
,
0
)
;
if
(
lOl\
lllOOlO
.
Do\
t\

(
llOOO\
lOO\
lO
->
lOOOlOOO\
OO
)
>
0.95f
)
{
lOl\
l\
ll\
OOlO\

=
llO\
OO\
lOOl\
O
->
lOOOlOOO\
OO
;
lOllllOO\
O\
O\
O
(
llllOlOOO\
l
,
lOlll\
lO\
OlOl
,
llOO\
OlOOlO
,
lOllllO\
OOl
,
lO\
lll\
lOO\
lO
)
;
}
}
}
}
for
(
lO\
O\
O\
llOOOl
=
0
;
lO\
OO\
llOOOl
<
ll\
OOOlO\
OlO\

->
lO\
llO\
llOll
;
lOO\
OllOO\
O\
l\

++
)
{
if
(
lO\
lllOOlOlO\

[
lO\
ll\
lOOllO
]
&
(
1
<<
lOOO\
l\
lOOOl\

)
)
continue
;
cons\
t
Ve\
c3
&
lOll\
llO\
O\
lOO
=
lO\
lllOlOOll
(
lO\
OOllO\
OOl
)
;
fl\
oat
llllO\
lOO\
Ol
=
0.0f
;
floa\
t
lOl\
lll\
OO\
lOl\

=
0.0f
;
Ve\
c3
lOllll\
OOlO\

;
Vec3
lOll\
llOOO\
l\

;
bool
lOll\
l\
lO\
O\
ll\
O
=
false\

;
if
(
Geo\
me\
t\
ry
::
Fi\
n\
dClo\
ses\
tPoin\
t\
OnL\
in\
eUnClam\
ped\

(
lO\
llllO\
OlOO
,
lOOlO\
lOlll
,
lOOlO\
ll\
O\
Ol\

,
lOlll\
lO\
OOl
)
)
{
Vec3
lO\
OOlOOOOO
=
lOllll\
OO\
O\
l
-
lO\
l\
ll\
lOOlOO
;
float
lllOO\
lO\
lOl\

=
lO\
OO\
lOO\
O\
OO
.
GetMagn\
itude\
Sqrd\

(
)
;
if
(
lll\
OOlOlOl\

<
lOl\
l\
lOlO\
OOO
.
lOlOl\
OOl\
OOO
*
lO\
l\
llOlOOOO
.
lOlO\
lOOlOOO
)
{
const\

flo\
a\
t\

lOllllOOll
=
Sqrt
(
lllO\
OlO\
lOl
)
;
lOll\
llOOlO
=
lOO\
OlOOOOO
/
lO\
llllOO\
ll\

;
lOllllO\
O\
lO\
l\

=
lOlll\
OlOOOO
.
lOlO\
l\
OO\
lOOO\

-
lOlll\
lOOll
;
lOllllO\
OllO
=
tr\
ue
;
lOl\
lllOOOl
=
lO\
l\
ll\
l\
O\
Ol\
OO
;
}
}
if
(
lOlll\
OOOO\
O\
l
->
lOllO\
l\
llOOl
&&
!
lOl\
lllOOllO
&&
Geom\
etry
::
TestSphe\
r\
eMovem\
en\
tAg\
ainstLine
(
lOlll\
l\
OOlOO
,
(
lOll\
llOOlO\
O
/
lOl\
llO\
OOOO\
O
)
*
lOllOlllll\
l
,
lOlll\
OO\
l\
l\
lO
[
0
]
,
lOlll\
OO\
lllO
[
1
]
,
lOlllOlO\
OO\
O
.
lOlOlO\
OlOOO
,
llllOlO\
OO\
l
,
lO\
l\
lllO\
OOl
,
lOl\
lllOOlO
)
)
{
TA_\
ASS\
ERT
(
ll\
ll\
OlO\
OOl
>
0.0f
)
;
lO\
l\
l\
llOOllO\

=
tr\
ue
;
lOl\
lllOOlO
=
-
lOl\
lllOOlO
;
lO\
llll\
OOOl
-=
lOll\
llO\
O\
lO
*
lOlllOlO\
OOO
.
lOlO\
lO\
OlO\
O\
O
;
}
if
(
lOll\
llOOllO
)
{
lOll\
lO\
ll\
lll
(
llll\
O\
lOO\
Ol
,
lOl\
l\
l\
l\
OOl\
Ol\

,
ll\
OOO\
lOOlO
,
lO\
O\
O\
llOO\
O\
l
,
true
,
lOllllO\
OlOO
,
lOll\
llOOO\
l
,
lOllllOOlO
)
;
}
}
}
}
