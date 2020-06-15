#include "BibGrafos.h"

namespace BibGrafos
{
	Intervalo::Intervalo()
	{
		id = 0;
		inicio = 0;
		fim = 0;
	}

	Intervalo::Intervalo(int Id, int Inicio, int Fim)
	{
		id = Id;
		inicio = Inicio;
		fim = Fim;
	}

	Intervalo::~Intervalo()
	{
	}

	int Intervalo::Inicio() const
	{
		return inicio;
	}

	int Intervalo::Fim() const
	{
		return fim;
	}

	int Intervalo::Id() const
	{
		return id;
	}
}
