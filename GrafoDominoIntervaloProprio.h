#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class GrafoDominoIntervaloProprio:
		public GrafoCordal
	{

	public:
		GrafoDominoIntervaloProprio();
		GrafoDominoIntervaloProprio(int n);
		GrafoDominoIntervaloProprio(int n, int* vet);
		GrafoDominoIntervaloProprio(int n, int** matAdj);
		GrafoDominoIntervaloProprio(int n, set<set<int>> cliques);
		~GrafoDominoIntervaloProprio();

		static Grafo* ConstruirAleatorio(int n);
		static Grafo* ConstruirAleatorio(int n, int* vet);
	};
}
