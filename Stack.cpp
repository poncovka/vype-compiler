/*!
 * \author: Martin Šifra <xsifra00@stud.fit.vutbr.cz>
 * \author: Vendula Poncová <xponco00@stud.fit.vutbr.cz>
 */

#include "Stack.h"

Stack::Stack(unsigned size) {
	this->size = size;
	sp = size;
	fp = size;
}

void Stack::push(unsigned i) {
	sp -= i;
}

void Stack::pop(unsigned i) {
	sp += i; 
}

