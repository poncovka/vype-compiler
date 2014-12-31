'''
test_examples.py

Tests with examples from assignment..
'''

from base import TestCase, Error

class ExamplesTestCase(TestCase):
  prefix = 'ex'
#-----------------------------------
  def test_factorial(self):
    self.input = \
'''

/* Program 1: Vypocet faktorialu (iterativne) */
int main(void) { // Hlavni telo programu
  int a, vysl;
  print("Zadejte cislo pro vypocet faktorialu");
  a = read_int();
  if (a < 0) {
    print("\nFaktorial nelze spocitat\n"); }
  else
  {
    vysl = 1;
    while (a > 0)
    {
      vysl = vysl * a;
      a = a - 1;
      } // endwhile
    print("\nVysledek je: ", vysl, "\n");
  } // endi
} // main

'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_recursion(self):
    self.input = \
'''

/* Program 2: Vypocet faktorialu (rekurzivne) */

int factorial (int);

int factorial (int n)
{
  int decremented_n;
  int temp_result;
  
  if (n < 2)
  {
    return 1;
  }
  else
  {
    decremented_n = n - 1;
    temp_result = factorial(decremented_n);
  }
  
  return (int)n * temp_result;
  
} // end of factorial


int main(void)
{
  int a; int vysl;
  print("Zadejte cislo pro vypocet faktorialu");
  a = read_int();
  if (a < 0)
  {
    print("\nFaktorial nelze spocitat\n");
  }
  else
  {
  print("\nVysledek je:", factorial(a), "\n");
  }
}

'''
    self.output = Error.OK
    self.execute()

#-----------------------------------
  def test_string(self):
    self.input = \
'''

/* Program 3: Prace s retezci a vestavenymi funkcemi */

int main(void) { //Hlavni telo programu
  string str1, str2;
  int p; char zero;
  
  str1 = "Toto je nejaky text";
  str2 = strcat(str1, ", ktery jeste trochu obohatime");
  
  print(str1, ’\n’, str2, "\n");
  str1 = read_string();
  
  while ((int)(get_at(str1, p)) != 0)
  {
    p = p + 1;
  }
  print("\nDelka retezce \"", str1, "\", je ", p, " znaku.\n");
  
}

'''
    self.output = Error.OK
    self.execute()



