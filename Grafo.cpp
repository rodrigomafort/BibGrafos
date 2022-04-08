#include "BibGrafos.h"
#include <vector>
#include <set>
#include <algorithm>
#include <set>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>

namespace BibGrafos
{
	Grafo::Grafo()
	{
		//Construtor em branco
	}

	Grafo::Grafo(int n)
	{
		InicializarVertices(n);
	}

	Grafo::Grafo(int n, int** matAdj)
	{
		InicializarVertices(n);
		AdjList.LerMatrizAdjacencia(n, matAdj, Vertices);
	}

	Grafo::Grafo(int n, int m, int* lista)
	{
		InicializarVertices(n);

		for (int ct = 0; ct < m; ct++)
		{
			int k = ct * 2;
			int v = lista[k];
			int u = lista[k+1];

			Vertice V1 = VerticePorId(v);
			Vertice V2 = VerticePorId(u);

			AdicionarAdjacencia(v, u);
		}
	}

	Grafo::Grafo(Grafo* inst):
			Vertices(inst->Vertices.begin(), inst->Vertices.end()),
			mapaVertices(inst->mapaVertices.begin(), inst->mapaVertices.end()),
			AdjList(inst->AdjList)
	{
	}

	Grafo::Grafo(int n, int m, ParAdj* adjs)
	{
		InicializarVertices(n);

		for (int i = 0; i < m; i++)
		{
			int id1 = adjs[i].v1;
			int id2 = adjs[i].v2;

			Vertice v1 = VerticePorId(id1);
			Vertice v2 = VerticePorId(id2);

			AdicionarAdjacencia(v1,v2);
		}
	}

	Grafo::Grafo(int n, int m, ParAdj* vertices, ParAdj* adjs)
	{
		for (int i = 0; i < n; i++)
		{
			AdicionarVertice(vertices[i].v1,vertices[i].v2);
		}

		for (int i = 0; i < m; i++)
		{
			AdicionarAdjacencia(adjs[i].v1,adjs[i].v2);
		}
	}

	Grafo::Grafo(int n, set<set<int>> cliques)
	{
		InicializarVertices(n);
		for (set<int> c : cliques)
		{
			for (int v : c)
			{
				for (int u : c)
				{
					if (v < u)
						AdicionarAdjacencia(v,u);
				}
			}
		}
	}

	bool Grafo::SalvarArquivoBinario(string nomeArquivo)
	{
		try
		{
			FILE* file = fopen(nomeArquivo.c_str(), "wb");

			ParAdj conf;
			conf.v1 = N();
			conf.v2 = M();

			ParAdj* vertices = (ParAdj*) malloc (N() * sizeof(ParAdj));
			ParAdj* arestas = (ParAdj*) malloc (M() * sizeof(ParAdj));

			ObterListaVertices(vertices);
			ObterListaArestas(arestas);

			fwrite(&conf,sizeof(ParAdj), 1, file);
			fwrite(vertices,sizeof(ParAdj), N(), file);
			fwrite(arestas,sizeof(ParAdj), M(), file);
			fclose(file);
			return true;
		}
		catch(exception e)
		{
			return false;
		}
	}

	Grafo Grafo::ConstruirDeArquivoBinario(string nomeArquivo)
	{
		int n;
		int m;

		FILE *file;
		file = fopen(nomeArquivo.c_str(), "rb");

		ParAdj conf;
		fread(&conf, sizeof(ParAdj), 1, file);

		n = conf.v1;
		m = conf.v2;

		ParAdj *vertices = (ParAdj*) malloc(n * sizeof(ParAdj));
		ParAdj *adjs = (ParAdj*) malloc(m * sizeof(ParAdj));

		for (int i = 0; i < n; i++)
			fread(&vertices[i], sizeof(ParAdj), 1, file);

		for (int i = 0; i < m; ++i)
			fread(&adjs[i], sizeof(ParAdj), 1, file);

		fclose(file);

		Grafo *G = new Grafo(n, m, vertices, adjs);

		free(vertices);
		free(adjs);
		return G;
	}


	int Grafo::N() const
	{
		return Vertices.size();
	}

	int Grafo::M() const
	{
		return AdjList.M();
	}

	Grafo* Grafo::ConstruirAleatorio(int n)
	{
		Grafo* G = new Grafo();
		
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);

		//Parte 1: Construindo Grafo Conexo
		vector<Vertice> anteriores;
		for (int i = 1; i <= n; i++)
		{
			Vertice v = G->AdicionarVertice(i);

			if (anteriores.size() > 0)
			{
				uniform_int_distribution<int> rndSet(0, anteriores.size() - 1);
				int pos = rndSet(generator);
				G->AdicionarAdjacencia(v, anteriores[pos]);
			}
			anteriores.push_back(v);
		}

		//Grafo tem agora n-1 arestas
		//Ponto M�nimo: n-1 arestas -> �rvore
		//Ponto M�ximo: (n� - n) / 2 -> Grafo Completo

		uniform_int_distribution<int> rndSet(n - 1, ((n*n) - n) / 2);
		int M = rndSet(generator);
		int add = M - (n - 1);

		uniform_int_distribution<int> rndCand(1, n);

		while (add > 0)
		{
			Vertice v1 = G->VerticePorId(rndCand(generator));			
			Vertice v2;
			do
			{
				v2 = G->VerticePorId(rndCand(generator));
			} while (v1 == v2);

			add = add - 1;

			G->AdicionarAdjacencia(v1, v2);
		}

		return G;
	}

	Grafo* Grafo::ConstruirAleatorio(int n, double prob)
	{
		
		Grafo* G = NULL;

		do
		{
			if (G != NULL) delete(G);
			G = new Grafo();

			unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
			default_random_engine generator(seed);
			uniform_real_distribution<double> distribution(0.0, 1.0);

			for (int i = 1; i <= n; i++)
			{
				G->AdicionarVertice(i);
			}

			for (int i = 1; i <= n; i++)
			{
				//A restrição j > i força que cada aresta seja considerada uma única vez
				for (int j = i + 1; j <= n; j++)
				{
					//Aresta entre vértices i e j
					double d = distribution(generator);
					if (d <= prob)
						G->AdicionarAdjacencia(i, j);
				}
			}
		} while (G->TestarConexidade() == false);

		return G;
	}


	void Grafo::InicializarVertices(int tam)
	{
		for (int i = 1; i <= tam; i++)
			AdicionarVertice(i);
	}

	int Grafo::ObterNovoId()
	{
		int id;
		if (!Vertices.empty())
		{
			Vertice ultimo = *Vertices.rbegin();
			id = ultimo.Id() + 1;
		}
		else
		{
			id = 1;
		}
		return id;
	}

	Vertice Grafo::AdicionarVertice(int id)
	{
		Vertice v(id);
		Vertices.insert(v);
		AdjList.AdicionarVertice(v);
		mapaVertices.insert(pair<int,Vertice>(v.Id(),v));

		return v;
	}

	Vertice Grafo::AdicionarVertice(Vertice v)
	{
		Vertices.insert(v);
		AdjList.AdicionarVertice(v);
		mapaVertices.insert(pair<int,Vertice>(v.Id(),v));
		return v;
	}

	Vertice Grafo::AdicionarVertice()
	{
		Vertice v(ObterNovoId());
		Vertices.insert(v);
		AdjList.AdicionarVertice(v);
		mapaVertices.insert(pair<int,Vertice>(v.Id(),v));
		return v;
	}

	Vertice Grafo::AdicionarVertice(int id, int rotulo)
	{
		Vertice v(id,rotulo);
		Vertices.insert(v);
		AdjList.AdicionarVertice(v);
		mapaVertices.insert(pair<int,Vertice>(v.Id(),v));
		return v;
	}

	void Grafo::RemoverVertice(Vertice v)
	{
		AdjList.RemoverVertice(v);
		Vertices.erase(v);
	}

	Vertice Grafo::operator[](int const id) const
	{
		try
		{
			return mapaVertices.at(id);
		}
		catch (...)
		{
			cout << "ERRO: Grafo::Operador[]" << id << endl;
			return NULL;
		}
	}

	Vertice Grafo::VerticePorId(int const id) const
	{
		try
		{
			return mapaVertices.at(id);
		}
		catch (...)
		{
			cout << "ERRO: Grafo::VerticePorId()" << id << endl;
			return NULL;
		}
	}

	Vertice Grafo::VerticePorIndice(int const indice) const
	{
		return VerticePorId(indice+1);
	}

	bool Grafo::AdicionarAdjacencia(Vertice v, Vertice u)
	{
		return AdjList.AdicionarAdjacencia(v, u);
	}
	
	bool Grafo::AdicionarAdjacencia(int vId, int uId)
	{
		Vertice v = mapaVertices[vId];
		Vertice u = mapaVertices[uId];
		return AdicionarAdjacencia(v, u);
	}

	bool Grafo::RemoverAdjacencia(Vertice v, Vertice u)
	{
		return AdjList.RemoverAdjacencia(v, u);
	}

	int Grafo::Grau(Vertice v) const
	{
		return AdjList[v].size();
	}

	set<Vertice> Grafo::AdjacentesFechado(Vertice v) const
	{
		set<Vertice> adj(AdjList[v]);
		adj.insert(v);
		return adj;
	}

	set<Vertice> Grafo::Adjacentes(Vertice v) const
	{
		set<Vertice> adj = AdjList[v];
		return adj;
	}

	vector<Vertice> Grafo::AdjacentesEmS(Vertice v, vector<Vertice> S) const
	{
		set<Vertice> sa = AdjList[v];
		vector<Vertice> adj(sa.begin(), sa.end());
		sort(adj.begin(), adj.end());

		vector<Vertice> sClone(S.begin(), S.end());
		sort(sClone.begin(), sClone.end());

		vector<Vertice> adjEmS;
		set_intersection(adj.begin(), adj.end(), sClone.begin(), sClone.end(), inserter(adjEmS, adjEmS.end()));

		return adjEmS;
	}

	set<Vertice> Grafo::AdjacentesEmS(Vertice v, set<Vertice> S) const
	{
		set<Vertice> adj = AdjList[v];

		set<Vertice> adjEmS;
		set_intersection(adj.begin(), adj.end(), S.begin(), S.end(), inserter(adjEmS, adjEmS.end()));
		return adjEmS;
	}

	string Grafo::ObterStringMatrizAdjacencias() const
	{
		int n = N();
		string s = "[";
		for (int i = 1; i <= n; i++)
		{
			s.append("[");
			for (int j = 1; j <= n; j++)
			{
				Vertice a = *Vertices.find(i);
				Vertice b = *Vertices.find(j);
				if (AdjList[a].count(b))
				{
					s.append("1");
				}
				else
				{
					s.append("0");
				}
				if (j < n)
				{
					s.append(", ");
				}
			}
			if (i < n)
			{
				s.append("],\n");
			}
			else
			{
				s.append("]]");
			}
		}

		return s;
	}

	string Grafo::ObterStringListaAdjacencias() const
	{
		stringstream sb;
		for (Vertice v : Vertices)
		{
			sb << v.Id() << "\t";

			int ct = 0;
			for (Vertice u : Adjacentes(v))
			{
				ct++;
				sb << u.Id();
				sb << " ";
			}
			sb << "\n";
		}
		return sb.str();
	}

	string Grafo::ObterDetalhes() const
	{
		return "";
	}

	unsigned long Grafo::ObterNumeroCombinacoes() const
	{
		unsigned long c = 1;
		for (Vertice v : Vertices)
		{
			c = c * (Grau(v) + 1);
		}
		return c;
	}

	Grafo* Grafo::ObterGrafoBarra() const
	{
		Grafo* GB = new Grafo();

		map<Vertice, Vertice> mapa;

		for (Vertice v : Vertices)
		{
			Vertice nv = GB->AdicionarVertice(v.Id());
			mapa.insert(pair<Vertice, Vertice>(v, nv));
		}

		for (Vertice v : Vertices)
		{
			for (Vertice u : Vertices)
			{
				if (v.Id() > u.Id())
				{
					if (AdjList.VerificarAdjacencia(v, u) == false)
					{
						GB->AdicionarAdjacencia(mapa[v], mapa[u]);
					}
				}
			}
		}


		return GB;
	}

	Grafo* Grafo::SubGrafoInduzidoPorRemocao(vector<Vertice> cremover) const
	{
		vector<Vertice> cv;
		sort(cremover.begin(), cremover.end());
		set_difference(Vertices.begin(), Vertices.end(), cremover.begin(), cremover.end(), inserter(cv, cv.end()));

		return ObterSubgrafoInduzido(cv);
	}

	Grafo* Grafo::ObterSubgrafoInduzido(vector<Vertice> cv) const
	{
		Grafo* SG = new Grafo();

		sort(cv.begin(), cv.end());

		map<Vertice, Vertice> mapa;
		for (Vertice v : cv)
		{
			Vertice u = SG->AdicionarVertice(v.Id());
			mapa.insert(pair<Vertice, Vertice>(v, u));
		}

		for (Vertice v : cv)
		{
			for (Vertice u : cv)
			{
				if (v.Id() > u.Id())
				{
					if (AdjList.VerificarAdjacencia(v, u))
					{
						SG->AdicionarAdjacencia(mapa[v], mapa[u]);
					}
				}
			}
		}

		return SG;
	}

	string Grafo::ObterDot() const
	{
		stringstream dotFile;


		dotFile << "graph G {\n";
		if (info != "") dotFile << "\tlabel=\"" << info << "\";\n";

		for (Vertice v : Vertices)
		{
			dotFile << "\t" << v.Id() << "[shape=circle];\n";
		}

		for (Vertice v : Vertices)
		{
			for (Vertice u : Adjacentes(v))
			{
				if (v.Id() > u.Id())
				{
					dotFile << "\t" << v.Id() << " -- " << u.Id() << ";\n";
				}
			}
		}

		dotFile << "}\n";

		return dotFile.str();

	}

	bool Grafo::VerificarAdjacencia(Vertice v, Vertice u) const
	{
		return AdjList.VerificarAdjacencia(v, u);
	}

	bool Grafo::VerificarGemeoVerdadeiro(Vertice v, Vertice u) const
	{
		set<Vertice> adjv = AdjList[v];
		set<Vertice> adju = AdjList[u];

		adjv.insert(v);
		adju.insert(u);

		return adjv == adju;
	}

	bool Grafo::TestarConexidade() const
	{
		if (Vertices.size() > 1)
		{
			map<Vertice, bool> tocado;
			for (Vertice v : Vertices)
			{
				tocado.insert(pair<Vertice, bool>(v, false));
			}

			queue<Vertice> fila;

			Vertice v = *Vertices.begin();
			fila.push(v);

			while (fila.empty() == false)
			{
				Vertice v = fila.front();
				fila.pop();
				tocado[v] = true;

				for (Vertice u : Adjacentes(v))
				{
					if (tocado[u] == false)
					{
						fila.push(u);
						tocado[u] = true;
					}
				}
			}

			for (Vertice v : Vertices)
			{
				if (tocado[v] == false) return false;
			}
			return true;
		}
		else
		{
			return true;
		}
	}

	int** Grafo::ObterMatrizAdjacencias() const
	{
		int n = Vertices.size();
		int** mat = new int*[n];

		vector<Vertice> vet(Vertices.begin(), Vertices.end());

		for (int i = 0; i < n; i++)
		{
			mat[i] = new int[n];
			for (int j = 0; j < n; j++)
			{
				mat[i][j] = VerificarAdjacencia(vet[i], vet[j]);
			}
		}

		return mat;
	}

	void Grafo::ObterListaAdjacencias(int* lista) const
	{
		int m = AdjList.M();

		int ct = 0;
		for (Vertice v : Vertices)
		{
			for (Vertice u : AdjList[v])
			{
				if (v.Id() > u.Id())
				{
					int k = (ct * 2);
					lista[k] = v.Id();
					lista[k+1] = u.Id();
					ct++;
				}
			}
		}
	}

	int Grafo::ObterGrauMaximo() const
	{
		return AdjList.ObterGrauMaximo();
	}

	int Grafo::ObterGrauMinimo() const
	{
		return AdjList.ObterGrauMinimo();
	}

	stringstream Grafo::ObterListaParAdjs() const
	{
		stringstream ss;
		for (Vertice v : Vertices)
		{
			for (Vertice u : Adjacentes(v))
			{
				if (v.Id() < u.Id())
				{
					ParAdj p;
					p.v1 = v.Id();
					p.v2 = u.Id();

					char chr[sizeof(p)];
					memcpy(chr, &p, sizeof(p));
					ss.write(chr, sizeof(p));
				}
			}
		}
		return ss;
	}

	stringstream Grafo::ObterListaVertices() const
	{
		stringstream ss;
		for (Vertice v : Vertices)
		{
			ParAdj p;
			p.v1 = v.Id();
			p.v2 = v.Rotulo();

			char chr[sizeof(p)];
			memcpy(chr, &p, sizeof(p));
			ss.write(chr, sizeof(p));
		}
		return ss;
	}

	void Grafo::ProcessarStream(int m, stringstream* stream, ParAdj* pares)
	{
		stream->read((char*)pares, m * sizeof(ParAdj));
	}

	bool bsearch(vector<Vertice>::iterator inicio, vector<Vertice>::iterator fim, int valor)
	{
		int tam = distance(inicio,fim);
		if (tam > 1)
		{
				vector<Vertice>::iterator meio = inicio + (tam/2);
				if (*meio == valor)
				{
					return true;
				}
				else
				{
					if (*meio > valor)
					{
						return bsearch(inicio,meio,valor);
					}
					else
					{
						return bsearch(meio,fim,valor);
					}
				}
		}
		else
		{
			return *inicio == valor;
		}
	}


	bool Grafo::TestarVertice(int const id) const
	{
		vector<Vertice> vet(Vertices.begin(), Vertices.end());
		return bsearch(vet.begin(), vet.end(), id);
	}

	long Grafo::ContarTriangulos() const
	{
		long triangulos = 0;
		for (Vertice  a : Vertices)
		{
			for (Vertice b : Adjacentes(a))
			{
				if (a.Id() >= b.Id()) continue;
				for (Vertice c : Adjacentes(b))
				{
					if (a.Id() >= c.Id() || b.Id() >= c.Id()) continue;
					if (VerificarAdjacencia(a,c)) triangulos = triangulos + 1;
				}
			}
		}
		return triangulos;
	}

	vector<pair<Vertice,Vertice>> Grafo::ObterListaAdjacencias() const
	{
		return AdjList.ObterListaAdjacencias();
	}

	void Grafo::ObterListaVertices(ParAdj* vertices)
	{
		int i = 0;
		for (Vertice v : Vertices)
		{
			vertices[i].v1 = v.Id();
			vertices[i].v2 = v.Rotulo();
			i++;
		}
	}

	void Grafo::ObterListaArestas(ParAdj* arestas)
	{
		vector<pair<Vertice,Vertice>> adjs = AdjList.ObterListaAdjacencias();
		int i = 0;
		for (pair<Vertice,Vertice> p : adjs)
		{
			arestas[i].v1 = p.first.Id();
			arestas[i].v2 = p.second.Id();
			i++;
		}
	}

	string Grafo::ObterString(set<Vertice> vet)
	{
		stringstream s;
		s << "{ ";
		int i = 0;
		for (set<Vertice>::iterator it = vet.begin(); it != vet.end();it++)
		{
			s << it->Id();
			if (i < vet.size() - 1) s << ", ";
			i++;
		}
		s << " }";
		return s.str();
	}

	string Grafo::ObterString(vector<Vertice> vet)
	{
		stringstream s;
		s << "{ ";
		for (int i = 0; i < vet.size(); i++)
		{
			s << vet[i].Id();
			if (i < vet.size() - 1) s << ", ";
		}
		s << " }";
		return s.str();
	}


}

