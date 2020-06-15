#include "BibGrafos.h"
namespace BibGrafos
{
	GrafoCordal::GrafoCordal() : Grafo()
	{

	}

	GrafoCordal::GrafoCordal(Grafo G) : Grafo(G)
	{
	}

	GrafoCordal::GrafoCordal(int n) : Grafo(n)
	{

	}

	GrafoCordal::GrafoCordal(int n, int** matAdj) : Grafo(n, matAdj)
	{
		InicializarVertices(n);
		AdjList.LerMatrizAdjacencia(n, matAdj, Vertices);
	}

	Grafo * GrafoCordal::ConstruirAleatorio(int n)
	{
		GrafoCordal* G = new GrafoCordal(n);
		return G;
	}

	GrafoCordal::GrafoCordal(int n, set<set<int>> cliques) : Grafo(n,cliques)
	{

	}


	vector<Vertice> GrafoCordal::ObterEEP()
	{
		struct decrescente {
			bool operator ()(pair<Vertice, vector<int>> const& a, pair<Vertice, vector<int>> const& b) const {
				return lexicographical_compare(a.second.begin(), a.second.end(), b.second.begin(), b.second.end());
				//return a.second > b.second;
			}
		};

		//Buscar e remover vértices simpliciais
		//Um vértice é simplicial se, e somente se, ele pertencer a apenas uma clique
		vector<Vertice> V2(Vertices.begin(), Vertices.end());
		int i = V2.size() + 1;


		list<Vertice> EEP;

		map<Vertice, vector<int>> label;
		map<Vertice, int> index;

		for (Vertice v : V2)
		{
			label.insert(pair<Vertice, vector<int>>(v, vector<int>()));
			index.insert(pair<Vertice, int>(v, 0));
		}

		while (V2.size() > 0)
		{
			vector<pair<Vertice, vector<int>>> filtrado;
			for (pair<Vertice, vector<int>> par : label)
			{
				if (find(V2.begin(), V2.end(), par.first) != V2.end())
				{
					filtrado.push_back(par);
				}
			}

			sort(filtrado.begin(), filtrado.end(), decrescente());
			Vertice v = filtrado.back().first;
			//Vertice v = (*filtrado.begin()).first;

			//Vertice v = label.Where(x => V2.Contains(x.Key)).OrderByDescending(x => x.Value).First().Key;

			EEP.push_front(v);

			i = i - 1;
			index[v] = i;

			for (Vertice u : Adjacentes(v))
			{
				if (index[u] == 0)
				{
					//label[u] = label[u] + (char)(97 + i);
					label[u].push_back(i);
				}
			}

			V2.erase(remove(V2.begin(), V2.end(), v), V2.end());
		}

		//    vector<Vertice> out { make_move_iterator(begin(EEP)), make_move_iterator(end(EEP)) };
		vector<Vertice> out(EEP.begin(), EEP.end());
		return out;
	}

	bool GrafoCordal::TestarEEP(vector<Vertice> eep)
	{
		struct comparador {
			vector<Vertice> eep;
			comparador(vector<Vertice> arg)
			{
				eep = arg;
			}
			bool operator ()(Vertice const& a, Vertice const& b) const {
				ptrdiff_t posA = find(eep.begin(), eep.end(), a) - eep.begin();
				ptrdiff_t posB = find(eep.begin(), eep.end(), b) - eep.begin();
				return posA < posB;
			}
		};

		map<Vertice, set<Vertice>> A;
		for (Vertice v : Vertices)
		{
			set<Vertice> lista;
			A.insert(pair<Vertice, set<Vertice> >(v, lista));
		}


		for (int i = 0; i < N() - 1; i++)
		{
			Vertice v = eep[i];
			set<Vertice> adjV = Adjacentes(v);

			vector<Vertice> adjFiltrado;
			for (Vertice x : adjV)
			{
				ptrdiff_t posX = find(eep.begin(), eep.end(), x) - eep.begin();
				if (i < posX)
				{
					adjFiltrado.push_back(x);
				}
			}
			if (adjFiltrado.size() != 0)
			{
				sort(adjFiltrado.begin(), adjFiltrado.end(), comparador(eep));
				Vertice u = adjFiltrado[0];				
				A[u].insert(adjFiltrado.begin(), adjFiltrado.end());
				A[u].erase(u);
			}

			vector<Vertice> diff;
			set_difference(begin(A[v]), A[v].end(), adjV.begin(), adjV.end(), inserter(diff, diff.begin()));
			if (diff.size() > 0)
			{
				return false;
			}
		}
		return true;
	}

	bool GrafoCordal::TestarCordal()
	{
		vector<Vertice> eep = ObterEEP();
		return TestarEEP(eep);
	}

	ArvoreCliques GrafoCordal::ObterArvoreCliques()
	{
		vector<Vertice> eep = ObterEEP();
		return ObterArvoreCliques(eep);
	}

	ArvoreCliques GrafoCordal::ObterArvoreCliques(vector<Vertice> EEP)
	{
		struct comparador {
			vector<Vertice> eep;
			comparador(vector<Vertice> arg)
			{
				eep = arg;
			}
			bool operator ()(Vertice const& a, Vertice const& b) const {
				ptrdiff_t posA = find(eep.begin(), eep.end(), a) - eep.begin();
				ptrdiff_t posB = find(eep.begin(), eep.end(), b) - eep.begin();
				return posA < posB;
			}
		};



		map<int, set<Vertice>> Q;
		queue<pair<int, int>> estrutura;

		map<Vertice, int> N;

		int q = 1;
		int raiz;

		for (Vertice v : Vertices)
		{
			N.insert(pair<Vertice, int>(v, 0));
		}

		set<Vertice> nq;
		Q.insert(pair<int, set<Vertice>>(q, nq));

		Vertice ult = EEP[EEP.size() - 1];
		Q[q].insert(ult);
		N[ult] = q;
		raiz = q;

		for (int i = EEP.size() - 2; i >= 0; i--)
		{
			Vertice v = EEP[i];
			set<Vertice> adjV = Adjacentes(v);
			vector<Vertice> X;
			for (Vertice u : adjV)
			{
				ptrdiff_t posU = find(EEP.begin(), EEP.end(), u) - EEP.begin();
				if (posU > i)
					X.push_back(u);
			}
			sort(X.begin(), X.end(), comparador(EEP));
			if (X.size() == 0)
			{
				cout << "Erro";
			}
			Vertice w = X[0];
			int k = N[w];
			if (X.size() < Q[k].size())
			{
				//Nova clique maximal
				q = q + 1;

				set<Vertice> nova;
				Q.insert(pair<int, set<Vertice>>(q, nova));

				Q[q].insert(v);
				Q[q].insert(X.begin(), X.end());

				//QT.AdicionarClique(Q[q], Q[k]);
				estrutura.emplace(q, k);

				N[v] = q;

			}
			else
			{
				//Clique Qk � aumentada
				Q[k].insert(v);
				int ct = Q[k].size();

				N[v] = k;
			}
		}

		ArvoreCliques QT;

		NoClique* noRaiz = QT.AdicionarRaiz(Q[raiz]);
		while (estrutura.size() > 0)
		{
			pair<int, int> p = estrutura.front();
			estrutura.pop();

			set<Vertice> noAtual = Q[p.first];
			set<Vertice> ancestral = Q[p.second];

			QT.AdicionarClique(Q[p.first], Q[p.second]);
		}

		return QT;
	}

	set<set<Vertice>> GrafoCordal::ObterCliquesMaximais()
	{
		vector<Vertice> eep = ObterEEP();
		return ObterCliquesMaximais(eep);
	}

	set<set<Vertice>> GrafoCordal::ObterCliquesMaximais(vector<Vertice> EEP)
	{
		set<set<Vertice>> cliques;

		ArvoreCliques ac = ObterArvoreCliques(EEP);

		for (NoClique* c : ac.cliques)
		{
			set<Vertice> nova_c(c->vertices.begin(), c->vertices.end());
			cliques.insert(nova_c);
		}

		return cliques;
	}
}
