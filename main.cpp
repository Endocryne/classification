#include <ios>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

enum class Color 
{ 
  red, 
  green, 
  blue 
};

enum class Size 
{
   small, 
   medium, 
   large 
};

struct Product
{
  string name;
  Color color;
  Size size;
};

template <typename T> struct Spec
{
  virtual bool is_satisfied(T* item) = 0;
};

template <typename T> struct Filter 
{
  virtual vector<T*> filter(vector<T*> items,
                            Spec<T>& spec) = 0;
};

struct BF : Filter<Product> 
{
  vector<Product*> filter(vector<Product*> items,
                          Spec<Product> &spec) override 
  {
    vector<Product*> result;
    for(auto& i : items)
      if(spec.is_satisfied(i))
        result.push_back(i);
    return result;
  }
};

struct ColorSpec : Spec<Product>
{
  Color color;
  ColorSpec(Color color) : color(color) {}
  
  bool is_satisfied(Product* i) override {
    return i->color == color;
  }
};

struct SizeSpec : Spec<Product>
{
  Size size;
  explicit SizeSpec(const Size size)  : size{size} { }

  bool is_satisfied(Product* i) override {
    return i->size == size;
  }
};

template <typename T> struct AndSpec : Spec<T>
{
  Spec<T>& first;
  Spec<T>& second;

  AndSpec(Spec<T> &first,
          Spec<T> &second) : first(first), second(second) {}
  bool is_satisfied(T *item) override {
    return first.is_satisfied(item) && second.is_satisfied(item);
  } 
};

int main()
{
  Product apple{"Apple", Color::green, Size::small};
  Product tree{"Tree", Color::green, Size::large};
  Product house{"House", Color::blue, Size::large};

  vector<Product*> all = { &apple, &tree, &house };

  BF bf;
  ColorSpec green(Color::green);
  for(auto& i : bf.filter(all, green))
    cout << i->name << " is green.\n";

  SizeSpec large(Size::large);
  AndSpec<Product> g_a_l(green, large);

  for(auto& i : bf.filter(all, g_a_l))
    cout << i->name << " is green and large.\n";

  getchar();
  return 0;
}
