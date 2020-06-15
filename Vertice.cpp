#include "BibGrafos.h"
namespace BibGrafos
{
	int Vertice::Id() const
	{
		return id;
	}

	int Vertice::Indice() const
	{
		return id - 1;
	}

	int Vertice::Rotulo() const
	{
		return rotulo;
	}


	Vertice::Vertice(int pId)
	{
		id = pId;
		rotulo = -1;
	}

	Vertice::Vertice(int pId, int pRotulo)
	{
		id = pId;
		rotulo = pRotulo;
	}

	Vertice::Vertice()
	{
		id = -1;
		rotulo = -1;
	}

	bool Vertice::operator< (const Vertice& outro) const
	{
		return id < outro.Id();
	}

	bool Vertice::operator> (const Vertice& outro) const
	{
		return id > outro.Id();
	}

	bool Vertice::operator== (const Vertice& outro) const
	{
		return id == outro.Id();
	}

	bool Vertice::operator!= (const Vertice& outro) const
	{
		return id != outro.Id();
	}
}
