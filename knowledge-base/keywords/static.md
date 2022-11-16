#keyword 

# `static`

If a class data member is declared as `static`:
- the data member belongs to the class, *not the specific object*
- useful to store class-wide information

If a class method is declared as `static`:
- it's independent of any particular object
- can only access `static` data members and functions, not even `this` pointer
- can be called using the class name
- can be called even without any object is created

```c++
/* .hpp */
class Player {
private:
    static int num_players; // cannot be initialized here
public:
    static int get_num_players();
};

/* .cpp */
int Player::num_players = 0; // init in the .cpp
Player::Player(std::string name_val)
    : name{name_val} {
        ++num_players;
}
Player::~Player() {
    --num_players;
}
int Player::get_num_players() {
    return num_players;
}
```
