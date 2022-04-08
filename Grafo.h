#include "BibGrafos.h"
#pragma once


namespace BibGrafos
{
	typedef struct
	{
		int v1;
		int v2;
	} ParAdj;

	class Grafo
	{
		public:
			ListaAdjacencia AdjList;
			set<Vertice> Vertices;
			map<int,Vertice> mapaVertices;

			int N() const;
			int M() const;

			string info;

			virtual ~Grafo() = default;
			Grafo();
			Grafo(int n);
			Grafo(int n, int** adjMat);
			Grafo(int n, int m, int* lista);
			Grafo(int n, int m, ParAdj* adjs);
			Grafo(int n, int m, ParAdj* vertices, ParAdj* adjs);
			Grafo(int n, set<set<int>> cliques);
			Grafo(Grafo* inst);

			bool SalvarArquivoBinario(string nomeArquivo);
			static Grafo ConstruirDeArquivoBinario(string nomeArquivo);

			void InicializarVertices(int n);
			int ObterNovoId();

			int ObterGrauMaximo() const;
			int ObterGrauMinimo() const;

			Vertice operator[](int const id) const;
			Vertice VerticePorId(int const id) const;
			Vertice VerticePorRotulo(int const rotulo) const;
			Vertice VerticePorIndice(int const indice) const;

			bool TestarVertice(int const id) const;

			virtual Vertice AdicionarVertice();
			virtual Vertice AdicionarVertice(int id);
			virtual Vertice AdicionarVertice(int id, int rotulo);
			virtual Vertice AdicionarVertice(Vertice v);

			virtual void RemoverVertice(Vertice v);
			virtual bool AdicionarAdjacencia(Vertice v, Vertice u);
			virtual bool AdicionarAdjacencia(int vId, int uID);
			virtual bool RemoverAdjacencia(Vertice v, Vertice u);

			static Grafo* ConstruirAleatorio(int n);
			static Grafo* ConstruirAleatorio(int n, double prob);

			virtual bool TestarConexidade() const;

			virtual int Grau(Vertice v) const;
			virtual set<Vertice> AdjacentesFechado(Vertice v) const;
			virtual set<Vertice> Adjacentes(Vertice v) const;
			virtual vector<Vertice> AdjacentesEmS(Vertice v, vector<Vertice> S) const;
			virtual set<Vertice> AdjacentesEmS(Vertice v, set<Vertice> S) const;

			virtual bool VerificarAdjacencia(Vertice v, Vertice u) const;
			virtual bool VerificarGemeoVerdadeiro(Vertice v, Vertice u) const;

			virtual Grafo* SubGrafoInduzidoPorRemocao(vector<Vertice> CV) const;
			virtual Grafo* ObterSubgrafoInduzido(vector<Vertice> cv) const;

			virtual int** ObterMatrizAdjacencias() const;

			virtual void ObterListaAdjacencias(int* lista) const;
			virtual vector<pair<Vertice,Vertice>> ObterListaAdjacencias() const;

			virtual stringstream ObterListaParAdjs() const;
			virtual stringstream ObterListaVertices() const;

			virtual void ObterListaVertices(ParAdj* vertices);
			virtual void ObterListaArestas(ParAdj* arestas);

			static void ProcessarStream(int m, stringstream* stream, ParAdj* pares);

			virtual string ObterStringListaAdjacencias() const;
			virtual string ObterStringMatrizAdjacencias() const;
			virtual string ObterDetalhes() const;

			unsigned long ObterNumeroCombinacoes() const;
			virtual Grafo* ObterGrafoBarra() const;
			virtual string ObterDot() const;
			virtual long ContarTriangulos() const;

			static string ObterString(set<Vertice> vet);
			static string ObterString(vector<Vertice> vet);
	};
	typedef Grafo*(ChamadaConstrutor)(int n);
}

