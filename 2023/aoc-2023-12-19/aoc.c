#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PARAM_NONE -1
#define PARAM_X    0
#define PARAM_M    1
#define PARAM_A    2
#define PARAM_S    3

#define RESULT_ACCEPT 1
#define RESULT_REJECT 2

#define MAX_WORKFLOW_NAME  32

typedef struct s_test {
	int param;
	int greater;
	long long int value;
	char then[MAX_WORKFLOW_NAME];
} test_t;




#define MAX_TESTS_BY_WORKFLOW  6

typedef struct s_workflow {
	char    name[MAX_WORKFLOW_NAME];
	test_t  tests[MAX_TESTS_BY_WORKFLOW];
	int     nb_tests;
} workflow_t;


workflow_t *Workflows = NULL;
int Nb_workflows = 0;



void add_workflow(const char *line)
{
	int i = 0;
	int idx;

	Workflows = realloc(Workflows, (Nb_workflows + 1) * sizeof(workflow_t));
	workflow_t *wf = &(Workflows[Nb_workflows]);
	wf->nb_tests = 0;

	for (idx = 0; line[idx] != '{'; idx++)
		wf->name[idx] = line[idx];
	wf->name[idx] = '\0';

	idx++;

	for (;;) {
		if ((line[idx + 1] != '<') && (line[idx + 1] != '>')) {
			// Name of a workflow
			wf->tests[wf->nb_tests].param = PARAM_NONE;
			i = 0;
			while (line[idx] != '}') {
				wf->tests[wf->nb_tests].then[i] = line[idx];
				idx++;
				i++;
			}
			wf->tests[wf->nb_tests].then[i] = '\0';
			wf->nb_tests++;
			break;
		}

		if (line[idx] == 'x')
			wf->tests[wf->nb_tests].param = PARAM_X;
		else if (line[idx] == 'm')
			wf->tests[wf->nb_tests].param = PARAM_M;
		else if (line[idx] == 'a')
			wf->tests[wf->nb_tests].param = PARAM_A;
		else if (line[idx] == 's')
			wf->tests[wf->nb_tests].param = PARAM_S;
		else {
			fprintf(stderr, "Unexpected parameter %c in line %s\n", line[idx], line);
			exit(EXIT_FAILURE);
		}
		idx++;
		if (line[idx] == '>')
			wf->tests[wf->nb_tests].greater = 1;
		else if (line[idx] == '<')
			wf->tests[wf->nb_tests].greater = 0;
		else {
			fprintf(stderr, "Unexpected operator %c in line %s\n", line[idx], line);
			exit(EXIT_FAILURE);
		}
		idx++;

		int n;
		if (sscanf(&(line[idx]), "%lld%n", &wf->tests[wf->nb_tests].value, &n) != 1) {
			fprintf(stderr, "Unable to read value in line %s\n", line);
			exit(EXIT_FAILURE);
		}
		idx += n;

		if (line[idx] != ':') {
			fprintf(stderr, "Missing ':' in line %s\n", line);
			exit(EXIT_FAILURE);
		}
		idx++;

		i = 0;
		while ((line[idx] != '}') && (line[idx] != ',')) {
			wf->tests[wf->nb_tests].then[i] = line[idx];
			idx++;
			i++;
		}
		wf->tests[wf->nb_tests].then[i] = '\0';

		wf->nb_tests++;

		if (line[idx] == '}')
			break;
		idx++;
	}

	Nb_workflows++;
}



typedef struct {
	long long int x;
	long long int m;
	long long int a;
	long long int s;
} part_t;

part_t *Parts = NULL;
int Nb_parts = 0;



void add_part(const char *line)
{
	long long int x, m, a, s;

	if (sscanf(line, "{x=%lld,m=%lld,a=%lld,s=%lld}", &x, &m, &a, &s) != 4)
		return;

	Parts = realloc(Parts, (Nb_parts + 1)*sizeof(part_t));
	Parts[Nb_parts].x = x;
	Parts[Nb_parts].m = m;
	Parts[Nb_parts].a = a;
	Parts[Nb_parts].s = s;
	Nb_parts++;
}



void load_input(void)
{
	char line[1024];

	while(fgets(line, 1024, stdin) != NULL) {

		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;

		add_workflow(line);
	}

	while(fgets(line, 1024, stdin) != NULL) {

		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;

		add_part(line);
	}
	fprintf(stderr, "   Loaded: %d workflows and %d parts\n", Nb_workflows, Nb_parts);
}


int run_rule(const char *name, long long int x, long long int m, long long int a, long long int s);


int run_test(test_t *ts, int *result, long long int x, long long int m, long long int a, long long int s)
{
	long long int v;

	if (ts->param == PARAM_NONE) {
		if (strcmp(ts->then, "A") == 0) {
			*result = RESULT_ACCEPT;
		} else if (strcmp(ts->then, "R") == 0) {
			*result = RESULT_REJECT;
		} else {
			if (run_rule(ts->then, x,  m, a, s)) {
				*result = RESULT_ACCEPT;
			} else {
				*result = RESULT_REJECT;
			}
		}
		return 1;
	}

	if (ts->param == PARAM_X)
		v = x;
	else if (ts->param == PARAM_M)
		v = m;
	else if (ts->param == PARAM_A)
		v = a;
	else if (ts->param == PARAM_S)
		v = s;
	else {
		fprintf(stderr, "Unknown parameter %d\n", ts->param);
		exit(EXIT_FAILURE);
	}

	if (((ts->greater) && (v <= ts->value))
	 || ((!ts->greater) && (v >= ts->value)))
		return 0;

	if (strcmp(ts->then, "A") == 0) {
		*result = RESULT_ACCEPT;
	} else if (strcmp(ts->then, "R") == 0) {
		*result = RESULT_REJECT;
	} else {
		if (run_rule(ts->then, x, m, a, s)) {
			*result = RESULT_ACCEPT;
		} else {
			*result = RESULT_REJECT;
		}
	}
	return 1;
}



int run_rule(const char *name, long long int x, long long int m, long long int a, long long int s)
{
	int t;
	int r;
	workflow_t *wf = NULL;

	//fprintf(stderr, "(%s)", name);

	for (int i = 0; i < Nb_workflows; i++) {
		if (strcmp(Workflows[i].name, name) == 0) {
			wf = &(Workflows[i]);
			break;
		}
	}
	if (wf == NULL) {
		fprintf(stderr, "Unable to find workflow '%s'\n", name);
		exit(EXIT_FAILURE);
	}

	for (t = 0; t < wf->nb_tests; t++) {
		if (run_test(&(wf->tests[t]), &r, x, m, a, s))
			break;
	}
	return (r == RESULT_ACCEPT);
}




int test_part(long long int x, long long int m, long long int a, long long int s)
{
	return run_rule("in", x, m, a, s);
}


long long int Accepted_parts_sum = 0;


void test_parts(void)
{
	for (int p = 0; p < Nb_parts; p++) {
		//fprintf(stderr, "  > testing part %d...", p);
		if (test_part(Parts[p].x, Parts[p].m, Parts[p].a, Parts[p].s)) {
			//fprintf(stderr, "accepted\n");
			Accepted_parts_sum += Parts[p].x + Parts[p].m + Parts[p].a + Parts[p].s;
		} else {
			//fprintf(stderr, "rejected\n");
		}
	}
}



void part_1(void)
{
	load_input();
	test_parts();
	printf(" Accepted parts sum = %lld\n", Accepted_parts_sum);
}


typedef struct {
	long long int a;
	long long int b;
} range_t;

typedef struct {
	range_t param[4];
} range_4d_t;


long long int Possibilities = 0;

range_4d_t *alloc_range(int a0, int b0, int a1, int b1, int a2, int b2, int a3, int b3)
{
	range_4d_t *rg = malloc(sizeof(range_4d_t));
	rg->param[0].a = a0;
	rg->param[0].b = b0;
	rg->param[1].a = a1;
	rg->param[1].b = b1;
	rg->param[2].a = a2;
	rg->param[2].b = b2;
	rg->param[3].a = a3;
	rg->param[3].b = b3;
	return rg;
}


void run_rule_on_range(const char *name, range_4d_t *rg)
{
	workflow_t *wf = NULL;

	if (strcmp(name, "A") == 0) {
		Possibilities +=  (rg->param[0].b - rg->param[0].a + 1)
				* (rg->param[1].b - rg->param[1].a + 1)
				* (rg->param[2].b - rg->param[2].a + 1)
				* (rg->param[3].b - rg->param[3].a + 1);
		free(rg);
		return;
	}
	if (strcmp(name, "R") == 0) {
		free(rg);
		return;
	}

	for (int i = 0; i < Nb_workflows; i++) {
		if (strcmp(Workflows[i].name, name) == 0) {
			wf = &(Workflows[i]);
			break;
		}
	}
	if (wf == NULL) {
		fprintf(stderr, "Unable to find workflow '%s'\n", name);
		exit(EXIT_FAILURE);
	}
	for (int t = 0; t < wf->nb_tests; t++) {
		test_t *ts = &(wf->tests[t]);

		if (ts->param == PARAM_NONE) {
			run_rule_on_range(ts->then, rg);
			return;
		}

		if (((ts->greater) && (rg->param[ts->param].a > ts->value))
		 || ((!ts->greater) && (rg->param[ts->param].b < ts->value))) {
			run_rule_on_range(ts->then, rg);
			return;
		}

		if (((ts->greater) && (rg->param[ts->param].b <= ts->value))
		 || ((!ts->greater) && (rg->param[ts->param].a >= ts->value))) {
			continue;
		}

		range_4d_t *rg1 = alloc_range(rg->param[0].a, rg->param[0].b,rg->param[1].a, rg->param[1].b, rg->param[2].a, rg->param[2].b, rg->param[3].a, rg->param[3].b);

		if (ts->greater) {
			rg->param[ts->param].b = ts->value;
			rg1->param[ts->param].a = ts->value + 1;
		} else {
			rg->param[ts->param].a = ts->value;
			rg1->param[ts->param].b = ts->value - 1;
		}
		run_rule_on_range(ts->then, rg1);
	}
}


void part_2(void)
{
	load_input();
	run_rule_on_range("in", alloc_range(1, 4000, 1, 4000, 1, 4000, 1, 4000));
	printf("  Possibilities: %lld\n", Possibilities);
}



int main(int argc, char *argv[])
{
	int part;

	if ((argc < 2) || (sscanf(argv[1], "%d", &part) != 1)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	switch(part) {
		case 1:
			part_1();
			break;
		case 2:
			part_2();
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}
