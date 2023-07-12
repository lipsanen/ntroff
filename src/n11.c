#include "suftab.c"
#include <string.h>

static int suftab[8192];
static int suftab_initialized = 0;
extern int sufind[26];

// lipsanen: moved suftab inside the nroff binary

lazy_suftab_init()
{
    if(suftab_initialized) 
        return 0;
    
    // suftab generation code adapted from Kernighan's suftabmake
    suftab_initialized = 1;    
    int suflet = 0;
    int filepos = 26 * sizeof(int);
    int yes = 1;
    int no = 0;
    int nothing;
    #define deftoc(table, exists) if (exists) {sufind[suflet++] = filepos; \
                                                filepos += sizeof(table); } \
                                    else sufind[suflet++] = 0;
    #define puttab(table) memcpy((char*)suftab + filepos, table, sizeof(table));

	deftoc(sufa, yes)
	deftoc(nothing, no)	/* no b */
	deftoc(sufc, yes)
	deftoc(sufd, yes)
	deftoc(sufe, yes)
	deftoc(suff, yes)
	deftoc(sufg, yes)
	deftoc(sufh, yes)
	deftoc(sufi, yes)
	deftoc(nothing, no)	/* no j */
	deftoc(sufk, yes)
	deftoc(sufl, yes)
	deftoc(sufm, yes)
	deftoc(sufn, yes)
	deftoc(sufo, yes)
	deftoc(sufp, yes)
	deftoc(nothing, no)	/* no q */
	deftoc(sufr, yes)
	deftoc(sufs, yes)
	deftoc(suft, yes)
	deftoc(nothing, no)	/* no u */
	deftoc(nothing, no)	/* no v */
	deftoc(nothing, no)	/* no w */
	deftoc(nothing, no)	/* no x */
	deftoc(sufy, yes)
	deftoc(nothing, no)	/* no z */
    
	puttab(sufa);
	puttab(sufc);
	puttab(sufd);
	puttab(sufe);
	puttab(suff);
	puttab(sufg);
	puttab(sufh);
	puttab(sufi);
	puttab(sufk);
	puttab(sufl);
	puttab(sufm);
	puttab(sufn);
	puttab(sufo);
	puttab(sufp);
	puttab(sufr);
	puttab(sufs);
	puttab(suft);
	puttab(sufy);

    return 0;
}

rdsufb(i)
char *i;
{
    lazy_suftab_init();
    return suftab[(int)i];
}
