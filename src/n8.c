#include "tdef.h"
#include "nroff.h"

/*
troff8.c

hyphenation
*/

char hbuf[NHEX];
char *nexth = hbuf;
int *hyend;
extern int *nroff_env_wdend;
extern int *nroff_env_hyptr[];
extern int **hyp;
extern int nroff_env_hyoff;
extern int suffid;
extern int sufind[26];
char *sufoff = -1;
extern int noscale;
extern int xxx;
#define THRESH 160 /*digram goodness threshold*/
int thresh = THRESH;

hyphen(wp)
int *wp;
{
	register *i, j;

	i = wp;
	while(punct(*i++));
	if(!alph(*--i))return 0;
	nroff_env.wd_start = i++;
	while(alph(*i++));
	hyend = nroff_env.wdend = --i-1;
	while(punct(*i++));
	if(*--i)return 0;
	if((nroff_env.wdend-nroff_env.wd_start-4) < 0)return 0;
	hyp = nroff_env.hyptr;
	*hyp = 0;
	nroff_env.hyoff = 2;
	if(exword() || suffix())goto rtn;
	digram();
rtn:
	*hyp++ = 0;
	if(*nroff_env.hyptr)for(j = 1; j;){
		j = 0;
		for(hyp = nroff_env.hyptr+1; *hyp != 0; hyp++){
			if(*(hyp-1) > *hyp){
				j++;
				i = *hyp;
				*hyp = *(hyp-1);
				*(hyp-1) = i;
			}
		}
	}
}
punct(i)
int i;
{
	if(!i || alph(i))return(0);
	else return(1);
}
alph(i)
int i;
{
	register j;

	j = i & CMASK;
	if(((j >= 'A') && (j <= 'Z')) || ((j >= 'a') && (j <= 'z')))
		return(1);
	else return(0);
}
caseht(){
	register i;

	thresh = THRESH;
	if(skip())return 0;
	noscale++;
	thresh = atoi();
	noscale = 0;
}
casehw(){
	register i, k;
	register char *j;

	k = 0;
	while(!skip()){
		if((j = nexth) >= (hbuf + NHEX - 2)){
		full:
			prstr("Exception word list full.\n");
			*nexth = 0;
			return 0;
		}
		while(1){
			if((i = getch()) & MOT)continue;
			if(((i &= CMASK) == ' ') || (i == '\n')){
				*j++ = 0;
				nexth = j;
				*j = 0;
				if(i == ' ')break;
					else return 0;
			}
			if(i == '-'){
				k = 0200;
				continue;
			}
			*j++ = maplow(i) | k;
			k = 0;
			if(j >= (hbuf + NHEX - 2))goto full;
		}
	}
}
exword(){
	register int *w, i;
	register char *e;
	char *save;

	e = hbuf;
	while(1){
		save = e;
		if(*e == 0)return(0);
		w = nroff_env.wd_start;
		while((*e && (w <= hyend)) &&
		      ((*e & 0177) == maplow(*w & CMASK))){e++; w++;};
		if(!*e){
			if(((w-1) == hyend) ||
			   ((w == nroff_env.wdend) && ((i = maplow(*w & CMASK)) == 's'))){
				w = nroff_env.wd_start;
				for(e = save; *e; e++){
					if(*e & 0200)*hyp++ = w;
					if(hyp > (nroff_env.hyptr+NHYP-1))
						hyp = nroff_env.hyptr+NHYP-1;
					w++;
				}
				return(1);
			}else{e++; continue;}
		}else while(*e++);
	}
}
suffix(){
	register int *w;
	register char *s, *s0;
	int i;
	char *off;

again:
	if(!alph(i = *hyend & CMASK))return(0);
	if(i < 'a')i -= 'A'; else i -= 'a';
	if(!(off = sufind[i]))return(0);
	while(1){
		if((i = *(s0 = getsuf(off)) & 017) == 0)return(0);
		off += i;
		s = s0 + i - 1;
		w = hyend - 1;
		while(((s > s0) && (w >= nroff_env.wd_start)) &&
			((*s & 0177) == maplow(*w))){s--; w--;};
		if(s == s0)break;
	}
	s = s0 + i - 1;
	w = hyend;
	if(*s0 & 0200)goto mark;
	while(s > s0){
		w--;
		if(*s-- & 0200){
		mark:
			hyend = w - 1;
			if(*s0 & 0100)continue;
			if(!chkvow(w))return(0);
			*hyp++ = w;
		}
	}
	if(*s0 & 040)return(0);
	if(exword())return(1);
	goto again;
}
maplow(i)
int i;
{
	if((i &= CMASK) < 'a')i += 'a' - 'A';
	return(i);
}
vowel(i)
int i;
{
	switch(maplow(i)){
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		case 'y':
			return(1);
		default:
			return(0);
	}
}
chkvow(w)
int *w;
{
	while(--w >= nroff_env.wd_start)if(vowel(*w & CMASK))return(w);
	return(0);
}
getsuf(x)
char *x;
{
	register char *s;
	register cnt;
	static char suff[20];

	s = suff;
	for(cnt = ((*s++ = rdsufb(x++)) & 017); cnt > 1; cnt--)
		*s++ = rdsufb(x++);
	suff[suff[0] & 017] = 0;
	return(suff);
}
digram(){
	register *w, val;
	int *nhyend, *maxw, maxval;
	extern char bxh[], bxxh[], xxh[], xhx[], hxx[];

again:
	if(!(w=chkvow(hyend+1)))return 0;
	hyend = w;
	if(!(w=chkvow(hyend)))return 0;
	nhyend = w;
	maxval = 0;
	w--;
	while((++w < hyend) && (w < (nroff_env.wdend-1))){
		val = 1;
		if(w == nroff_env.wd_start)val *= dilook('a',*w,bxh);
		else if(w == nroff_env.wd_start+1)val *= dilook(*(w-1),*w,bxxh);
		else val *= dilook(*(w-1),*w,xxh);
		val *= dilook(*w, *(w+1), xhx);
		val *= dilook(*(w+1), *(w+2), hxx);
		if(val > maxval){
			maxval = val;
			maxw = w + 1;
		}
	}
	hyend = nhyend;
	if(maxval > thresh)*hyp++ = maxw;
	goto again;
}
dilook(a,b,t)
int a, b;
char t[26][13];
{
	register i, j;

	i = t[maplow(a)-'a'][(j = maplow(b)-'a')/2];
	if(!(j & 01))i >>= 4;
	return(i & 017);
}
