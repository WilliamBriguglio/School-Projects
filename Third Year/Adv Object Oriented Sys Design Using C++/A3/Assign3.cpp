#include <iostream>
#include <sstream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <iterator>

class ship
{
public:
  // All ships must have a name so prohibit the default constructor...
  ship() = delete;

  // Prohibit copies...
  ship(ship const&) = delete;
  ship& operator =(ship const&) = delete;

  // Permit moves...
  ship(ship&& s): name_(std::move(s.name_)){}
  ship& operator =(ship&& s){
      do_ship_only_move(std::move(s));
      return *this;
  }

  virtual ~ship(){
    delete[] m_array;
  }//virtual deconstructor

  // Permit specifying a name...
  explicit ship(std::string const& name): name_(name){}
  explicit ship(std::string&& name): name_(std::move(name)) {}

  virtual void sail() = 0;//must be overridden in concrete classes derived from ship

  virtual std::string get_ship_type() const{
      return std::string("ship");
  }

  std::string const& get_name() const{
      return name_;
  }
  std::string get_type_name_addr() const{
      std::ostringstream buf;
      buf << get_ship_type() << ' ' << get_name() << " [" << this << ']';
      return buf.str();
  }

protected:
  void do_ship_only_move(ship&& s){
    name_ = std::move(s.name_); //move ship's member variables(i.e. name_)
  };

private:
  std::string name_;
};

/**#######################################################################**/

class battle_ship : public virtual ship
{
  public:

    battle_ship() = delete;//prohibit the default constructor

    battle_ship(battle_ship const&) = delete; //prohibit copies
    battle_ship& operator =(battle_ship const&) = delete;

    battle_ship(battle_ship&& s): ship(std::move(s)){} //permit moves
    battle_ship& operator =(battle_ship&& s){
      ship::operator =(std::move(s));
      do_ship_only_move(std::move(s));
      return *this;
    }

    explicit battle_ship(std::string const& name): ship(name){}//permit constructors where name must be spcified
    explicit battle_ship(std::string&& name): ship(name){}

    virtual std::string get_ship_type() const override{ //over ride ship's get_ship_type() function
      return std::string("battle_ship");
    }

    virtual void sail() override{ //over ride ship's sail() function
        std::cout << get_type_name_addr() << " off to war!\n";
    }

    virtual void fire_weapon(){
        std::cout << get_type_name_addr() << " firing cannon!\n";
    }

  protected:
    void do_battle_ship_only_move(battle_ship&&){};//do nothing since battle_ship has no member variables
};

/**#######################################################################**/

template <typename T>
class cargo_ship : public virtual ship
{
  public:
    cargo_ship() = delete;//prohibit the default constructor

    cargo_ship(cargo_ship const&) = delete;//prohibit copies
    cargo_ship& operator =(cargo_ship const&) = delete;

    cargo_ship(cargo_ship&& s): ship(std::move(s)){//permit moves
        this->cargo_ = std::move(s.cargo_);
    }
    cargo_ship& operator =(cargo_ship&& s){
      ship::operator =(std::move(s));
      do_cargo_ship_only_move(std::move(s));
      return *this;
    }

    explicit cargo_ship(std::string const& name): ship(name), cargo_(){}//permit constructors where name must be spcified
    explicit cargo_ship(std::string&& name): ship(name), cargo_(){}

    template <typename Iter>//permit constructor where name, and a Iterator to the first and last element of a container with  "cargo" must be spcified
    explicit cargo_ship(std::string name, Iter first, Iter last): ship(name), cargo_(first, last){}

    virtual std::string get_ship_type() const override{//over ride ship's get_ship_type() function
      return std::string("cargo_ship");
    }

    virtual void sail() override{//over ride ship's sail() function
        std::cout << get_type_name_addr() << " setting sail.\n";
    }

    std::vector<T>& get_cargo(){
        return cargo_;
    }

    std::vector<T> const& get_cargo() const {
        return cargo_;
    }

  protected:
    void do_cargo_ship_only_move(cargo_ship&& s){
        this->cargo_=std::move(s.cargo_);//move cargo_ship's member variables(i.e. cargo_)
    }

    std::vector<T> cargo_;
};

/**#######################################################################**/

template <typename T>
class smuggler_ship : public virtual cargo_ship<T>, public virtual battle_ship
{
  public:
    smuggler_ship() = delete;//prohibit the default constructor

    smuggler_ship(smuggler_ship const&) = delete;//prohibit copies
    smuggler_ship& operator =(smuggler_ship const&) = delete;

    //permit moves
    smuggler_ship(smuggler_ship&& s): ship(std::move(s)), cargo_ship<T>(std::move(s)), battle_ship(std::move(s)) {}
    smuggler_ship& operator =(smuggler_ship&& s){
      do_smuggler_ship_only_move(std::move(s));
      return *this;
    }

    //permit constructor where name must be spcified
    explicit smuggler_ship(std::string const& name): ship(name), cargo_ship<T>(name), battle_ship(name){}
    explicit smuggler_ship(std::string&& name): ship(name), cargo_ship<T>(name), battle_ship(name){}

    virtual std::string get_ship_type() const override{ //over ride ship's get_ship_type() function
      return std::string("smuggler_ship");
    }

    virtual void sail() override{ //over ride ship's sail() function
        std::cout << get_type_name_addr() << " off to pillage!\n";
    }

    virtual void fire_weapon() override{ //over ride battle_ship's fire_weapon() function
        std::cout << get_type_name_addr() << " launching harpoon!\n";
    }

    void steal(ship& s){
        auto cs = dynamic_cast<cargo_ship<T>*>(&s);
        auto ss = dynamic_cast<smuggler_ship<T>*>(&s);
        if ( ss != nullptr ){
            std::ostringstream buf;
            buf << get_type_name_addr() << " cannot steal from another " << ss->get_type_name_addr() << '!';
            throw std::runtime_error(buf.str());
        }else if(cs != nullptr){
            std::cout << get_type_name_addr() << " is stealing half of " << cs->get_type_name_addr() <<	"!\n";

            auto cs_cargo = cs->get_cargo();
            auto middle = begin(cs_cargo) + distance(begin(cs_cargo), end(cs_cargo))/2;
            auto cs_end = end(cs_cargo);

            std::vector<int>::iterator e;
            for(e = middle; e != cs_end; e++){
                this->cargo_.emplace_back(std::move(*e));
            }
            cs_cargo.erase(middle, cs_end);
        }else{
            std::ostringstream buf;
            buf << get_type_name_addr() << " cannot steal from a " << s.get_type_name_addr() << '!';
            throw std::runtime_error(buf.str());
        }

    }

  protected:
    void do_smuggler_ship_only_move(smuggler_ship&& s){ //moves smuggler_ship's member varibales and inherited variables
        do_ship_only_move(std::move(s));    //moves variable "name_" inherited from ship
        do_cargo_ship_only_move(std::move(s)); //moves variable "cargo_" inherited from cargo_ship
        do_battle_ship_only_move(std::move(s)); ////moves nothing since no variables are inherited from battle_ship
    }

};

/**#######################################################################**/

void sail_all_ships(std::vector<std::unique_ptr<ship>>& ships)
{
  for (auto& ship : ships)
    ship->sail();
}

void fight(battle_ship& b1, battle_ship& b2){
  b1.fire_weapon();
  b2.fire_weapon();
}

void sail(std::unique_ptr<ship> const& s){
  s->sail();
}

template <typename ShipType, typename... Args>
std::unique_ptr<ship> create_ship(Args&&... args)
{
  return std::unique_ptr<ship>(new ShipType(std::forward<Args>(args)...));
}

template <typename ShipType, typename... Args>
std::unique_ptr<ShipType> create_ship_as_is(Args&&... args)
{
  return std::unique_ptr<ShipType>(new ShipType(std::forward<Args>(args)...));
}

/**#######################################################################**/

int main()
{
  using namespace std;

  // Using a unique_ptr to own each ship and ensure its eventual destruction...
  vector<unique_ptr<ship>> shipyard;
  shipyard.emplace_back(create_ship<cargo_ship<int>>("Storage Master"));
  shipyard.emplace_back(create_ship<battle_ship>("HMS Awesome"));
  shipyard.emplace_back(create_ship<smuggler_ship<int>>("Bounty Hunter"));

  // NOTE: If uncommented, the next line should compile...
  //vector<unique_ptr<ship>> shipyard_new_location(std::move(shipyard));

  // NOTE: If uncommented, the next line should NOT compile...
  //vector<unique_ptr<ship>> wont_work2(shipyard);

  // NOTE: If uncommented, the next declaration should NOT compile...
  /*vector<smuggler_ship<int>> wont_work2{
    smuggler_ship<int>("Thief 1 Ship"),
    smuggler_ship<int>("Thief 2 Ship")
  };*/

  // Anchors aweigh!
  cout << "Sail all ships...\n";
  sail_all_ships(shipyard);

  // Create a fight between two battle ships.
  cout << "\nFight between b1 and b2...\n";
  battle_ship b1("HMCS Hunter");
  battle_ship b2("SS Teacup");
  fight(b1, b2);

  // A battle ship fights a smuggler ship....
  cout << "\nFight b1 with last ship in shipyard...\n";
  fight(
    b1,
    dynamic_cast<smuggler_ship<int>&>(*shipyard.back())
  );

  cout << "\nSail one-by-one...\n";
  for (auto const& i : shipyard)
    sail(i);

  cout << "\nSteal cargo...\n";
  auto ss = create_ship_as_is<smuggler_ship<int>>("Pirate Ship");
  auto cs = create_ship<cargo_ship<int>>("Cargo Ship");
  auto bs = create_ship<battle_ship>("Non-Cargo Ship");
  ss->steal(*cs);
  try { ss->steal(*bs); } catch (exception& e) { cout << e.what() << '\n'; }
}
