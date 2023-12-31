#define NDI 5
#define NN 170
#define NNAMES 14
struct d_t {int op,dnl,dimac,ditrap,ditf,alss,blss,nls,mkline,
		maxl,hnl,curd;};
extern struct d_t d[NDI];
extern struct {int pn,nl,yr,hp,ct,dn,mo,dy,dw,ln,dl,st,sb,cd;
	int vxx[NN-NNAMES];} v ;

extern int block;
extern int nroff_env_ics;
extern int nroff_env_ic;
extern int nroff_env_icf;
extern int nroff_env_chbits;
extern int nroff_env_nmbits;
extern int nroff_env_apts;
extern int nroff_env_apts1;
extern int nroff_env_pts;
extern int nroff_env_pts1;
extern int nroff_env_font;
extern int nroff_env_font1;
extern int nroff_env_sps;
extern int nroff_env_spacesz;
extern int nroff_env_lss;
extern int nroff_env_lss1;
extern int nroff_env_ls;
extern int nroff_env_ls1;
extern int nroff_env_ll;
extern int nroff_env_ll1;
extern int nroff_env_lt;
extern int nroff_env_lt1;
extern int nroff_env_ad;
extern int nroff_env_nms;
extern int nroff_env_ndf;
extern int nroff_env_fi;
extern int nroff_env_cc;
extern int nroff_env_c2;
extern int nroff_env_ohc;
extern int nroff_env_tdelim;
extern int nroff_env_hyf;
extern int nroff_env_hyoff;
extern int nroff_env_un1;
extern int nroff_env_tabc;
extern int nroff_env_dotc;
extern int nroff_env_adsp;
extern int nroff_env_adrem;
extern int nroff_env_nel;
extern int nroff_env_admod;
extern int *nroff_env_wordp;
extern int nroff_env_spflg;
extern int *nroff_env_linep;
extern int *nroff_env_wdend;
extern int *nroff_env_wd_start;
extern int nroff_env_wne;
extern int nroff_env_ne;
extern int nroff_env_nc;
extern int nroff_env_nb;
extern int nroff_env_lnmod;
extern int nroff_env_nwd;
extern int nroff_env_nn;
extern int nroff_env_ni;
extern int nroff_env_ul;
extern int nroff_env_cu;
extern int nroff_env_ce;
extern int nroff_env_in;
extern int nroff_env_in1;
extern int nroff_env_un;
extern int nroff_env_wch;
extern int nroff_env_pendt;
extern int *nroff_env_pendw;
extern int nroff_env_pendnf;
extern int nroff_env_spread;
extern int nroff_env_it;
extern int nroff_env_itmac;
extern int nroff_env_lnsize;
extern int *nroff_env_hyptr[NHYP];
extern int nroff_env_tabtab[NTAB];
extern int nroff_env_line[LNSIZE];
extern int nroff_env_word[WDSIZE];
extern int nroff_env_blockxxx[EVS-64-NHYP-NTAB-WDSIZE-LNSIZE];
/*spare 7 words*/
extern int nroff_env_oline[LNSIZE+1];

struct nroff_env_t { 
	int block;
	int ics;
	int ic;
	int icf;
	int chbits;
	int nmbits;
	int apts;
	int apts1;
	int pts;
	int pts1;
	int font;
	int font1;
	int sps;
	int spacesz;
	int lss;
	int lss1;
	int ls;
	int ls1;
	int ll;
	int ll1;
	int lt;
	int lt1;
	int ad;
	int nms;
	int ndf;
	int fi;
	int cc;
	int c2;
	int ohc;
	int tdelim;
	int hyf;
	int hyoff;
	int un1;
	int tabc;
	int dotc;
	int adsp;
	int adrem;
	int lastl;
	int nel;
	int admod;
	int *wordp;
	int spflg;
	int *linep;
	int *wdend;
	int *wd_start;
	int wne;
	int ne;
	int nc;
	int nb;
	int lnmod;
	int nwd;
	int nn;
	int ni;
	int ul;
	int cu;
	int ce;
	int in;
	int in1;
	int un;
	int wch;
	int pendt;
	int *pendw;
	int pendnf;
	int spread;
	int it;
	int itmac;
	int lnsize;
	int *hyptr[NHYP];
	int tabtab[NTAB];
	int line[LNSIZE];
	int word[WDSIZE];
	int blockxxx[EVS-64-NHYP-NTAB-WDSIZE-LNSIZE];
	/*spare 7 words*/
	int oline[LNSIZE+1];
};

extern struct nroff_env_t nroff_env;