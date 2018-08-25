/*
 ============================================================================
 Name        : defensa_cacastillo.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

// XXX: http://codeforces.com/contest/954/problem/G
#if 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>

#ifdef CACA_COMUN_LOG
#include <execinfo.h>
#endif

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#include <stdarg.h>
#endif

#if 1
#ifndef LONG_LONG_MAX
#define LONG_LONG_MAX LONG_MAX
#endif

#ifndef ULONG_LONG_MAX
#define ULONG_LONG_MAX ULONG_MAX
#endif

#define CACA_COMUN_TAM_MAX_LINEA (16*200000)
#define CACA_LOG_MAX_TAM_CADENA 2000

#define CACA_COMUN_BUF_STATICO (char[1000] ) { '\0' }

#define BITCH_VECTOR_NUM_BITS (sizeof(bitch_vector) * 8)

#define CACA_COMUN_ASSERT_DUROTE 0
#define CACA_COMUN_ASSERT_SUAVECITO 1
#define CACA_COMUN_ASSERT_NIMADRES 2

#define CACA_COMUN_VALOR_INVALIDO ((tipo_dato)UINT_MAX)
#define CACA_COMUN_IDX_INVALIDO ((natural)CACA_COMUN_VALOR_INVALIDO)
//#define CACA_COMUN_LOG

typedef unsigned int natural;
typedef int tipo_dato;
typedef long long entero_largo;
typedef unsigned long long entero_largo_sin_signo;
typedef long long bitch_vector;
typedef char byteme;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

/*
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_SUAVECITO
 #define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_NIMADRES
 */
#define CACA_COMUN_TIPO_ASSERT CACA_COMUN_ASSERT_DUROTE

#define assert_timeout_dummy(condition) 0;

#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_DUROTE
#define assert_timeout(condition) assert(condition);
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_SUAVECITO
#define assert_timeout(condition) if(!(condition)){while(1){printf("");};abort();}
#endif
#if CACA_COMUN_TIPO_ASSERT == CACA_COMUN_ASSERT_NIMADRES
#define assert_timeout(condition) 0
#endif

#ifdef CACA_COMUN_LOG
#define caca_log_debug(formato, args...) \
		do \
		{ \
			size_t profundidad = 0; \
			void *array[CACA_LOG_MAX_TAM_CADENA]; \
 			profundidad = backtrace(array, CACA_LOG_MAX_TAM_CADENA); \
			caca_log_debug_func(formato,__FILE__, __func__, __LINE__,profundidad,##args); \
		} \
		while(0);
#else
#define caca_log_debug(formato, args...) 0;
#endif

#define caca_comun_max(x,y) ((x) < (y) ? (y) : (x))
#define caca_comun_min(x,y) ((x) < (y) ? (x) : (y))

#define caca_comun_calloc_local(tipo) (&(tipo){0})

void caca_log_debug_func(const char *format, ...);

void caca_comun_current_utc_time(struct timespec *ts) {

#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	ts->tv_sec = mts.tv_sec;
	ts->tv_nsec = mts.tv_nsec;
#else
#ifdef CACA_COMUN_LOG
	clock_gettime(CLOCK_REALTIME, ts);
#endif
#endif

}

void caca_comun_timestamp(char *stime) {
	time_t ltime;
	struct tm result;
	long ms;
#ifndef ONLINE_JUDGE
	struct timespec spec;
#endif
	char parte_milisecundos[50];

	ltime = time(NULL);

#ifndef ONLINE_JUDGE
	localtime_r(&ltime, &result);
	asctime_r(&result, stime);
#endif

	*(stime + strlen(stime) - 1) = ' ';

#ifndef ONLINE_JUDGE
	caca_comun_current_utc_time(&spec);
	ms = round(spec.tv_nsec / 1.0e3);
#endif
	sprintf(parte_milisecundos, "%ld", ms);
	strcat(stime, parte_milisecundos);
}
#ifdef CACA_COMUN_LOG
void caca_log_debug_func(const char *format, ...) {

	va_list arg;
	va_list arg2;
	const char *PEDAZO_TIMESTAMP_HEADER = "tiempo: %s; ";
	const char *HEADER =
	"archivo: %s; funcion: %s; linea %d; nivel: %zd caca 8====D ";
	char formato[CACA_LOG_MAX_TAM_CADENA + sizeof(HEADER)
	+ sizeof(PEDAZO_TIMESTAMP_HEADER)] = {'\0'};
	char pedazo_timestamp[sizeof(PEDAZO_TIMESTAMP_HEADER) + 100] = {'\0'};
	char cadena_timestamp[100] = {'\0'};

	caca_comun_timestamp(cadena_timestamp);
	sprintf(pedazo_timestamp, PEDAZO_TIMESTAMP_HEADER, cadena_timestamp);

	strcpy(formato, pedazo_timestamp);
	strcat(formato, HEADER);
	strcat(formato, format);
	strcat(formato, "\n");

	va_start(arg, format);
	va_copy(arg2, arg);
	vprintf(formato, arg2);
	va_end(arg2);
	va_end(arg);
	setbuf(stdout, NULL);
}
#endif

#ifdef CACA_COMUN_LOG
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%12d", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}

static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%2u",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	int i, j;
	natural inicio_buffer_act = 0;
	for (int i = 0; i < filas_tam; i++) {
		caca_comun_arreglo_a_cadena(matrix + i * columas_tam, columas_tam,
				buffer + inicio_buffer_act);
		inicio_buffer_act += strlen(buffer + inicio_buffer_act);
		buffer[inicio_buffer_act++] = '\n';
		/*		caca_log_debug("pero q mierda inicio buffer act %u %s",inicio_buffer_act,buffer);*/
	}
	return buffer;
}

static char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(
		entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
		char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;
#ifdef ONLINE_JUDGE
	return NULL;
#endif

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos,
				"%llu", *(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	*(ap_buffer + characteres_escritos) = '\0';
	return ap_buffer;
}
#else
static char *caca_comun_arreglo_a_cadena(tipo_dato *arreglo, int tam_arreglo,
		char *buffer) {
	return NULL;
}
static char *caca_comun_arreglo_a_cadena_natural(natural *arreglo,
		natural tam_arreglo, char *buffer) {
	return NULL;
}
char *caca_comun_matrix_a_cadena(tipo_dato *matrix, natural filas_tam,
		natural columas_tam, char *buffer) {
	return NULL;
}

static char *caca_comun_arreglo_a_cadena_entero_largo_sin_signo(
		entero_largo_sin_signo *arreglo, entero_largo_sin_signo tam_arreglo,
		char *buffer) {
	return NULL;
}
#endif
void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static int caca_comun_lee_matrix_long_stdin(tipo_dato *matrix, int *num_filas,
		int *num_columnas, int num_max_filas, int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	tipo_dato numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(CACA_COMUN_TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas
			&& fgets(linea, CACA_COMUN_TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			indice_columnas++;
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

static inline natural caca_comun_cuenta_bitchs(tipo_dato num) {
	natural bitch_cnt = 0;
	tipo_dato num_mod = 0;
	num_mod = num;
	while (num_mod) {
		num_mod &= ~(num_mod & (-num_mod));
		bitch_cnt++;
	}
	return bitch_cnt;
}

static inline char caca_comun_letra_a_valor_minuscula(char letra) {
	return letra - 'a';
}

static inline natural caca_comun_max_natural(natural *nums, natural nums_tam) {
	natural max = 0;

	for (int i = 0; i < nums_tam; i++) {
		natural num_act = nums[i];
		if (num_act > max) {
			max = num_act;
		}
	}

	return max;
}

// XXX: https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
static inline char *caca_comun_trimea(char *cad, natural cad_tam) {
	char c = cad[0];
	char tmp = '\0';
	natural i = 0;
	natural j = 0;

	caca_log_debug("entrada %s cad_tam %u", cad, cad_tam);
	while (j < cad_tam && cad[j] != '\0') {
		caca_log_debug("en j %u car %c", j, cad[j]);
		while (j < cad_tam && !isalpha(cad[j])) {
			caca_log_debug("brincando j %u car %c", j, cad[j]);
			j++;
		}
		caca_log_debug("aora j %u car %c", j, cad[j]);
		if (j == cad_tam) {
			caca_log_debug("q ped");
			break;
		}
		tmp = cad[i];
		cad[i] = cad[j];
		cad[j] = tmp;
		i++;
		j++;
	}
	caca_log_debug("mierda '%c'", cad[j]);

	i = 0;
	while (isalpha(cad[i++]))
		;
	caca_log_debug("salida %s", cad);
	cad[i - 1] = '\0';

	return cad;
}

#endif

#endif

typedef entero_largo_sin_signo largote;

typedef struct ventana_acumuladora {
	largote *a;
	natural a_tam;
	natural i;
	natural tam_ventana;
	largote disponible;
	largote disponible_original;
	largote acumulado;
	largote objetivo;
	largote *anadidos;
	largote min;
} ventana_acumuladora;

#define CACA_INVALIDA ((largote)-1)

static inline largote avanza(ventana_acumuladora *v) {
	largote r = CACA_INVALIDA;
	largote sumado = 0;
	largote por_anadir = 0;
	natural i_orig = v->i;

	assert(v->i < v->a_tam);

	sumado = v->acumulado + v->a[v->i];
	assert_timeout(v->disponible_original || !v->acumulado);

	por_anadir = v->objetivo - caca_comun_min(sumado, v->objetivo);
	caca_log_debug(
			"en i %u el acum %llu, sumado %llu, por anadir %llu, dispon %llu",
			v->i, v->acumulado, sumado, por_anadir, v->disponible);

	if (por_anadir <= v->disponible) {
		natural idx_anadido = caca_comun_min(v->i + v->tam_ventana,
				v->a_tam - 1);
		assert_timeout(v->disponible_original || !por_anadir);
		v->disponible -= por_anadir;

		v->anadidos[idx_anadido] = por_anadir;

		if (v->i >= v->tam_ventana) {
			v->acumulado -= v->anadidos[v->i - v->tam_ventana];
		}
		v->acumulado += v->anadidos[idx_anadido];
		v->i++;

		r = por_anadir;
	}
	assert_timeout(v->disponible_original || (!v->disponible));

	/*
	 caca_log_debug("obj %llu act %llu r %lld", v->objetivo, v->a[i_orig], r);
	 */
	assert_timeout(v->objetivo >= v->min);
	assert_timeout(v->a[i_orig] >= v->min);
	assert_timeout(
			v->disponible_original || ((v->objetivo <= v->a[i_orig] && !r) || (v->objetivo > v->a[i_orig] && r == CACA_INVALIDA) ));
	return r;
}

static inline largote es_alcanzable(largote *a, natural a_tam, largote k,
		natural r, largote objetivo, largote min) {
	largote res = 0;
	ventana_acumuladora *v = NULL;
	natural i = 0;
	bool min_visto = falso;

	v = calloc(1, sizeof(ventana_acumuladora));
	assert(v);
	v->anadidos = calloc(a_tam, sizeof(largote));
	assert(v->anadidos);

	v->a = a;
	v->a_tam = a_tam;
	v->disponible = k;
	v->disponible_original = k;
	v->objetivo = objetivo;
	v->tam_ventana = r;
	v->min = min;

	for (i = 0; i < a_tam; i++) {
		largote ra = avanza(v);
		assert_timeout(
				k || ((a[i]>=objetivo && !ra)|| (a[i]<objetivo && ra==CACA_INVALIDA)));
		if (a[i] == min) {
			min_visto = verdadero;
		}
		caca_log_debug("en i %u el res de avanzar %llu", i, ra);
		if (ra == CACA_INVALIDA) {
			res = CACA_INVALIDA;
			break;
		}
		res = v->disponible;
		assert_timeout(k || !v->disponible);
	}

	free(v->anadidos);
	free(v);

	if (!k && objetivo == min) {
		assert_timeout(!res);
	}
	if (!k && objetivo > min) {
		assert_timeout(min < 5E5 * 1E9);
//		assert_timeout(min);
//		assert_timeout(res || min_visto);
//		assert_timeout(i < a_tam);
//		assert_timeout(res);
//		assert_timeout(res==CACA_INVALIDA);
	}
//	assert_timeout( k || ((objetivo==min && !res)|| (objetivo>min && res==CACA_INVALIDA)));

	return res;
}

typedef int (*busqueda_binaria_comparar)(void *bb_ctx, largote objetivo);

static inline largote busqueda_binaria(void *busqueda_binaria_ctx,
		busqueda_binaria_comparar cmp_fn, largote min, largote max, largote k) {
	entero_largo i = min;
	entero_largo j = max;
	largote r = CACA_INVALIDA;

	while (i <= j) {
		entero_largo mid = i + ((j - i) >> 1);
		int rc = cmp_fn(busqueda_binaria_ctx, mid);
		caca_log_debug("en i %lld mid %lld j %lld el res %d", i, mid, j, rc);
//		assert_timeout(mid >= min);
//		assert_timeout(k || ((mid == min && !rc) || (mid > min && rc == -1)));
		if (rc < 0) {
			j = mid - 1;
		} else {
			if (rc > 0) {
				i = mid + 1;
				r = mid;
			} else {
				r = mid;
				break;
			}
		}
	}

	return r;
}

typedef struct datos_busqueda_binaria {
	largote *a;
	natural a_tam;
	largote k;
	natural r;
	largote min;
} datos_busqueda_binaria;

int prueba_objetivo(void *dp, largote objetivo) {
	int r = 0;
	largote disponiblidad = 0;
	datos_busqueda_binaria *d = dp;
	bool min_visto = falso;

	for (natural i = 0; i < d->a_tam; i++) {
		if (d->a[i] == d->min) {
			min_visto = verdadero;
		}
	}
//	assert_timeout(min_visto);

	disponiblidad = es_alcanzable(d->a, d->a_tam, d->k, d->r, objetivo, d->min);
	caca_log_debug("las disp %llu con obj %llu", disponiblidad, objetivo);

	assert_timeout(objetivo >= d->min);
//	assert_timeout( d->k ||((objetivo==d->min && !disponiblidad)||(objetivo>d->min && disponiblidad==CACA_INVALIDA)));

	if (disponiblidad) {
		if (disponiblidad == CACA_INVALIDA) {
			r = -1;
		} else {
			r = 1;
		}
	} else {
		r = 0;
	}

	return r;
}

#define MAX_MIERDA ((largote)1E18)
#define MAX_CACA ((natural)5E5)
#define MAX_ASS (MAX_CACA + (1E9+MAX_MIERDA/MAX_CACA))
natural a[MAX_CACA] = { 0 };
largote acumulados[MAX_CACA] = { 0 };

static inline largote acumular_fuerza_bruta(natural *a, natural i, natural j) {
	largote r = 0;
	caca_log_debug("fuck %u %u", i, j);
	for (natural k = i; k <= j; k++) {
		r += a[k];
	}
	caca_log_debug("r %llu", r);
	return r;
}

static inline largote* acumular(natural *a, natural a_tam, largote *acumulados,
		natural acumulados_tam, natural r, largote *res) {
	assert_timeout(acumulados_tam >= a_tam);
	largote acum = 0;
	largote min = ULONG_LONG_MAX;
//	largote min = MAX_ASS;
	largote max = 0;
	bool min_puesto = falso;

	for (natural i = 0; i < r; i++) {
		acum += a[i];
	}

	for (natural i = 0; i < a_tam; i++) {
		if (i > r) {
			acum -= a[i - r - 1];
		}
//		caca_log_debug("i %u + r %u = %u", i, r, i+r);
		if (i + r < a_tam) {
			acum += a[i + r];
		}
		acumulados[i] = acum;

		/*
		 largote caca = acumular_fuerza_bruta(a,
		 caca_comun_max(0, (int )((int )i - (int )r)),
		 caca_comun_min(a_tam - 1, i + r));
		 assert_timeout(acum == caca);
		 */

		if (acum < min || !min_puesto) {
			min = acum;
			min_puesto = verdadero;
		}
		if (acum > max) {
			max = acum;
		}
	}
	assert_timeout(min!=ULONG_LONG_MAX);
	bool min_visto = falso;
	for (natural i = 0; i < a_tam; i++) {
		if (acumulados[i] == min) {
			min_visto = verdadero;
		}
	}
//	assert_timeout(min_visto);
	res[0] = min;
	res[1] = max;
	return res;
}

static inline void defensa_caca_main() {
	natural n = 0;
	natural r = 0;
	largote k = 0;
	datos_busqueda_binaria *d = caca_comun_calloc_local(datos_busqueda_binaria);
	largote extremos[2] = { 0 };

	scanf("%u %u %llu\n", &n, &r, &k);

	for (natural i = 0; i < n; i++) {
		scanf("%u", a + i);
	}

	caca_log_debug("ptuos %s",
			caca_comun_arreglo_a_cadena_natural(a, n, CACA_COMUN_BUF_STATICO));

	acumular(a, n, acumulados, MAX_CACA, r, extremos);
	caca_log_debug("acumulados %s el min %llu",
			caca_comun_arreglo_a_cadena_entero_largo_sin_signo(acumulados, n, CACA_COMUN_BUF_STATICO),
			extremos[0]);

	bool min_visto = falso;
	for (natural i = 0; i < n; i++) {
		if (acumulados[i] == extremos[0]) {
			min_visto = verdadero;
		}
	}
//	assert_timeout(min_visto);

	d->a = acumulados;
	d->a_tam = n;
	d->k = k;
	d->r = r;
	d->min = extremos[0];

	caca_log_debug("a %s, k %llu, r %u",
			caca_comun_arreglo_a_cadena_natural(a, n, CACA_COMUN_BUF_STATICO),
			k, r);

	largote re = busqueda_binaria(d, prueba_objetivo, extremos[0],
			extremos[1] + k, k);
	caca_log_debug("caca %llu", re);
//	assert_timeout(re >= extremos[0]);
//	assert_timeout(k || re == extremos[0]);
	printf("%llu\n", re);
}

int main(void) {
	caca_log_debug("pero q mierda");
	defensa_caca_main();
	return EXIT_SUCCESS;
}
