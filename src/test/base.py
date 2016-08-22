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
import sys

class Error():

  OK       = 0
  LEXICAL  = 1
  SYNTAX   = 2
  SEMANTIC = 3
  CODEGEN  = 4
  INTERN   = 5
  VALGRIND = 6
  
class TestCase(unittest.TestCase):

  program     = '../vype'
  prefix      = 'testcase'

  def setUp(self):
    self.input = ""
    self.output = Error.OK

  def execute(self):
    # create input file
    
    method_name = inspect.stack()[1][3].lower()
    
    input_file  = 'temp/{}_{}.in'.format(self.prefix, method_name)
    output_file = 'temp/{}_{}.out'.format(self.prefix, method_name)
    error_file = 'temp/{}_{}.err'.format(self.prefix, method_name)
    valgrind_file = 'temp/{}_{}.val'.format(self.prefix, method_name)
    
    f = open(input_file, 'w')
    f.write(self.input)
    f.close()
    
    # execute program
    err = open(error_file, 'wb')
    result = subprocess.call([self.program, input_file, output_file], stderr = err)
    err.close();
    
    # compare return codes
    self.assertEqual(result, self.output, "Expected return code: {}".format(self.output)) 
    
    # try valgrind
    err = open(valgrind_file, 'wb')
    result = subprocess.call(["valgrind", "-q", "--leak-check=full","--error-exitcode={}".format(Error.VALGRIND), 
                              self.program, input_file, output_file], stderr = err)
    err.close();

    if result == Error.VALGRIND:
      sys.stderr.write('WARNING: Memory leak detected in {} {}.\n'.format(self.prefix, method_name))
    
  def tearDown(self):
    pass
    
#  def test(self):
#    self.input = "ahoj"
#    self.output = Error.LEXICAL
#    self.execute()

