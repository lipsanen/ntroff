#include "tdef.h"
#include "nroff.h"
/*
troff5.c

misc processing requests
*/

extern int ascii;
extern int nonumb;
extern int nroff_env_admod;
extern int nroff_env_ad;
extern int nroff_env_fi;
extern int nroff_env_cc;
extern int nroff_env_c2;
extern int nroff_env_ohc;
extern int nroff_env_tabc;
extern int nroff_env_dotc;
extern int nroff_env_pendnf;
extern int nroff_env_hyf;
extern int nroff_env_ce;
extern int po;
extern int po1;
extern int nroff_env_nc;
extern int nroff_env_in;
extern int nroff_env_un;
extern int nroff_env_un1;
extern int nroff_env_in1;
extern int nroff_env_ll;
extern int nroff_env_ll1;
extern int nroff_env_lt;
extern int nroff_env_lt1;
extern int nlist[NTRAP];
extern int mlist[NTRAP];
extern int lgf;
extern int pl;
extern int npn;
extern int npnflg;
extern int *frame;
extern struct d_t *dip;
extern int copyf;
extern char nextf[];
extern int trap;
extern int nroff_env_lss;
extern int em;
extern int evlist[EVLSZ];
extern int evi;
extern int ibf;
extern int ev;
extern int ch;
extern int nflush;
extern int tty;
extern int ttys[3];
extern int quiet;
extern int iflg;
extern int eschar;
extern int lit;
extern int *litlev;
extern int nroff_env_ls;
extern int nroff_env_ls1;
extern int nroff_env_tabtab[];
extern char trtab[];
extern int nroff_env_ul;
extern int nroff_env_cu;
extern int sfont;
extern int nroff_env_font;
extern int fontlab[];
extern int nroff_env_it;
extern int nroff_env_itmac;
extern int noscale;
extern int nroff_env_ic;
extern int nroff_env_icf;
extern int nroff_env_ics;
extern int *vlist;
extern int sv;
extern int esc;
extern int nroff_env_nn;
extern int nroff_env_nms;
extern int nroff_env_ndf;
extern int nroff_env_lnmod;
extern int nroff_env_ni;
extern int nroff_env_lnsize;
extern int nroff_env_nb;
extern int offset;
extern int nlflg;
extern int nroff_env_apts, nroff_env_apts1, nroff_env_pts, nroff_env_pts1, nroff_env_font, nroff_env_font1;
extern int ulfont;
extern int ulbit;
extern int error;
extern int nroff_env_nmbits;
extern int nroff_env_chbits;
extern int nroff_env_tdelim;
extern int xxx;
int iflist[NIF];
int ifx;
extern struct contab {
	int rq;
	int (*f)();
}contab[NM];
static char Sccsid[] = "@(#)n5.c	1.6 of 5/27/77";

casead(){
	register i;

	nroff_env.ad = 1;
	/*leave admod alone*/
	if(skip())return 0;
	switch(i = getch() & CMASK){
		case 'r':	/*right adj, left ragged*/
			nroff_env.admod = 2;
			break;
		case 'l':	/*left adj, right ragged*/
			nroff_env.admod = nroff_env.ad = 0;	/*same as casena*/
			break;
		case 'c':	/*centered adj*/
			nroff_env.admod = 1;
			break;
		case 'b': case 'n':
			nroff_env.admod = 0;
			break;
		case '0': case '2': case '4':
			nroff_env.ad = 0;
		case '1': case '3': case '5':
			nroff_env.admod = (i - '0')/2;
	}
}
casena(){
	nroff_env.ad = 0;
}
casefi(){
	tbreak();
	nroff_env.fi++;
	nroff_env.pendnf = 0;
	nroff_env.lnsize = LNSIZE;
}
casenf(){
	tbreak();
	nroff_env.fi = 0;
/* can't do while oline is only LNSIZE
	lnsize = LNSIZE + WDSIZE;
*/
}
casers(){
	dip->nls = 0;
}
casens(){
	dip->nls++;
}
chget(c)
int c;
{
	register i;

	if(skip() ||
	  ((i = getch()) & MOT) ||
	  ((i&CMASK) == ' ') ||
	  ((i&CMASK) == '\n')){
		ch = i;
		return(c);
	}else return(i & BMASK);
}
casecc(){
	nroff_env.cc = chget('.');
}
casec2(){
	nroff_env.c2 = chget('\'');
}
casehc(){
	nroff_env.ohc = chget(OHC);
}
casetc(){
	nroff_env.tabc = chget(0);
}
caselc(){
	nroff_env.dotc = chget(0);
}
casehy(){
	register i;

	nroff_env.hyf = 1;
	if(skip())return 0;
	noscale++;
	i = atoi();
	noscale = 0;
	if(nonumb)return 0;
	nroff_env.hyf = max(i,0);
}
casenh(){
	nroff_env.hyf = 0;
}
max(aa,bb)
int aa,bb;
{
	if(aa>bb)return(aa);
	else return(bb);
}
casece(){
	register i;

	noscale++;
	skip();
	i = max(atoi(),0);
	if(nonumb)i = 1;
	tbreak();
	nroff_env.ce = i;
	noscale = 0;
}
casein(){
	register i;

	if(skip())i = nroff_env.in1;
	else i = max(hnumb(&nroff_env.in),0);
	tbreak();
	nroff_env.in1 = nroff_env.in;
	nroff_env.in = i;
	if(!nroff_env.nc){
		nroff_env.un = nroff_env.in;
		setnel();
	}
}
casell(){
	register i;

	if(skip())i = nroff_env.ll1;
	else i = max(hnumb(&nroff_env.ll),INCH/10);
	nroff_env.ll1 = nroff_env.ll;
	nroff_env.ll = i;
	setnel();
}
caselt(){
	register i;

	if(skip())i = nroff_env.lt1;
	else i = max(hnumb(&nroff_env.lt),0);
	nroff_env.lt1 = nroff_env.lt;
	nroff_env.lt = i;
}
caseti(){
	register i;

	if(skip())return 0;
	i = max(hnumb(&nroff_env.in),0);
	tbreak();
	nroff_env.un1 = i;
	setnel();
}
casels(){
	register i;

	noscale++;
	if(skip())i = nroff_env.ls1;
	else i = max(inumb(&nroff_env.ls),1);
	nroff_env.ls1 = nroff_env.ls;
	nroff_env.ls = i;
	noscale = 0;
}
casepo(){
	register i;

	if(skip())i = po1;
	else i = max(hnumb(&po),0);
	po1 = po;
	po = i;
#ifndef NROFF
	if(!ascii)esc += po - po1;
#endif
}
casepl(){
	register i;

	skip();
	if((i = vnumb(&pl)) == 0)pl = 11 * INCH; /*11in*/
		else pl = i;
	if(v.nl > pl)v.nl = pl;
}
casewh(){
	register i, j, k;

	lgf++;
	skip();
	i = vnumb(0);
	if(nonumb)return 0;
	skip();
	j = getrq();
	if((k=findn(i)) != NTRAP){
		mlist[k] = j;
		return 0;
	}
	for(k=0; k<NTRAP; k++)if(mlist[k] == 0)break;
	if(k == NTRAP){
		prstrfl("Cannot plant trap.\n");
		return 0;
	}
	mlist[k] = j;
	nlist[k] = i;
}
casech(){
	register i, j, k;

	lgf++;
	skip();
	if(!(j=getrq()))return 0;
		else for(k=0; k<NTRAP; k++)if(mlist[k] == j)break;
	if(k == NTRAP)return 0;
	skip();
	i = vnumb(0);
	if(nonumb)mlist[k] = 0;
	nlist[k] = i;
}
findn(i)
int i;
{
	register k;

	for(k=0; k<NTRAP; k++)
		if((nlist[k] == i) && (mlist[k] != 0))break;
	return(k);
}
casepn(){
	register i;

	skip();
	noscale++;
	i = max(inumb(&v.pn),0);
	noscale = 0;
	if(!nonumb){
		npn = i;
		npnflg++;
	}
}
casebp(){
	register i, savframe;

	if(dip->op)return 0;
	savframe = frame;
	skip();
	if((i = inumb(&v.pn)) < 0)i = 0;
	tbreak();
	if(!nonumb){
		npn = i;
		npnflg++;
	}else if(dip->nls)return 0;
	eject(savframe);
}
casetm(x) int x;{
	register i;
	char tmbuf[NTM];

	lgf++;
	copyf++;
	if(skip() && x)prstrfl("User Abort.");
	for(i=0; i<NTM-2;)if((tmbuf[i++]=getch()) == '\n')break;
	if(i == NTM-2)tmbuf[i++] = '\n';
	tmbuf[i] = 0;
	prstrfl(tmbuf);
	copyf--;
}
casesp(a)
int a;
{
	register i, j, savlss;

	tbreak();
	if(dip->nls || trap)return 0;
	i = findt1();
	if(!a){
		skip();
		j = vnumb(0);
		if(nonumb)j = nroff_env.lss;
	}else j = a;
	if(j == 0)return 0;
	if(i < j)j = i;
	savlss = nroff_env.lss;
	if(dip->op)i = dip->dnl; else i = v.nl;
	if((i + j) < 0)j = -i;
	nroff_env.lss = j;
	newline(0);
	nroff_env.lss = savlss;
}
casert(){
	register a, *p;

	skip();
	if(dip->op)p = &dip->dnl; else p = &v.nl;
	a = vnumb(p);
	if(nonumb)a = dip->mkline;
	if((a < 0) || (a >= *p))return 0;
	nroff_env.nb++;
	casesp(a - *p);
}
caseem(){
	lgf++;
	skip();
	em = getrq();
}
casefl(){
	tbreak();
	flusho();
}
caseev(){
	register nxev;
	extern int block;

	if(skip()){
e0:
		if(evi == 0)return 0;
		nxev =  evlist[--evi];
		goto e1;
	}
	noscale++;
	nxev = atoi();
	noscale = 0;
	if(nonumb)goto e0;
	flushi();
	if((nxev >= NEV) || (nxev < 0) || (evi >= EVLSZ)){
		prstrfl("Cannot do ev.\n");
		if(error)done2(040);else edone(040);
		return 0;
	}
	evlist[evi++] = ev;
e1:
	if(ev == nxev)return 0;
	lseek(ibf, ev*sizeof(nroff_env), 0);
	write(ibf, &nroff_env, sizeof(nroff_env));
	lseek(ibf, nxev*sizeof(nroff_env), 0);
	read(ibf, &nroff_env, sizeof(nroff_env));
	ev = nxev;
}
caseel(){
	if(--ifx < 0){
		ifx = 0;
		iflist[0] = 0;
	}
	caseif(2);
}
caseie(){
	if(ifx >= NIF){
		prstr("if-else overflow.\n");
		ifx = 0;
		edone(040);
	}
	caseif(1);
	ifx++;
}
caseif(x)
int x;
{
	register i, notflag, true;

	if(x == 2){
		notflag = 0;
		true = iflist[ifx];
		goto i1;
	}
	true = 0;
	skip();
	if(((i = getch()) & CMASK) == '!'){
		notflag = 1;
	}else{
		notflag = 0;
		ch = i;
	}
	i = atoi();
	if(!nonumb){
		if(i > 0)true++;
		goto i1;
	}
	switch((i = getch()) & CMASK){
		case 'e':
			if(!(v.pn & 01))true++;
			break;
		case 'o':
			if(v.pn & 01)true++;
			break;
#ifdef NROFF
		case 'n':
			true++;
		case 't':
#endif
#ifndef NROFF
		case 't':
			true++;
		case 'n':
#endif
		case ' ':
			break;
		default:
			true = cmpstr(i);
	}
i1:
	true ^= notflag;
	if(x == 1)iflist[ifx] = !true;
	if(true){
	i2:
		do{
		v.hp = 0;
		}
		while(((i = getch()) & CMASK) == ' ');
		if((i & CMASK) == LEFT)goto i2;
		ch = i;
		nflush++;
	}else{
		copyf++;
		if(eat(LEFT) == LEFT){
			while(eatblk(RIGHT,LEFT) != RIGHT)nlflg = 0;
		}
		copyf--;
	}
}
eatblk(right,left)
int right,left;
{
	register i;

e0:
	while(((i = getch() & CMASK) != right) &&
		(i != left) &&
		(i != '\n'));
	if(i == left){
		while((i=eatblk(right,left)) != right)nlflg = 0;
		goto e0;
	}
	return(i);
}
cmpstr(delim)
int delim;
{
	register i, j, p;
	int begin, cnt, k;
	int savapts, savapts1, savfont, savfont1,
		savpts, savpts1;

	if(delim & MOT)return(0);
	delim &= CMASK;
	if(dip->op)wbfl();
	if((offset = begin = alloc()) == 0)return(0);
	cnt = 0;
	v.hp = 0;
	savapts = nroff_env.apts;
	savapts1 = nroff_env.apts1;
	savfont = nroff_env.font;
	savfont1 = nroff_env.font1;
	savpts = nroff_env.pts;
	savpts1 = nroff_env.pts1;
	while(((j = (i=getch()) & CMASK) != delim) && (j != '\n')){
		wbf(i);
		cnt++;
	}
	wbt(0);
	k = !cnt;
	if(nlflg)goto rtn;
	p = begin;
	nroff_env.apts = savapts;
	nroff_env.apts1 = savapts1;
	nroff_env.font = savfont;
	nroff_env.font1 = savfont1;
	nroff_env.pts = savpts;
	nroff_env.pts1 = savpts1;
	mchbits();
	v.hp = 0;
	while(((j = (i=getch()) & CMASK) != delim) && (j != '\n')){
		if(rbf0(p) != i){
			eat(delim);
			k = 0;
			break;
		}
		p = incoff(p);
		k = !(--cnt);
	}
rtn:
	nroff_env.apts = savapts;
	nroff_env.apts1 = savapts1;
	nroff_env.font = savfont;
	nroff_env.font1 = savfont1;
	nroff_env.pts = savpts;
	nroff_env.pts1 = savpts1;
	mchbits();
	offset = dip->op;
	free(begin);
	return(k);
}
caserd(){

	lgf++;
	skip();
	getname();
	if(!iflg){
		if(quiet){
			ttys[2] &= ~ECHO;
			stty(0,ttys);
			prstrfl(""); /*bell*/
		}else{
			if(nextf[0]){
				prstr(nextf);
				prstr(":");
			}else{
				prstr(""); /*bell*/
			}
		}
	}
	collect();
	tty++;
	pushi(-1);
}
rdtty(){
	int onechar;

	onechar = 0;
	if(read(0, &onechar, 1) == 1){
		if(onechar == '\n')tty++;
			else tty = 1;
		if(tty != 3)return(onechar);
	}
	popi();
	tty = 0;
	if(quiet){
		ttys[2] |= ECHO;
		stty(0,ttys);
	}
	return(0);
}
caseec(){
	eschar = chget('\\');
}
caseeo(){
	eschar = 0;
}
caseli(){

	skip();
	lit = max(inumb(0),1);
	litlev = frame;
	if((!dip->op) && (v.nl == -1))newline(1);
}
caseta(){
	register i, j;

	nroff_env.tabtab[0] = nonumb = 0;
	for(i=0; ((i < (NTAB-1)) && !nonumb); i++){
		if(skip())break;
		nroff_env.tabtab[i] = max(hnumb(&nroff_env.tabtab[max(i-1,0)]),0) & TMASK;
		if(!nonumb) switch(j = ch & CMASK){
			case 'C':
				nroff_env.tabtab[i] |= CTAB;
				break;
			case 'R':
				nroff_env.tabtab[i] |= RTAB;
				break;
			default: /*includes L*/
				break;
			}
		nonumb = ch = 0;
	}
	nroff_env.tabtab[i] = 0;
}
casene(){
	register i, j;

	skip();
	i = vnumb(0);
	if(nonumb)i = nroff_env.lss;
	if(i > (j = findt1())){
		i = nroff_env.lss;
		nroff_env.lss = j;
		dip->nls = 0;
		newline(0);
		nroff_env.lss = i;
	}
}
casetr(){
	register i, j;

	lgf++;
	skip();
	while((i = getch() & CMASK) != '\n'){
		if((i & MOT) || ((j = getch()) & MOT))return 0;
		if((j &= CMASK) == '\n')j = ' ';
		trtab[i] = j;
	}
}
casecu(){
	nroff_env.cu++;
	caseul();
}
caseul(){
	register i;

	noscale++;
	if(skip())i = 1;
	else i = atoi();
	if(nroff_env.ul && (i == 0)){
		nroff_env.font = sfont;
		nroff_env.ul = nroff_env.cu = 0;
	}
	if(i){
		if(!nroff_env.ul){
			sfont = nroff_env.font;
			nroff_env.font = ulfont;
		}
		nroff_env.ul = i;
	}
	noscale = 0;
	mchbits();
}
caseuf(){
	register i, j;

	if(skip() || !(i = getrq()) || (i == 'S') ||
		((j = find(i,fontlab))  == -1))
			ulfont = 1; /*default position 2*/
	else ulfont = j;
#ifdef NROFF
	if(ulfont == 0)ulfont = 1;
#endif
	ulbit = ulfont<<9;
}
caseit(){
	register i;

	lgf++;
	nroff_env.it = nroff_env.itmac = 0;
	noscale++;
	skip();
	i = atoi();
	skip();
	if(!nonumb && (nroff_env.itmac = getrq()))nroff_env.it = i;
	noscale = 0;
}
casemc(){
	register i;

	if(nroff_env.icf > 1)nroff_env.ic = 0;
	nroff_env.icf = 0;
	if(skip())return 0;
	nroff_env.ic = getch();
	nroff_env.icf = 1;
	skip();
	i = max(hnumb(0),0);
	if(!nonumb)nroff_env.ics = i;
}
casemk(){
	register i, j;

	if(dip->op)j = dip->dnl; else j = v.nl;
	if(skip()){
		dip->mkline = j;
		return 0;
	}
	if((i = getrq()) == 0)return 0;
	vlist[findr(i)] = j;
}
casesv(){
	register i;

	skip();
	if((i = vnumb(0)) < 0)return 0;
	if(nonumb)i = 1;
	sv += i;
	caseos();
}
caseos(){
	register savlss;

	if(sv <= findt1()){
		savlss = nroff_env.lss;
		nroff_env.lss = sv;
		newline(0);
		nroff_env.lss = savlss;
		sv = 0;
	}
}
casenm(){
	register i;

	nroff_env.lnmod = nroff_env.nn = 0;
	if(skip())return 0;
	nroff_env.lnmod++;
	noscale++;
	i = inumb(&v.ln);
	if(!nonumb)v.ln = max(i,0);
	getnm(&nroff_env.ndf,1);
	getnm(&nroff_env.nms,0);
	getnm(&nroff_env.ni,0);
	noscale = 0;
	nroff_env.nmbits = nroff_env.chbits;
}
getnm(p,min)
int *p, min;
{
	register i;

	eat(' ');
	if(skip())return 0;
	i = atoi();
	if(nonumb)return 0;
	*p = max(i,min);
}
casenn(){
	noscale++;
	skip();
	nroff_env.nn = max(atoi(),1);
	noscale = 0;
}
caseab(){
	dummy();
	casetm(1);
	done2(0);
}
dummy(){}
