#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class Vertice
	{
		private:
			int id;
			int rotulo;

		public:
			int Id() const;
			int Indice() const;
			int Rotulo() const;

			Vertice();
			Vertice(int Id);
			Vertice(int id, int rotulo);

			bool operator< (const Vertice& outro) const;
			bool operator> (const Vertice& outro) const;
			bool operator== (const Vertice& outro) const;
			bool operator!= (const Vertice& outro) const;
	};
}
