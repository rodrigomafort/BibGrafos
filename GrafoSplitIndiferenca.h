#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	struct DetalhesSplitIndiferenca
	{
		int Caso;
		set<Vertice> C1;
		set<Vertice> C2;
		set<Vertice> C3;
		Vertice v;
		Vertice w;

		DetalhesSplitIndiferenca(int caso, set<Vertice> c1)
		{
			Caso = caso;
			C1 = c1;
		}
		DetalhesSplitIndiferenca(int caso, set<Vertice> c1, set<Vertice> c2, Vertice pv)
		{
			Caso = caso;
			C1 = c1;
			C2 = c2;
			v = pv;
		}
		DetalhesSplitIndiferenca(int caso, set<Vertice> c1, set<Vertice> c2, set<Vertice> c3, Vertice pv, Vertice pw)
		{
			Caso = caso;
			C1 = c1;
			C2 = c2;
			C3 = c3;
			v = pv;
			w = pw;
		}
	};

	class GrafoSplitIndiferenca : public GrafoCordal
	{
	public:

		int Caso();
		GrafoSplitIndiferenca();
		GrafoSplitIndiferenca(int n);
		GrafoSplitIndiferenca(int n, int classe);
		GrafoSplitIndiferenca(int n, int** matAdj);

		static Grafo* ConstruirAleatorio(int n);
		
		static Grafo ConstruirAleatorio(int n, int classe);
		virtual string ObterDetalhes();

		DetalhesSplitIndiferenca Detalhar();

	private:
		void ConstruirCaso1(int n);
		void ConstruirCaso2(int n);
		void ConstruirCaso3(int n);
		void ConstruirCaso4(int n);

		string ImprimirListaConjunto(vector<Vertice> lista);
		string ImprimirListaConjunto(set<Vertice> lista);
	};
}
