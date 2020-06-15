#include "BibGrafos.h"
namespace BibGrafos
{
	GrafoDominoIntervaloProprio::GrafoDominoIntervaloProprio()
	{
	}

	GrafoDominoIntervaloProprio::GrafoDominoIntervaloProprio(int n, set<set<int>> cliques): GrafoCordal(n,cliques)
	{

	}

	GrafoDominoIntervaloProprio::GrafoDominoIntervaloProprio(int n) : GrafoCordal(n)
	{
		vector<set<Vertice>> cliques(n,set<Vertice>());
		set<Vertice> I;
		int k = 0;

		random_device rndDev;
		mt19937_64 rndEngine(rndDev());
		stringstream debug;

		for (int i = 1; i <= n; i++)
		{
			Vertice v = AdicionarVertice(i);
			uniform_int_distribution<int> rnd(0, 3);

			int x = rnd(rndEngine);

			if (x == 0 || i == 1)
			{
				//Adicionar vértice na clique k
				cliques[k].insert(v);
			}
			else
			{
				if (x == 1 || I.size() == 0)
				{
					//Adicionar vértice na interseção I
					cliques[k].insert(v);
					I.insert(v);
				}
				else
				{
					if (x == 2)
					{
						//Adicionar vértice em uma nova clique k + 1
						//O vértice v está contido SOMENTE na clique k
						k = k + 1;
						cliques[k].insert(I.begin(), I.end());
						cliques[k].insert(v);
						I.clear();
					}
					else
					{
						//Adicionar vértice em uma nova clique k + 1
						//O vértice v está contido na clique k E na clique k + 1 (se houver k+1)
						k = k + 1;
						cliques[k].insert(I.begin(), I.end());
						cliques[k].insert(v);
						I.clear();
						I.insert(v);
					}
				}
			}
			for (Vertice u : cliques[k])
			{
				if (u != v)
				{
					AdicionarAdjacencia(v,u);
				}
			}
			debug << x << " ";
		}
		info = debug.str();
	}

	GrafoDominoIntervaloProprio::GrafoDominoIntervaloProprio(int n, int* vet) : GrafoCordal(n)
	{
		vector<set<Vertice>> cliques(n,set<Vertice>());
		set<Vertice> I;
		int k = 0;

		stringstream debug;
		for (int i = 1; i <= n; i++)
		{
			Vertice v = AdicionarVertice(i);
			uniform_int_distribution<int> rnd(0, 3);

			int x = vet[i-1];

			if (x == 0 || i == 1)
			{
				//Adicionar vértice na clique k
				cliques[k].insert(v);
			}
			else
			{
				if (x == 1 || I.size() == 0)
				{
					//Adicionar vértice na interseção I
					cliques[k].insert(v);
					I.insert(v);
				}
				else
				{
					if (x == 2 || i == n)
					{
						//Adicionar vértice em uma nova clique k + 1
						//O vértice v está contido SOMENTE na clique k
						k = k + 1;
						cliques[k].insert(I.begin(), I.end());
						cliques[k].insert(v);
						I.clear();
					}
					else
					{
						//Adicionar vértice em uma nova clique k + 1
						//O vértice v está contido na clique k E na clique k + 1 (se houver k+1)
						k = k + 1;
						cliques[k].insert(I.begin(), I.end());
						cliques[k].insert(v);
						I.clear();
						I.insert(v);
					}
				}
			}

			for (Vertice u : cliques[k])
			{
				if (u != v)
				{
					AdicionarAdjacencia(v,u);
				}
			}
			debug << x << " ";
		}
		info = debug.str();
	}


	GrafoDominoIntervaloProprio::GrafoDominoIntervaloProprio(int n, int ** matAdj) : GrafoCordal(n, matAdj)
	{
	}

	GrafoDominoIntervaloProprio::~GrafoDominoIntervaloProprio()
	{
	}

	Grafo* GrafoDominoIntervaloProprio::ConstruirAleatorio(int n)
	{
		GrafoDominoIntervaloProprio* G = new GrafoDominoIntervaloProprio(n);
		return G;
	}

	Grafo* GrafoDominoIntervaloProprio::ConstruirAleatorio(int n, int* vet)
	{
		GrafoDominoIntervaloProprio* G = new GrafoDominoIntervaloProprio(n,vet);
		return G;
	}
}
