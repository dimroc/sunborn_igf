//---------------------------------------------------------------------------------
// File Name: zpta005.inl
//
// Copyright (C) 2004 - 2006 True Axis Pty Ltd, Australia.
// All Rights Reserved.
//
//---------------------------------------------------------------------------------

if\

(
(
(
llOll\
ll\
Ol\
Ol
.
lOl\
OllOOOl
&
Collision
::
FLAG_F\
RICTION
)
==
0
)
&&
(
lOOlOllOlO\
ll
||
!
lOOlOllO\
llOO
||
lOOlOl\
lOllOl\

<
0.0f
||
(
llOl\
l\
l\
l\
OlOl
.
lO\
lOllOOO\
l\

&
Collisio\
n\

::
FLAG_C\
ONSTRA\
IN_\
IN\
_BOTH_DIRE\
CT\
IONS
)
)
)
{
if
(
(
llO\
l\
l\
l\
lOlO\
l
.
lO\
lOllO\
OOl
&
Coll\
is\
ion
::
FLAG\
_ROTATIO\
N_C\
O\
N\
STRA\
I\
NT
)
==
0
)
{
if
(
!
lOO\
l\
OllOlO\
ll\

)
{
fl\
oat
lOO\
l\
O\
llO\
l\
l\
l\
O
=
0.0f
;
if
(
!
lO\
OlO\
llOllll
)
{
#ifdef lOOlOlllOOOO
if
(
lOll\
O\
OlO\
llO
.
w
==
0.0f
)
#endif
{
float
lOOlOlllOOOl\

=
lOOlOlllOOlO
(
(
Vec\
3
*
)
&
lOl\
l\
OO\
lOllO
,
&
ll\
Olll\
lOl\
Ol\

.
lOOOO\
lllll
,
&
lOll\
OO\
O\
lOl\
l
->
lOOlOlllO\
Oll
,
&
llOllllO\
lO\
l
.
lOOOlO\
OOO\
O
,
&
lOllOOO\
l\
O\
ll
->
GetWorldS\
pace\
Invers\
eI\
nertia
(
)
,
lO\
ll\
O\
OO\
l\
O\
l\
l\

->
Get\
In\
ve\
rs\
eM\
ass
(
)
)
;
lOllOOlOll\
O
.
w
=
lO\
OlOlll\
OOOl
;
}
lOO\
lO\
llOll\
lO
+=
lOllOOlOllO
.
w
;
}
if
(
!
lOO\
l\
O\
ll\
lO\
lOO\

)
{
#ifdef lOOlOlllOOOO
if
(
lOllO\
O\
l\
Ol\
ll
.
w
==
0.0f
)
#endif
{
flo\
at
lOOlOlllOlOl
=
lOOlOlll\
OOlO
(
(
Vec\
3
*
)
&
lOllOOlOlll
,
&
ll\
O\
llll\
OlO\
l
.
lOOOOlllll
,
&
lOllO\
OOl\
lOO
->
lOOlO\
ll\
lO\
Ol\
l
,
&
llO\
ll\
ll\
O\
lO\
l
.
lOOOl\
O\
O\
O\
OO
,
&
lOl\
lOO\
OllOO
->
GetWorld\
Sp\
a\
ceI\
nv\
e\
r\
seInert\
ia
(
)
,
lOll\
OOOllOO
->
GetInv\
e\
rseMass
(
)
)
;
lOll\
OOlOl\
ll
.
w
=
lOO\
lOl\
llO\
lOl
;
}
lO\
OlO\
llOlllO
+=
lOl\
lOOlOlll
.
w
;
}
lOll\
O\
OlO\
O\
l\
O
=
lOOlOllOl\
lO\
l
/
lO\
Ol\
Ol\
lO\
lllO
;
}
if\

(
!
lOO\
lOllOlO\
ll
||
lOll\
OOlO\
O\
lO\

<
0.0f
||
(
llO\
ll\
llOlOl
.
lOlOll\
OOO\
l
&
Collision
::
FL\
A\
G_\
CONSTRA\
IN_\
IN_BOT\
H_\
DIREC\
TIONS
)
||
!
lOOlOl\
lOllOO
)
{
lOllO\
OlOOl\
O
*=
lOOlO\
lllOl\
lO\

;
if\

(
lOO\
lOlllOl\
ll
)
llOl\
l\
ll\
OlOl
.
lOllOOlO\
OlO
+=
Fa\
bs\

(
lO\
llOOlOOlO\

)
;
if
(
!
lO\
Ol\
O\
llOlOl\
l
)
{
if\

(
!
lOOl\
Oll\
Ollll
)
{
Ve\
c3
lOOOO\
llO\
lOO
=
(
Vec3\

&
)
lO\
l\
lOOlOl\
lO\

;
#ifdef lOOllllllOO
lOO\
OOllOlOO
.
lOOl\
lllllll
=
0
;
#endif				
lOOl\
O\
l\
l\
l\
lOOO
(
&
lOllOOO\
lOl\
l
->
lOOlOl\
l\
ll\
OOl
,
&
llOllllOlOl
.
lO\
OOlOOOOO
,
(
lOllOOOlOll
->
lOOlOllllOlO
*
-
lOll\
OOlO\
OlO
)
)
;
lOO\
lOll\
llOOO\

(
&
lOl\
l\
OOO\
lOl\
l\

->
lOOlO\
l\
l\
llOll\

,
&
lOOOO\
ll\
Ol\
OO
,
(
lO\
OlO\
lll\
llOO\

*
-
lOl\
lOOlOOlO
)
)
;
lOll\
OO\
O\
l\
Oll
->
lOO\
l\
Ol\
l\
l\
llOl
(
Fabs
(
lOl\
lOOlO\
O\
l\
O
)
)
;
}
if
(
!
lOOl\
O\
l\
l\
lOl\
OO
)
{
Vec3
lOO\
O\
OllOlOO
=
(
Ve\
c3
&
)
lOl\
l\
O\
OlOlll
;
#ifdef lOOllllllOO
lO\
O\
O\
Oll\
Ol\
O\
O\

.
lOOll\
l\
l\
lll\
l
=
0
;
#endif				
lOOlO\
llllOO\
O
(
&
lOllOOO\
llOO
->
lO\
O\
l\
Oll\
ll\
OO\
l\

,
&
llOll\
l\
lO\
l\
Ol
.
lOOOlOOOOO
,
(
lOllOOO\
llO\
O
->
lOOlOllllOl\
O
*
lOllOOl\
OOlO\

)
)
;
lOO\
lO\
ll\
ll\
OOO
(
&
lOl\
l\
OOOll\
OO
->
lOOlOl\
lllOll
,
&
lOOOO\
llOlOO\

,
(
lOOlOl\
l\
lllOO\

*
lO\
llOOlOOlO
)
)
;
lO\
ll\
OOOl\
lOO
->
lOOlOlllll\
Ol
(
Fabs\

(
lOllOOl\
OOlO
)
)
;
}
}
if
(
lOOl\
O\
ll\
Oll\
O\
O
)
{
if\

(
lOOlOlllll\
l\
O\

||
(
llOllllO\
l\
Ol
.
lO\
l\
Ol\
lOOOl\

&
Collis\
io\
n\

::
FLAG_NO_F\
R\
ICTION\

)
!=
0
||
lOll\
OO\
l\
O\
Ol\
O
>
-
lO\
OlOll\
lllll
*
Min
(
lOll\
OOOlOll\

->
lOl\
OlOl\
OOlO\

,
lOll\
OOOl\
lO\
O
->
lOlOlOlOOlO\

)
)
{
retur\
n
lOO\
l\
lOOOOOOO
;
}
}
float
lO\
O\
llO\
OO\
O\
OO\
l
=
Min
(
lOllOO\
O\
l\
Oll
->
lO\
O\
llOOOO\
OlO
(
)
,
lOllOOO\
ll\
OO\

->
lOOl\
lOOOOOlO\

(
)
)
;
Ve\
c3\

lO\
Oll\
OOO\
OOll
;
lOOllOOO\
O\
l\
OO\

(
&
lOOll\
OOO\
O\
O\
ll
,
&
llO\
llllOlOl
.
lOO\
OOll\
lll\

,
&
lOl\
lOO\
OlOll
->
Get\
Linea\
r\
Veloc\
ity
(
)
,
&
lOll\
OO\
OlOll\

->
Ge\
tA\
ngul\
a\
r\
V\
el\
ocity\

(
)
,
&
lOllOOOlOll
->
GetCent\
er\
OfMas\
s
(
)
,
&
lOllOOO\
ll\
OO
->
GetLi\
n\
ear\
V\
elo\
cit\
y
(
)
,
&
lOllO\
OO\
llOO\

->
Ge\
t\
AngularV\
e\
locity
(
)
,
&
lOl\
lOOO\
llOO\

->
GetCen\
te\
r\
OfM\
as\
s
(
)
)
;
lOOll\
O\
OOO\
Oll
-=
ll\
Ollll\
O\
lO\
l
.
lOOO\
l\
OOOO\
O
*
llOllll\
OlO\
l\

.
lOO\
O\
lOO\
OOO\

.
Do\
t\

(
lOOl\
lOOOOOll
)
;
float\

lOO\
l\
lO\
OO\
OlOl\

=
lOOllOOO\
OOll\

.
Get\
M\
ag\
nitud\
e
(
)
;
if
(
lOOllOO\
OOlO\
l
<
0.001f
*
lOOllOOOOOOl
)
{
llOll\
llO\
lO\
l\

.
lOl\
Oll\
OOO\
l
|=
Col\
l\
ision
::
FLAG_FRICTI\
O\
N\
_GE\
NERAT\
IN\
G_\
IMPULS\
E2
;
lOOll\
OOO\
O\
llO\

(
)
;
retu\
rn
lOOl\
lO\
OOO\
OOO
;
}
Vec\
3
lO\
Oll\
OOOOlll
=
lO\
Ol\
lOOOO\
Oll\

/
lOOllO\
O\
OOl\
Ol
;
Vec3\

lO\
OllO\
O\
O\
lO\
OO
;
Ve\
c3\

lO\
O\
ll\
OOOlO\
Ol
;
floa\
t
lOOlOllO\
ll\
lO
=
0.0f
;
if
(
!
lOOlOllOlll\
l\

)
{
lO\
Ol\
OllOl\
l\
lO\

=
lOO\
l\
O\
l\
l\
lO\
O\
lO\

(
&
lOOllOOOlOOO\

,
&
llOllllOlOl
.
lOOOOl\
llll
,
&
lOll\
OOOlOll
->
lOOlOlllOOl\
l
,
&
lOOllOOOO\
lll\

,
&
lOl\
lOOOlOll
->
Ge\
tWo\
r\
ldSpa\
ceI\
n\
ve\
rseIn\
ertia\

(
)
,
lOl\
l\
OOOlOll
->
Ge\
tIn\
v\
ers\
eMass\

(
)
)
;
}
if\

(
!
lOOlOlllOlO\
O
)
{
lO\
O\
lOllO\
lllO
+=
lOOlOl\
llOO\
l\
O
(
&
lOOl\
lOOOlOOl
,
&
ll\
Ol\
ll\
l\
O\
l\
Ol
.
lOOOO\
lllll
,
&
lOl\
lOOOllOO\

->
lOOlOlllOOll
,
&
lOOllOO\
OOlll
,
&
lO\
ll\
O\
O\
Oll\
OO
->
Ge\
tWor\
l\
d\
SpaceIn\
v\
er\
seIne\
rtia
(
)
,
lOll\
OOOllO\
O
->
Ge\
tIn\
ver\
s\
eMass
(
)
)
;
}
TA_AS\
S\
E\
R\
T
(
lOOlOll\
O\
lllO
!=
0
)
;
flo\
a\
t
lOOllOOOl\
OlO
=
lOO\
llOO\
OOlOl
/
lOO\
l\
O\
llOlllO
;
if
(
lOOlOllO\
l\
lOO
&&
(
llOl\
l\
l\
lOlO\
l\

.
lOlOllO\
O\
Ol
&
Collisio\
n
::
FL\
AG_FRI\
CTI\
ON_GE\
N\
E\
RATING_I\
MPULSE\

)
==
0
&&
(
(
ll\
Oll\
llO\
lOl
.
lO\
lOllOOOl\

&
Col\
lis\
ion
::
FLAG_FRICTI\
ON_\
GEN\
ERATING_IM\
P\
ULS\
E2
)
==
0
||
lOO\
ll\
OOOOl\
Ol
>
1.0f
*
lO\
Ol\
lOOOOO\
O\
l
)
)
{
TA_A\
S\
SERT
(
lOl\
lOO\
l\
O\
Ol\
O
<
0.0f
)
;
float
lOOl\
lOOO\
lOll\

=
-
lOllO\
OlOOlO
*
ll\
Olll\
l\
OlOl
.
lOll\
O\
OlOlOO
;
if
(
lO\
Ol\
lO\
OOl\
O\
lO
>
lO\
O\
llOOOlOll
)
{
lOOl\
l\
OOO\
lOlO
=
lOO\
ll\
OOOlOll\

;
}
else
{
llO\
lll\
lOlOl
.
lOlOll\
OOOl
|=
Co\
llision
::
FL\
AG\
_F\
R\
ICTION_\
G\
E\
NERATING\
_I\
MPULSE2
;
lOOllOOO\
OllO
(
)
;
}
}
TA_\
A\
S\
SERT
(
Flo\
atIsOK
(
lOOll\
O\
O\
Ol\
OlO\

)
)
;
lOOllOOOlOlO\

*=
lOO\
l\
lO\
OOll\
OO
;
if\

(
!
lOOlO\
l\
lOllll\

)
{
lOOlOllll\
OOO
(
&
lOll\
O\
O\
OlOll
->
lOO\
lOl\
l\
llOO\
l
,
&
lOOllOOOOll\
l
,
(
lOllOO\
OlOll
->
lOOl\
OllllOlO
*
-
lOO\
llOOOlOlO\

)
)
;
lOOl\
Olll\
lOO\
O\

(
&
lOllOO\
OlOll
->
lO\
OlOllll\
Ol\
l
,
&
lOOl\
l\
OOOlOOO
,
(
lOOlO\
l\
l\
l\
llO\
O
*
-
lO\
OllOO\
OlOlO
)
)
;
lOl\
lOOOlOll
->
lOOlO\
lllllOl
(
Fab\
s
(
lO\
O\
llOOOl\
OlO
)
)
;
}
if\

(
!
lO\
OlOlllOlOO\

)
{
lOOl\
Ollll\
OOO\

(
&
lOllO\
OOllOO
->
lO\
O\
l\
Olll\
l\
OOl
,
&
lOOl\
lOOO\
Olll
,
(
lOllOO\
OllO\
O
->
lOO\
lO\
llllOlO
*
lOOl\
lOOOlOl\
O
)
)
;
lOOlOllllOOO
(
&
lOllOOOllO\
O\

->
lOO\
l\
OllllOl\
l\

,
&
lOOllO\
OO\
lOOl\

,
(
lO\
Ol\
Ol\
l\
lllOO
*
lOO\
l\
l\
O\
OOl\
Ol\
O\

)
)
;
lOllO\
O\
OllO\
O\

->
lOO\
lO\
llll\
lOl\

(
Fa\
bs
(
lO\
OllOO\
Ol\
O\
l\
O
)
)
;
}
}
}
else
{
if
(
lOOlOl\
l\
OlO\
l\
l
)
{
return
lO\
Oll\
OOOOO\
OO
;
}
fl\
oa\
t
lOOlO\
llOll\
lO\

=
0
;
{
if
(
!
lO\
OlO\
llOllll
)
{
const
Vec3
lOOllOOOll\
Ol\

=
lOllOOOl\
Ol\
l
->
lO\
OlOl\
l\
lOO\
ll
-
ll\
OllllOlOl
.
lOOOOl\
llll
;
Vec3
lO\
O\
OO\
llOlO\
O
=
ll\
O\
llllOlO\
l
.
lO\
O\
O\
lO\
O\
OOO\

.
Cros\
s
(
lOOllOO\
Ol\
lOl
)
*
llOllllOlO\
l
.
lOllOOO\
l\
Oll
->
GetWorld\
S\
paceI\
nverseIne\
rt\
ia
(
)
;
lOOlOllOl\
ll\
O\

+=
-
llO\
llllO\
lOl
.
lOOOl\
OOOOO
.
Do\
t
(
lOO\
O\
OllOl\
OO
.
Cross
(
lOOll\
OOOl\
lOl
)
)
;
}
if
(
!
lOO\
lOll\
lOl\
OO
)
{
const
Vec3
lOOl\
lO\
OOlllO
=
lOllOOOllOO
->
lOOl\
OlllO\
O\
ll
-
ll\
O\
lll\
lO\
l\
Ol
.
lO\
OOOl\
l\
lll
;
Vec3
lO\
OOOl\
lO\
l\
O\
O
=
llO\
l\
lllO\
lOl
.
lOOO\
lOOO\
OO
.
Cro\
s\
s
(
lOOllOO\
Ol\
llO\

)
*
lOll\
OO\
OllO\
O
->
Ge\
t\
WorldSpac\
eInverseIne\
rtia\

(
)
;
lO\
Ol\
Ol\
lO\
lllO\

+=
-
llOl\
lllO\
lOl
.
lOOOl\
OOOO\
O
.
Dot
(
lO\
OO\
Ol\
lOlO\
O
.
Cross\

(
lOOllO\
O\
Oll\
lO\

)
)
;
}
}
fl\
oat
lOl\
lOOlOOlO
=
lOOlO\
ll\
OllO\
l
/
lOOlOl\
lOlllO
;
if
(
lOllOOl\
OOl\
O
<
0.0f
||
(
llOll\
l\
lO\
lOl\

.
lOlOll\
OO\
Ol
&
Co\
llis\
ion
::
FLAG\
_CON\
ST\
R\
AIN\
_IN\
_BO\
TH_\
DIRECT\
I\
ONS
)
)
{
lOll\
OOlOOlO\

=
-
lOllOOlOO\
lO\

;
llOll\
llOlO\
l
.
lOllOOlO\
OlO\

+=
lOl\
lO\
OlOOlO
;
lOll\
OOl\
OOl\
O\

=
-
lO\
llO\
OlOOlO
;
if\

(
!
lOO\
lO\
llOll\
ll\

)
{
lOllOOOl\
Ol\
l
->
lOOlOllllO\
ll
-=
(
ll\
Ol\
lll\
OlOl
.
lOOOl\
O\
OOOO
*
lOllOO\
lO\
OlO
)
.
Cros\
s\

(
lOl\
lOOO\
l\
Oll
->
Ge\
tC\
enter\
Of\
Ma\
ss\

(
)
-
llOllllO\
l\
Ol\

.
lOOOOl\
llll
)
*
lOll\
OOOl\
O\
ll
->
Ge\
t\
Wo\
rldSpaceInve\
rseInertia
(
)
;
}
if
(
!
lOO\
lOlll\
OlO\
O\

)
{
lOllOOOl\
lO\
O
->
lOO\
lO\
l\
lllOll
+=
(
llOllll\
O\
lO\
l
.
lO\
OOlO\
O\
OO\
O
*
lO\
llO\
O\
l\
OOlO\

)
.
Cr\
oss
(
lO\
l\
l\
OOO\
llOO
->
Ge\
tCen\
te\
rOfM\
as\
s
(
)
-
llOllllOlOl
.
lOOOOlll\
l\
l\

)
*
lOllOO\
OllOO
->
GetWor\
ld\
Spac\
eInv\
er\
s\
eInertia
(
)
;
}
}
}
}
