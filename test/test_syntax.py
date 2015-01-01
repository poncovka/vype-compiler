'''
test_syntax.py

Tests with syntax errors.
'''

from base import TestCase, Error

class SyntaxTestCase(TestCase):
  prefix = 'syn'
  
#-----------------------------------
  def test_syn1(self):
    self.input = \
r'''
int main(void) {
  a;
}
'''
    self.output = Error.SYNTAX
    self.execute()  
    
#-----------------------------------
  def test_syn2(self):
    self.input = \
r'''

int f(){}

int main(void) {
}
'''
    self.output = Error.SYNTAX
    self.execute()  
    
#-----------------------------------
  def test_syn3(self):
    self.input = \
r'''
int main(void) {
  if (1) {}
}
'''
    self.output = Error.SYNTAX
    self.execute()  

#-----------------------------------
  def test_syn4(self):
    self.input = \
r'''
int main(void) {
  {{{}}}{{}}}
}
'''
    self.output = Error.SYNTAX
    self.execute()  

#-----------------------------------
  def test_syn5(self):
    self.input = \
r'''
int main(void) {
  int a;
  a = 5 + 6 ** 7 ;
}
'''
    self.output = Error.SYNTAX
    self.execute()  

#-----------------------------------
  def test_syn6(self):
    self.input = \
r'''
int main(void) {
  while(){}
}
'''
    self.output = Error.SYNTAX
    self.execute()  

#-----------------------------------
  def test_syn7(self):
    self.input = \
r'''
int main(void) {
  while(1);
}
'''
    self.output = Error.SYNTAX
    self.execute()  

#-----------------------------------
  def test_syn8(self):
    self.input = \
r'''

int f (int a, char c, strig s,) {
  return 1;
}

int main(void) {
}
'''
    self.output = Error.SYNTAX
    self.execute()  

#-----------------------------------
  def test_syn9(self):
    self.input = \
r'''
int main(void) {
  while(1){};
}
'''
    self.output = Error.SYNTAX
    self.execute()  

#-----------------------------------
  def test_syn10(self):
    self.input = \
r'''
int main(void) {
  break;
}
'''
    self.output = Error.SYNTAX
    self.execute()  

#-----------------------------------
  def test_syn11(self):
    self.input = \
r'''
int main(void) {
  int if;
}
'''
    self.output = Error.SYNTAX
    self.execute() 

#-----------------------------------
  def test_syn12(self):
    self.input = \
r'''
int main(void) {
  int void;
}
'''
    self.output = Error.SYNTAX
    self.execute() 

#-----------------------------------
  def test_syn13(self):
    self.input = \
r'''
int main(void) {
  int;
}
'''
    self.output = Error.SYNTAX
    self.execute() 

#-----------------------------------
  def test_syn14(self):
    self.input = \
r'''
int main (void) {
  int a;
  char c;
  string s;
  
  a = 3 + ()/2;
}
'''
    self.output = Error.SYNTAX
    self.execute()

# end of file
