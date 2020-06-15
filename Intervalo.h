#include "BibGrafos.h"
#pragma once

namespace BibGrafos
{
	class Intervalo
	{
		protected:
			int inicio;
			int fim;
			int id;
		public:

			Intervalo();
			Intervalo(int id, int inicio, int fim);

			~Intervalo();

			int Inicio() const;
			int Fim() const;
			int Id() const;
		};
}
