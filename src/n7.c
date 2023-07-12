#include "tdef.h"
#include "nroff.h"
#include "tw.h"
#ifdef NROFF
#define GETCH gettch
#endif
#ifndef NROFF
#define GETCH getch
#endif

/*
troff7.c

text
*/

extern struct d_t *dip;
extern int pl;
extern int trap;
extern int flss;
extern int npnflg;
extern int npn;
extern int stop;
extern int nflush;
extern int *ejl;
extern int ejf;
extern int ascii;
extern int donef;
extern int nroff_env_wch;
extern int dpn;
extern int ndone;
extern int nroff_env_lss;
extern int pto;
extern int pfrom;
extern int print;
extern int nlist[NTRAP];
extern int mlist[NTRAP];
extern int *frame;
extern int *stk;
extern int *pnp;
extern int nroff_env_nb;
extern int nroff_env_ic;
extern int nroff_env_icf;
extern int nroff_env_ics;
extern int nroff_env_ne;
extern int nroff_env_ll;
extern int nroff_env_un;
extern int nroff_env_un1;
extern int nroff_env_in;
extern int nroff_env_ls;
extern int nroff_env_spread;
extern int totout;
extern int nroff_env_nwd;
extern int *nroff_env_pendw;
extern int nroff_env_line[];
extern int ch;
extern int nroff_env_ce;
extern int nroff_env_fi;
extern int nlflg;
extern int nroff_env_pendt;
extern int nroff_env_sps;
extern int nroff_env_adsp;
extern int nroff_env_pendnf;
extern int over;
extern int nroff_env_adrem;
extern int nroff_env_nel;
extern int nroff_env_ad;
extern int nroff_env_ohc;
extern int nroff_env_hyoff;
extern int nhyp;
extern int nroff_env_spflg;
extern int nroff_env_word[];
extern int *nroff_env_wordp;
extern int nroff_env_wne;
extern int nroff_env_chbits;
extern int cwidth;
extern int widthp;
extern int nroff_env_hyf;
extern int xbitf;
extern int vflag;
extern int nroff_env_ul;
extern int nroff_env_cu;
extern int nroff_env_font;
extern int sfont;
extern int nroff_env_it;
extern int nroff_env_itmac;
extern int *nroff_env_hyptr[NHYP];
extern int **hyp;
extern int nroff_env_lnmod;
extern int nroff_env_admod;
extern int nroff_env_nn;
extern int nroff_env_nms;
extern int nroff_env_ndf;
extern int nroff_env_ni;
extern int nform;
extern int nroff_env_lnsize;
extern int po;
extern int ulbit;
extern int *vlist;
extern int nrbits;
extern int nroff_env_nmbits;
extern int xxx;
int brflg;
static char Sccsid[] = "@(#)n7.c	1.2 of 3/4/77";

tbreak(){
	register *i, j, pad;

	trap = 0;
	if(nroff_env.nb)return 0;
	if((!dip->op) && (v.nl == -1)){
		newline(1);
		return 0;
	}
	if(!nroff_env.nc){
		setnel();
		if(!nroff_env.wch)return 0;
		if(nroff_env.pendw)getword(1);
		movword();
	}else if(nroff_env.pendw && !brflg){
		getword(1);
		movword();
	}
	*nroff_env.linep = dip->nls = 0;
#ifdef NROFF
	if(!dip->op)horiz(po);
#endif
	if(nroff_env.lnmod)donum();
	nroff_env.lastl = nroff_env.ne;
	if(brflg != 1){
		totout = 0;
	}else if(nroff_env.ad){
		if((nroff_env.lastl = (nroff_env.ll - nroff_env.un)) < nroff_env.ne)nroff_env.lastl = nroff_env.ne;
	}
	if(nroff_env.admod && nroff_env.ad && (brflg != 2)){
		nroff_env.lastl = nroff_env.ne;
		nroff_env.adsp = nroff_env.adrem = 0;
#ifdef NROFF
		if(nroff_env.admod == 1)nroff_env.un +=  quant(nroff_env.nel/2,t.Adj);
#endif
#ifndef NROFF
		if(nroff_env.admod == 1)nroff_env.un += nroff_env.nel/2;
#endif NROFF
		else if(nroff_env.admod ==2)nroff_env.un += nroff_env.nel;
	}
	totout++;
	brflg = 0;
	if(nroff_env.lastl > dip->maxl)dip->maxl = nroff_env.lastl;
	horiz(nroff_env.un);
	for(i = nroff_env.line;nroff_env.nc > 0;){
		if(((j = *i++) & CMASK) == ' '){
			pad = 0;
			do{
				pad += width(j);
				nroff_env.nc--;
			  }while(((j = *i++) & CMASK) == ' ');
			i--;
			pad += nroff_env.adsp;
			if(nroff_env.adrem){
				if(nroff_env.adrem < 0){
#ifdef NROFF
					pad -= t.Adj;
					nroff_env.adrem += t.Adj;
				}else if((totout&01) ||
					((nroff_env.adrem/t.Adj)>=(--nroff_env.nwd))){
					pad += t.Adj;
					nroff_env.adrem -= t.Adj;
#endif
#ifndef NROFF
					pad--;
					nroff_env.adrem++;
				}else{
					pad++;
					nroff_env.adrem--;
#endif
				}
			}
			horiz(pad);
		}else{
			pchar(j);
			nroff_env.nc--;
		}
	}
	if(nroff_env.ic){
		if((j = nroff_env.ll - nroff_env.un - nroff_env.lastl + nroff_env.ics) > 0)horiz(j);
		pchar(nroff_env.ic);
	}
	if(nroff_env.icf)nroff_env.icf++;
		else nroff_env.ic = 0;
	nroff_env.ne = nroff_env.nwd = 0;
	nroff_env.un = nroff_env.in;
	setnel();
	newline(0);
	if(*(int*)dip){if(dip->dnl > dip->hnl)dip->hnl = dip->dnl;}
	else{if(v.nl > dip->hnl)dip->hnl = v.nl;}
	for(j=nroff_env.ls-1; (j >0) && !trap; j--)newline(0);
	nroff_env.spread = 0;
}
donum(){
	register i, nw;
	extern pchar();

	nrbits = nroff_env.nmbits;
	nw = width('1' | nrbits);
	if(nroff_env.nn){
		nroff_env.nn--;
		goto d1;
	}
	if(v.ln%nroff_env.ndf){
		v.ln++;
	d1:
		nroff_env.un += nw*(3+nroff_env.nms+nroff_env.ni);
		return 0;
	}
	i = 0;
	if(v.ln<100)i++;
	if(v.ln<10)i++;
	horiz(nw*(nroff_env.ni+i));
	nform = 0;
	fnumb(v.ln,pchar);
	nroff_env.un += nw*nroff_env.nms;
	v.ln++;
}
text(){
	register i;
	static int spcnt;

	nflush++;
	if((!dip->op) && (v.nl == -1)){newline(1); return 0;}
	setnel();
	if(nroff_env.ce || !nroff_env.fi){
		nofill();
		return 0;
	}
	if(nroff_env.pendw)goto t4;
	if(nroff_env.pendt)if(spcnt)goto t2; else goto t3;
	nroff_env.pendt++;
	if(spcnt)goto t2;
	while(((i = GETCH()) & CMASK) == ' ')spcnt++;
	if(nlflg){
	t1:
		nflush = nroff_env.pendt = ch = spcnt = 0;
		callsp();
		return 0;
	}
	ch = i;
	if(spcnt){
	t2:
		tbreak();
		if(nroff_env.nc || nroff_env.wch)goto rtn;
		nroff_env.un += spcnt*nroff_env.sps;
		spcnt = 0;
		setnel();
		if(trap)goto rtn;
		if(nlflg)goto t1;
	}
t3:
	if(nroff_env.spread)goto t5;
	if(nroff_env.pendw || !nroff_env.wch)
	t4:
		if(getword(0))goto t6;
	if(!movword())goto t3;
t5:
	if(nlflg)nroff_env.pendt = 0;
	nroff_env.adsp = nroff_env.adrem = 0;
	if(nroff_env.ad){
		nroff_env.adsp = nroff_env.nel/(nroff_env.nwd-1);
#ifdef NROFF
		nroff_env.adsp = (nroff_env.adsp/t.Adj)*t.Adj;
#endif
		nroff_env.adrem = nroff_env.nel - nroff_env.adsp*(nroff_env.nwd-1);
	}
	brflg = 1;
	tbreak();
	nroff_env.spread = 0;
	if(!trap)goto t3;
	if(!nlflg)goto rtn;
t6:
	nroff_env.pendt = 0;
	ckul();
rtn:
	nflush = 0;
}
nofill(){
	register i, j;

	if(!nroff_env.pendnf){
		over = 0;
		tbreak();
		if(trap)goto rtn;
		if(nlflg){
			ch = nflush = 0;
			callsp();
			return 0;
		}
		nroff_env.adsp = nroff_env.adrem = 0;
		nroff_env.nwd = 10000;
	}
	while((j = ((i = GETCH()) & CMASK)) != '\n'){
		if(j == nroff_env.ohc)continue;
		if(j == CONT){
			nroff_env.pendnf++;
			nflush = 0;
			flushi();
			ckul();
			return 0;
		}
		storeline(i,-1);
	}
	if(nroff_env.ce){
		nroff_env.ce--;
		if((i=quant(nroff_env.nel/2,HOR)) > 0)nroff_env.un += i;
	}
	if(!nroff_env.nc)storeline(FILLER,0);
	brflg = 2;
	tbreak();
	ckul();
rtn:
	nroff_env.pendnf = nflush = 0;
}
callsp(){
	register i;

	if(flss)i = flss; else i = nroff_env.lss;
	flss = 0;
	casesp(i);
}
ckul(){
	if(nroff_env.ul && (--nroff_env.ul == 0)){
			nroff_env.cu = 0;
			nroff_env.font = sfont;
			mchbits();
	}
	if(nroff_env.it && (--nroff_env.it == 0) && nroff_env.itmac)control(nroff_env.itmac,0);
}
storeline(c,w){
	register i;

	if((c & CMASK) == JREG){
		if((i=findr(c>>BYTE)) != -1)vlist[i] = nroff_env.ne;
		return 0;
	}
	if(nroff_env.linep >= (nroff_env.line + nroff_env.lnsize - 1)){
		if(!over){
			prstrfl("Line overflow.\n");
			over++;
		c = 0343;
		w = -1;
		goto s1;
		}
		return 0;
	}
s1:
	if(w == -1)w = width(c);
	nroff_env.ne += w;
	nroff_env.nel -= w;
	*nroff_env.linep++ = c;
	nroff_env.nc++;
}
newline(a)
int a;
{
	register i, j, nlss;
	int opn;

	if(a)goto nl1;
	if(dip->op){
		j = nroff_env.lss;
		pchar1(FLSS);
		if(flss)nroff_env.lss = flss;
		i = nroff_env.lss + dip->blss;
		dip->dnl += i;
		pchar1(i);
		pchar1('\n');
		nroff_env.lss = j;
		dip->blss = flss = 0;
		if(dip->alss){
			pchar1(FLSS);
			pchar1(dip->alss);
			pchar1('\n');
			dip->dnl += dip->alss;
			dip->alss = 0;
		}
		if(dip->ditrap && !dip->ditf &&
			(dip->dnl >= dip->ditrap) && dip->dimac)
			if(control(dip->dimac,0)){trap++; dip->ditf++;}
		return 0;
	}
	j = nroff_env.lss;
	if(flss)nroff_env.lss = flss;
	nlss = dip->alss + dip->blss + nroff_env.lss;
	v.nl += nlss;
#ifndef NROFF
	if(ascii){dip->alss = dip->blss = 0;}
#endif
	pchar1('\n');
	flss = 0;
	nroff_env.lss = j;
	if(v.nl < pl)goto nl2;
nl1:
	ejf = dip->hnl = v.nl = 0;
	ejl = frame;
	if(donef){
		if((!nroff_env.nc && !nroff_env.wch) || ndone)done1(0);
		ndone++;
		donef = 0;
		if(frame == stk)nflush++;
	}
	opn = v.pn;
	v.pn++;
	if(npnflg){
		v.pn = npn;
		npn = npnflg = 0;
	}
nlpn:
	if(v.pn == pfrom){
		print++;
		pfrom = -1;
	}else if(opn == pto){
		print = 0;
		opn = -1;
		chkpn();
		goto nlpn;
		}
	if(stop && print){
		dpn++;
		if(dpn >= stop){
			dpn = 0;
			dostop();
		}
	}
nl2:
	trap = 0;
	if(v.nl == 0){
		if((j = findn(0)) != NTRAP)
			trap = control(mlist[j],0);
	} else if((i = findt(v.nl-nlss)) <= nlss){
		if((j = findn1(v.nl-nlss+i)) == NTRAP){
			prstrfl("Trap botch.\n");
			done2(-5);
		}
		trap = control(mlist[j],0);
	}
}
findn1(a)
int a;
{
	register i, j;

	for(i=0; i<NTRAP; i++){
		if(mlist[i]){
			if((j = nlist[i]) < 0)j += pl;
			if(j == a)break;
		}
	}
	return(i);
}
chkpn(){
	pto = *(pnp++);
	pfrom = pto & ~MOT;
	if(pto == -1){
		flusho();
		done1(0);
	}
	if(pto & MOT){
		pto &= ~MOT;
		print++;
		pfrom = 0;
	}
}
findt(a)
int a;
{
	register i, j, k;

	k = 32767;
	if(dip->op){
		if(dip->dimac && ((i = dip->ditrap -a) > 0))k = i;
		return(k);
	}
	for(i=0; i<NTRAP; i++){
		if(mlist[i]){
			if((j = nlist[i]) < 0)j += pl;
			if((j -= a)  <=  0)continue;
			if(j < k)k = j;
		}
	}
	i = pl - a;
	if(k > i)k = i;
	return(k);
}
findt1(){
	register i;

	if(dip->op)i = dip->dnl;
		else i = v.nl;
	return(findt(i));
}
eject(a)
int *a;
{
	register savlss;

	if(dip->op)return 0;
	ejf++;
	if(a)ejl = a;
		else ejl = frame;
	if(trap)return 0;
e1:
	savlss = nroff_env.lss;
	nroff_env.lss = findt(v.nl);
	newline(0);
	nroff_env.lss = savlss;
	if(v.nl && !trap)goto e1;
}
movword(){
	register i, w, *wp;
	int savwch, hys;

	over = 0;
	wp = nroff_env.wordp;
	if(!nroff_env.nwd){
		while(((i = *wp++) & CMASK) == ' '){
			nroff_env.wch--;
			nroff_env.wne -= width(i);
		}
		wp--;
	}
	if((nroff_env.wne > nroff_env.nel) &&
	   !nroff_env.hyoff && nroff_env.hyf &&
	   (!nroff_env.nwd || (nroff_env.nel > 3*nroff_env.sps)) &&
	   (!(nroff_env.hyf & 02) || (findt1() > nroff_env.lss))
	  )hyphen(wp);
	savwch = nroff_env.wch;
	hyp = nroff_env.hyptr;
	nhyp = 0;
	while(*hyp && (*hyp <= wp))hyp++;
	while(nroff_env.wch){
		if((nroff_env.hyoff != 1) && (*hyp == wp)){
			hyp++;
			if(!nroff_env.wd_start ||
			   ((wp > (nroff_env.wd_start+1)) &&
			    (wp < nroff_env.wdend) &&
			    (!(nroff_env.hyf & 04) || (wp < (nroff_env.wdend-1))) &&
			    (!(nroff_env.hyf & 010) || (wp > (nroff_env.wd_start+2)))
			   )
			  ){
				nhyp++;
				storeline(IMP,0);
			}
		}
		i = *wp++;
		w = width(i);
		nroff_env.wne -= w;
		nroff_env.wch--;
		storeline(i,w);
	}
	if(nroff_env.nel >= 0){
		nroff_env.nwd++;
		return(0);
	}
	xbitf = 1;
	hys = width(0200); /*hyphen*/
m1:
	if(!nhyp){
		if(!nroff_env.nwd)goto m3;
		if(nroff_env.wch == savwch)goto m4;
	}
	if(*--nroff_env.linep != IMP)goto m5;
	if(!(--nhyp))
		if(!nroff_env.nwd)goto m2;
	if(nroff_env.nel < hys){
		nroff_env.nc--;
		goto m1;
	}
m2:
	if(((i = *(nroff_env.linep-1) & CMASK) != '-') &&
	   (i != 0203)
	  ){
	*nroff_env.linep = (*(nroff_env.linep-1) & ~CMASK) | 0200;
	w = width(*nroff_env.linep);
	nroff_env.nel -= w;
	nroff_env.ne += w;
	nroff_env.linep++;
/*
	hsend();
*/
	}
m3:
	nroff_env.nwd++;
m4:
	nroff_env.wordp = wp;
	return(1);
m5:
	nroff_env.nc--;
	w = width(*nroff_env.linep);
	nroff_env.ne -= w;
	nroff_env.nel += w;
	nroff_env.wne += w;
	nroff_env.wch++;
	wp--;
	goto m1;
}
horiz(i)
int i;
{
	vflag = 0;
	if(i)pchar(makem(i));
}
setnel(){
	if(!nroff_env.nc){
		nroff_env.linep = nroff_env.line;
		if(nroff_env.un1 >= 0){
			nroff_env.un = nroff_env.un1;
			nroff_env.un1 = -1;
		}
		nroff_env.nel = nroff_env.ll - nroff_env.un;
		nroff_env.ne = nroff_env.adsp = nroff_env.adrem = 0;
	}
}
getword(x)
int x;
{
	register i, j, swp;
	int noword;

	noword = 0;
	if(x)if(nroff_env.pendw){
		*nroff_env.pendw = 0;
		goto rtn;
	}
	if(nroff_env.wordp = nroff_env.pendw)goto g1;
	hyp = nroff_env.hyptr;
	nroff_env.wordp = nroff_env.word;
	over = nroff_env.wne = nroff_env.wch = 0;
	nroff_env.hyoff = 0;
	while(1){
		j = (i = GETCH()) & CMASK;
		if(j == '\n'){
			nroff_env.wne = nroff_env.wch = 0;
			noword = 1;
			goto rtn;
		}
		if(j == nroff_env.ohc){
			nroff_env.hyoff = 1;
			continue;
		}
		if(j == ' '){
			storeword(i,cwidth);
			continue;
		}
		break;
	}
	swp = widthp;
	storeword(' ' | nroff_env.chbits, -1);
	if(nroff_env.spflg){
		storeword(' ' | nroff_env.chbits, -1);
		nroff_env.spflg = 0;
	}
	widthp = swp;
g0:
	if(j == CONT){
		nroff_env.pendw = nroff_env.wordp;
		nflush = 0;
		flushi();
		return(1);
	}
	if(nroff_env.hyoff != 1){
		if(j == nroff_env.ohc){
			nroff_env.hyoff = 2;
			*hyp++ = nroff_env.wordp;
			if(hyp > (nroff_env.hyptr+NHYP-1))hyp = nroff_env.hyptr+NHYP-1;
			goto g1;
		}
		if((j == '-') ||
		   (j == 0203) /*3/4 Em dash*/
		  )if(nroff_env.wordp > nroff_env.word+1){
			nroff_env.hyoff = 2;
			*hyp++ = nroff_env.wordp + 1;
			if(hyp > (nroff_env.hyptr+NHYP-1))hyp = nroff_env.hyptr+NHYP-1;
		}
	}
	storeword(i,cwidth);
g1:
	j = (i = GETCH()) & CMASK;
	if(j != ' '){
		if(j != '\n')goto g0;
		j = *(nroff_env.wordp-1) & CMASK;
		if((j == '.') ||
		   (j == '!') ||
		   (j == '?'))nroff_env.spflg++;
	}
	*nroff_env.wordp = 0;
rtn:
	nroff_env.wd_start = 0;
	nroff_env.wordp = nroff_env.word;
	nroff_env.pendw = 0;
	*hyp++ = 0;
	setnel();
	return(noword);
}
storeword(c,w)
int c, w;
{

	if(nroff_env.wordp >= &nroff_env.word[WDSIZE - 1]){
		if(!over){
			prstrfl("Word overflow.\n");
			over++;
			c = 0343;
			w = -1;
		goto s1;
		}
		return 0;
	}
s1:
	if(w == -1)w = width(c);
	nroff_env.wne += w;
	*nroff_env.wordp++ = c;
	nroff_env.wch++;
}
#ifdef NROFF
extern char trtab[];
gettch(){
	register int i, j;

	if(!((i = getch()) & MOT) && (i & ulbit)){
		j = i&CMASK;
		if(nroff_env.cu && (trtab[j] == ' '))
			i = ((i & ~ulbit)& ~CMASK) | '_';
		if(!nroff_env.cu && (j>32) && (j<0370) && !(*t.codetab[j-32] & 0200))
			i &= ~ulbit;
	}
	return(i);
}
#endif
