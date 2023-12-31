#include "tdef.h"
#include "nroff.h"
#include "tw.h"
#include <errno.h>
#include <time.h>
/*
troff1.c

consume options, initialization, main loop,
input routines, escape function calling
*/

extern int stdi;
extern int waitf;
extern int nofeed;
extern int quiet;
extern int ptid;
extern int ascii;
extern int npn;
extern int xflg;
extern int stop;
extern char ibuf[IBUFSZ];
extern char xbuf[IBUFSZ];
extern char *ibufp;
extern char *xbufp;
extern char *eibuf;
extern char *xeibuf;
extern int cbuf[NC];
extern int *cp;
extern int *vlist;
extern int nx;
extern int mflg;
extern int ch;
extern int pto;
extern int pfrom;
extern int cps;
extern int nroff_env_chbits;
extern int suffid;
extern int sufind[26];
extern char suftab[];
extern int ibf;
extern int ttyod;
extern int ttys[3];
extern int iflg;
extern int ioff;
extern int init;
extern int rargc;
extern char **argp;
extern char trtab[256];
extern int lgf;
extern int copyf;
extern int eschar;
extern int ch0;
extern int cwidth;
extern int ip;
extern int nlflg;
extern int *nxf;
extern int *ap;
extern int *frame;
extern int *stk;
extern int donef;
extern int nflush;
extern int nchar;
extern int rchar;
extern int nfo;
extern int ifile;
extern int fc;
extern int padc;
extern int nroff_env_tabc;
extern int nroff_env_dotc;
extern int raw;
extern int nroff_env_tabtab[NTAB];
extern char nextf[];
extern int nfi;
#ifdef NROFF
extern char termtab[];
extern int tti;
#endif
extern int ifl[NSO];
extern int offl[NSO];
extern int ipl[NSO];
extern int ifi;
extern int nroff_env_pendt;
extern int flss;
extern int nroff_env_fi;
extern int lg;
extern char ptname[];
extern int print;
extern int nonumb;
extern int pnlist[];
extern int *pnp;
extern int nroff_env_nb;
extern int trap;
extern int *litlev;
extern int tflg;
extern int ejf;
extern int *ejl;
extern int lit;
extern int nroff_env_cc;
extern int nroff_env_c2;
extern int nroff_env_spread;
extern int gflag;
extern int nroff_env_oline[];
extern int *olinep;
extern int dpn;
extern int noscale;
extern char *unlkp;
extern int nroff_env_pts;
extern int level;
extern int ttysave;
extern int nroff_env_tdelim;
extern int dotT;
extern int tabch, ldrch;
extern int eqflg;
extern no_out;
extern int hflg;
extern int xxx;
char ttyx[] = "/dev/ttyx";
extern struct contab {
	int rq;
	int (*f)();
}contab[NM];
int ms[] = {31,28,31,30,31,30,31,31,30,31,30,31};
#ifndef NROFF
int acctf;
#endif
static char Sccsid[] = "@(#)n1.c	1.7 of 4/26/77";

main(argc,argv)
int argc;
char **argv;
{
	char *p, *q;
	int *cframe;
	register i, j;
	extern catch(), fpecatch(), kcatch();

	signal(SIGHUP,catch);
	if(signal(SIGINT,catch) & 01){
		signal(SIGHUP,1);
		signal(SIGINT,1);
		signal(SIGQUIT,1);
	}
	signal(SIGFPE,fpecatch);
	signal(SIGPIPE,catch);
	signal(SIGKILL,kcatch);
	init1(argv[0][0]);
options:
	while(--argc > 0 && (++argv)[0][0]=='-')
		switch(argv[0][1]){

		case 0:
			goto start;
		case 'i':
			stdi++;
			continue;
		case 'q':
			quiet++;
			continue;
		case 'n':
			npn = cnum(&argv[0][2]);
			continue;
		case 'p':
			xflg = 0;
			cps = cnum(&argv[0][2]);
			continue;
		case 's':
			if(!(stop = cnum(&argv[0][2])))stop++;
			continue;
		case 'r':
			vlist[findr(argv[0][2])] = cnum(&argv[0][3]);
			continue;
		case 'm':
			p = &nextf[nfi];
			q = &argv[0][2];
			while((*p++ = *q++) != 0);
			mflg++;
			continue;
		case 'o':
			getpn(&argv[0][2]);
			continue;
#ifdef NROFF
		case 'h':
			hflg++;
			continue;
		case 'z':
			no_out++;
			continue;
		case 'e':
			eqflg++;
			continue;
		case 'T':
			p = &termtab[tti];
			q = &argv[0][2];
			if(!((*q) & 0177))continue;
			while((*p++ = *q++) != 0);
			dotT++;
			continue;
#endif
#ifndef NROFF
		case 'z':
			no_out++;
		case 'a':
			ascii = 1;
			nofeed++;
		case 't':
			ptid = 1;
			continue;
		case 'w':
			waitf = 1;
			continue;
		case 'f':
			nofeed++;
			continue;
		case 'x':
			xflg = 0;
			continue;
		case 'b':
			if(open(ptname,1) < 0)prstr("Busy.\n");
			else prstr("Available.\n");
			done3(0);
		case 'g':
			stop = ptid = gflag = 1;
			dpn = 0;
			continue;
#endif
		default:
			pto = cnum(&argv[0][1]);
			continue;
		}

	if(argv[0][0] == '+'){
		pfrom = cnum(&argv[0][1]);
		print = 0;
		if(argc > 0)goto options;
	}

start:
	argp = argv;
	rargc = argc;
	init2();
	setexit();
loop:
	copyf = lgf = nroff_env.nb = nflush = nlflg = 0;
	cframe = frame;
	i = getch();
	if((frame != cframe) && ejf && (frame <= ejl)){
		nflush++;
		trap = 0;
		ch = i;
		eject(0);
		goto loop;
	}
	if(nroff_env.pendt)goto lt;
	if(lit && (frame <= litlev)){
		lit--;
		goto lt;
	}
	if((j = (i & CMASK)) == XPAR){
		copyf++;
		tflg++;
		for(;(i & CMASK) != '\n';)pchar(i = getch());
		tflg = 0;
		copyf--;
		goto loop;
	}
	if((j == nroff_env.cc) || (j == nroff_env.c2)){
		if(j == nroff_env.c2)nroff_env.nb++;
		copyf++;
		while(((j=((i=getch()) & CMASK)) == ' ') ||
			(j == '\t'));
		ch = i;
		copyf--;
		control(getrq(),1);
		flushi();
		goto loop;
	}
lt:
	ch = i;
	text();
	goto loop;
}
catch(){
/*
	prstr("Interrupt\n");
*/
	done3(01);
}
fpecatch(){
	prstrfl("Floating Exception.\n");
	signal(SIGFPE,fpecatch);
}
kcatch(){
	signal(SIGKILL,1);
	done3(01);
}
#ifndef NROFF
acctg() {
	acctf = open("/usr/actg/data/troffactg",1);
	setuid(getuid());
}
#endif
init1(a)
char a;
{
	register char *p;
	register i;

#ifndef NROFF
	acctg();/*open troff actg file while mode 4755*/
#endif
	lazy_suftab_init();
	init_env();
	char template_name[] = "/tmp/taXXXXXX";
	mktemp(template_name);
	p = template_name;	
	if(a == 'a')p = template_name + 5;
	int creat_result = creat(p, 0600);
	int close_result = close(creat_result);
	if(close_result < 0){
		prstr("Cannot create temp file.\n");
		exit(-1);
	}
	ibf = open(p, 2);
	for(i=256; --i;)trtab[i]=i;
	trtab[UNPAD] = ' ';
	mchbits();
	if(a != 'a')unlkp = p;
}
init2()
{
	register i,j;
	extern int block;

	ttyod = 2;
	j = 0;
	i = 'x';
	iflg = j;
	gtty(j,ttys);
	ttysave = ttys[2];
	if(ascii)mesg(0);

	if((!ptid) && (!waitf)){
		if((ptid = open(ptname,1)) < 0){
			prstr("Typesetter busy.\n");
			done3(-2);
		}
	}
	for(i=NEV; i--;)write(ibf, &nroff_env, sizeof(nroff_env));
	olinep = nroff_env.oline;
	ibufp = eibuf = ibuf;
	v.hp = ioff = init = 0;
	v.nl = -1;
	cvtime();
	frame = stk = setbrk(DELTA);
	nxf = frame + STKSIZE;
	nx = mflg;
}
setexit()
{
	return 0;
}
reset()
{
	return 0;
}
int
cvtime()
{
	time_t	tt;
	struct tm *tm;

	tt = time((time_t *) 0);
	tm = localtime(&tt);
	v.dy = tm->tm_yday;
	v.dw = tm->tm_wday + 1;
	v.yr = tm->tm_year;
	v.mo = tm->tm_mon + 1;

	return (0);
}
cnum(a)
char *a;
{
	register i;

	ibufp = a;
	eibuf = -1;
	i = atoi();
	ch = 0;
	return(i);
}
mesg(f)
int f;
{
	static int mode;

	if(!f){
		stat(ttyx,cbuf);
		mode = cbuf[2];
		chmod(ttyx,mode & ~022);
	}else{
		chmod(ttyx,mode);
	}
}
prstrfl(s)
char *s;
{
	flusho();
	prstr(s);
}
prstr(s)
char *s;
{
	register i;

	for(i=0;*s;i++)s++;
	write(ttyod,s-i,i);
}
control(a,b)
int a,b;
{
	register i,j;

	i = a;
	if((i == 0) || ((j = findmn(i)) == -1))return(0);
	if(contab[j].rq & MMASK){
		*nxf = 0;
		if(b)collect();
		flushi();
		return(pushi(contab[j].f));
	}else{
		if(!b)return(0);
		return((*contab[j].f)(0));
	}
}

getrq(){
	register i,j;

	if(((i=getach()) == 0) ||
	   ((j=getach()) == 0))goto rtn;
	i |= (j<<BYTE);
rtn:
	return(i);
}
gtty() {
	return 0;
}
stty() {
	return 0;
}
getch(){
	register int i, j, k;

	level++;
g0:
	if(ch){
		if(((i = ch) & CMASK) == '\n')nlflg++;
		ch = 0;
		level--;
		return(i);
	}

	if(nlflg){
		level--;
		return('\n');
	}

	if((k = (i = getch0()) & CMASK) != ESC){
		if(i & MOT)goto g2;
		if(k == FLSS){
			copyf++; raw++;
			i = getch0();
			if(!nroff_env.fi)flss = i;
			copyf--; raw--;
			goto g0;
		}
		if(k == RPT){
			setrpt();
			goto g0;
		}
		if(!copyf){
			if((k == 'f') && lg && !lgf){
				i = getlg(i);
				goto g2;
			}
			if((k == fc) || (k == tabch) || (k == ldrch)){
				if((i=setfield(k)) == 0)goto g0; else goto g2;
			}
			if(k == 010){
				i = makem(-width(' ' | nroff_env.chbits));
				goto g2;
			}
		}
		goto g2;
	}
	k = (j = getch0()) & CMASK;
	if(j & MOT){
		i = j;
		goto g2;
	}
/*
	if(k == nroff_env.tdelim){
		i = TDELIM;
		nroff_env.tdelim = IMP;
		goto g2;
	}
*/
	switch(k){

		case '\n':	/*concealed newline*/
			goto g0;
		case 'n':	/*number register*/
			setn();
			goto g0;
		case '*':	/*string indicator*/
			setstr();
			goto g0;
		case '$':	/*argument indicator*/
			seta();
			goto g0;
		case '{':	/*LEFT*/
			i = LEFT;
			goto gx;
		case '}':	/*RIGHT*/
			i = RIGHT;
			goto gx;
		case '"':	/*comment*/
			while(((i=getch0()) & CMASK ) != '\n');
			goto g2;
		case ESC:	/*double backslash*/
			i = eschar;
			goto gx;
		case 'e':	/*printable version of current eschar*/
			i = PRESC;
			goto gx;
		case ' ':	/*unpaddable space*/
			i = UNPAD;
			goto gx;
		case '|':	/*narrow space*/
			i = NARSP;
			goto gx;
		case '^':	/*half of narrow space*/
			i = HNSP;
			goto gx;
		case '\'':	/*\(aa*/
			i = 0222;
			goto gx;
		case '`':	/*\(ga*/
			i = 0223;
			goto gx;
		case '_':	/*\(ul*/
			i = 0224;
			goto gx;
		case '-':	/*current font minus*/
			i = 0210;
			goto gx;
		case '&':	/*filler*/
			i = FILLER;
			goto gx;
		case 'c':	/*to be continued*/
			i = CONT;
			goto gx;
		case ':':	/*lem's char*/
			i = COLON;
			goto gx;
		case '!':	/*transparent indicator*/
			i = XPAR;
			goto gx;
		case 't':	/*tab*/
			i = '\t';
			goto g2;
		case 'a':	/*leader (SOH)*/
			i = LEADER;
			goto g2;
		case '%':	/*ohc*/
			i = OHC;
			goto g2;
		case '.':	/*.*/
			i = '.';
		gx:
			i = (j & ~CMASK) | i;
			goto g2;
	}
g1:
	if(!copyf)
		switch(k){

			case 'p':	/*spread*/
				nroff_env.spread++;
				goto g0;
			case '(':	/*special char name*/
				if((i=setch()) == 0)goto g0;
				break;
			case 's':	/*size indicator*/
				setps();
				goto g0;
			case 'f':	/*font indicator*/
				setfont(0);
				goto g0;
			case 'w':	/*width function*/
				setwd();
				goto g0;
			case 'v':	/*vert mot*/
				if(i = vmot())break;
				goto g0;
			case 'h': 	/*horiz mot*/
				if(i = hmot())break;
				goto g0;
			case 'z':	/*zero with char*/
				i = setz();
				break;
			case 'l':	/*hor line*/
				setline();
				goto g0;
			case 'L':	/*vert line*/
				setvline();
				goto g0;
			case 'b':	/*bracket*/
				setbra();
				goto g0;
			case 'o':	/*overstrike*/
				setov();
				goto g0;
			case 'k':	/*mark hor place*/
				if((i=findr(getsn())) == -1)goto g0;
				vlist[i] = v.hp;
				goto g0;
			case 'j':	/*mark output hor place*/
				if(!(i=getach()))goto g0;
				i = (i<<BYTE) | JREG;
				break;
			case '0':	/*number space*/
				i = makem(width('0' | nroff_env.chbits));
				break;
			case 'x':	/*extra line space*/
				if(i = xlss())break;
				goto g0;
			case 'u':	/*half em up*/
			case 'r':	/*full em up*/
			case 'd':	/*half em down*/
				i = sethl(k);
				break;
			default:
				i = j;
		}
	else{
		ch0 = j;
		i = eschar;
	}
g2:
	if((i & CMASK) == '\n'){
		nlflg++;
		v.hp = 0;
		if(ip == 0)v.cd++;
	}
	if(!--level){
		j = width(i);
		v.hp += j;
		cwidth = j;
	}
	return(i);
}
char ifilt[32] = {0,001,002,003,0,005,006,007,010,011,012};
getch0(){
	register int i, j, k;

	if(ch0){i=ch0; ch0=0; return(i);}
	if(nchar){nchar--; return(rchar);}

again:
	if(cp){
		if((i = *cp++) == 0){
			cp = 0;
			goto again;
		}
	}else if(ap){
		if((i = *ap++) == 0){
			ap = 0;
			goto again;
		}
	}else if(ip){
		if(ip == -1)i = rdtty();
		else i = rbf();
	}else{
		if(donef)done(0);
		if(nx || ((ibufp >= eibuf) && (ibufp != -1))){
			if(nfo)goto g1;
		g0:
			if(nextfile()){
				if(ip)goto again;
				if(ibufp < eibuf)goto g2;
			}
		g1:
			nx = 0;
			if((j=read(ifile,ibuf,IBUFSZ)) <= 0)goto g0;
			ibufp = ibuf;
			eibuf = ibuf + j;
			if(ip)goto again;
		}
	g2:
		i = *ibufp++ & 0177;
		ioff++;
		if(i >= 040)goto g4; else i = ifilt[i];
	}
	if(raw)return(i);
	if((j = i & CMASK) == IMP)goto again;
	if((i == 0) && !init)goto again;
g4:
	if((copyf == 0) && ((i & ~BMASK) == 0) && ((i & CMASK) < 0370))
		i |= nroff_env.chbits;
	if((i & CMASK) == eschar)i = (i & ~CMASK) | ESC;
	return(i);
}
nextfile(){
	register char *p;

n0:
	if(ifile)close(ifile);
	if(nx){
		p = nextf;
		if(*p != 0)goto n1;
	}
	if(ifi > 0){
		if(popf())goto n0; /*popf error*/
		return(1); /*popf ok*/
	}
	if(rargc-- <= 0)goto n2;
	p = (argp++)[0];
n1:
	if((p[0] == '-') && (p[1] == 0)){
		ifile = 0;
	}else if((ifile=open(p,0)) < 0){
		prstr("Cannot open ");
		prstr(p);
		prstr("\n");
		nfo -= mflg;
		done(02);
	}
	nfo++;
	ioff = v.cd = 0;
	return(0);
n2:
	if((nfo -= mflg) && !stdi)done(0);
	nfo++;
	v.cd = ioff = ifile =  stdi = mflg = 0;
	return(0);
}
popf(){
	register i, *p, *q;

	ioff = offl[--ifi];
	ip = ipl[ifi];
	if((ifile = ifl[ifi]) == 0){
		p = xbuf;
		q = ibuf;
		ibufp = xbufp;
		eibuf = xeibuf;
		while(q < eibuf)*q++ = *p++;
		return(0);
	}
	if((lseek(ifile,ioff & ~(IBUFSZ-1),0) < 0) ||
	   ((i = read(ifile,ibuf,IBUFSZ)) < 0))return(1);
	eibuf = ibuf + i;
	ibufp = ibuf;
	if(tty(ifile) == 'x')
		if((ibufp = ibuf + (ioff & (IBUFSZ-1)))  >= eibuf)return(1);
	return(0);
}
flushi(){
	if(nflush)return 0;
	ch = 0;
	if((ch0 & CMASK) == '\n')nlflg++;
	ch0 = 0;
	copyf++;
	while(!nlflg){
		if(donef && (frame == stk))break;
		getch();
	}
	copyf--;
	v.hp = 0;
}
getach(){
	register i;

	lgf++;
	if(((i = getch()) & MOT) ||
	    ((i&CMASK) == ' ') ||
	    ((i&CMASK) == '\n')||
	    (i & 0200)){
			ch = i;
			i = 0;
	}
	lgf--;
	return(i & 0177);
}
casenx(){
	lgf++;
	skip();
	getname();
	nx++;
	nextfile();
	nlflg++;
	ip = ap = nchar = nroff_env.pendt = 0;
	frame = stk;
	nxf = frame + STKSIZE;
}
getname(){
	register int i, j, k;

	lgf++;
	for(k=0; k < (NS-1); k++){
		if(((j=(i=getch()) & CMASK) <= ' ') ||
			(j > 0176))break;
		nextf[k] = j;
	}
	nextf[k] = 0;
	ch = i;
	lgf--;
	return(nextf[0]);
}
caseso(){
	register i, *p, *q;

	lgf++;
	if(skip() || !getname() || ((i=open(nextf,0)) <0) ||
		(ifi >= NSO))return 0;
	flushi();
	ifl[ifi] = ifile;
	ifile = i;
	offl[ifi] = ioff;
	ioff = 0;
	ipl[ifi] = ip;
	ip = 0;
	nx++;
	nflush++;
	if(!ifl[ifi++]){
		p = ibuf;
		q = xbuf;
		xbufp = ibufp;
		xeibuf = eibuf;
		while(p < eibuf)*q++ = *p++;
	}
}
getpn(a)
char *a;
{
	register i, neg;
	LONG0 atoi1();

	if((*a & 0177) == 0)return 0;
	neg = 0;
	ibufp = a;
	eibuf = -1;
	noscale++;
	while((i = getch() & CMASK) != 0)switch(i){
		case '+':
		case ',':
			continue;
		case '-':
			neg = MOT;
			goto d2;
		default:
			ch = i;
		d2:
			i = atoi1();
			if(nonumb)goto fini;
			else{
				*pnp++ = i | neg;
				neg = 0;
				if(pnp >= &pnlist[NPN-2]){
					prstr("Too many page numbers\n");
					done3(-3);
				}
			}
		}
fini:
	if(neg)*pnp++ = -2;
	*pnp = -1;
	ch = noscale = print = 0;
	pnp = pnlist;
	if(*pnp != -1)chkpn();
}
setrpt(){
	register i, j;

	copyf++;raw++;
	i = getch0();
	copyf--;raw--;
	if((i < 0) ||
	   (((j = getch0()) & CMASK) == RPT))return 0;
	rchar = j;
	nchar = i & BMASK;
}
