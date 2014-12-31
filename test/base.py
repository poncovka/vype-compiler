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
import inspect
import os

class Error():

  OK       = 0
  LEXICAL  = 1
  SYNTAX   = 2
  SEMANTIC = 3
  CODEGEN  = 4
  INTERN   = 5

class TestCase(unittest.TestCase):

  program     = '../vype'
  prefix      = 'testcase'

  def setUp(self):
    self.input = ""
    self.output = Error.OK
    self.result = None

  def execute(self):
    # create input file
    
    method_name = inspect.stack()[1][3].lower()
    
    input_file  = 'temp/{}_{}.in'.format(self.prefix, method_name)
    output_file = 'temp/{}_{}.out'.format(self.prefix, method_name)
    error_file = 'temp/{}_{}.err'.format(self.prefix, method_name)
    
    f = open(input_file, 'w')
    f.write(self.input)
    f.close()
    
    # execute program
    err = open(error_file, 'wb')
    self.result = subprocess.call([self.program, input_file, output_file], stderr = err)
    err.close();
    
    # compare return codes
    self.assertEqual(self.result, self.output, "Expected return code: {}".format(self.output)) 
    
  def tearDown(self):
    pass
    
#  def test(self):
#    self.input = "ahoj"
#    self.output = Error.LEXICAL
#    self.execute()

