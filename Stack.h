/*!
 * \author: Martin Šifra <xsifra00@stud.fit.vutbr.cz>
 * \author: Vendula Poncová <xponco00@stud.fit.vutbr.cz>
 */


#ifndef STACK_H
#define	STACK_H

#include <stack>

using namespace std;

class Stack {
public:
	int sp;
	int fp;
	
	Stack(unsigned size);
	void push(unsigned i);
	void pop(unsigned i);

private:
	int size;

};

#endif	/* STACK_H */

