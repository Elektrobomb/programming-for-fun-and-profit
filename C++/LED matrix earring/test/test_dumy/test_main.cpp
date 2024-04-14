#define DOCTEST_CONFIG_IMPLEMENT  // REQUIRED: Enable custom main()
#include <doctest.h>

// TEST_CASE ...
// TEST_SUITE ...

int add(int a, int b)
{
  return a + b;
}

TEST_CASE("Addition")
{
  CHECK(add(1, 1) == 2);
  CHECK(add(2, 2) == 4);
}

TEST_CASE("Subtraction")
{
  CHECK(add(-1, 1) == 0);
  CHECK(add(2, -2) == 0);
  CHECK(add(2, -1) == 1);
}

int multiply(int a, int b)
{
  return a * b;
}

TEST_CASE("Multiplication")
{
  CHECK(multiply(2, 3) == 6);
  CHECK(multiply(4, 5) == 20);
}

int subtract(int a, int b)
{
  return a - b;
}

TEST_CASE("Subtraction")
{
  CHECK(subtract(5, 3) == 2);
  CHECK(subtract(10, 5) == 5);
}

int divide(int a, int b)
{
  return a / b;
}

TEST_CASE("Division")
{
  CHECK(divide(10, 2) == 5);
  CHECK(divide(20, 4) == 5);
}

std::string concatenate(const std::string& str1, const std::string& str2)
{
  return str1 + str2;
}

TEST_CASE("String Concatenation")
{
  CHECK(concatenate("Hello, ", "world!") == "Hello, world!");
  CHECK(concatenate("GitHub ", "Copilot") == "GitHub Copilot");
}

// Add more test functions and TEST_CASEs here


int main(int argc, char **argv)
{
  doctest::Context context;

  // BEGIN:: PLATFORMIO REQUIRED OPTIONS
  context.setOption("success", true);     // Report successful tests
  context.setOption("no-exitcode", true); // Do not return non-zero code on failed test case
  // END:: PLATFORMIO REQUIRED OPTIONS

  // YOUR CUSTOM DOCTEST OPTIONS

  context.applyCommandLine(argc, argv);
  return context.run();
}