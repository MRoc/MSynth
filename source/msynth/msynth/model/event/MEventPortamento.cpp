#include "MEventPortamento.h"

/** 
 * constructor
 *
 * frequency - the target frequency in Hz
 * portamentoLength - the length of the portamento in samples
 *
 */
MEventPortamento::MEventPortamento( FP frequency, unsigned int portamentoLength ) :
	MEventNoteOn( frequency ),
	ivPortamentoLength( portamentoLength )
{
	ivType = PORTAMENTO;
}

/**
 * destructor
 */
MEventPortamento::~MEventPortamento()
{
}

/**
 * returns the length of the portamento in samples
 */
unsigned int MEventPortamento::getPortamentoLength()
{
	return ivPortamentoLength;
}