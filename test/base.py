'''
base.py

Parent class for all test cases.

Names of files:        test*.py
Names of test methods: test*

How to run the tests:
  python3 -m unittest discover
  python3 -m unittest test_example
  python3 -m unittest test_example.ExampleTestCase.test_ok
  
'''

import unittest
import subprocess

class Error():

  OK       = 0
  LEXICAL  = 1
  SYNTAX   = 2
  SEMANTIC = 3
  CODEGEN  = 4
  INTERN   = 5

class TestCase(unittest.TestCase):

  program     = '../vype'
  input_file  = 'temp/input'
  output_file = 'temp/out.asm'

  def setUp(self):
    self.input = ""
    self.output = Error.OK
    self.result = None

  def execute(self):
    # create input file
    
    f = open(self.input_file, 'w')
    f.write(self.input)
    f.close()
    
    # execute program
    self.result = subprocess.call([self.program, self.input_file, self.output_file])
    
    # compare return codes
    self.assertEqual(self.result, self.output, "Expected return code: {}".format(self.output)) 
    
  def tearDown(self):
    pass
    
#  def test(self):
#    self.input = "ahoj"
#    self.output = Error.LEXICAL
#    self.execute()

