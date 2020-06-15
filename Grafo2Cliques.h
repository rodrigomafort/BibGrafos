#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class Grafo2Cliques : public GrafoCordal
	{
	public:
		virtual ~Grafo2Cliques() = default;
		Grafo2Cliques();
		Grafo2Cliques(int n);
		Grafo2Cliques(int C1, int C2, int Inter);
		Grafo2Cliques(int n, int** matAdj);
		Grafo2Cliques(Grafo G);
		
		static Grafo* ConstruirAleatorio(int n);
		static Grafo ConstruirParticionado(int A1, int A2, int A3);
		static vector<Grafo2Cliques> ConstruirTodos(int n);
		static vector<Grafo2Cliques> ConstruirTodosRestrito(int n);
	};
}
