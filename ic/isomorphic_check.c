#include "nauty.h"

int main()
{
	FILE *in = fopen("data.in", "r");
	FILE *out = fopen("data.out", "w");

	DYNALLSTAT(int, lab1, lab1_sz);
	DYNALLSTAT(int, lab2, lab2_sz);
	DYNALLSTAT(int, ptn, ptn_sz);
	DYNALLSTAT(int, orbits, orbits_sz);
	DYNALLSTAT(int, map, map_sz);
	DYNALLSTAT(graph, g1, g1_sz);
	DYNALLSTAT(graph, g2, g2_sz);
	DYNALLSTAT(graph, cg1, cg1_sz);
	DYNALLSTAT(graph, cg2, cg2_sz);

	static DEFAULTOPTIONS_GRAPH(options);
	statsblk stats;
	int n, e, m, i;
	size_t k;

	/* Select option for canonical labelling */
	options.getcanon = 1;

	fscanf(in, "%d %d", &n, &e);

	m = SETWORDSNEEDED(n);
	nauty_check(WORDSIZE, m, n, NAUTYVERSIONID);

	DYNALLOC1(int, lab1, lab1_sz, n, "malloc");
	DYNALLOC1(int, lab2, lab2_sz, n, "malloc");
	DYNALLOC1(int, ptn, ptn_sz, n, "malloc");
	DYNALLOC1(int, orbits, orbits_sz, n, "malloc");
	DYNALLOC1(int, map, map_sz, n, "malloc");

	/* Now make the first graph */
	DYNALLOC2(graph, g1, g1_sz, n, m, "malloc");
	EMPTYGRAPH(g1, m, n);

	for (i = 0; i < e; i++)
	{
		int u, v;

		fscanf(in, "%d %d", &u, &v);

		ADDONEEDGE(g1, u, v, m);
	}

	/* Now make the second graph */
	DYNALLOC2(graph, g2, g2_sz, n, m, "malloc");
	EMPTYGRAPH(g2, m, n);

	for (int i = 0; i < e; i++)
	{
		int u, v;

		fscanf(in, "%d %d", &u, &v);

		ADDONEEDGE(g2, u, v, m);
	}

	/* Create canonical graphs */
	DYNALLOC2(graph, cg1, cg1_sz, n, m, "malloc");
	DYNALLOC2(graph, cg2, cg2_sz, n, m, "malloc");

	densenauty(g1, lab1, ptn, orbits, &options, &stats, m, n, cg1);
	densenauty(g2, lab2, ptn, orbits, &options, &stats, m, n, cg2);

	/* Compare canonically labelled graphs */
	for (k = 0; k < m * (size_t)n; k++)
	{
		if (cg1[k] != cg2[k])
		{
			break;
		}
	}
	if (k == m * (size_t)n)
	{
		fprintf(out, "%s\n", "Isomorphic");

		for (int i = 0; i < n; i++)
			map[lab1[i]] = lab2[i];

		for (int i = 0; i < n; i++)
			fprintf(out, "%d %d\n", i, map[i]);
	}
	else
	{
		fprintf(out, "%s\n", "Not isomorphic");
	}

	fclose(in);
	fclose(out);
}
