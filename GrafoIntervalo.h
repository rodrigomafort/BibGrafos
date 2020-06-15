#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class GrafoIntervalo :
		public GrafoCordal
	{
		map<Vertice, Intervalo> intervalos;
	public:
		GrafoIntervalo();
		GrafoIntervalo(int n);
		GrafoIntervalo(int n, int** matAdj);

		static Grafo* ConstruirAleatorio(int n);

		~GrafoIntervalo();

		virtual void ConstruirIntervalos(int n);

		void AdicionarIntervalo(int id, int inicio, int fim);
		void AdicionarIntervalo(int inicio, int fim);
		virtual void AdicionarIntervalo(Intervalo i);

		virtual string ObterDetalhes();

		vector<set<Vertice>> ObterOrdenacaoCliques();

		vector<Vertice> ObterSequenciaGulosa();
	};
}