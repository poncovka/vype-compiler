'''
test_example.py

Example of test case.
'''
from base import TestCase, Error

class ExampleTestCase(TestCase):

  def test_ok(self):
    self.input = "Example of the input."
    self.output = Error.OK
    self.execute()

  def test_err(self):
    self.input = \
'''

Very
long
example.

'''
    self.output = Error.LEXICAL
    self.execute()




