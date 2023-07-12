#include "tdef.h"
#include "tw.h"
#include "nroff.h"
#include <string.h>
static char Sccsid[] = "@(#)ni.c	1.3 of 4/26/77";
char obuf[OBUFSZ];
char *obufp  = obuf;
int r[NN] = {'%','nl','yr','hp','ct','dn','mo','dy','dw','ln','dl','st','sb','c.'};
int pto = 10000;
int pfrom = 1;
int print = 1;
char nextf[NS] = "/usr/lib/tmac.xxxxx";
int nfi = 14;
#ifdef NROFF
char termtab[NS] = "/usr/lib/term/37";
int tti = 14;
#endif
int init = 1;
int fc = IMP;
int eschar = '\\';
int pl = 11*INCH;
int po = PO;
int dfact = 1;
int dfactd = 1;
int res = 1;
int smnt = 4;
int ascii = ASCII;
int ptid = PTID;
char ptname[] = "/dev/cat";
int lg = LG;
int pnlist[NPN] = {-1};
int *pnp = pnlist;
int npn = 1;
int npnflg = 1;
int oldbits = -1;
int xflg = 1;
int dpn = -1;
int totout = 1;
int ulfont = 1;
int ulbit = 1<<9;
int tabch = TAB;
int ldrch = LEADER;
int xxx;
extern caseds(), caseas(), casesp(), caseft(), caseps(), casevs(),
casenr(), caseif(), casepo(), casetl(), casetm(), casebp(), casech(),
casepn(), tbreak(), caseti(), casene(), casenf(), casece(), casefi(),
casein(), caseli(), casell(), casens(), casemk(), casert(), caseam(),
casede(), casedi(), caseda(), casewh(), casedt(), caseit(), caserm(),
casern(), casead(), casers(), casena(), casepl(), caseta(), casetr(),
caseul(), caselt(), casenx(), caseso(), caseig(), casetc(), casefc(),
caseec(), caseeo(), caselc(), caseev(), caserd(), caseab(), casefl(),
done(), casess(), casefp(), casecs(), casebd(), caselg(), casehc(),
casehy(), casenh(), casenm(), casenn(), casesv(), caseos(), casels(),
casecc(), casec2(), caseem(), caseaf(), casehw(), casemc(), casepm(),
casecu(), casepi(), caserr(), caseuf(), caseie(), caseel(), casepc(),
caseht();
struct contab {
	int rq;
	int (*f)();
}contab[NM] = {
	'ds',caseds,
	'as',caseas,
	'sp',casesp,
	'ft',caseft,
	'ps',caseps,
	'vs',casevs,
	'nr',casenr,
	'if',caseif,
	'ie',caseie,
	'el',caseel,
	'po',casepo,
	'tl',casetl,
	'tm',casetm,
	'bp',casebp,
	'ch',casech,
	'pn',casepn,
	'br',tbreak,
	'ti',caseti,
	'ne',casene,
	'nf',casenf,
	'ce',casece,
	'fi',casefi,
	'in',casein,
	'li',caseli,
	'll',casell,
	'ns',casens,
	'mk',casemk,
	'rt',casert,
	'am',caseam,
	'de',casede,
	'di',casedi,
	'da',caseda,
	'wh',casewh,
	'dt',casedt,
	'it',caseit,
	'rm',caserm,
	'rr',caserr,
	'rn',casern,
	'ad',casead,
	'rs',casers,
	'na',casena,
	'pl',casepl,
	'ta',caseta,
	'tr',casetr,
	'ul',caseul,
	'cu',casecu,
	'lt',caselt,
	'nx',casenx,
	'so',caseso,
	'ig',caseig,
	'tc',casetc,
	'fc',casefc,
	'ec',caseec,
	'eo',caseeo,
	'lc',caselc,
	'ev',caseev,
	'rd',caserd,
	'ab',caseab,
	'fl',casefl,
	'ex',done,
	'ss',casess,
	'fp',casefp,
	'cs',casecs,
	'bd',casebd,
	'lg',caselg,
	'hc',casehc,
	'hy',casehy,
	'nh',casenh,
	'nm',casenm,
	'nn',casenn,
	'sv',casesv,
	'os',caseos,
	'ls',casels,
	'cc',casecc,
	'c2',casec2,
	'em',caseem,
	'af',caseaf,
	'hw',casehw,
	'mc',casemc,
	'pm',casepm,
#ifdef NROFF
	'pi',casepi,
#endif
	'uf',caseuf,
	'pc',casepc,
	'ht',caseht,
};

/*
troff environment block
*/
struct nroff_env_t nroff_env;

init_env()
{
	memset(&nroff_env, 0, sizeof(nroff_env));
	nroff_env.block = 0; 
	nroff_env.ics = ICS; 
	nroff_env.ic = 0; 
	nroff_env.icf = 0; 
	nroff_env.chbits = 0; 
	nroff_env.nmbits = 0; 
	nroff_env.apts = PS; 
	nroff_env.apts1 = PS; 
	nroff_env.pts = PS; 
	nroff_env.pts1 = PS; 
	nroff_env.font = FT; 
	nroff_env.font1 = FT; 
	nroff_env.sps = SPS; 
	nroff_env.spacesz = SS; 
	nroff_env.lss = VS; 
	nroff_env.lss1 = VS; 
	nroff_env.ls = 1; 
	nroff_env.ls1 = 1; 
	nroff_env.ll = LL; 
	nroff_env.ll1 = LL; 
	nroff_env.lt = LL; 
	nroff_env.lt1 = LL; 
	nroff_env.ad = 1; 
	nroff_env.nms = 1; 
	nroff_env.ndf = 1; 
	nroff_env.fi = 1; 
	nroff_env.cc = '.'; 
	nroff_env.c2 = '\''; 
	nroff_env.ohc = OHC; 
	nroff_env.tdelim = IMP; 
	nroff_env.hyf = 1; 
	nroff_env.hyoff = 0; 
	nroff_env.un1 = -1; 
	nroff_env.tabc = 0; 
	nroff_env.dotc = '.'; 
	nroff_env.adsp = 0; 
	nroff_env.adrem = 0; 
	nroff_env.lastl = 0; 
	nroff_env.nel = 0; 
	nroff_env.admod = 0; 
	nroff_env.spflg = 0; 
	nroff_env.wne = 0; 
	nroff_env.ne = 0; 
	nroff_env.nc = 0; 
	nroff_env.nb = 0; 
	nroff_env.lnmod = 0; 
	nroff_env.nwd = 0; 
	nroff_env.nn = 0; 
	nroff_env.ni = 0; 
	nroff_env.ul = 0; 
	nroff_env.cu = 0; 
	nroff_env.ce = 0; 
	nroff_env.in = 0; 
	nroff_env.in1 = 0; 
	nroff_env.un = 0; 
	nroff_env.wch = 0; 
	nroff_env.pendt = 0; 
	nroff_env.pendw = 0; 
	nroff_env.pendnf = 0; 
	nroff_env.spread = 0; 
	nroff_env.it = 0; 
	nroff_env.itmac = 0; 
	nroff_env.lnsize = LNSIZE;	
	nroff_env.sps = EM;
	nroff_env.ics = EM*2; 
}
