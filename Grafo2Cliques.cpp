#include "BibGrafos.h"
namespace BibGrafos
{
	Grafo2Cliques::Grafo2Cliques(Grafo G) : GrafoCordal(G)
	{

	}

	Grafo * Grafo2Cliques::ConstruirAleatorio(int n)
	{
		Grafo2Cliques* G = new Grafo2Cliques(n);
		return G;
	}

	Grafo2Cliques::Grafo2Cliques(int n) : GrafoCordal(n)
	{
		random_device rd;
		mt19937 rng(rd());
		uniform_int_distribution<int> rndC1(2, n - 4);
		int C1 = rndC1(rng);

		uniform_int_distribution<int> rndC2(2, n - (C1 + 2));
		int C2 = rndC2(rng);

		int Inter = n - (C1 + C2);

		new (this) Grafo2Cliques(C1, C2, Inter);
	}


	Grafo2Cliques::Grafo2Cliques(int C1, int C2, int Inter) : GrafoCordal(C1 + C2)
	{
		int n = C1 + C2 + Inter;

		//Criar clique 1, exceto Inter
		vector<Vertice> clique1;
		for (int i = 1; i <= C1; i++)
		{
			Vertice v = AdicionarVertice(i);
			for (Vertice u : clique1)
			{
				AdicionarAdjacencia(v, u);
			}
			clique1.push_back(v);
		}

		//Criar clique 2, exceto Inter
		vector<Vertice> clique2;
		for (int i = n; i > n - C2; i--)
		{
			Vertice v = AdicionarVertice(i);
			for (Vertice u : clique2)
			{
				AdicionarAdjacencia(v, u);
			}
			clique2.push_back(v);
		}

		//Criar Inter
		vector<Vertice> clique_inter;
		for (int i = 1; i <= Inter; i++)
		{
			int id = C1 + i;
			Vertice v = AdicionarVertice(id);
			for (Vertice u : clique1)
			{
				AdicionarAdjacencia(v, u);
			}
			for (Vertice u : clique2)
			{
				AdicionarAdjacencia(v, u);
			}
			for (Vertice u : clique_inter)
			{
				AdicionarAdjacencia(v, u);
			}
			clique_inter.push_back(v);
		}
	}

	Grafo2Cliques::Grafo2Cliques(int n, int** matAdj) : GrafoCordal(n, matAdj)
	{
		InicializarVertices(n);
		AdjList.LerMatrizAdjacencia(n, matAdj, Vertices);
	}

	Grafo2Cliques::Grafo2Cliques()
	{
	}

	Grafo Grafo2Cliques::ConstruirParticionado(int C1, int C2, int Inter)
	{
		Grafo2Cliques G(C1, C2, Inter);
		return G;
	}

	vector<Grafo2Cliques> Grafo2Cliques::ConstruirTodos(int n)
	{
		vector<Grafo2Cliques> resultado;
		for (int C1 = 1; C1 <= n - 2; C1++)
		{
			for (int C2 = 1; C2 <= n - (C1 + 1); C2++)
			{
				if (C1 <= C2)
				{
					int inter = n - (C1 + C2);
					resultado.push_back(Grafo2Cliques(C1, C2, inter));
				}
			}
		}
		return resultado;


	}

	vector<Grafo2Cliques> Grafo2Cliques::ConstruirTodosRestrito(int n)
	{
		vector<Grafo2Cliques> resultado;
		for (int C1 = 2; C1 <= n - 4; C1++)
		{
			for (int C2 = 2; C2 <= n - (C1 + 2); C2++)
			{
				int inter = n - (C1 + C2);
				resultado.push_back(Grafo2Cliques(C1, C2, inter));
			}
		}
		return resultado;
	}
}
