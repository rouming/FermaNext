#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <errno.h>

#include "simple_truss_calc.h"

static float li[15];
static float amb[15][2];
static char s_lin[11], s_for[11];
static float d[15][3];
static float kob[18][18];
static long ITOP[15][2];   // topologiya stergney
static float F[15];        // nachalnie ploshadi
static float COR[9][2];    // koordinati uzlov
static long MS[9][2];      // zakrepleniya uzlov
static float P[18][3];     // nagruzki
static long NST;           // chislo stergney
static long NYZ;           // chislo uzlov
static long NY;            // chislo zakrepleniy
static float E;            // modul uprugosti
static long NSN;           // chislo sluchaev nagrugeniya
static float SD;           // dopuskaemoe napryagenie
static long N1;            // chislo neizvestinih
static float U[18][3];     // peremeshenie uzlov
static float SIGP[15][3];  // napryajenie v sterjnyah
static float kgv;

static void mgk ()
{
	float K1;
	long i = 0, j = 0, n = 0, k = 0, m = 0, l = 0, k2 = 0;
	long FORLIM;

	for (i = 0; i <= 17; i++) {
		for (j = 0; j <= 17; j++)
			kob[i][j] = 0.0;
	}
	FORLIM = NST;
	for (i = 0; i < FORLIM; i++) {
		for (n = 1; n <= 2; n++) {
			k = ITOP[i][n-1];
			for (m = 1; m <= 2; m++) {
				l = ITOP[i][m-1];
				K1 = -1.0;
				if (n == m)
					K1 = 1.0;
				kob[k * 2 - 2][l * 2 - 2] += d[i][0] * F[i] * K1;
				kob[k * 2 - 1][l * 2 - 2] += d[i][1] * F[i] * K1;
				kob[k * 2 - 2][l * 2 - 1] += d[i][1] * F[i] * K1;
				kob[k * 2 - 1][l * 2 - 1] += d[i][2] * F[i] * K1;
			}
		}
	}
	FORLIM = NYZ;
	for (i = 1; i <= FORLIM; i++) {
		for (j = 1; j <= 2; j++) {
			k2 = i * 2 + j - 2;
			if (MS[i-1][j-1] == 0)
				kob[k2-1][k2-1] = E * 1000000.0;
		}
	}
}

static void napr ()
{
	long i = 0, j = 0, k = 0, n = 0;
	long FORLIM, FORLIM1;

	for (j = 0; j <= 2; j++) {
		for (i = 0; i <= 14; i++)
			SIGP[i][j] = 0.0;
	}
	FORLIM = NSN;
	for (j = 0; j < FORLIM; j++) {
		FORLIM1 = NST;
		for (i = 0; i < FORLIM1; i++) {
			k = ITOP[i][0];
			n = ITOP[i][1];
			SIGP[i][j] = -(((U[k * 2 - 2][j] - U[n * 2 - 2][j]) *
							amb[i][0] + (U[k * 2 - 1][j] - U[n * 2 - 1][j]) *
							amb[i][1]) * E / li[i]);
		}
	}
}

static void resly ()
{
	long i;
	long j = 0, i1 = 0, k = 0, iin = 0;
	float z;
	long FORLIM, FORLIM1, FORLIM2;

	for (i = 0; i <= 17; i++) {
		for (j = 0; j <= 2; j++)
			U[i][j] = 0.0;
	}

	kgv = 0.0;
	FORLIM = N1;
	for (i = 0; i < FORLIM; i++) {
		FORLIM1 = N1;
		for (j = i + 1; j <= FORLIM1; j++) {
			if (i == 0)
				goto _L65;
			i1 = i;
			for (k = 0; k < i1; k++)
				kob[i][j-1] -= kob[j-1][k] * kob[i][k];

		_L65:
			if (i - j == -1) {
				if (kob[i][i] > 0)
					goto _L80;
				else {
					kgv = 1.0;
					goto _L222;
				}
			} else
				goto _L63;
		_L80:
			kob[i][i] = sqrt(kob[i][i]);
			goto _L78;
		_L63:
			kob[j-1][i] = kob[i][j-1] / kob[i][i];
		_L78: ;
		}
	}


	FORLIM = NSN;
	for (j = 0; j < FORLIM; j++) {
		FORLIM1 = N1;
		for (i = 0; i < FORLIM1; i++) {
			z = P[i][j];
			if (i == 0)
				goto _L71;
			i1 = i;
			for (k = 0; k < i1; k++)
				z -= kob[i][k] * U[k][j];
		_L71:
			U[i][j] = z / kob[i][i];


		}
		FORLIM1 = N1;
		for (iin = 1; iin <= FORLIM1; iin++) {
			i = N1 - iin + 1;
			z = U[i-1][j];
			if (i - N1 == 0)
				goto _L75;
			i1 = i + 1;
			FORLIM2 = N1;
			for (k = i1 - 1; k < FORLIM2; k++)
				z -= kob[k][i-1] * U[k][j];
		_L75:
			U[i-1][j] = z / kob[i-1][i-1];
		}
	}
_L222: ;
}

int simple_truss_calc (
	const char* in_file,
	const char* out_file )
{
	FILE *d1 = NULL, *dd = NULL;
	long mg = 0;
	long i, j, i1, jj, k, ka;
	float co;
	float sg = 0.0, fm = 0.0, sima = 0.0;
	float s1, s2, s3;
	float fp[15];
	float sim[15];
	long FORLIM, FORLIM1;
	char *TEMP;
	double TEMP1, TEMP2;

	d1 = fopen(in_file, "r");
	if ( d1 == 0 )
		return errno;

	dd = fopen(out_file, "w");
	if ( dd == 0 ) {
		fclose(d1);
		return errno;
	}
	fprintf(dd, "%s\n", in_file);

	fscanf(d1, "%ld%*[^\n]", &NST);
	getc(d1);
	fprintf(dd, "%ld\n", NST);

	fscanf(d1, "%ld%*[^\n]", &NYZ);
	getc(d1);
	fprintf(dd, "%ld\n", NYZ);

	fscanf(d1, "%ld%*[^\n]", &NY);
	getc(d1);
	fprintf(dd, "%ld\n", NY);

	fscanf(d1, "%g%*[^\n]", &E);
	getc(d1);
	fprintf(dd, "% .14E\n", E);

	fscanf(d1, "%ld%*[^\n]", &NSN);
	getc(d1);
	fprintf(dd, "%ld\n", NSN);

	fscanf(d1, "%g%*[^\n]", &SD);
	getc(d1);
	fprintf(dd, "% .14E\n", SD);

	fprintf(dd, "\nMassiv topologiy stergney\n");
	FORLIM = NST;
	for (i = 0; i < FORLIM; i++) {
		fscanf(d1, "%ld%ld%*[^\n]", ITOP[i], &ITOP[i][1]);
		getc(d1);
		fprintf(dd, "%ld%ld\n", ITOP[i][0], ITOP[i][1]);
	}

	fprintf(dd, "\nMassiv coordinat uzlov\n");
	FORLIM = NYZ;
	for (i = 0; i < FORLIM; i++) {
		fscanf(d1, "%g%g%*[^\n]", COR[i], &COR[i][1]);
		getc(d1);
		fprintf(dd, "% .14E% .14E\n", COR[i][0], COR[i][1]);
	}

	fprintf(dd, "\nMassiv nachalnih ploshadey\n");
	FORLIM = NST;
	for (i = 0; i < FORLIM; i++) {
		fscanf(d1, "%g%*[^\n]", &F[i]);
		getc(d1);
		fprintf(dd, "% .14E\n", F[i]);
	}


	fprintf(dd, "\nMassiv zakrepleniya uzlov\n");
	FORLIM = NYZ;
	for (i = 0; i < FORLIM; i++) {
		fscanf(d1, "%ld%ld%*[^\n]", MS[i], &MS[i][1]);
		getc(d1);
		fprintf(dd, "%ld%ld\n", MS[i][0], MS[i][1]);
	}

	fprintf(dd, "\nMassiv znacheniy nagruzok\n");
	FORLIM = NSN;
	for (i = 0; i < FORLIM; i++) {
		putc('\n', dd);
		FORLIM1 = NYZ * 2;
		for (i1 = 0; i1 < FORLIM1; i1++) {
			fscanf(d1, "%g%*[^\n]", &P[i1][i]);
			getc(d1);
			fprintf(dd, "% .14E\n", P[i1][i]);
		}
	}
	fgets(s_lin, 11, d1);
	TEMP = strchr(s_lin, '\n');
	if (TEMP != NULL)
		*TEMP = 0;
	fgets(s_for, 11, d1);
	TEMP = strchr(s_for, '\n');
	if (TEMP != NULL)
		*TEMP = 0;
	fprintf(dd, "%s\n", s_lin);
	fprintf(dd, "%s\n", s_for);



	/*     Vicheclenie dlin i zagotovok jestkostey stergney*/

	for (i = 0; i <= 14; i++) {
		li[i] = 0.0;
		fp[i] = 0.0;
		for (j = 0; j <= 2; j++) {
			d[i][j] = 0.0;
			for (jj = 0; jj <= 1; jj++)
				amb[i][jj] = 0.0;
		}
	}
	FORLIM = NST;
	for (k = 0; k < FORLIM; k++) {
		i = ITOP[k][0];
		j = ITOP[k][1];

		TEMP1 = COR[j-1][0] - COR[i-1][0];
		TEMP2 = COR[j-1][1] - COR[i-1][1];
		li[k] = sqrt(TEMP1 * TEMP1 + TEMP2 * TEMP2);
		amb[k][0] = (COR[j-1][0] - COR[i-1][0]) / li[k];
		amb[k][1] = (COR[j-1][1] - COR[i-1][1]) / li[k];
		TEMP1 = amb[k][0];
		d[k][0] = E * (TEMP1 * TEMP1) / li[k];
		d[k][1] = E * amb[k][0] * amb[k][1] / li[k];
		TEMP1 = amb[k][1];
		d[k][2] = E * (TEMP1 * TEMP1) / li[k];
	}

	N1 = NYZ + NYZ;
	co = 1 / SD;

	FORLIM = NST;
	/*     Zasilka malih chisel v ploshadi virojdaushisya stergney*/

	for (i = 0; i < FORLIM; i++) {
		if (F[i] > fm)
			fm = F[i];
	}
	fm *= 0.1e-14;
	FORLIM = NST;
	for (ka = 0; ka < FORLIM; ka++) {
		if (F[ka] < fm)
			F[ka] = fm;
	}


	/*     Raschet napryajennogo sostoyaniya konstrukciy*/

	mgk();
	resly();
	if (kgv == 1)
		goto _L450;
	napr();

	FORLIM = NST;
	/*     Vichislenie maksimalnogo napryajeniya iz 3-h sluchaev nagrujeniya*/


	for (i = 0; i < FORLIM; i++) {
		s1 = fabs(SIGP[i][0]);
		s2 = fabs(SIGP[i][1]);
		s3 = fabs(SIGP[i][2]);
		if (s1 >= s2)
			sim[i] = s1;
		else
			sim[i] = s2;
		if (sim[i] <= s3)
			sim[i] = s3;

		/*TODO:   SIM[I]:=AMAX1(S1,S2,S3);*/
		if (sim[i] > sima)
			sima = sim[i];
	}

	if (sima >= SD)
		goto _L455;

	if (sima <= SD)
		goto _L555;

_L450:
	for (i = 0; i <= 17; i++) {
		for (j = 0; j <= 2; j++)
			U[i][j] = 0.0;
	}
	for (i = 0; i <= 14; i++) {
		fp[i] = 0.0;
		for (j = 0; j <= 2; j++)
			SIGP[i][j] = 0.0;
	}
	goto _L454;
_L455:
	mg = 1;
	sg = 0.0;
	FORLIM = NST;
	for (i = 0; i < FORLIM; i++) {
		sg += sim[i] * F[i] * li[i];
		fp[i] = sim[i] * co * F[i];
	}
	goto _L454;

	/*   Raschet silovogo vesa konstrukcii i potrebnih ploshadey sterjney*/

_L555:
	mg = 2;
	sg = 0.0;
	FORLIM = NST;
	for (i = 0; i < FORLIM; i++) {
		sg += sim[i] * F[i] * li[i];
		fp[i] = sim[i] * co * F[i];
	}

_L454:
	fprintf(dd, "Silovoy ves konstrukcii\n");
	fprintf(dd, "% .14E\n", sg);

	fprintf(dd, "Parameter opredeleyaushiy uslovie perehoda\n");
	fprintf(dd, "%ld\n", mg);


	fprintf(dd, "Dlini stergeney\n");
	FORLIM = NST;
	for (i = 0; i < FORLIM; i++)
		fprintf(dd, "% .14E\n", li[i]);

	fprintf(dd, "Peremeshenie uzlov\n");
	FORLIM = NSN;
	for (i = 0; i < FORLIM; i++) {
		putc('\n', dd);
		FORLIM1 = NYZ;
		for (i1 = 1; i1 <= FORLIM1; i1++)
			fprintf(dd, "% .14E % .14E\n", U[i1 * 2 - 2][i], U[i1 * 2 - 1][i]);
	}


	fprintf(dd, "Napryajenie v sterjnyah\n");
	FORLIM = NSN;
	for (i = 0; i < FORLIM; i++) {
		fprintf(dd, "\n");
		FORLIM1 = NST;
		for (i1 = 0; i1 < FORLIM1; i1++)
			fprintf(dd, "% .14E\n", SIGP[i1][i]);
	}

	fprintf(dd, "\nPotrebnie ploshadi stergney\n");
	FORLIM = NST;
	for (i = 0; i < FORLIM; i++)
		fprintf(dd, "% .14E\n", fp[i]);

	if (dd != NULL)
		fclose(dd);
	dd = NULL;
	if (d1 != NULL)
		fclose(d1);
	d1 = NULL;

	return 0;
}
