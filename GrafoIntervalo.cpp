#include "BibGrafos.h"
namespace BibGrafos
{
	GrafoIntervalo::GrafoIntervalo()
	{
	}

	GrafoIntervalo::~GrafoIntervalo()
	{
	}

	GrafoIntervalo::GrafoIntervalo(int n) : GrafoCordal(n)
	{
		ConstruirIntervalos(n);
	}

	GrafoIntervalo::GrafoIntervalo(int n, int** matAdj) : GrafoCordal(n, matAdj)
	{
	}

	Grafo * GrafoIntervalo::ConstruirAleatorio(int n)
	{
		GrafoIntervalo* G = new GrafoIntervalo(n);
		return G;
	}

	void GrafoIntervalo::ConstruirIntervalos(int n)
	{
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);

		bool cont = false;
		do
		{
			cont = false;
			vector<int> posicoes;
			for (int i = 0; i < n * 2; i++)
			{
				posicoes.push_back(i);
			}

			for (int i = 1; i <= n; i++)
			{
				std::uniform_int_distribution<int> posRnd1(0, posicoes.size() - 1);
				int pos1 = posRnd1(generator);
				int ini = posicoes[pos1];
				posicoes.erase(posicoes.begin() + pos1);

				std::uniform_int_distribution<int> posRnd2(0, posicoes.size() - 1);
				int pos2 = posRnd2(generator);
				int fim = posicoes[pos2];
				posicoes.erase(posicoes.begin() + pos2);

				if (ini > fim) { int temp = ini; ini = fim; fim = temp; }

				AdicionarIntervalo(ini, fim);
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

	string GrafoIntervalo::ObterDetalhes()
	{
		stringstream sb;
		sb << "Grafo de Intervalo:\n Intervalos: \n";
		for (Vertice v : Vertices)
		{
			sb << v.Id() << "\t" << intervalos[v].Inicio() << "\t" << intervalos[v].Fim() << "\n";
		}
		sb << "Ordenação Linear das Cliques Maximais:\n";
		vector<set<Vertice>> cliques = ObterOrdenacaoCliques();
		int ct = 1;
		for (set<Vertice> c : cliques)
		{
			sb << "C" << ct << "\t";
			for (Vertice v : c)
			{
				sb << v.Id() << " ";
			}
			sb << "\n";
			ct++;
		}
		return sb.str();
	}

	void GrafoIntervalo::AdicionarIntervalo(int id, int inicio, int fim)
	{
		Intervalo i = Intervalo(id, inicio, fim);
		AdicionarIntervalo(i);
	}

	void GrafoIntervalo::AdicionarIntervalo(int inicio, int fim)
	{
		Intervalo i = Intervalo(ObterNovoId(), inicio, fim);
		AdicionarIntervalo(i);
	}

	void GrafoIntervalo::AdicionarIntervalo(Intervalo i)
	{
		bool coincide = false;
		for (Vertice v : Vertices)
		{
			Intervalo vi = intervalos[v];
			if (vi.Inicio() == i.Inicio() || vi.Fim() == i.Fim() || vi.Inicio() == i.Fim() || vi.Fim() == i.Inicio())
			{
				coincide = true;
				//throw exception("Intervalos coincidentes");
			}
		}

		if (coincide == false)
		{
			Vertice v = AdicionarVertice(i.Id());
			intervalos.insert(pair<Vertice, Intervalo>(v, i));

			for (Vertice w : Vertices)
			{
				if (v != w)
				{
					Intervalo iw = intervalos[w];
					for (int x = iw.Inicio(); x <= iw.Fim(); x++)
					{
						if (x >= i.Inicio() && x <= i.Fim())
						{
							AdicionarAdjacencia(v, w);
							break;
						}
					}
				}
			}
		}
	}

	vector<set<Vertice>> GrafoIntervalo::ObterOrdenacaoCliques()
	{
		int minInt = -1;
		int maxInt = -1;

		for (Vertice v : Vertices)
		{
			Intervalo iv = intervalos[v];
			if (minInt == -1 || iv.Inicio() < minInt) minInt = iv.Inicio();
			if (maxInt == -1 || iv.Fim() > maxInt) maxInt = iv.Fim();
		}

		vector<set<Vertice>> cliques;

		set<Vertice> cAnt;
		for (int i = minInt; i <= maxInt; i++)
		{
			set<Vertice> cNova;
			for (Vertice v : Vertices)
			{
				Intervalo iv = intervalos[v];
				if (i >= iv.Inicio() && i <= iv.Fim())
				{
					cNova.insert(v);
				}
			}
			//Se todos os vértices de cAnt estão em cNova então cNova > cAnt
			if (includes(cNova.begin(), cNova.end(), cAnt.begin(), cAnt.end()) == false)
			{
				bool inclusa = false;
				for (set<Vertice> c : cliques)
				{
					if (includes(c.begin(), c.end(), cAnt.begin(), cAnt.end()))
					{
						inclusa = true;
						break;
					}
				}
				if (inclusa == false)
					cliques.push_back(cAnt);
			}
			cAnt = cNova;
		}


		return cliques;
	}


	vector<Vertice> GrafoIntervalo::ObterSequenciaGulosa()
	{
		vector<set<Vertice>> cliques = ObterOrdenacaoCliques();

		vector<Vertice> seq;
		map<Vertice, int> H;
		map<int, vector<Vertice>> bucket;
		map<Vertice, vector<int>> aux;

		for (Vertice v : Vertices)
		{
			H.insert(pair<Vertice, int>(v, -1));
			aux.insert(pair<Vertice, vector<int>>(v, vector<int>()));
		}


		for (int i = 0; i < cliques.size(); i++)
		{
			bucket.insert(pair<int, vector<Vertice>>(i, vector<Vertice>()));
		}

		for (int i = 0; i < cliques.size(); i++)
		{
			for (Vertice v : cliques[i])
			{
				if (H[v] == -1)
				{
					bucket[i].push_back(v);
					aux[v].push_back(i);
				}

				H[v] = i;

				for (int b : aux[v])
				{
					bucket[b].erase(remove(bucket[b].begin(), bucket[b].end(), v), bucket[b].end());
					bucket[b].push_back(v);
				}

			}
		}

		for (int i = 0; i < cliques.size(); i++)
		{
			for (Vertice v : bucket[i])
			{
				seq.push_back(v);
			}
		}


		return seq;
	}
}
