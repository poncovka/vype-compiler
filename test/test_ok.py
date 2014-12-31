'''
test_ok.py

Tests with correct inputs.

Method:
  def test_(self):
    self.input = \
'''
'''
    self.output = Error.OK
    self.execute()
    
'''

from base import TestCase, Error

class OKTestCase(TestCase):
  prefix = 'ok'
#-----------------------------------
  def test_main(self):
    self.input = \
'''
int main(void) {
}
'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_defvar(self):
    self.input = \
'''
int main(void) {
  int a, b, c;
}
'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_blocks(self):
    self.input = \
'''
int main(void) {
  int a, b, c;
  a = 1;
  b = 2;
  c = 3;
  {
    int a, b;
    a = 10;
    b = 20;
    {
      int a;
      a = 100;
    }
  }
}
'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_expr(self):
    self.input = \
'''
int main(void) {
  return (((1 + 2) * 3) / 4) % 5 - 6;
}
'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_exprvar(self):
    self.input = \
'''
int main(void) {
  int a, b, c;
  a = 1;
  b = 3;
  c = 5;
  return (((a + 2) * b) / 4) % c - 6;
}
'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_bool(self):
    self.input = \
'''
int main(void) {
  return 1 < 2 <= 3 > 4 >= 5 == 6 != 7 && 8 || !9 ;
}
'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_boolvar(self):
    self.input = \
'''
int main(void) {
  int a,b,c;
  a = 2;
  b = 5;
  c = 9;
  return 1 < a <= 3 > 4 >= b == 6 != 7 && 8 || !c ;
}
'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_cast(self):
    self.input = \
'''
int main(void) {
  int x;
  char c;
  string s;
  
  x = 10;
  c = '\n';
  s = "string\n";
  
  x = (int) c;
  c = (char) x;
  s = (string) c;
}
'''
    self.output = Error.OK
    self.execute()


#-----------------------------------
  def test_call(self):
    self.input = \
'''

int f (int x, char c, string s) {
  int y;
  char d;
  string r;
  
  x = x + y;
  d = (char) x;
  r = "string";
  r = s;
  
  return x;
}

int main(void) {
  int a,b,c;
  char d,e,f;
  string s;
  
  a = 1;
  b = 2;
  c = 3;
  d = 'a';
  e = 'b';
  f = 'c';
  s = "defghijkl";
  
  return a + f((a + b) / c - 100, (char)((int) d), s) / b;
}
'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_fce(self):
    self.input = \
'''
string s(string);

int i(int);

int main(void) {
  string s;
  int i;
  
  s = s("string");
  i = i(5);
}

string s(string x) {
  return x;
}

int i(int x) {
  return x;
}
'''
    self.output = Error.OK
    self.execute()
    
#-----------------------------------
  def test_fce2(self):
    self.input = \
'''
int x(int a, int b) {
   int d, e, f;
   d = e;
   return f;
}

int y(int a, int b, int c) {
   int d, e, f;
   d = e;
   return x(a,b);
}

int main(void) {
  y(1,2,3);
}
'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_infinite(self):
    self.input = \
'''
int i(int x) {
  return i(x);
}

int main(void) {
  int i;
  i = i(5);
}
'''

#-----------------------------------
  def test_recursion(self):
    self.input = \
'''
// Returns sum of 5+4+3+2+1+0

int i(int x) {
  if (x == 0) {
    return x;
  }
  else {
    return x + i(x);
  }
}

int main(void) {
  int i;
  i = i(5);
}
'''

