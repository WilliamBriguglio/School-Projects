#include <map>
#include <stdexcept>
#include <memory>
#include <tuple>
#include <limits>
#include <iostream>

using uint_type = unsigned long long;

class ack{

  public:
    using tuple_type = std::tuple<uint_type, uint_type>;

    uint_type operator()(uint_type const& m, uint_type const& n) const
    {
      tuple_type a(m,n);    //I had to change the code you provided to this because the original code wouldn't compile as detailed in the emails I sent you
      return calculate(a);
    }

    uint_type calculate(tuple_type const& args) const
    {
      auto e = cache_.find(args);
      if(e != std::end(cache_)){ //if cache args is not found in cache
        return e->second;
      }
      uint_type retval;
      uint_type const m = std::get<0>(args);
      uint_type const n = std::get<1>(args);

      if( m == 0){
        if(n < std::numeric_limits<uint_type>::max()){ //case 1
            retval = n+1;
        }else{
            throw std::range_error("ack::unit_tpye overflow");
        }
      }else if((m > 0) && (n == 0)){ //case 2
        tuple_type b(m-1,1);
        retval = calculate(b);
      }else{ //case 3
        tuple_type c(m,n-1);
        tuple_type d(m-1,calculate(c));
        retval = calculate(d);
      }

      cache_.insert({args, retval}); // ad value to cache

      return retval;
    }

  private:
    static std::map<tuple_type, uint_type> cache_; //declare static container

};

std::map<ack::tuple_type, uint_type> ack::cache_; //declare static container

int main()
{

  using std::cout;

  ack f;

  for (uint_type m=0; m != 5; ++m)
  {
    for (uint_type n=0; n != 10; ++n)
    {
      cout << "f(" << m << ',' << n << ") = ";
      try
      {
        cout << f(m,n) << '\n';
      }
      catch (std::range_error &e)
      {
        cout << e.what() << '\n';
      }
      catch (std::bad_alloc &e)
      {
        cout << e.what() << '\n';
      }
      catch (...)
      {
          cout << "<UNKNOWN ERROR>\n";
      }
    }
    cout << '\n';
  }

}
