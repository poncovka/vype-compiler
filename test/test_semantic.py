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
