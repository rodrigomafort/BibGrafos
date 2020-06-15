#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class GrafoIntervaloProprio :
		public GrafoIntervalo
	{
	public:
		GrafoIntervaloProprio();
		GrafoIntervaloProprio(int n);
		GrafoIntervaloProprio(int n, int** matAdj);

		static Grafo* ConstruirAleatorio(int n);		

		~GrafoIntervaloProprio();

		virtual void ConstruirIntervalos(int n);
	};
}