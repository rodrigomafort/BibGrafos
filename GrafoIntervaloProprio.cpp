#include "BibGrafos.h"
namespace BibGrafos
{
	GrafoIntervaloProprio::GrafoIntervaloProprio() : GrafoIntervalo()
	{
	}

	GrafoIntervaloProprio::GrafoIntervaloProprio(int n) : GrafoIntervalo()
	{
		ConstruirIntervalos(n);
	}

	GrafoIntervaloProprio::GrafoIntervaloProprio(int n, int** matAdj) : GrafoIntervalo(n, matAdj)
	{
	}

	Grafo * GrafoIntervaloProprio::ConstruirAleatorio(int n)
	{
		GrafoIntervaloProprio* G = new GrafoIntervaloProprio(n);
		return G;
	}

	GrafoIntervaloProprio::~GrafoIntervaloProprio()
	{
	}

	void GrafoIntervaloProprio::ConstruirIntervalos(int n)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);

		bool cont = false;
		do
		{
			cont = false;
			int maxFim = n;
			set<int> usados;
			for (int i = 1; i <= n; i++)
			{
				int ini;
				int fim;
				do
				{
					std::uniform_int_distribution<int> posRnd1(1, maxFim);
					ini = posRnd1(generator);
					fim = ini + n;
				} while (binary_search(usados.begin(), usados.end(), ini) || binary_search(usados.begin(), usados.end(), fim));

				if (fim > maxFim) maxFim = fim;

				AdicionarIntervalo(ini, fim);
				usados.insert(ini);
				usados.insert(fim);
			}

			if (TestarConexidade() == false)
			{
				while (Vertices.size() > 0)
				{
					Vertices.erase(Vertices.begin());
				}
				cont = true;
			}

		} while (cont == true);
	}
}