'''
test_lexical.py

Tests with lexical errors.
'''

from base import TestCase, Error

class LexicalTestCase(TestCase):
  prefix = 'lex'
  
#-----------------------------------
  def test_lex1(self):
    self.input = \
r'''
int main(void) {
  int abcd#efg;
}
'''
    self.output = Error.LEXICAL
    self.execute() 
    
#-----------------------------------
  def test_lex2(self):
    self.input = \
r'''
int main(void) {
  string s;
  s = "TEST;
}
'''
    self.output = Error.LEXICAL
    self.execute() 
    
#-----------------------------------
  def test_lex3(self):
    self.input = \
r'''
int main(void) {
  string s;
  /* s = "TEST";
}
'''
    self.output = Error.LEXICAL
    self.execute() 

#-----------------------------------
  def test_lex4(self):
    self.input = \
r'''
int main(void) {
  char c;
  c = 'x;
}
'''
    self.output = Error.LEXICAL
    self.execute() 

#-----------------------------------
  def test_lex5(self):
    self.input = \
r'''
int main(void) {
  string s;
  s = "TES\T";
}
'''
    self.output = Error.LEXICAL
    self.execute() 

#-----------------------------------
  def test_lex6(self):
    self.input = \
r'''
int main(void) {
  string s;
  s = "TE'ST";
}
'''
    self.output = Error.LEXICAL
    self.execute() 

#-----------------------------------
  def test_lex7(self):
    self.input = \
r'''
int main(void) {
  string s;
  s = "TEST"";
}
'''
    self.output = Error.LEXICAL
    self.execute() 

#-----------------------------------
  def test_lex8(self):
    self.input = \
r'''
int main(void) {
  string s;
  s = "T
  EST";
}
'''
    self.output = Error.LEXICAL
    self.execute() 

#-----------------------------------
  def test_lex9(self):
    self.input = \
r'''
int main(void) {
  char c;
  c = '';
}
'''
    self.output = Error.LEXICAL
    self.execute() 

#-----------------------------------
  def test_lex10(self):
    self.input = \
r'''
int main(void) {
  char c;
  c = '
  ';
}
'''
    self.output = Error.LEXICAL
    self.execute() 

#-----------------------------------
  def test_lex11(self):
    self.input = \
r'''
int main(void) {
  ~
}
'''
    self.output = Error.LEXICAL
    self.execute() 

#-----------------------------------
  def test_lex12(self):
    self.input = \
r'''
int main(void) {
  char c;
  c = 'abc';
}
'''
    self.output = Error.LEXICAL
    self.execute() 

# end of file
