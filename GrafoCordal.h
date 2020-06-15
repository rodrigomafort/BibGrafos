#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class GrafoCordal : public Grafo
	{
	public:
		virtual ~GrafoCordal() = default;
		virtual vector<Vertice> ObterEEP();
		virtual bool TestarEEP(vector<Vertice> EEP);
		virtual bool TestarCordal();

		virtual ArvoreCliques ObterArvoreCliques();
		virtual ArvoreCliques ObterArvoreCliques(vector<Vertice> EEP);

		virtual set<set<Vertice>> ObterCliquesMaximais();
		virtual set<set<Vertice>> ObterCliquesMaximais(vector<Vertice> EEP);

		GrafoCordal();
		GrafoCordal(Grafo G);
		GrafoCordal(int n);
		GrafoCordal(int n, int** adjMat);
		GrafoCordal(int n, set<set<int>> cliques);

		static Grafo* ConstruirAleatorio(int n);		
	};
}
