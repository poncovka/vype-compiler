'''
test_semantic.py

Tests with semantic errors.
'''

from base import TestCase, Error

class SemanticTestCase(TestCase):
  prefix = 'sem'

#-----------------------------------
  def test_missing_main(self):
    self.input = \
r'''
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_missing_main2(self):
    self.input = \
r'''
int f (int a, string b) {
  string c, d;
  return a + 10;
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_main_params(self):
    self.input = \
r'''
int main (int a, string b) {
  string c, d;
  return a + 10;
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_main_return(self):
    self.input = \
r'''
string main (void) {
  string c, d;
  return c;
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_decl1(self):
    self.input = \
r'''

string f (int, int, string);

string f (int a, char b, string s) {
  return s;
}

int main (void) {}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_decl2(self):
    self.input = \
r'''

string f (int, char);

string f (int a, char b, string s) {
  return s;
}

int main (void) {}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_decl3(self):
    self.input = \
r'''

string f (int, char, string);

string f (int a, int b, string s) {
  return s;
}

int main (void) {}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_decl4(self):
    self.input = \
r'''

string f (int, char, string);

string f (int a, string s) {
  return s;
}

int main (void) {}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_decl5(self):
    self.input = \
r'''

string f (int, char, string);

string ff (int a, char b, string s) {
  return s;
}

int main (void) {}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_decl6(self):
    self.input = \
r'''

string f (int, char, string);

int f (int a, int b, string s) {
  return a;
}

int main (void) {}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_decl7(self):
    self.input = \
r'''

string f (int, char, string);

int f (int a, char b, string s) {
  return a;
}

int f (int a, char b, string s) {
  return a;
}

int main (void) {}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_decl8(self):
    self.input = \
r'''

string f (int, char, string);

int f (int a, char b, string s) {
  return a;
}

string f (int, char, string);

int main (void) {}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_call1(self):
    self.input = \
r'''

string f (int, char, string);

string f (int a, char b, string s) {
  return s;
}

int main (void) {
  print(f(1,'a', 'a'));
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_call2(self):
    self.input = \
r'''

string f (int, char, string);

string f (int a, char b, string s) {
  return s;
}

int main (void) {
  int s;
  s = f(1,'a', "abcde");
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_return(self):
    self.input = \
r'''

string f (int, char, string);

string f (int a, char b, string s) {
  return a;
}

int main (void) {
  print(f(1,'a', "abcde"));
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_var1(self):
    self.input = \
r'''

string f (int, char, string);

string f (int a, char b, string s) {
  int a;
  return s;
}

int main (void) {
  print(f(1,'a', "abcde"));
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_var2(self):
    self.input = \
r'''

string f (int, char, string);

string f (int a, char b, string s) {
  int b;
  return s;
}

int main (void) {
  print(f(1,'a', "abcde"));
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_undef1(self):
    self.input = \
r'''

string f (int, char, string);

string f (int a, char b, string s) {
  return undef;
}

int main (void) {
  print(f(1,'a', "abcde"));
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_undef2(self):
    self.input = \
r'''

string f (int, char, string);

string f (int a, char b, string s) {
  return undef(a,b,c);
}

int main (void) {
  print(f(1,'a', "abcde"));
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_expr1(self):
    self.input = \
r'''
int main (void) {
  int a;
  char c;
  string s;
  
  a = s;
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_expr2(self):
    self.input = \
r'''
int main (void) {
  int a;
  char c;
  string s;
  
  a = (int)s;
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_expr3(self):
    self.input = \
r'''
int main (void) {
  int a;
  char c;
  string s;
  
  a = s / 5;
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_expr4(self):
    self.input = \
r'''
int main (void) {
  int a;
  char c;
  string s;
  
  a = !c;
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_expr5(self):
    self.input = \
r'''
int main (void) {
  int a;
  char c;
  string s;
  
  a = 3 + (char)'c'/2;
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_expr6(self):
    self.input = \
r'''
int main (void) {
  int a;
  char c;
  string s;
  
  undef = 3 + (char)'c'/2;
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_cond1(self):
    self.input = \
r'''
int main (void) {
  int a;
  char c;
  string s;
  
  while(s) {
    a = a + 1;
  }
  
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_cond2(self):
    self.input = \
r'''
int main (void) {
  int a;
  char c;
  string s;
  
  if(c) {
    a = a + 1;
  }
  else {
  }
  
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_void(self):
    self.input = \
r'''

void f (int a, char b, string s) {
  return;
}

int main (void) {
  int a;
  a = f(1,'a', "abcd");
}
'''
    self.output = Error.SEMANTIC
    self.execute()

#-----------------------------------
  def test_noparam(self):
    self.input = \
r'''

string f (void) {
  return "TEST";
}

int main (void) {
  int a;
  print(f(a));
}
'''
    self.output = Error.SEMANTIC
    self.execute()



