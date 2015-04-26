
#include "bait/bait_static.hpp"
#include "bait/bait_dynamic.hpp"

#include "bait/bait_print_static.hpp"
#include "bait/bait_print_dynamic.hpp"

#include <iostream>

using namespace std;

#if 1
using BT = bait::DynamicBT<>;
template <typename... Ts>
void print(Ts&&... ts) {return bait::print_dynamic(forward<Ts>(ts)...);}
#else
using BT = bait::StaticBT;
template <typename... Ts>
void print(Ts&&... ts) {return bait::print_static(forward<Ts>(ts)...);}
#endif

using bait::status;
status find_player() { return status::SUCCESS; }
status player_in_range() { return status::SUCCESS; }
status player_out_range() { return status::SUCCESS; }
status attack() { return status::SUCCESS; }
status walk_toward_player() { return status::SUCCESS; }
status walk_randomly() { return status::SUCCESS; }
status stand() { return status::SUCCESS; }

auto behavior =
        BT::sequence(
                BT::sequence(
                        BT::until_fail(
                                find_player
                        ),
                        BT::selector(
                                BT::sequence(
                                        player_in_range,
                                        BT::inverter(BT::inverter(attack))
                                ),
                                walk_toward_player,
                                BT::selector(
                                        BT::selector(
                                                player_out_range
                                        )
                                )
                        )
                ),
                BT::until_fail(
                        BT::selector(
                                BT::inverter(
                                        BT::sequence(
                                                walk_randomly
                                        )
                                ),
                                stand
                        )
                )
        );
auto behavior2 = BT::simplify(behavior);

int main() {
    print(cout, behavior, "");
    print(cout, behavior2, "");

    using strings = string[3];
    cout << (strings{"SUCCESS", "FAILURE", "RUNNING"}[(int) behavior2()]) << endl;

    cout << sizeof(behavior) << endl;
    cout << sizeof(behavior2) << endl;
}
